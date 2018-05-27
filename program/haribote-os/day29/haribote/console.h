#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "sheets.h"

struct CONSOLE {
  struct SHEET *sht;
  int cur_x, cur_y, cur_c;
  struct TIMER *timer;
};

void console_task (struct SHEET *sheet, int memtotal);
void cons_newline (struct CONSOLE *cons);

void cmd_start(struct CONSOLE *cons, char *cmdline, int memtotal);
void cons_runcmd (char *cmdline, struct CONSOLE *cons, int *fat, int memtotal);
void cmd_mem (struct CONSOLE *cons, int memtotal);
void cmd_cls (struct CONSOLE *cons);
void cmd_dir (struct CONSOLE *cons);
void cmd_type (struct CONSOLE *cons, int *fat, char *cmdline);
void cmd_hlt (struct CONSOLE *cons, int *fat);
void cmd_exit(struct CONSOLE *cons, int *fat);
void cons_putchar (struct CONSOLE *cons, int chr, char move);
void cons_putstr0 (struct CONSOLE *cons, char *s);
void cons_putstr1 (struct CONSOLE *cons, char *s, int l);
void cmd_ncst(struct CONSOLE *cons, char *cmdline, int memtotal);
void cmd_langmode (struct CONSOLE *cons, char *cmdline);

int cmd_app (struct CONSOLE *cons, int *fat, char *cmdline);

void hrb_api_linewin (struct SHEET *sht, int x0, int y0, int x1, int y1, int col);

#endif // __CONSOLE_H__
