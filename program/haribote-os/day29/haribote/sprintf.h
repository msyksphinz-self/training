#ifndef __SPRINTF_H__
#define __SPRINTF_H__

#include <stdarg.h>
void sprintf (char *str, char *fmt, ...);

int strcmp  (const char *a, const char *b);
int strncmp (const char *a, const char *b, const int n);
int memcmp  (const void *p1, const void *p2, const int n);

#endif // __SPRINTF_H__
