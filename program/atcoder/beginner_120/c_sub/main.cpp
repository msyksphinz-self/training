#include <iostream>
#include <vector>
#include <stdio.h>
#include <cmath>

#include <bits/stdc++.h>
#include <vector>

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

typedef long long int lli;
typedef long long ll;
typedef vector<int> vi;
typedef vector< vector<int> > Matrix;

int main ()
{
  int N;
  cin >> N;
  int dp[301];
  int NG[3];
  cin >> NG[0];
  cin >> NG[1];
  cin >> NG[2];

  rep(i, 300) {
    dp[i] = 10001;
  }

  dp[N] = 0;
  rrep(i, N) {
    bool not_check = false;
    rep(j, 3) {
      if (NG[j] == i) {
        not_check = true;
        break;
      }
    }
    if (not_check) {
      continue;
    }
    repj(j, 1, 4) {
      dp[i-j] = min(dp[i]+1, dp[i-j]);
    }
  }
  if (dp[0] > 100) {
    printf("NO\n");
  } else {
    printf("YES\n");
  }

  return 0;
}
