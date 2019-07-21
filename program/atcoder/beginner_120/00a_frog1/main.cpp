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

int main() {
  int N;
  int h[100001];
  int dp[100001];
  cin >> N;
  rep(i, N) {
    cin >> h[i];
  }
  rep(i, N) {
    dp[i] = INT_MAX;
  }

  dp[0] = 0;
  rep(i, N) {
    repj(s, 1, 3) {
      dp[i+s] = min(dp[i+s], dp[i] + abs(h[i] - h[i+s]));
    }
  }
  printf("%d\n", dp[N-1]);

  return 0;
}
