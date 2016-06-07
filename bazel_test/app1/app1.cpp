#include <stdio.h>
#include "app2/app2.hpp"
#include "app3/app3.hpp"

int main ()
{
  printf ("App2 calling () ... %d\n", app2(1, 2));
  printf ("HOGE is = %d\n", HOGE);

  return 0;
}
