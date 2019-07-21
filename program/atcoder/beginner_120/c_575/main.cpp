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


bool is_753 (long n)
{
  bool val[10] = {false};
  while (n > 0) {
    int tmp = n % 10;
    // printf("n=%d, tmp = %d\n", n);
    if (!(tmp == 7 ||
          tmp == 5 ||
          tmp == 3)) {
      return false;
    }
    val[tmp] = true;
    n /= 10;
  }
  return (val[3] && val[5] && val[7]);
}


long dfs (long n, long N)
{
  long ans = 0;
  if (n > N) {
    return 0;
  }
  if (is_753(n)) { ans += 1; }
  ans += dfs(n * 10 + 3, N);
  ans += dfs(n * 10 + 5, N);
  ans += dfs(n * 10 + 7, N);

  return ans;
}


int main ()
{
  long N;
  cin >> N;

  printf ("%ld\n", dfs (0, N));

  return 0;
}
