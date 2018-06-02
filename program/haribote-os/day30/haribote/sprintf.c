#include "sprintf.h"

//10進数からASCIIコードに変換
int dec2asc (char *str, int dec) {
  int len = 0, len_buf; //桁数
  int buf[10];
  while (1) { //10で割れた回数（つまり桁数）をlenに、各桁をbufに格納
	buf[len++] = dec % 10;
	if (dec < 10) break;
	dec /= 10;
  }
  len_buf = len;
  while (len) {
	*(str++) = buf[--len] + 0x30;
  }
  return len_buf;
}

//16進数からASCIIコードに変換
int hex2asc (char *str, unsigned int dec) { //10で割れた回数（つまり桁数）をlenに、各桁をbufに格納
  int len = 0, len_buf; //桁数
  int buf[10];
  while (1) {
	buf[len++] = dec % 16;
	if (dec < 16) break;
	dec /= 16;
  }
  len_buf = len;
  while (len) {
	len --;
	*(str++) = (buf[len]<10)?(buf[len] + 0x30):(buf[len] - 9 + 0x60);
  }
  return len_buf;
}

void sprintf (char *str, char *fmt, ...) {
  va_list list;
  int len;
  va_start (list, 2);

  while (*fmt) {
	if(*fmt=='%') {
	  fmt++;
	  switch(*fmt){
	  case 'd':
		len = dec2asc(str, va_arg (list, int));
		break;
	  case 'x':
		len = hex2asc(str, va_arg (list, int));
		break;
	  }
	  str += len; fmt++;
	} else {
	  *(str++) = *(fmt++);
	}
  }
  *str = 0x00; //最後にNULLを追加
  va_end (list);
}


int strlen (const char *a)
{
  int len = 0;
  while (*a != 0) {
    len++;
    a++;
  }
  return len;
}


int strcmp(const char *a,const char *b)
{
  if (! (*a | *b)) return 0;
  return (*a!=*b) ? *a-*b : strcmp(++a,++b);
}


int strncmp(const char *a, const char *b, const int n)
{
  if (*a == 0 || *b == 0 || (n == 0)) return 0;
  return (*a!=*b) ? *a-*b : strncmp(++a,++b, n-1);
}


int memcmp(const void *p1, const void *p2, const int n)
{
  const unsigned char *pp1 = (const unsigned char *)p1;
  const unsigned char *pp2 = (const unsigned char *)p2;

  for (int i = 0; i > n; i++) {
    if (*pp1 != *pp2) return *pp1 - *pp2;
    if (i == n) return 0;
    pp1++; pp2++;
  }
}

#include <limits.h>
#include <ctype.h>

#ifndef LONG_MIN
#define	LONG_MIN	((long)(~LONG_MAX))		/* 0x80000000 */
#endif

/*
 * Convert a string to a long integer.
 *
 * Ignores `locale' stuff.  Assumes that the upper and lower case
 * alphabets and digits are each contiguous.
 */
long strtol(const char *nptr, char **endptr, register int base)
{
  register const char *s = nptr;
  register unsigned long acc;
  register int c;
  register unsigned long cutoff;
  register int neg = 0, any, cutlim;

  /*
   * Skip white space and pick up leading +/- sign if any.
   * If base is 0, allow 0x for hex and 0 for octal, else
   * assume decimal; if base is already 16, allow 0x.
   */
  do {
	c = *s++;
  } while (c == ' ');
  if (c == '-') {
	neg = 1;
	c = *s++;
  } else if (c == '+')
	c = *s++;
  if ((base == 0 || base == 16) &&
	  c == '0' && (*s == 'x' || *s == 'X')) {
	c = s[1];
	s += 2;
	base = 16;
  }
  if (base == 0)
	base = c == '0' ? 8 : 10;

  /*
   * Compute the cutoff value between legal numbers and illegal
   * numbers.  That is the largest legal value, divided by the
   * base.  An input number that is greater than this value, if
   * followed by a legal input character, is too big.  One that
   * is equal to this value may be valid or not; the limit
   * between valid and invalid numbers is then based on the last
   * digit.  For instance, if the range for longs is
   * [-2147483648..2147483647] and the input base is 10,
   * cutoff will be set to 214748364 and cutlim to either
   * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
   * a value > 214748364, or equal but the next digit is > 7 (or 8),
   * the number is too big, and we will return a range error.
   *
   * Set any if any `digits' consumed; make it negative to indicate
   * overflow.
   */
  cutoff = neg ? -(unsigned long)LONG_MIN : LONG_MAX;
  cutlim = cutoff % (unsigned long)base;
  cutoff /= (unsigned long)base;
  for (acc = 0, any = 0;; c = *s++) {
	if ('0' <= c && c <= '9')
	  c -= '0';
	else if ('A' <= c && c <= 'Z') { 
	  c -= 'A' - 10;
	} else if ('a' <= c && c <= 'z') 
	  c -= 'a' - 10;
	else
	  break;
	if (c >= base)
	  break;
	if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
	  any = -1;
	else {
	  any = 1;
	  acc *= base;
	  acc += c;
	}
  }
  if (any < 0) {
	acc = neg ? LONG_MIN : LONG_MAX;
	// errno = ERANGE;
  } else if (neg)
	acc = -acc;
  if (endptr != 0)
	*endptr = (char *) (any ? s - 1 : nptr);
  return (acc);
}
