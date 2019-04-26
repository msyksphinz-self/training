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

// int keiro (int W, int H)
// {
//   if (W <= 1 && H <= 1) {
//     return 2;
//   } else if (W <= 1) {
//     return keiro (W, H-1) + 1;
//   } else if (H <= 1) {
//     return keiro (W-1, H) + 1;
//   } else {
//     return keiro (W, H-1) + keiro (W-1, H);
//   }
// }
//
// int t[500000][500000];

int main ()
{
  int W, H;
  cin >> W >> H;
  W--; H--;

  rep(i, W) {
    t[1][i] = i+1;
    t[i][1] = i+1;
  }
  repj(i, 2, H + 1) {
    repj(j, 2, W + 1) {
      t[i][j] = t[i-1][j]+t[i][j-1];
    }
  }

  printf ("%d\n", t[H][W]);

  return 0;
}
