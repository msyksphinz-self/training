#include <iostream>
#include <vector>
#include <stdio.h>
#include <cmath>

using namespace std;

int main ()
{
  int N;
  cin >> N;

  int a[101], b[101];
  for (int i = 0; i < N; i++) {
    cin >> a[i];
  }
  for (int i = 0; i < N; i++) {
    cin >> b[i];
  }

  int maxi = 0;
  for (int i = 0; i < N; i++) {
    int total = 0;
    for (int s = 0; s < N; s++) {
      if (s <= i) {
        total += a[s];
      }
      if (s >= i) {
        total += b[s];
      }
    }
    maxi = max(total, maxi);
  }

  printf("%d\n", maxi);

  return 0;
}
