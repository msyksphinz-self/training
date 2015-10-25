#include <iostream>
#include <stdint.h>

int main (void)
{
    int32_t N, B, C;
    std::cin >> N >> B >> C;

    int32_t *A = new int32_t[N];

    for (int i = 0; i < N; i++) {
        std::cin >> A[i];
    }

    int32_t happy = 0;
    int i;
    for (i = N-1; i>= 0; i--) {
        if (B <= C) {
            C -= B;
            happy = happy + A[i]*B;
        } else {
            break;
        }
    }
    happy = happy + A[i]*C;

    std::cout << happy << '\n';

    return 0;
}
