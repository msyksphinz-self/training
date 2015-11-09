#include <iostream>

int main ()
{
    int X, Y;
    std::cin >> X >> Y;

    std::cout << ((X < Y) ? Y : X) << '\n';

    return 0;
}
