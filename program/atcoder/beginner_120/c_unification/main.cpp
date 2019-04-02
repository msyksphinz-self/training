#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;

int main ()
{
  string in;
  cin >> in;

  int b1 = 0, b0 = 0;
  for (char ch : in) {
    if (ch == '0') b0 ++;
    if (ch == '1') b1 ++;
  }
  printf("%d\n", 2 * min(b0, b1));

}
