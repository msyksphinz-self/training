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

int bitCount(uint bits) {
  int cnt = 0;
  for(; bits != 0; bits>>=1) {
    cnt += bits & 1;
  }
  return cnt;
}


int main() {
  int N, M, P, Q, R;
  cin >> N >> M >> P >> Q >> R;

  int w_list[18*18], m_list[18*18], p_list[18*18];
  rep(i, R) {
    int x, y, z;
    cin >> x >> y >> z;
    x--; y--;
    w_list[i] = x;
    m_list[i] = y;
    p_list[i] = z;
  }
  int ans = 0;
  rep(w_bit, 1 << N) {
    if (bitCount(w_bit) > P) { continue; }
    rep(m_bit, 1 << M) {
      if (bitCount(m_bit) > Q) { continue; }
      int point = 0;
      // printf("women %x, men %x\n", w_bit, m_bit);
      rep(i, R) {
        // printf("choco %d ", i);
        if ((w_bit & (1 << w_list[i])) &&
            (m_bit & (1 << m_list[i]))) {
          // printf("match %d\n", p_list[i]);
          point += p_list[i];
        } else {
          // printf("dont match\n");
        }
      }
      ans = max(ans, point);
    }
  }

  printf("%d\n", ans);

  return 0;
}
