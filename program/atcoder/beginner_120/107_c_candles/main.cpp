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
  int N, K;
  cin >> N >> K;

  int x[100000];

  rep (i, N) {
    cin >> x[i];
  }

  int ans = INT_MAX;
  rep (i, N) {
    int res = abs(x[i]);
    if (i+K <= N) { res += abs(x[i+K-1] - x[i]); }
    else if(i-K+1 >= 0) { res += abs(x[i-K+1] - x[i]); }
    else continue;

    // printf("%d\n", res);

    ans = min(res, ans);
  }

  printf("%d\n", ans);

  return 0;
}
