#ifndef __TIMER_H__
#define __TIMER_H__

#include "fifo.h"

#define MAX_TIMER 500

struct TIMER {
  unsigned int timeout, flags;
  struct FIFO *fifo;
  unsigned char data;
};

struct TIMERCTL {
  unsigned int count;
  struct TIMER timer[MAX_TIMER];
};


void init_pit(void);
struct TIMER *timer_alloc (void);
void timer_free (struct TIMER *timer);
void timer_init (struct TIMER *timer, struct FIFO8 *fifo, unsigned char data);
void timer_settime (struct TIMER *timer, unsigned int timeout);
void inthandler20(int *esp);


#endif // __TIMER_H__
