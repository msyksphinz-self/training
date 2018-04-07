#ifndef __MTASK_H__
#define __MTASK_H__

#include "bootpack.h"
#include "memory.h"

#define MAX_TASKS  1000
#define TASK_GDT0  3

/* Task TSS */
struct TSS32 {
  int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
  int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
  int es, cs, ss, ds, fs, gs;
  int ldtr, iomap;
};

struct TASK {
  int sel, flags;   // sel = GDT index
  struct TSS32 tss;
};

struct TASKCTL {
  int running;   /* Number of running tasks */
  int now;       /* Current taks index */
  struct TASK *tasks[MAX_TASKS];
  struct TASK tasks0[MAX_TASKS];
};

struct TASK *task_init (struct MEMMAN *memman);
struct TASK *task_alloc(void);
void task_run (struct TASK *task);
void task_switch(void);

void mt_init (void);
void mt_taskswitch (void);

#endif // __MTASK_H__

