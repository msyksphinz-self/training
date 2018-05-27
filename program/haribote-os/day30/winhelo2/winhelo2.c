#include "apilib.h"
#include "sprintf.h"

char buf[150 * 50];
void HariMain (void)
{
  int win;
  win = api_openwin (buf, 150, 50, -1, "hello2");

  api_boxfilwin (win,  8, 36, 141, 43, 3  /* yellow */);
  api_putstrwin (win, 28, 28, 0 /* black */, 12, "hello, world");
  for (;;) {
	if (api_getkey(1) == 0x0a) {
	  break;
	}
  }
  
  api_end ();
}
