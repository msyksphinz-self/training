#include <bits/stdc++.h>

#define ipair pair<int,int>
#define rep(I,N) for(int I=0; I<N; I++)
#define repj(i,j,n) for(int i=j;i<n;i++)

#define PB(a,i) (a).push_back(i)
#define ALL(a) (a).begin(),(a).end()

#define OK(i) cout<<"OK "<<i<<endl
#define Yes cout<<"Yes"<<endl
#define YES cout<<"YES"<<endl
#define No cout<<"No"<<endl
#define NO cout<<"NO"<<endl

using namespace std;

typedef long long int lli;
typedef long long ll;
typedef vector<int> vi;
typedef vector< vector<int> > Matrix;

int main() {

  int N, M;
  cin >> N >> M;
  int rel[10][10] = {{0}};
  rep(i, M) {
    int j, k;
    cin >> j >> k;
    j--;
    k--;
    rel[j][k] = rel[k][j] = 1;
  }

  rep(i, N) {  // from-node
    int k = 0;
    rep(j, N) {
      if (rel[i][j] == 1) {
        rep (x, N) {
          if (rel[j][x] == 1 && rel[i][x] == 0 && x != i) {
            rel[i][x] = 2;
            k++;
          }
        }
      }
    }
    cout << k << '\n';
  }

  return 0;

}
