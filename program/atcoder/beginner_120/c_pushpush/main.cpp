#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

using namespace std;

int main ()
{
  int count;
  cin >> count;
  long *in = new long[count];
  for (int i = 0; i < count; i++) {
    cin >> in[i] ;
  }

  int i = count - 1;
  for (; i >= 0; i -= 2) {
    printf("%ld ", in[i]);
  }
  if (i == -1) i = 0;
  else i = 1;
  for (;i < count; i+=2) {
    printf("%ld ", in[i]);
  }
  printf("\n");
}
