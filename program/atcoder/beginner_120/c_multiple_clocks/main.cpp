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

long long gcd(long long a,long long b)
{
  if(b==0)
    return a;
  return gcd(b,a%b);
}

long long lcm(long long a,long long b){
  long long g=gcd(a,b);
  return a / g * b;
}

int main ()
{
  int N;
  cin >> N;

  uint64_t t;
  cin >> t;
  uint64_t res = t;

  rep(i, N-1) {
    uint64_t t;
    cin >> t;
    res = lcm (res, t);
  }

  printf("%ld\n", res);

  return 0;
}
