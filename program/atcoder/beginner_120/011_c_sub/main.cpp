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
  int N, NG[3];
  cin >> N;
  rep(i, 3) {
    cin >> NG[i];
  }
  int dp[301];
  rep(i, 301) {
    dp[i] = 10000;
  }

  dp[N] = 0;
  rrep(i, N) {
    bool not_check = false;
    rep(step, 3) {
      if (NG[step] == i) {
        not_check = true;
        break;
      }
    }
    if (not_check) continue;

    rep(step, 3) {
      dp[i-step] = min(dp[i]+1, dp[i-step]);
    }
  }

  if (dp[0] > 100) {
    printf("NO\n");
  } else {
    printf("YES\n");
  }


  return 0;
}
