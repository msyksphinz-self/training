#include <iostream>
#include <vector>
#include <stdio.h>
#include <cmath>

using namespace std;

int main ()
{
  int N;
  cin >> N;
  int n[100001] = {0};
  for (int i = 0; i < N; i++) {
    int in;
    cin >> in;
    if ((N % 2) == 0 && (in % 2) == 0) {
      printf("0\n");
      return 0;
    } else if ((N % 2) == 1 && (in % 2) == 1) {
      printf("0\n");
      return 0;
    }
    n[in]++;
    if (in == 0 && n[0] == 2) {
      printf("0\n");
      return 0;
    }
    if (n[in] == 3) {
      printf("0\n");
      return 0;
    }
  }

  long ret = 1;
  for(int i = 0; i < (N / 2); i++) {
    ret = ret * 2;
    if (ret > pow(10, 9) + 7) {
      ret = ret - (pow(10, 9) + 7);
    }
  }
  printf("%ld\n", ret);

  return 0;
}
