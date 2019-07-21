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


vector<pair<pair<int, int>, int>> v_bridge;
int p_city[10000];
int p_year[10000];

int dfs(int v, int limit_year, bool visited[100000])
{
  int ret = 1;
  // printf("visitd city %d\n", v);
  for(auto it = v_bridge.begin(); it != v_bridge.end(); it++) {
    pair <int, int> bridge = (*it).first;
    int a = bridge.first;
    int b = bridge.second;
    int y = (*it).second;
    if (a == v && y > limit_year && visited[b] == false) {
      visited[b] = true;
      ret += dfs(b, limit_year, visited);
      // visited[b] = false;
    }
  }
  return ret;
}


int main ()
{
  int N, M;
  cin >> N >> M;
  rep(i, M) {
    int a, b, y;
    cin >> a >> b >> y;
    a--;b--;
    pair<int, int> bridge = pair<int, int>(a, b);
    v_bridge.push_back(pair<pair<int, int>, int>(bridge, y));
    pair<int, int> bridge2 = pair<int, int>(b, a);
    v_bridge.push_back(pair<pair<int, int>, int>(bridge2, y));
  }

  int Q;
  cin >> Q;
  rep(i, Q) {
    cin >> p_city[i] >> p_year[i];
    p_city[i]--;
  }

  // for(auto it = v_bridge.begin(); it != v_bridge.end(); it++) {
  //   pair <int, int> bridge = (*it).first;
  //   int y = (*it).second;
  //   cout << bridge.first << ", " << bridge.second << ", " << y << '\n';
  // }

  rep(i, Q) {
    bool visited[100000] = {false};
    visited[p_city[i]] = true;
    printf("%d\n", dfs(p_city[i], p_year[i], visited));
  }

  return 0;
}
