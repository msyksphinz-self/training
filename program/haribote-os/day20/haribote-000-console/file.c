#include <stdint.h>
#include "file.h"

void file_readfat (int *fat, unsigned char *img)
{
  int i ,j = 0;
  for (i = 0; i < 2800; i+=2) {
    fat[i + 0] = (img[j + 0]      | img[j + 1] << 8) & 0xfff;
    fat[i + 1] = (img[j + 1] >> 4 | img[j + 2] << 4) & 0xfff;
    j += 3;
  }
  return;
}


void file_loadfile (int clustno, int size, char *buf, int *fat, char *img)
{
  int i;
  for (;;) {
    if (size <= 512) {
      for (i = 0; i < size; i++) {
        buf[i] = img[clustno * 512 + i];
      }
      break;
    }
    for (i = 0; i < 512; i++) {
      buf[i] = img[clustno * 512 + i];
    }

    size -= 512;
    buf  += 512;
    clustno = fat[clustno];
  }

  return;
}


struct FILEINFO *file_search (char *name, struct FILEINFO *finfo, int max)
{
  int i, x;
  char s[12];
  for (i = 0; i < 11; i++) {
    s[i] = ' ';
  }
  i = 0;
  for (x = 5; i < 11 && name[x] != 0; x++) {
    if (name[x] == '.' && i <= 8) {
      i = 8;
    } else {
      s[i] = name[x];
      if ('a' <= s[i] && s[i] <= 'z') {
        // change lower to upper
        s[i] -= 0x20;
      }
      i++;
    }
  }
  // Find file
  for (x = 0; x < 224;) {
    if (finfo[x].name[0] == 0x00) {
      break;
    }
    if ((finfo[x].type & 0x18) == 0) {
      for (int idx = 10; idx >= 0; idx--) {
        // for (int idx = 0; idx < 11; idx++) {
        if (finfo[x].name[idx] != s[idx]) {
          goto type_next_file;
        }
      }
      return finfo + i;  // Find file
    }
  type_next_file:
    x++;
  }
  return 0;
}
