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
  char s[11];
  cin >> s;
  int N = strlen(s);
  long res = 0;

  rep(i, 1 << (N-1)) {
    long sm = 0;
    long a = s[0] - '0';
    rep(j, N-1) {
      if (i & (1 << j)) {
        sm += a;
        a = 0;
      }
      a = a * 10 + (s[j+1] - '0');
    }
    sm += a;
    res += sm;
  }

  printf("%ld\n", res);
  return 0;
}
