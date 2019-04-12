#include <iostream>
#include <vector>
#include <stdio.h>
#include <cmath>

#include <bits/stdc++.h>
#include <vector>

using namespace std;

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

typedef long long int lli;
typedef long long ll;
typedef vector<int> vi;
typedef vector< vector<int> > Matrix;

int main ()
{
  int N;
  cin >> N;
  int h[100000];
  int c[100000] = {0};

  rep(i, N) {
    cin >> h[i];
  }
  c[0] = 0;
  c[1] = abs(h[1] - h[0]);

  repj(i, 2, N) {
    c[i] = min(c[i-2]+abs(h[i]-h[i-2]),
               c[i-1]+abs(h[i]-h[i-1]));
  }

  printf("%ld\n", c[N-1]);

  return 0;
}
