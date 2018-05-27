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
