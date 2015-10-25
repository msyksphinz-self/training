#include <iostream>
#include <stdint.h>

int64_t day_max;
int64_t *move, *stay;

int travel (int day, int city, int money)
{
    if (day == day_max) {
        return money;
    }
    int money_next_city = -1, money_stay = -1;
    if (money+stay[city] >= 0) {
        money_next_city = travel (day+1, city, money+stay[city]);
    }
    if (money+move[city] >= 0) {
        money_stay      = travel (day+1, city+1, money+move[city]);
    }

    return money_next_city < money_stay ? money_stay : money_next_city;
}


int main ()
{
    std::cin >> day_max;
    move = new int64_t[day_max];
    stay = new int64_t[day_max];

    for (int i = 0; i < day_max; i++) {
        std::cin >> move[i];
    }
    for (int i = 0; i < day_max; i++) {
        std::cin >> stay[i];
    }

//    for (int i = 0; i < day_max; i++) {
//        std::cout << move[i] << ' ';
//    }
//    std::cout << '\n';
//
//    for (int i = 0; i < day_max; i++) {
//        std::cout << stay[i] << ' ';
//    }
//    std::cout << '\n';

    std::cout << travel (0, 0, 0) << '\n';

    return 0;
}
