#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

jmp_buf jmp_div;

void divide_test (int a, int b)
{
  if (b == 0) {
    longjmp(jmp_div, 1);
  }
  int div = a / b;
  printf ("divide_test (%d, %d) = %d\n", a, b, div);
  return;
}


int main()
{
  if (setjmp(jmp_div) == 0) {
    divide_test (223, 7);
    divide_test (571, 13);
    divide_test (311, 0);
  } else {
    fprintf (stderr, "divide_test failure\n");
    return EXIT_FAILURE;
  }

  return 0;
}
