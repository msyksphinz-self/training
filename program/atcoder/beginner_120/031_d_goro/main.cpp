#include <math.h>
#include <limits.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

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

vector<pair<char *, char *>> list;


int dfs (int d_curr, int s_curr, int try_n, vector<char *> sep_str)
{
  if (d_curr == strlen(list[try_n].first) - 1) {
    char *tmp_str = new char[4];
    strcpy(tmp_str, &list[try_n].second[s_curr]);
    sep_str.push_back(tmp_str);

    for(auto &str_i : sep_str) {
      printf("%s ", str_i);
    }
    printf("\n");

    sep_str.pop_back();

    return 0;
  }

  repj(s_pos, s_curr + 1, strlen(list[try_n].second)) {
    char *tmp_str = new char[28];
    strncpy(tmp_str, &list[try_n].second[s_curr], (s_pos - s_curr));
    // printf(" %s len = %d\n", tmp_str, s_pos - s_curr);
    sep_str.push_back(tmp_str);
    dfs(d_curr + 1, s_pos, try_n, sep_str);
    sep_str.pop_back();
  }
  return 0;
}


int main() {
  int K, N;
  cin >> K >> N;

  rep(i, N) {
    char *str = new char[28];
    char *k_str = new char[4];
    cin >> k_str >> str;
    list.push_back(make_pair(k_str, str));
  }

  sort(list.begin(), list.end());

  rep(i, N) {
    printf("%s %s\n", list[i].first, list[i].second);
  }

  vector<char *> sep_list;
  dfs (0, 0, 0, sep_list);


  return 0;
}
