#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;

int main ()
{
  int H, W;
  cin >> H >> W;
  char s[50][50];

  for (int i = 0; i < H; i++) {
    for (int j = 0; j < W; j++) {
      cin >> s[i][j];
    }
  }

  for (int i = 0; i < H; i++) {
    for (int j = 0; j < W; j++) {
      if (s[i][j] == '#') {
        if ((j > 0 && s[i][j-1] == '#') ||
            (i > 0 && s[i-1][j] == '#') ||
            (j < W-1 && s[i][j+1] == '#') ||
            (i < H-1 && s[i+1][j] == '#')) {
        } else {
          printf ("No\n");
          return 0;
        }
      }
    }
  }
  printf("Yes\n");

}
