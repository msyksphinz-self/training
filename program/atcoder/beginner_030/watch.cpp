#include <iostream>
#include <math.h>

int main ()
{
    int hour, minute;
    std::cin >> hour >> minute;

    double hour_angle   = static_cast<double>((hour   % 12)) / 12.0 * 360.0;
    double minute_angle = static_cast<double>((minute % 60)) / 60.0 * 360.0;
    hour_angle += minute_angle / 12.0;

    // std::cout << "hour = " << hour_angle << ", minute = " << minute_angle << '\n';

    double f_angle = fabs (hour_angle - minute_angle);
    if (f_angle > 180) {
        std::cout << (360.0 - f_angle) << '\n';
    } else {
        std::cout << f_angle << '\n';
    }

    return 0;
}
