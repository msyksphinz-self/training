#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__

void putfonts8_asc (char *vram, int xsize, int x, int y, char c, unsigned char *s);
void putfonts8 (char *vram, int xsize, int x, int y, char c, char *font);
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void init_screen(char *vram, int x, int y);
void putfonts8_asc (char *vram, int xsize, int x, int y, char c, unsigned char *s);
void init_mouse_cursor8 (char *mouse, char bc);
void putblock8_8(char *vram, int vxsize, int pxsize, int pysize, int px0, int py, char *buf, int bxsize);
void make_window8(unsigned char *buf, int xsize, int ysize, char *title, char act);
void make_textbox8 (struct SHEET *sht, int x0, int y0, int sx, int sy, int c);

#endif // __GRAPHIC_H__

