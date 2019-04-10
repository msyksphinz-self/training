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

int graph[50][50];

bool dfs(int v1, int v2, int N, bool visited[10])
{
  bool is_all_visited = true;
  if (v1 == v2) {
    return true;
  }
  rep(i, N) {
    if (visited[i] == false) {
      is_all_visited = false;
      break;
    }
  }

  if (is_all_visited) return false;

  bool ret = false;
  rep(i, N) {
    if (graph[v1][i] && visited[i] == false) {
      visited[i] = true;
      ret = dfs(i, v2, N, visited);
      if (ret) {
        return true;
      }
      visited[i] = false;
    }
  }
  return ret;
}

int main() {
  int N, M;
  cin >> N >> M;

  rep(i, 50) {
    rep(j, 50) {
      graph[i][j] = 0;
    }
  }

  rep(i, M) {
    int j, k;
    cin >> j >> k;
    j--;
    k--;
    graph[j][k] = graph[k][j] = 1;
  }

  int bridge = 0;
  rep (i, N) {
    int edge = 0;
    repj (j, i+1, N) {
      if (graph[i][j] ) {
        graph[i][j] = 0;

        bool visited[50] = {false};
        visited[i] = true;

        bool is_reach = dfs(i, j, N, visited);
        bridge += !is_reach;
        // printf("bridge from %d to %d = %d\n", i, j, is_reach);

        graph[i][j] = true;
        visited[i] = false;
      }
    }
  }
  printf("%d\n", bridge);

  return 0;

}
