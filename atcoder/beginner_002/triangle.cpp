#include <iostream>

int main ()
{
    int xa, ya, xb, yb, xc, yc;
    std::cin >> xa >> ya >> xb >> yb >> xc >> yc;

    xb -= xa; yb -= ya;
    xc -= xa; yc -= ya;
    double area = xb * yc - xc * yb;
    area = (area >= 0.0f) ? area : -area;
    area /= 2.0;

    std::cout << area << '\n';

    return 0;
}
