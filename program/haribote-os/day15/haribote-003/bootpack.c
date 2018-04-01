#include "bootpack.h"

extern struct TIMERCTL timerctl;

#define EFLAGS_AC_BIT      0x00040000
#define CR0_CACHE_DISABLE  0x60000000

void putfonts8_asc_sht (struct SHEET *sht, int x, int y, int c, int b, char *s, int l)
{
  boxfill8(sht->buf, sht->bxsize, b, x, y, x+l*8-1, y+15);
  putfonts8_asc(sht->buf, sht->bxsize, x, y, c, s);
  sheet_refresh (sht, x, y, x+l*8, y+16);
  return;
}

void task_b_main (struct SHEET *sht_back)
{
  struct FIFO32 fifo;
  struct TIMER *timer_1s, *timer_put;
  int i, fifobuf[128], count = 0, count0 = 0;
  char s[11];

  fifo32_init(&fifo, 128, fifobuf);
  timer_put = timer_alloc();
  timer_init(timer_put, &fifo, 1);
  timer_settime(timer_put, 1);
  timer_1s = timer_alloc();
  timer_init(timer_1s, &fifo, 100);
  timer_settime(timer_1s, 100);
  
  for (;;) {
    count++;
    io_cli();
    if (fifo32_status(&fifo) == 0) {
      io_stihlt();
    } else {
      i = fifo32_get(&fifo);
      io_sti();
      if (i == 1) {
        sprintf (s, "%d", count);
        putfonts8_asc_sht (sht_back, 0, 144, COL8_FFFFFF, COL8_008484, s, 11);
        timer_settime (timer_put, 1);
      } else if (i == 100) {
        sprintf (s, "%d", count - count0);
        putfonts8_asc_sht (sht_back, 0, 128, COL8_FFFFFF, COL8_008484, s, 11);
        count0 = count;
        timer_settime(timer_1s, 100);
      }
    }
  }
}


void HariMain(void)
{
  int i;
  struct BOOTINFO *binfo = (struct BOOTINFO *)0x0ff0;
  int xsize = (*binfo).scrnx;
  int ysize = (*binfo).scrny;
  char *vram = (*binfo).vram;
  char msg[40], mcursor[256];
  int mx = xsize/2;
  int my = ysize/2;
  int fifobuf[128];
  struct MOUSE_DEC mdec;
  unsigned char s[32];
  unsigned int memtotal, count = 0;
  struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;
  int cursor_x, cursor_c;
  
  struct SHTCTL *shtctl;
  struct SHEET *sht_back, *sht_mouse, *sht_win;
  unsigned char *buf_back, buf_mouse[256], *buf_win;
  struct TIMER *timer1, *timer2, *timer3, *timer_ts;
  struct FIFO32 fifo;

  static char keytable[0x54] = {
    0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^', 0,   0,
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '@', '[', 0,   0,   'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', ':', 0,   0,   ']', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.'
  };

  init_gdtidt ();
  init_pic ();
  io_sti ();

  fifo32_init(&fifo, 32, fifobuf);

  init_pit();
  io_out8(PIC0_IMR, 0xf8); /* Allow PIT and Keyboard (11111000) */
  io_out8(PIC1_IMR, 0xef); /* Allow Mouse (11101111) */
  init_keyboard (&fifo, 256);
  enable_mouse (&fifo, 512, &mdec);

  timer1 = timer_alloc();
  timer_init(timer1, &fifo, 10);
  timer_settime(timer1, 1000);

  timer2 = timer_alloc();
  timer_init(timer2, &fifo, 3);
  timer_settime(timer2, 300);

  timer3 = timer_alloc();
  timer_init(timer3, &fifo, 1);
  timer_settime(timer3, 50);
  
  memtotal = memtest(0x00400000, 0xbfffffff);
  memman_init (memman);
  memman_free (memman, 0x00001000, 0x009e000);   /* 0x00001000 - 0x0009efff */
  memman_free (memman, 0x00400000, memtotal - 0x00400000);

  init_pallete();
  shtctl = shtctl_init (memman, binfo->vram, binfo->scrnx, binfo->scrny);
  sht_back  = sheet_alloc(shtctl);
  sht_mouse = sheet_alloc(shtctl);
  sht_win   = sheet_alloc(shtctl);
  buf_back = (unsigned char *)memman_alloc_4k (memman, binfo->scrnx * binfo->scrny);
  buf_win  = (unsigned char *)memman_alloc_4k (memman, 160 * 52);
  sheet_setbuf (sht_back,  buf_back, binfo->scrnx, binfo->scrny, -1);
  sheet_setbuf (sht_mouse, buf_mouse, 16, 16, 99);
  sheet_setbuf (sht_win,   buf_win,   160, 52, -1);
  init_screen (buf_back, xsize, ysize);
  init_mouse_cursor8 (buf_mouse, 99);
  make_window8(buf_win, 160, 52, "counter");
  
  sprintf (s, "(%d, %d)", mx, my);
  putfonts8_asc (buf_back, binfo->scrnx, 0, 0, COL8_FFFFFF, s);
  sprintf (s, "Memory %dMB, free : %dKB", 
           memtotal / (1024 * 1024), memman_total(memman) / 1024);
  putfonts8_asc(buf_back, binfo->scrnx, 0, 32, COL8_FFFFFF, s);

  make_textbox8 (sht_win, 8, 28, 144, 16, COL8_FFFFFF);
  cursor_x = 8;
  cursor_c = COL8_FFFFFF;
  
  sheet_slide (sht_back,  0, 0);
  sheet_slide (sht_mouse, mx, my);
  sheet_slide (sht_win,   80, 72);
  sheet_updown (sht_back,  0);
  sheet_updown (sht_win,   1);
  sheet_updown (sht_mouse, 2);

  sheet_refresh (sht_back, 0, 0, binfo->scrnx, 48);

  //=====================
  // Task Settings
  //=====================

  mt_init();
  
  struct TSS32 tas_a, tas_b;

  tas_a.ldtr = 0;
  tas_a.iomap = 0x40000000;
  tas_b.ldtr = 0;
  tas_b.iomap = 0x40000000;

  struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *)ADR_GDT;
  set_segmdesc (gdt + 3, 103, (int)&tas_a, AR_TSS32);
  set_segmdesc (gdt + 4, 103, (int)&tas_b, AR_TSS32);

  load_tr(3 * 8);
  
  int tas_b_esp;
  tas_b_esp = memman_alloc_4k (memman, 64 * 1024) + 64 * 1024;
  *((int *) (tas_b_esp + 4)) = (int) sht_back;
  
  tas_b.eip = (int)&task_b_main;
  tas_b.eflags = 0x00000202;
  tas_b.eax = 0;
  tas_b.ecx = 0;
  tas_b.edx = 0;
  tas_b.ebx = 0;
  tas_b.esp = tas_b_esp;
  tas_b.ebp = 0;
  tas_b.esi = 0;
  tas_b.edi = 0;
  tas_b.es = 1 * 8;
  tas_b.cs = 2 * 8;
  tas_b.ss = 1 * 8;
  tas_b.ds = 1 * 8;
  tas_b.fs = 1 * 8;
  tas_b.gs = 1 * 8;

  //=======================
  // Task Switch for 0.02s
  //=======================
  timer_ts = timer_alloc();
  timer_init(timer_ts, &fifo, 2);
  timer_settime(timer_ts, 2);
  
  for (;;) {
	count ++;
	io_cli();
	if (fifo32_status(&fifo) == 0) {
	  io_sti();
	} else {
      i = fifo32_get(&fifo);
      io_sti();
      /* if (i == 2) {
        farjmp(0, 4 * 8);
        timer_settime(timer_ts, 2);
        } else */
      if (256 <= i && i <= 511) {  // Keyboard Data
		sprintf (s, "%x", i - 256);
        putfonts8_asc_sht (sht_back, 0, 16, COL8_FFFFFF, COL8_008484, s, 2);
        if (i < 256 + 0x54) {
          if (keytable[i - 256] != 0) {
            s[0] = keytable[i-256];
            s[1] = 0;
            putfonts8_asc_sht (sht_win, cursor_x, 28, COL8_000000, COL8_FFFFFF, s, 1);
            cursor_x += 8;
          }
        }
        if (i == 256 + 0x0e && cursor_x > 8) {  // Backspace
          putfonts8_asc_sht(sht_win, cursor_x, 28, COL8_000000, COL8_FFFFFF, " ", 1);
          cursor_x -= 8;
        }
        boxfill8 (sht_win->buf, sht_win->bxsize, cursor_c, cursor_x, 28, cursor_x + 7, 43);
        sheet_refresh(sht_win, cursor_x, 28, cursor_x + 8, 44);
	  } else if (512 <= i && i <= 767) {  // Mouse Data
        if (mouse_decode(&mdec, i-512) != 0) {
          sprintf (s, "[lcr %d %d]", mdec.x, mdec.y);
          if ((mdec.btn & 0x01) != 0) { s[1] = 'L'; }
          if ((mdec.btn & 0x02) != 0) { s[3] = 'R'; }
          if ((mdec.btn & 0x04) != 0) { s[2] = 'C'; }
          putfonts8_asc_sht (sht_back, 32, 16, COL8_FFFFFF, COL8_008484, s, 15);
          
          mx += mdec.x;
          my += mdec.y;
          if (mx < 0) { mx = 0; } if (mx > binfo->scrnx - 1) { mx = binfo->scrnx - 1; }
          if (my < 0) { my = 0; } if (my > binfo->scrny - 1) { my = binfo->scrny - 1; }

          sprintf(s, "(%d, %d)", mx, my);
          putfonts8_asc_sht (sht_back, 0, 0, COL8_FFFFFF, COL8_008484, s, 10);

          sheet_slide (sht_mouse, mx, my);
          if ((mdec.btn & 0x01) != 0) {
            sheet_slide (sht_win, mx - 80, my - 8);
          }
        }
      } else if (i == 10) {
        putfonts8_asc_sht (sht_back, 0, 64, COL8_FFFFFF, COL8_008484, "10[sec]", 7);
      } else if (i == 3) {
        putfonts8_asc_sht (sht_back, 0, 80, COL8_FFFFFF, COL8_008484, "3[sec]", 6);
        count = 0;
      } else if (i <= 1) {
        if (i != 0) {
          timer_init(timer3, &fifo, 0);
          cursor_c = COL8_000000;
        } else {
          timer_init(timer3, &fifo, 1);
          cursor_c = COL8_FFFFFF;
        }
        timer_settime(timer3, 50);
        boxfill8 (sht_win->buf, sht_win->bxsize, cursor_c, cursor_x, 28, cursor_x + 7, 43);
        sheet_refresh(sht_win, cursor_x, 28, cursor_x + 8, 44);
      }
    }
  }
}


