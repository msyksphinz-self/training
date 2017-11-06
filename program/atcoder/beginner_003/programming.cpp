#include <iostream>
#include <algorithm>
#include <iomanip>

int main (void)
{
    int N, K;
    std::cin >> N >> K;
    int *R = new int[N];

    for (int i = 0; i < N; i++) {
        std::cin >> R[i];
    }
    std::sort (R, R+N);

    double rate = 0;
    for (int i = N - K; i < N; i++) {
        rate = (rate + static_cast<double>(R[i])) / 2.0;
    }

    std::cout << std::fixed << std::setprecision(6) << rate << '\n';
}
