#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "fifo.h"

void wait_KBC_sendready (void);
void init_keyboard (struct FIFO32 *fifo, int data0);
void inthandler21(int *esp);

#endif // __KEYBOARD_H__


