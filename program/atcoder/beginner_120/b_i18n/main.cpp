#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

using namespace std;

int main ()
{
  string in;

  cin >> in;

  printf("%c%d%c\n", in[0], in.size()-2, in[in.size()-1]);
}
