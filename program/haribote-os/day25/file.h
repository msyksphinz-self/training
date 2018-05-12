#ifndef __FILE_H__
#define __FILE_H__

struct FILEINFO {
  uint8_t  name[8], ext[3], type;
  uint8_t  reserve[10];
  uint16_t time, date, clustno;
  uint32_t size;
};


void file_readfat (int *fat, unsigned char *img);
void file_loadfile (int clustno, int size, char *buf, int *fat, char *img);

struct FILEINFO *file_search (char *name, struct FILEINFO *finfo, int max);

#endif // __FILE_H__

