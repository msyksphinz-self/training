#include "bootpack.h"

extern struct FIFO8 keyfifo;
extern struct FIFO8 mousefifo;

extern struct TIMERCTL timerctl;

#define EFLAGS_AC_BIT      0x00040000
#define CR0_CACHE_DISABLE  0x60000000

void putfonts8_asc_sht (struct SHEET *sht, int x, int y, int c, int b, char *s, int l)
{
  boxfill8(sht->buf, sht->bxsize, b, x, y, x+l*8-1, y+15);
  putfonts8_asc(sht->buf, sht->bxsize, x, y, c, s);
  sheet_refresh (sht, x, y, x+l*8, y+16);
  return;
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
  unsigned int memtotal, count = 0;
  struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;

  struct SHTCTL *shtctl;
  struct SHEET *sht_back, *sht_mouse, *sht_win;
  unsigned char *buf_back, buf_mouse[256], *buf_win,
	timerbuf[8];
  struct TIMER *timer1, *timer2, *timer3;
  struct FIFO8 timerfifo;
  
  init_gdtidt ();
  init_pic ();
  io_sti ();

  fifo8_init(&keyfifo,   32, keybuf   );
  fifo8_init(&mousefifo, 32, mousebuf );

  fifo8_init(&timerfifo, 8,  timerbuf);

  init_pit();
  io_out8(PIC0_IMR, 0xf8); /* Allow PIT and Keyboard (11111000) */
  io_out8(PIC1_IMR, 0xef); /* Allow Mouse (11101111) */
  init_keyboad ();
  enable_mouse (&mdec);

  timer1 = timer_alloc();
  timer_init(timer1, &timerfifo, 10);
  timer_settime(timer1, 1000);

  timer2 = timer_alloc();
  timer_init(timer2, &timerfifo, 3);
  timer_settime(timer2, 300);

  timer3 = timer_alloc();
  timer_init(timer3, &timerfifo, 1);
  timer_settime(timer3, 50);
  
  memtotal = memtest(0x00400000, 0xbfffffff);
  memman_init (memman);
  memman_free (memman, 0x00001000, 0x009e000);   /* 0x00001000 - 0x0009efff */
  memman_free (memman, 0x00400000, memtotal - 0x00400000);

  init_pallete();
  shtctl = shtctl_init (memman, binfo->vram, binfo->scrnx, binfo->scrny);
  sht_back  = sheet_alloc(shtctl);
  sht_mouse = sheet_alloc(shtctl);
  sht_win   = sheet_alloc(shtctl);
  buf_back = (unsigned char *)memman_alloc_4k (memman, binfo->scrnx * binfo->scrny);
  buf_win  = (unsigned char *)memman_alloc_4k (memman, 160 * 52);
  sheet_setbuf (sht_back,  buf_back, binfo->scrnx, binfo->scrny, -1);
  sheet_setbuf (sht_mouse, buf_mouse, 16, 16, 99);
  sheet_setbuf (sht_win,   buf_win,   160, 52, -1);
  init_screen (buf_back, xsize, ysize);
  init_mouse_cursor8 (buf_mouse, 99);
  make_window8(buf_win, 160, 52, "counter");
  
  sprintf (s, "(%d, %d)", mx, my);
  putfonts8_asc (buf_back, binfo->scrnx, 0, 0, COL8_FFFFFF, s);
  sprintf (s, "Memory %dMB, free : %dKB", 
           memtotal / (1024 * 1024), memman_total(memman) / 1024);
  putfonts8_asc(buf_back, binfo->scrnx, 0, 32, COL8_FFFFFF, s);

  sheet_slide (sht_back,  0, 0);
  sheet_slide (sht_mouse, mx, my);
  sheet_slide (sht_win,   80, 72);
  sheet_updown (sht_back,  0);
  sheet_updown (sht_win,   1);
  sheet_updown (sht_mouse, 2);

  sheet_refresh (sht_back, 0, 0, binfo->scrnx, 48);
  
  for (;;) {
	count ++;
	
	io_cli();
	if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo) + fifo8_status(&timerfifo) == 0) {
	  io_sti();
	} else {
	  if (fifo8_status(&keyfifo) != 0) {
		i = fifo8_get(&keyfifo);
		io_sti();
		sprintf (s, "%x", i);
        putfonts8_asc_sht (sht_back, 0, 16, COL8_FFFFFF, COL8_008484, s, 2);
	  } else if (fifo8_status(&mousefifo) != 0) {
		i = fifo8_get(&mousefifo);
		io_sti();
        if (mouse_decode(&mdec, i) != 0) {
          sprintf (s, "[lcr %d %d]", mdec.x, mdec.y);
          if ((mdec.btn & 0x01) != 0) { s[1] = 'L'; }
          if ((mdec.btn & 0x02) != 0) { s[3] = 'R'; }
          if ((mdec.btn & 0x04) != 0) { s[2] = 'C'; }
          putfonts8_asc_sht (sht_back, 32, 16, COL8_FFFFFF, COL8_008484, s, 15);
          
          mx += mdec.x;
          my += mdec.y;
          if (mx < 0) { mx = 0; } if (mx > binfo->scrnx - 1) { mx = binfo->scrnx - 1; }
          if (my < 0) { my = 0; } if (my > binfo->scrny - 1) { my = binfo->scrny - 1; }

          sprintf(s, "(%d, %d)", mx, my);
          putfonts8_asc_sht (sht_back, 0, 0, COL8_FFFFFF, COL8_008484, s, 10);

          sheet_slide (sht_mouse, mx, my);
        }
	  } else if (fifo8_status(&timerfifo) != 0) {
        i = fifo8_get (&timerfifo);
        io_sti();
        if (i == 10) {
          putfonts8_asc_sht (sht_back, 0, 64, COL8_FFFFFF, COL8_008484, "10[sec]", 7);
          sprintf(s, "%d", count);
          putfonts8_asc_sht (sht_win, 40, 28, COL8_000000, COL8_C6C6C6, s, 10);
        } else if (i == 3) {
          putfonts8_asc_sht (sht_back, 0, 80, COL8_FFFFFF, COL8_008484, "3[sec]", 6);
          count = 0;
        } else {
          if (i != 0) {
            timer_init(timer3, &timerfifo, 0);
            boxfill8(buf_back, binfo->scrnx, COL8_FFFFFF, 8, 96, 15, 111);
          } else {
            timer_init(timer3, &timerfifo, 1);
            boxfill8(buf_back, binfo->scrnx, COL8_008484, 8, 96, 15, 111);
          }
          timer_settime(timer3, 50);
          sheet_refresh(sht_back, 8, 96, 16, 112);
        }
      }
	}
  }
}


