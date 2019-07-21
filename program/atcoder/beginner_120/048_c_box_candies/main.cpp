#include <math.h>
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
  int N, x;
  int a[100002];
  cin >> N >> x;

  a[0] = 0;
  repj(i, 1, N+1) {
    cin >> a[i] ;
  }

  long ans = 0;
  repj(i, 1, N+1) {
    int sub = 0;
    if (a[i] + a[i-1] > x) {
      sub = min(a[i], a[i] + a[i-1] - x);
    }
    ans += sub;
    a[i] -= sub;
  }

  printf("%ld\n", ans);
  return 0;
}
