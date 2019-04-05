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
  int N, M;
  cin >> N;
  cin >> M;
  vector<int> yd[100000];
  int P[100000];
  int Y[100000];
  for (int i = 0; i < M; i++) {
    cin >> P[i] >> Y[i];
    yd[P[i]-1].push_back(Y[i]);
  }
  for (int p = 0; p < N; p++) {
    std::sort(yd[p].begin(), yd[p].end());
  }
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < yd[P[i]-1].size(); j++) {
      if (yd[P[i]-1][j] == Y[i]) {
        printf("%06d%06d\n", P[i], j+1);
      }
    }
  }
}
