#include "timer.h"
#include "mtask.h"

struct TASKCTL *taskctl;
struct TIMER *task_timer;

struct TASK *task_init (struct MEMMAN *memman)
{
  int i;
  struct TASK *task, *idle;
  struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) ADR_GDT;
  taskctl = (struct TASKCTL *)memman_alloc_4k (memman, sizeof(struct TASKCTL));
  for (i = 0;i < MAX_TASKS; i++) {
    taskctl->tasks0[i].flags = 0;
    taskctl->tasks0[i].sel = (TASK_GDT0 + i) * 8;
    set_segmdesc(gdt + TASK_GDT0 + i, 103, (int) &taskctl->tasks0[i].tss, AR_TSS32);
  }
  for (i = 0; i < MAX_TASKLEVELS; i++) {
	taskctl->level[i].running = 0;
	taskctl->level[i].now = 0;
  }
  
  task = task_alloc();
  task->flags = 2;  // Running
  task->priority = 2;  // 0.02 second 
  task->level = 1;
  task_add(task);
  task_switchsub();
  load_tr(task->sel);
  task_timer = timer_alloc();
  timer_settime (task_timer, 2);

  idle = task_alloc ();
  idle->tss.esp = memman_alloc_4k (memman, 64 * 1024) + 64 * 1024;
  idle->tss.eip = (int)&task_idle;
  idle->tss.es = 1 * 8;
  idle->tss.cs = 2 * 8;
  idle->tss.ss = 1 * 8;
  idle->tss.ds = 1 * 8;
  idle->tss.fs = 1 * 8;
  idle->tss.gs = 1 * 8;
  task_run (idle, MAX_TASKLEVELS - 1, 1);
  
  return task;
}


struct TASK *task_alloc(void)
{
  int i;
  struct TASK *task;
  for (i = 0; i < MAX_TASKS; i++) {
    if (taskctl->tasks0[i].flags == 0) {
      task = &taskctl->tasks0[i];
      task->flags = 1;
      task->tss.eflags = 0x0000202;  // IF = 1
      task->tss.eax = 0;
      task->tss.ecx = 0;
      task->tss.edx = 0;
      task->tss.ebx = 0;
      task->tss.ebp = 0;
      task->tss.esi = 0;
      task->tss.edi = 0;
      task->tss.es  = 0;
      task->tss.ds  = 0;
      task->tss.fs  = 0;
      task->tss.gs  = 0;
      task->tss.ldtr= 0;
      task->tss.iomap = 0x40000000;
      return task;
    }
  }
  return 0;
}

         
void task_run (struct TASK *task, int level, int priority)
{
  if (level < 0) {
	level = task->level;
  }
  if (priority > 0) {
    task->priority = priority;
  }
  if (task->flags == 2 && task->level != level) {
	task_remove(task);
  }
  if (task->flags != 2) {
	task->level = level;
	task_add(task);
  }
  taskctl->lv_change = 1;
  
  return;
}


void task_switch(void)
{
  struct TASKLEVEL *tl = &taskctl->level[taskctl->now_lvl];
  struct TASK *new_task, *now_task = tl->tasks[tl->now];
  tl->now++;
  if (tl->now == tl->running) {
    tl->now = 0;
  }
  if (taskctl->lv_change != 0) {
	task_switchsub ();
	tl = &taskctl->level[taskctl->now_lvl];
  }
  new_task = tl->tasks[tl->now];
  timer_settime(task_timer, new_task->priority);
  if (new_task != now_task) {
	farjmp (0, new_task->sel);
  }
  
  return;
}


void task_sleep(struct TASK *task)
{
  struct TASK *now_task;
  if (task->flags == 2) { // if specified task is waken
	now_task = task_now();
	task_remove (task);
	if (task == now_task) {
	  task_switchsub ();
	  now_task = task_now ();
	  farjmp (0, now_task->sel);
	}
  }

  return;
}


struct TASK *task_now (void)
{
  struct TASKLEVEL *tl = &taskctl->level[taskctl->now_lvl];
  return tl->tasks[tl->now];
}


void task_add (struct TASK *task)
{
  struct TASKLEVEL *tl = &taskctl->level[task->level];
  tl->tasks[tl->running] = task;
  tl->running++;
  task->flags = 2;
  return;
}


void task_remove (struct TASK *task)
{
  int i;
  struct TASKLEVEL *tl = &taskctl->level[task->level];

  /* search task */
  for (i = 0; i < tl->running; i++) {
	if (tl->tasks[i] == task) {
	  break;
	}
  }

  tl->running--;
  if (i < tl->now) {
	tl->now--;
  }
  if (tl->now >= tl->running) {
	tl->now = 0;
  }
  task->flags = 1; // sleep

  /* shift task */
  for (; i < tl->running; i++) {
	tl->tasks[i] = tl->tasks[i+1];
  }

  return;
}


void task_switchsub (void)
{
  int i;
  for (i = 0; i < MAX_TASKLEVELS; i++) {
	if (taskctl->level[i].running > 0) {
	  break;
	}
  }
  taskctl->now_lvl = i;
  taskctl->lv_change = 0;

  return;
}


void task_idle(void)
{
  for (;;) {
    io_hlt();
  }
}


void console_task (struct SHEET *sheet, unsigned int memtotal)
{
  struct FIFO32 fifo;
  struct TIMER *timer;
  struct TASK *task = task_now ();
  int i, fifobuf[128], cursor_x = 16, cursor_y = 28, cursor_c = -1;
  char s[30], cmdline[30];
  struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;
  int x, y;
  
  fifo32_init (&task->fifo, 128, fifobuf, task);
  timer = timer_alloc ();
  timer_init (timer, &task->fifo, 1);
  timer_settime (timer, 50);

  putfonts8_asc_sht(sheet, 8, 28, COL8_FFFFFF, COL8_000000, ">", 1);
  
  for (;;) {
    io_cli();
    if (fifo32_status (&task->fifo) == 0) {
      task_sleep(task);
      io_sti ();
    } else {
      i = fifo32_get (&task->fifo);
      io_sti ();
      if (i <= 1) {   /* timer for cursor */
        if (i != 0) {
          timer_init (timer, &task->fifo, 0);
          if (cursor_c >= 0) {
            cursor_c = COL8_FFFFFF;
          }
        } else {
          timer_init (timer, &task->fifo, 1);
          if (cursor_c >= 0) {
            cursor_c = COL8_000000;
          }
        }
        timer_settime (timer, 50);
	  }
      if (i == 2) { // Cursor ON
        cursor_c = COL8_FFFFFF;
      }
      if (i == 3) { // Cursor OFF
        boxfill8 (sheet->buf, sheet->bxsize, COL8_000000, cursor_x, 28, cursor_x + 7, 43);
        cursor_c = -1;
      }
	  if (256 <= i && i <= 511) {  /* Keyboard data from Task-A */
		if (i == 8 + 256) {
		  // Backspace
		  if (cursor_x > 16) {
			putfonts8_asc_sht(sheet, cursor_x, cursor_y, COL8_FFFFFF, COL8_000000, " ", 1);
			cursor_x -= 8;
		  }
		} else if (i == 10 + 256) {
		  // Enter
          putfonts8_asc_sht (sheet, cursor_x, cursor_y, COL8_FFFFFF, COL8_000000, " ", 1);
          cmdline[cursor_x / 8 - 2] = 0;
          cursor_y = cons_newline(cursor_y, sheet);
          // Execute Command
          // if (cmdline[0] == 'm' && cmdline[1] == 'e' && cmdline[2] == 'm' && cmdline[3] == 0) {
          if (strcmp(cmdline, "mem") == 0) {
            /* mem command */
            sprintf(s, "total %dMB", memtotal / (1024 * 1024));
            putfonts8_asc_sht (sheet, 8, cursor_y, COL8_FFFFFF, COL8_000000, s, 30);
            cursor_y = cons_newline(cursor_y, sheet);
            sprintf(s, "free %dKB", memman_total(memman) / 1024);
            putfonts8_asc_sht (sheet, 8, cursor_y, COL8_FFFFFF, COL8_000000, s, 30);
            cursor_y = cons_newline(cursor_y, sheet);
            cursor_y = cons_newline(cursor_y, sheet);
          } else if (strcmp(cmdline, "cls") == 0) {
            /* cls command */
            for (y = 28; y < 28 + 128; y++) {
              for (x = 8; x < 8 + 240; x++) {
                sheet->buf[x + y * sheet->bxsize] = COL8_000000;
              }
            }
            sheet_refresh (sheet, 8, 28, 8 + 240, 28 + 128);
            cursor_y = 28;
          } else if (cmdline[0] != 0) {
            // Not Command Line and Empty
            putfonts8_asc_sht (sheet, 8, cursor_y, COL8_FFFFFF, COL8_000000, "Bad command.", 12);
            cursor_y = cons_newline(cursor_y, sheet);
            cursor_y = cons_newline(cursor_y, sheet);
          }
		  putfonts8_asc_sht (sheet, 8, cursor_y, COL8_FFFFFF, COL8_000000, ">", 1);
		  cursor_x = 16;
		} else {
		  // Normal charactor
		  if (cursor_x < 240) {
			s[0] = i - 256;
			s[1] = 0;
            cmdline[cursor_x / 8 - 2] = i - 256;
			putfonts8_asc_sht(sheet, cursor_x, cursor_y, COL8_FFFFFF, COL8_000000, s, 1);
			cursor_x += 8;
		  }
		}
	  }
      if (cursor_c >= 0) {
        boxfill8 (sheet->buf, sheet->bxsize, cursor_c, cursor_x, cursor_y, cursor_x + 7, cursor_y + 15);
      }
	  sheet_refresh (sheet, cursor_x, cursor_y, cursor_x + 8, cursor_y + 16);
    }
  }
}


int cons_newline (int cursor_y, struct SHEET *sheet)
{
  int x, y;
  if (cursor_y < 28 + 112) {
    cursor_y += 16;
  } else {  // scroll
    for (y = 28; y < 28 + 112; y++) {
      for (x = 8; x < 8 + 240; x++) {
    	sheet->buf[x + y * sheet->bxsize] = sheet->buf[x + (y + 16) * sheet->bxsize];
      }
    }
    for (y = 28 + 112; y < 28 + 128; y++) {
      for (x = 8; x < 8 + 240; x++) {
    	sheet->buf[x + y * sheet->bxsize] = COL8_000000;
      }
    }
    // show Prompt
    sheet_refresh (sheet, 8, 28, 8 + 240, 28 + 128);
  }
  return cursor_y;
}
