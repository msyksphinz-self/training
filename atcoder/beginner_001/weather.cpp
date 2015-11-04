#include <iostream>
#include <iomanip>

int main ()
{
    int m;
    std::cin >> m;

    float km = static_cast<float>(m) / 1000;

    if (m < 100) {
        std::cout << "00\n";
    } else if (m >= 100 && m <= 5000) {
        std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(km * 10) << '\n';
    } else if (m >= 6000 && m <= 30000) {
        std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(km + 50) << '\n';
    } else if (m >= 35000 && m <= 70000) {
        std::cout << std::setw(2) << std::setfill('0') << static_cast<int>((km - 30) / 5 + 80) << '\n';
    } else {
        std::cout << "89\n";
    }

    return 0;

}
