#include "bootpack.h"

extern struct FIFO8 keyfifo;
extern struct FIFO8 mousefifo;

#define EFLAGS_AC_BIT      0x00040000
#define CR0_CACHE_DISABLE  0x60000000

unsigned int memtest_sub(unsigned int start, unsigned int end);

unsigned int memtest (unsigned int start, unsigned int end)
{
  char flg486 = 0;
  unsigned int eflg, cr0, i;

  /* check i386 or i486 */
  eflg = io_load_eflags();
  eflg |= EFLAGS_AC_BIT;
  io_store_eflags (eflg);
  eflg = io_load_eflags();
  if ((eflg & EFLAGS_AC_BIT) != 0) {
    flg486 = 1;
  }
  eflg &= ~EFLAGS_AC_BIT;
  io_store_eflags(eflg);

  if (flg486 != 0) {
    cr0 = load_cr0();
    cr0 |= CR0_CACHE_DISABLE;  /* diasble cache */
    store_cr0 (cr0);
  }

  i = memtest_sub(start, end);

  if (flg486 != 0) {
    cr0 = load_cr0();
    cr0 &= ~CR0_CACHE_DISABLE;  /* enable cache */
    store_cr0(cr0);
  }

  return i;
}


unsigned int memtest_sub(unsigned int start, unsigned int end)
{
  unsigned int i, old, pat0 = 0xaa55aa55, pat1 = 0x55aa55aa;
  volatile unsigned int *p;
  for (i = start; i <= end; i += 0x1000) {
    p = (volatile unsigned int *)i;
    old = *p;
    *p = pat0;
    *p ^= 0xffffffff;
    if (*p != pat1) {
    not_memory:
      *p = old;
      break;
    }
    *p ^= 0xffffffff;
    if (*p != pat0) {
      goto not_memory;
    }
    *p = old;
  }
  return i;
}


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

  init_gdtidt ();
  init_pic ();
  io_sti ();

  fifo8_init(&keyfifo, 32, keybuf);
  fifo8_init(&mousefifo, 32, mousebuf);
  
  init_pallete();
  init_screen (vram, xsize, ysize);

  // putfonts8_asc (binfo->vram, binfo->scrnx, 8, 8, COL8_FFFFFF, "ABC 123");
  // 
  // putfonts8_asc (binfo->vram, binfo->scrnx, 31, 31, COL8_000000, "Haribote OS.");
  // putfonts8_asc (binfo->vram, binfo->scrnx, 30, 30, COL8_FFFFFF, "Haribote OS.");

  // sprintf(msg, "scrnx = %d", binfo->scrnx);
  // putfonts8_asc (binfo->vram, binfo->scrnx, 30, 48, COL8_FFFFFF, msg);

  init_mouse_cursor8 (mcursor, COL8_008484);
  putblock8_8 (binfo->vram, binfo->scrnx, 16, 16, mx,my, mcursor, 16);

  io_out8(PIC0_IMR, 0xf9); /* PIC1とキーボードを許可(11111001) */
  io_out8(PIC1_IMR, 0xef); /* マウスを許可(11101111) */

  memtotal = memtest(0x00400000, 0xbfffffff);
  memman_init (memman);
  memman_free (memman, 0x00001000, 0x0009e00);   /* 0x00001000 - 0x00009eff */
  memman_free (memman, 0x00400000, memtotal - 0x00400000);

  sprintf (s, "Memory %dMB, free : %dKB", 
           memtotal / (1024 * 1024), memman_total(memman) / 1024);
  putfonts8_asc(binfo->vram, binfo->scrnx, 0, 32, COL8_FFFFFF, s);

  init_keyboad ();
  enable_mouse (&mdec);
  
  for (;;) {
	io_cli();
	if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo) == 0) {
	  io_stihlt();
	} else {
	  if (fifo8_status(&keyfifo) != 0) {
		i = fifo8_get(&keyfifo);
		io_sti();
		sprintf (s, "%x", i);
		boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 48, 15, 48+31);
		putfonts8_asc(binfo->vram, binfo->scrnx, 0, 48, COL8_FFFFFF, s);
	  } else if (fifo8_status(&mousefifo) != 0) {
		i = fifo8_get(&mousefifo);
		io_sti();
        if (mouse_decode(&mdec, i) != 0) {
          sprintf (s, "[lcr %d %d]", mdec.x, mdec.y);
          if ((mdec.btn & 0x01) != 0) { s[1] = 'L'; }
          if ((mdec.btn & 0x02) != 0) { s[3] = 'R'; }
          if ((mdec.btn & 0x04) != 0) { s[2] = 'C'; }
          boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 32, 16, 32 + 15 * 16 -1, 31);
          putfonts8_asc(binfo->vram, binfo->scrnx, 32, 16, COL8_FFFFFF, s);
          
          boxfill8(binfo->vram, binfo->scrnx, COL8_008484, mx, my, mx + 15, my + 15);

          mx += mdec.x;
          my += mdec.y;
          if (mx < 0) { mx = 0; } if (mx > binfo->scrnx - 16) { mx = binfo->scrnx - 16; }
          if (my < 0) { my = 0; } if (my > binfo->scrny - 16) { my = binfo->scrny - 16; }

          sprintf(s, "(%d, %d)", mx, my);
          boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 0, 79, 15);
          putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);
          putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
        }
	  }
	}
  }
}


