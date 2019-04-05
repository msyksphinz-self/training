#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <functional>
#include <math.h>

using namespace std;

typedef long long ll;

int main ()
{
  ll N;
  cin >> N;
  ll a[100001+2];

  int p = 0;
  int d = 0;
  a[0] = 0;
  for (int i = 1; i <= N; i++) {
    cin >> a[i];
    d += abs(a[i] - p);
    p = a[i];
  }
  a[N+1] = 0;
  d += abs(p);
  printf("d=%d\n", d);

  for (int s = 1; s <= N; s++) {
    if ((a[s] < a[s-1]) && (a[s] < a[s+1]) ||
        (a[s] > a[s-1]) && (a[s] > a[s+1])) {
      printf("%d\n", d - 2 * min(abs(a[s] - a[s-1]), abs(a[s] - a[s+1])));
    } else {
      printf("%d\n", d);
    }
  }
}
