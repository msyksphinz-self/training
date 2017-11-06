#include <iostream>
#include <algorithm>

const int money[6] = {500, 100, 50, 10, 5, 1};

int solve_coin (int coin[], const int size, int total);

int main ()
{
    int coin[6] = {2, 0, 3, 1, 2, 3};

    int res = solve_coin (coin, 6, 620);

    std::cout << "Need to pay " << res << " coins.\n";
}


int solve_coin (int coin[], const int size, int total)
{
    int ans = 0;
    for (int head = 0; total > 0 && head < size; head++) {
        int res = std::min ((total / money[head]), coin[head]);
        total -= (res * money[head]);

        std::cout << money[head] << ", " << res << "\n";
        ans += res;
    }

    return ans;
}
