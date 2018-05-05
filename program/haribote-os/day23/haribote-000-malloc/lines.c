#include "a_nask.h"

void HariMain (void)
{
  char *buf;
  int win, i;
  unsigned int x, y;

  api_initmalloc();
  buf = api_malloc (160 * 100);
  win = api_openwin (buf, 160, 100, -1, "lines");
  for (i = 0; i < 8; i++) {
	api_linewin (win + 1,  8, 26, 77, i * 9 + 26, i);
	api_linewin (win + 1, 88, 26, i * 9 + 88, 89, i);
  }
  api_refreshwin (win, 6, 26, 154, 90);
  api_end ();
}

