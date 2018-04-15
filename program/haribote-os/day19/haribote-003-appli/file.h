#ifndef __FILE_H__
#define __FILE_H__

void file_readfat (int *fat, unsigned char *img);
void file_loadfile (int clustno, int size, char *buf, int *fat, char *img);

#endif // __FILE_H__

