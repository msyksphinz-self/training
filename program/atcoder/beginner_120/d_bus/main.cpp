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

int INF = 1000000;

int get_max_stop(int st, const int N, const int path[300][300])
{
  int stop[300];
  bool reach[300];

  rep(i, N) {
    stop[i] = INF;
    reach[i] = false;
  }

  stop[st] = 0;

  rep(count, N) {
    rep(i, N) {
      if (path[st][i] != 0) {
        stop[i] = min(stop[i], stop[st] + path[st][i]);
      }
    }
    reach[st] = true;
    int max_stop = INF;
    int max_bus = 0;
    rep(i, N) {
      if (!reach[i] && stop[i] < max_stop) {
        max_stop = stop[i];
        max_bus = i;
      }
    }
    st = max_bus;
  }

  int ret = 0;
  rep(i, N) {
    ret = max(ret, stop[i]);
  }
  return ret;
}


int main ()
{
  int N, M;
  cin >> N >> M;
  int path[300][300];
  rep(i, N) {
    rep(j, N) {
      path[i][j] = 0;
    }
  }

  rep(i, M) {
    int a, b, t;
    cin >> a >> b >> t;
    a--; b--;
    path[a][b] = t;
    path[b][a] = t;
  }

  int min_time = INF;
  rep(st, N) {
    int bus_time = get_max_stop(st, N, path);
    min_time = min(bus_time, min_time);
  }
  printf("%d\n", min_time);

  return 0;
}
