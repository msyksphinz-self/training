#ifndef __MTASK_H__
#define __MTASK_H__

#include <stdint.h>
#include "bootpack.h"
#include "memory.h"
#include "fifo.h"

#define MAX_TASKS      1000
#define MAX_TASKS_LV   100
#define MAX_TASKLEVELS 10
#define TASK_GDT0      3


/* Task TSS */
struct TSS32 {
  int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
  int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
  int es, cs, ss, ds, fs, gs;
  int ldtr, iomap;
};

struct FIFO32 {
  int *buf;
  int p, q, size, free, flags;
  struct TASK *task;
};

struct TASK {
  int sel, flags;   // sel = GDT index
  int level, priority;
  struct FIFO32 fifo;
  struct TSS32 tss;
  struct SEGMENT_DESCRIPTOR ldt[2];
  struct CONSOLE *cons;
  int ds_base, cons_stack;
};

struct TASKLEVEL {
  int running;
  int now;
  struct TASK *tasks[MAX_TASKS_LV];
};


struct TASKCTL {
  int  now_lvl;   /* Number of running level */
  char lv_change;       /* Next task switch, change level or not */
  struct TASKLEVEL level[MAX_TASKLEVELS];
  struct TASK tasks0[MAX_TASKS];
};

struct TASK *task_init (struct MEMMAN *memman);
struct TASK *task_alloc(void);
void task_run (struct TASK *task, int level, int priority);
void task_switch(void);
void task_sleep (struct TASK *task);
struct TASK *task_now (void);
void task_add (struct TASK *task);
void task_remove (struct TASK *task);
void task_switchsub (void);
void task_idle(void);

#endif // __MTASK_H__

