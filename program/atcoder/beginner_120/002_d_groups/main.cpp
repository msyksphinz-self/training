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
  int N, M;
  cin >> N >> M;

  int rel[12][12];

  rep (i, N) {
    rep(j, N) {
      rel[i][j] = false;
    }
  }

  rep (i, M) {
    int p1, p2;
    cin >> p1 >> p2;
    p1--; p2--;
    rel[p1][p2] = rel[p2][p1] = true;
  }

  int ans = 0;
  repj (S, 1, 1 << N) {
    int count = bitCount(S);
    if (ans > count) { continue; }

    bool ok = true;
    rep(i, N) {
      rep(j, i) {
        if(((S >> i) & (S >> j) & 1) && !rel[i][j]){
          ok = false;
        }
      }
    }
    if (ok) { ans = count; }
  }

  printf("%d\n", ans);

  return 0;
}
