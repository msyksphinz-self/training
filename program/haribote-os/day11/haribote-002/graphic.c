#include "bootpack.h"

void putfonts8_asc (char *vram, int xsize, int x, int y, char c, unsigned char *s)
{
  extern char hankaku[4096];

  for (; *s != 0x00;s++) {
	putfonts8 (vram, xsize, x, y, c, hankaku + *s * 16);
	x += 8;
  }
  return;
}


void putfonts8 (char *vram, int xsize, int x, int y, char c, char *font)
{
  for (int i = 0; i < 16; i++) {
	char d = font[i];
	char *p = vram + (y + i) * xsize + x;
	if ((d & 0x80) != 0) { p[0] = c; }
	if ((d & 0x40) != 0) { p[1] = c; }
	if ((d & 0x20) != 0) { p[2] = c; }
	if ((d & 0x10) != 0) { p[3] = c; }
	if ((d & 0x08) != 0) { p[4] = c; }
	if ((d & 0x04) != 0) { p[5] = c; }
	if ((d & 0x02) != 0) { p[6] = c; }
	if ((d & 0x01) != 0) { p[7] = c; }
  }
}


void init_pallete(void)
{
  static unsigned char table_rgb[16 * 3] = {
	0x00, 0x00, 0x00,   /*  0: Black       	*/
	0xff, 0x00, 0x00,   /*  1: Light Read  	*/
	0x00, 0xff, 0x00,   /*  2: Light Green 	*/
	0xff, 0xff, 0x00,   /*  3: Light Yellow */
	0x00, 0x00, 0xff,   /*  4: Light Blue   */
	0xff, 0x00, 0xff,   /*  5: Light Purple */
	0x00, 0xff, 0xff,   /*  6: Light Light Blue */
	0xff, 0xff, 0xff,   /*  7: White        */
	0xc6, 0xc6, 0xc6,   /*  8: Light Gray   */
	0x84, 0x00, 0x00,   /*  9: Dark Red     */
	0x00, 0x84, 0x00,   /* 10: Dark Green   */
	0x84, 0x84, 0x00,   /* 11: Dark Yellow  */
	0x00, 0x00, 0x84,   /* 12: Dark Blue    */
	0x84, 0x00, 0x84,   /* 13: Dark Purple  */
	0x00, 0x84, 0x84,   /* 14: Dark Light Blue */
	0x84, 0x84, 0x84    /* 15: Dark Gray    */
  };
  set_palette (0, 15, table_rgb);
  return;

}


void set_palette (int start, int end, unsigned char *rgb)
{
  int i, eflags;
  eflags = io_load_eflags ();
  io_cli ();                  /* clear interrupt flag */
  io_out8 (0x03c8, start);
  for (i = start; i <= end; i++) {
	io_out8(0x03c9, rgb[0] / 4);
	io_out8(0x03c9, rgb[1] / 4);
	io_out8(0x03c9, rgb[2] / 4);
	rgb += 3;
  }
  io_store_eflags (eflags);
  return;
}


void init_screen(char *vram, int x, int y)
{
	boxfill8(vram, x, COL8_008484,  0,     0,      x -  1, y - 29);
	boxfill8(vram, x, COL8_C6C6C6,  0,     y - 28, x -  1, y - 28);
	boxfill8(vram, x, COL8_FFFFFF,  0,     y - 27, x -  1, y - 27);
	boxfill8(vram, x, COL8_C6C6C6,  0,     y - 26, x -  1, y -  1);

	boxfill8(vram, x, COL8_FFFFFF,  3,     y - 24, 59,     y - 24);
	boxfill8(vram, x, COL8_FFFFFF,  2,     y - 24,  2,     y -  4);
	boxfill8(vram, x, COL8_848484,  3,     y -  4, 59,     y -  4);
	boxfill8(vram, x, COL8_848484, 59,     y - 23, 59,     y -  5);
	boxfill8(vram, x, COL8_000000,  2,     y -  3, 59,     y -  3);
	boxfill8(vram, x, COL8_000000, 60,     y - 24, 60,     y -  3);

	boxfill8(vram, x, COL8_848484, x - 47, y - 24, x -  4, y - 24);
	boxfill8(vram, x, COL8_848484, x - 47, y - 23, x - 47, y -  4);
	boxfill8(vram, x, COL8_FFFFFF, x - 47, y -  3, x -  4, y -  3);
	boxfill8(vram, x, COL8_FFFFFF, x -  3, y - 24, x -  3, y -  3);
	return;
}


void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
  int x, y;
  for (y = y0; y <= y1; y++) {
	for (x = x0; x <= x1; x++) {
	  vram[y * xsize + x] = c;
	}
  }
  return;
}




void init_mouse_cursor8 (char *mouse, char bc)
{
  static char cursor[16][16] = {
	"**************..",
	"*OOOOOOOOOOO*...",
	"*OOOOOOOOOO*....",
	"*OOOOOOOOO*.....",
	"*OOOOOOOO*......",
	"*OOOOOOO*.......",
	"*OOOOOOO*.......",
	"*OOOOOOOO*......",
	"*OOOO**OOO*.....",
	"*OOO*..*OOO*....",
	"*OO*....*OOO*...",
	"*O*......*OOO*..",
	"**........*OOO*.",
	"*..........*OOO*",
	"............*OO*",
	".............***"
  };

  int x, y;
  for (y = 0; y < 16; y++) {
	for (x = 0; x < 16; x++) {
	  if (cursor[y][x] == '*') {
		mouse[y * 16 + x] = COL8_000000;
	  }
	  if (cursor[y][x] == 'O') {
		mouse[y * 16 + x] = COL8_FFFFFF;
	  }
	  if (cursor[y][x] == '.') {
		mouse[y * 16 + x] = bc;
	  }
	}
  }
  return;
}

void putblock8_8(char *vram, int vxsize, int pxsize, int pysize, int px0, int py0, char *buf, int bxsize)
{
  int x, y;
  for (y = 0; y < pysize; y++) {
	for (x = 0; x < pxsize; x++) {
	  vram[(py0+y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
	}
  }

  return;
}


