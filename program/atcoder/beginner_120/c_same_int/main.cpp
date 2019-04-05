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
  int a, b, c;
  cin >> a >> b >> c;

  int d;
  if (a >= b && a >= c) {
    d = (a - b) + (a - c);
  } else if (b >= a && b >= c) {
    d = (b - a) + (b - c);
  } else if (c >= a && c >= b) {
    d = (c - a) + (c - b);
  }
  if (d % 2 == 0) {
    printf ("%d\n", d / 2);
  } else {
    printf ("%d\n", d / 2 + 2);
  }
}
