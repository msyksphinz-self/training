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
  int count, len;
  vector<string>r;
  cin >> count;
  cin >> len;
  for (int i = 0; i < count; i++) {
    string in;
    cin >> in;
    r.push_back(in);
  }
  std::sort(r.begin(), r.end());
  for (int i = 0; i < count; i++) {
    cout << r[i];
  }
}
