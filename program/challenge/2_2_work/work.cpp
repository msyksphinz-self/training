#include <algorithm>
#include <functional>
#include <iostream>

const int start[5] = {1, 2, 4, 6, 8};
const int TIME[5]  = {3, 5, 7, 9, 10};



int main ()
{
    std::pair<int, int> list[5];
    for (int i = 0; i < 5; i++) {
        list[i].first  = TIME[i];
        list[i].second = start[i];
    }

    std::sort (list, list+5);

    int ans = 0, t = 0;
    for (int i  = 0; i < 5; i++) {
        if (t < list[i].second) {
            ans++;
            t = list[i].first;
        }
    }

    std::cout << ans << "\n";

}
