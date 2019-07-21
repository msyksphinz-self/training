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

int graph[10][10];

int dfs(int v, int N, bool visited[10])
{
  bool is_all_visited = true;
  rep(i, N) {
    if (visited[i] == false) {
      is_all_visited = false;
      break;
    }
  }

  if (is_all_visited) return 1;

  int ret = 0;
  rep(i, N) {
    if (graph[v][i] && visited[i] == false) {
      visited[i] = true;
      ret += dfs(i, N, visited);
      visited[i] = false;
    }
  }
  return ret;
}

int main() {
  int N, M;
  cin >> N >> M;

  rep(i, 10) {
    rep(j, 10) {
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

  bool visited[10] = {false};
  visited[0] = true;

  cout << dfs(0, N, visited) << '\n';

  return 0;

}
