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
  int N;
  cin >> N;
  long a[10001];
  int res = 0;
  for (int i = 0; i < N; i++) {
    int tmp;
    cin >> tmp;
    int c = 0;
    while(tmp % 2 == 0) {
      tmp = tmp / 2;
      c++;
    }
    res += c;
  }
  printf("%d\n", res);
}
