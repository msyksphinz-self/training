#include <iostream>

int main ()
{
    int t_total, t_win;
    int a_total, a_win;

    std::cin >> t_total >> t_win >> a_total >> a_win;

    double t_rate = static_cast<double>(t_win) / static_cast<double>(t_total);
    double a_rate = static_cast<double>(a_win) / static_cast<double>(a_total);

    if (t_rate > a_rate) {
        std::cout << "TAKAHASHI\n";
    } else if (t_rate < a_rate) {
        std::cout << "AOKI\n";
    } else {
        std::cout << "DRAW\n";
    }

    return 0;
}
