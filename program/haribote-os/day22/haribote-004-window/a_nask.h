#ifndef __A_NASK_H__
#define __A_NASK_H__

void api_putchar (int c);
void api_putstr0 (char *s);
void api_end (void);
int  api_openwin (char *buf, int xsiz, int ysiz, int col_inv, char *title);
void api_putstrwin (int win, int x, int y, int col, int len, char *str);
void api_boxfilwin (int win, int x0, int y0, int x1, int y1, int col);

#endif // __A_NASK_H__