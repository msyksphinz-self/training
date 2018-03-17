#include "bootpack.h"

extern struct FIFO8 keyfifo;
extern struct FIFO8 mousefifo;

#define EFLAGS_AC_BIT      0x00040000
#define CR0_CACHE_DISABLE  0x60000000

void HariMain(void)
{
  int i;
  struct BOOTINFO *binfo = (struct BOOTINFO *)0x0ff0;
  int xsize = (*binfo).scrnx;
  int ysize = (*binfo).scrny;
  char *vram = (*binfo).vram;
  char msg[40], mcursor[256];
  int mx = xsize/2;
  int my = ysize/2;
  char keybuf[32], mousebuf[32];
  struct MOUSE_DEC mdec;
  unsigned char s[32];
  unsigned int memtotal;
  struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;

  struct SHTCTL *shtctl;
  struct SHEET *sht_back, *sht_mouse;
  unsigned char *buf_back, buf_mouse[256];
  
  init_gdtidt ();
  init_pic ();
  io_sti ();

  fifo8_init(&keyfifo, 32, keybuf);
  fifo8_init(&mousefifo, 32, mousebuf);
  
  io_out8(PIC0_IMR, 0xf9); /* PIC1とキーボードを許可(11111001) */
  io_out8(PIC1_IMR, 0xef); /* マウスを許可(11101111) */
  init_keyboad ();
  enable_mouse (&mdec);
  
  memtotal = memtest(0x00400000, 0xbfffffff);
  memman_init (memman);
  memman_free (memman, 0x00001000, 0x009e000);   /* 0x00001000 - 0x0009efff */
  memman_free (memman, 0x00400000, memtotal - 0x00400000);

  init_pallete();
  shtctl = shtctl_init (memman, binfo->vram, binfo->scrnx, binfo->scrny);
  sht_back  = sheet_alloc(shtctl);
  sht_mouse = sheet_alloc(shtctl);
  buf_back = (unsigned char *)memman_alloc_4k (memman, binfo->scrnx * binfo->scrny);
  sheet_setbuf (sht_back, buf_back, binfo->scrnx, binfo->scrny, -1);
  sheet_setbuf (sht_mouse, buf_mouse, 16, 16, 99);
  init_screen (buf_back, xsize, ysize);
  init_mouse_cursor8 (buf_mouse, 99);

  sprintf (s, "(%d, %d)", mx, my);
  putfonts8_asc (buf_back, binfo->scrnx, 0, 0, COL8_FFFFFF, s);
  sprintf (s, "Memory %dMB, free : %dKB", 
           memtotal / (1024 * 1024), memman_total(memman) / 1024);
  putfonts8_asc(buf_back, binfo->scrnx, 0, 32, COL8_FFFFFF, s);

  sheet_slide (shtctl, sht_back, 0, 0);
  sheet_slide (shtctl, sht_mouse, mx, my);
  sheet_updown (shtctl, sht_back, 0);
  sheet_updown (shtctl, sht_mouse, 1);

  sheet_refresh (shtctl);
  
  for (;;) {
	io_cli();
	if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo) == 0) {
	  io_stihlt();
	} else {
	  if (fifo8_status(&keyfifo) != 0) {
		i = fifo8_get(&keyfifo);
		io_sti();
		sprintf (s, "%x", i);
		boxfill8(buf_back, binfo->scrnx, COL8_008484, 0, 48, 15, 48+31);
		putfonts8_asc(buf_back, binfo->scrnx, 0, 48, COL8_FFFFFF, s);
        sheet_refresh (shtctl);
	  } else if (fifo8_status(&mousefifo) != 0) {
		i = fifo8_get(&mousefifo);
		io_sti();
        if (mouse_decode(&mdec, i) != 0) {
          sprintf (s, "[lcr %d %d]", mdec.x, mdec.y);
          if ((mdec.btn & 0x01) != 0) { s[1] = 'L'; }
          if ((mdec.btn & 0x02) != 0) { s[3] = 'R'; }
          if ((mdec.btn & 0x04) != 0) { s[2] = 'C'; }
          boxfill8(buf_back, binfo->scrnx, COL8_008484, 32, 16, 32 + 15 * 16 -1, 31);
          putfonts8_asc(buf_back, binfo->scrnx, 32, 16, COL8_FFFFFF, s);
          
          mx += mdec.x;
          my += mdec.y;
          if (mx < 0) { mx = 0; } if (mx > binfo->scrnx - 16) { mx = binfo->scrnx - 16; }
          if (my < 0) { my = 0; } if (my > binfo->scrny - 16) { my = binfo->scrny - 16; }

          sprintf(s, "(%d, %d)", mx, my);
          boxfill8(buf_back, binfo->scrnx, COL8_008484, 0, 0, 79, 15);
          putfonts8_asc(buf_back, binfo->scrnx, 0, 0, COL8_FFFFFF, s);
          sheet_slide (shtctl, sht_mouse, mx, my);
        }
	  }
	}
  }
}


