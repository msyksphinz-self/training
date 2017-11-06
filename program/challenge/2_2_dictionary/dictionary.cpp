#include <iostream>

#define N (6)

const char S[N+1] = "ACDBCB";

int main (void)
{
    char ans[N+1];
    int start = 0, end = N-1;
    int idx = 0;
    while (start <= end) {
        bool left = true;
        for (int i = 0; start+i <= end; i++) {
            if (S[start+i] < S[end-i]) {
                left = true;
                break;
            } else if (S[start+i] > S[end-i]) {
                left = false;
                break;
            }
        }
        if (left == true) {
            ans[idx++] = S[start];
            start++;
        } else {
            ans[idx++] = S[end];
            end--;
        }
    }
    ans[N] = '\0';

    std::cout << ans << '\n';
}
