#include "bootpack.h"

void HariMain(void)
{
  int i;
  struct BOOTINFO *binfo = (struct BOOTINFO *)0x0ff0;
  int xsize = (*binfo).scrnx;
  int ysize = (*binfo).scrny;
  char *vram = (*binfo).vram;

  init_gdtidt ();
  init_pic ();
  io_sti ();

  init_pallete();
  init_screen (vram, xsize, ysize);

  putfonts8_asc (binfo->vram, binfo->scrnx, 8, 8, COL8_FFFFFF, "ABC 123");

  putfonts8_asc (binfo->vram, binfo->scrnx, 31, 31, COL8_000000, "Haribote OS.");
  putfonts8_asc (binfo->vram, binfo->scrnx, 30, 30, COL8_FFFFFF, "Haribote OS.");

  char msg[127];
  sprintf(msg, "scrnx = %d", binfo->scrnx);
  putfonts8_asc (binfo->vram, binfo->scrnx, 30, 48, COL8_FFFFFF, msg);

  char mcursor[16][16];
  const int mx = xsize/2;
  const int my = ysize/2;
  
  init_mouse_cursor8 (mcursor, COL8_008484);
  putblock8_8 (binfo->vram, binfo->scrnx, 16, 16, mx,my, mcursor, 16);
  
  io_out8(PIC0_IMR, 0xf9); /* PIC1とキーボードを許可(11111001) */
  io_out8(PIC1_IMR, 0xef); /* マウスを許可(11101111) */
  
  for (;;) {
    io_hlt();
  }
}


