#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

using namespace std;

int main ()
{
  string in;
  cin >> in;

  vector<char> out;

  for(char ch : in) {
    if (ch == '0') out.push_back(ch);
    if (ch == '1') out.push_back(ch);
    if (ch == 'B') {
      if (!out.empty()) out.pop_back();
    }
  }

  for(auto ch : out) {
    printf("%c", ch);
  }

}
