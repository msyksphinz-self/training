#include <iostream>
#include <vector>
#include <queue>
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


queue<pair<int, int>> fifo;
int NX, NY;
int GX, GY;
int SX, SY;

int c[50][50];

int bfs()
{
  if (fifo.empty()) {
    return 0;
  }

  pair<int, int> tx = fifo.front();
  fifo.pop();
  int x = tx.first;
  int y = tx.second;

  if (y == GY && x == GX) {
    return c[y][x] - '0';
  }
  short base = c[y][x];
  if (y-1 >= 0 && x >= 0 && c[y-1][x] == '.') {
    fifo.push(pair<int, int>(x, y-1));
    c[y-1][x] = base+1;
  }
  if (x-1 >= 0 && c[y][x-1] == '.') {
    fifo.push(pair<int, int>(x-1, y));
    c[y][x-1] = base+1;
  }
  if (x+1 < NX && c[y][x+1] == '.') {
    fifo.push(pair<int, int>(x+1, y));
    c[y][x+1] = base+1;
  }
  if (y+1 < NY && c[y+1][x] == '.') {
    fifo.push(pair<int, int>(x, y+1));
    c[y+1][x] = base+1;
  }

  return bfs();
}

int main ()
{
  cin >> NY >> NX;
  cin >> SY >> SX;
  cin >> GY >> GX;
  SY--; SX--;
  GY--; GX--;

  rep(j, NY) {
    rep(i, NX) {
      char tmp;
      cin >> tmp;
      c[j][i] = tmp;
    }
  }

  fifo.push(pair<int, int>(SX, SY));
  c[SY][SX] = '0';

  printf("%d\n", bfs ());

  return 0;
}
