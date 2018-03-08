#include "bootpack.h"

void HariMain(void)
{
  int i;
  struct BOOTINFO *binfo = (struct BOOTINFO *)0x0ff0;
  int xsize = (*binfo).scrnx;
  int ysize = (*binfo).scrny;
  char *vram = (*binfo).vram;

  init_pallete();
  init_screen (vram, xsize, ysize);

  putfont8_asc (binfo->vram, binfo->scrnx, 8, 8, COL8_FFFFFF, "ABC 123");

  putfont8_asc (binfo->vram, binfo->scrnx, 31, 31, COL8_000000, "Haribote OS.");
  putfont8_asc (binfo->vram, binfo->scrnx, 30, 30, COL8_FFFFFF, "Haribote OS.");

  char mcursor[16][16];
  const int mx = xsize/2;
  const int my = ysize/2;
  
  init_mouse_cursor8 (mcursor, COL8_008484);
  putblock8_8 (binfo->vram, binfo->scrnx, 16, 16, mx,my, mcursor, 16);
  
  for (;;) {
    io_hlt();
  }
}


