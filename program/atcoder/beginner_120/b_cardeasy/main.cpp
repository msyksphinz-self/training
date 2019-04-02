#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

using namespace std;

int main ()
{
  string card[3];

  cin >> card[0];
  cin >> card[1];
  cin >> card[2];

  auto round = 0;

  while(!card[round].empty()) {
    switch(round) {
      case 0 : round = card[0][0] - 'a'; card[0].erase(0,1); break;
      case 1 : round = card[1][0] - 'a'; card[1].erase(0,1); break;
      case 2 : round = card[2][0] - 'a'; card[2].erase(0,1); break;
    }
  }

  printf("%c\n", round + 'A');
}
