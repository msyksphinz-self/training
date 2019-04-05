#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <functional>
#include <math.h>

using namespace std;

typedef long long ll;

ll chk(ll a[], int N, bool t)
{
  ll total = 0;
  ll ops = 0;
  for (int i = 0; i < N; i++) {
    total += a[i];
    if (t == true && (total < 1)) {
      ops += (1-total);
      total = 1;
    } else if (t == false && (total > -1)) {
      ops += (total+1);
      total = -1;
    }
    t = !t;
  }
  return ops;
}

int main ()
{
  ll N;
  cin >> N;
  ll a[1000001];

  for (ll i = 0; i < N; i++) {
    cin >> a[i];
  }

  printf("%ld\n", min(chk(a, N, true), chk(a, N, false)));

}
