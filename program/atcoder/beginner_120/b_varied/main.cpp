#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

using namespace std;

int main ()
{
  string str;
  cin >> str;
  int count[26] = {0};

  for(auto ch : str) {
    if (count[ch - 'a'] == 1) {
      printf("no\n");
      return 0;
    }
    count[ch - 'a'] ++;
  }
  printf ("yes\n");
}
