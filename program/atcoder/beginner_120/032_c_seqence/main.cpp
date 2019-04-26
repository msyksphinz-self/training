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

  long n[100000];

  rep(i, N) {
    cin >> n[i];
  }

  rep(i, N) {
    if (n[i] == 0) {
      printf("%d\n", N);
      return 0;
    }
  }

  int res = 0;

  rep(left, N) {
    long mult = 1;
    int right = left;

    while (right < N && mult * n[right] <= K) {
      mult *= n[right];
      ++right;
    }

    res = max(right - left, res);
  }

  printf("%d\n", res);
  return 0;
}
