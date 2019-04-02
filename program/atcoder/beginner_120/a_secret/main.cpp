#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

using namespace std;

int main ()
{
  string str;
  cin >> str;
  char head = str[0];

  if (head == str[1] &&
      head == str[2] &&
      head == str[3]) {
    printf ("SAME\n");
  } else {
    printf ("DIFFERENT\n");
  }
  return 0;
}
