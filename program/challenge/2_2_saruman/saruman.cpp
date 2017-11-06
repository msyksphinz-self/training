#include <iostream>
#include <algorithm>

const int N = 6;
const int R = 10;
int X[N] = {1, 7, 15, 20, 30, 50};

int solve ();

int main ()
{
    std::cout << "solve = " << solve () << '\n';
}


int solve ()
{
    std::sort(X, X+N);

    int i = 0, ans = 0;
    while (i < N) {
        int s = X[i++];
        while (i < N && X[i] <= (s+R)) i++;

        int p = X[i-1];

        while (i < N && X[i] <= (p+R)) i++;

        ans++;
    }

    return ans;
}
