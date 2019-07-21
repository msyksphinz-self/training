#include <math.h>
#include <limits.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

#define ipair pair<int,int>
#define rep(I,N) for(int I=0; I<N; I++)
#define rrep(I,N) for(int I=N; I>=0; I--)
#define repj(i,j,n) for(int i=j;i<n;i++)

#define PB(a,i) (a).push_back(i)
#define ALL(a) (a).begin(),(a).end()

#define OK(i) cout<<"OK "<<i<<endl
#define Yes cout<<"Yes"<<endl
#define YES cout<<"YES"<<endl
#define No cout<<"No"<<endl
#define NO cout<<"NO"<<endl

int bitCount(uint bits) {
  int cnt = 0;
  for(; bits != 0; bits>>=1) {
    cnt += bits & 1;
  }
  return cnt;
}

int main() {
  int N;
  cin >> N;

  uint16_t F[100];
  int P[100][11];

  rep (i, N) {
    F[i] = 0;
    rep (j, 10) {
      bool tmp;
      cin >> tmp;
      F[i] |= tmp << j;
    }
  }
  rep (i, N) {
    rep (j, 11) {
      cin >> P[i][j];
    }
  }

  int res = INT_MIN;
  repj (i, 1, 1 << 10) {
    int total = 0;
    rep(j, N) {
      int i_and = F[j] & i;
      int cntone = bitCount(i_and);
      total += P[j][cntone];
    }
    res = max(res, total);
  }

  printf("%d\n", res);

  return 0;
}
