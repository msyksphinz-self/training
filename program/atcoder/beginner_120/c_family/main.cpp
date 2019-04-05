#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

using namespace std;

int main ()
{
  int count;
  int yen[100];
  cin >> count;
  for (int i = 0; i < count; i++) {
    cin >> yen[i];
  }
  int max = yen[0];
  int sec = 0;
  for (int i = 1; i < count; i++) {
    if (yen[i] > max) {
      sec = max;
      max = yen[i];
    } else if (yen[i] > sec && yen[i] < max){
      sec = yen[i];
    }
  }

  printf("%d\n", sec);
}
