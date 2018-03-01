void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);

void HariMain(void)
{
  int i;
  char *p;

  init_pallete();
  p = (char *) 0xa0000;
  for (i = 0; i <= 0xffff; i++) {
    p[i] = i & 0x0f;
  }

  for (;;) {
    io_hlt();
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

  
