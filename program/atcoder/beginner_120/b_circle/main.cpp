#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <functional>
#include <math.h>

using namespace std;

int main ()
{
  int count;
  vector<int>r;
  cin >> count;
  for (int i = 0; i < count; i++) {
    int in;
    cin >> in;
    r.push_back(in);
  }
  std::sort(r.begin(), r.end(), std::greater<int>());
  double total = 0;
  int i = 0;
  for (int i = 0; i < count; i++) {
    if ((i % 2) == 0) {
      total += r.front() * r.front() * M_PI;
    } else {
      total -= r.front() * r.front() * M_PI;
    }
    r.erase(r.begin());
    i++;
  }

  printf("%.15lf\n", total);
}
