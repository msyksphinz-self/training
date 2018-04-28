#ifndef __MEMTEST_H__
#define __MEMTEST_H__

#define EFLAGS_AC_BIT      0x00040000
#define CR0_CACHE_DISABLE  0x60000000

unsigned int memtest (unsigned int start, unsigned int end);
unsigned int memtest_sub(unsigned int start, unsigned int end);

#endif // __MEMTEST_H__
