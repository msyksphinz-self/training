#include <bits/stdc++.h>
#include <vector>
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

int gcd (int a, int b)
{
  int tmp, r;
  if (a<b){
    tmp = a;
    a = b;
    b = tmp;
  }

  r = a % b;
  while(r!=0){
    a = b;
    b = r;
    r = a % b;
  }

  return b;
}

int main ()
{
  int N, X;
  cin >> N >> X;
  int *x = new int[N];
  rep(i, N) {
    cin >> x[i];
    x[i] = abs(X-x[i]);
  }

  int small_gcd = x[0];
  rep(i, N) {
    small_gcd = gcd(small_gcd, x[i]);
  }

  printf("%d\n", small_gcd);
}
