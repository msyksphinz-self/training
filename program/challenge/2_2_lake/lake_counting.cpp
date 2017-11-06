#include <iostream>
#include <random>

#define MAX_LAKE 100
bool lake[MAX_LAKE][MAX_LAKE];
bool visited[MAX_LAKE][MAX_LAKE];

uint32_t visit (int y, int x);
uint32_t solve (void);

int main (void)
{
    std::random_device rnd;

    for (int j = 0; j < MAX_LAKE; j++) {
        for (int i = 0; i < MAX_LAKE; i++) {
            if ((rnd() % 5) == 0) {
                lake[j][i] = true;
            } else {
                lake[j][i] = false;
            }
            visited[j][i] = false;
        }
    }

    for (int j = 0; j < MAX_LAKE; j++) {
        for (int i = 0; i < MAX_LAKE; i++) {
            std::cout << lake[j][i] << " ";
        }
        std::cout << '\n';
    }

    std::cout << "Lake Counting = " << solve () << "\n";

    return 0;
}


uint32_t visit (int y, int x)
{
    if (y < 0 || y >= MAX_LAKE) {
        return 0;
    }
    if (x < 0 || x >= MAX_LAKE) {
        return 0;
    }

    if (visited[y][x] == true) {
        return 0;
    }

    visited[y][x] = true;

    if (lake[y][x] == true) {
        visit (y-1, x-1);
        visit (y-1, x);
        visit (y-1, x+1);
        visit (y  , x-1);
        visit (y  , x+1);
        visit (y+1, x-1);
        visit (y+1, x);
        visit (y+1, x+1);
    }

    return static_cast<uint32_t>(lake[y][x]);
}


uint32_t solve (void)
{
    uint32_t num = 0;
    for (int y = 0; y < MAX_LAKE; y++) {
        for (int x = 0; x < MAX_LAKE; x++) {
            uint32_t res = visit (y, x);
            num += res;
            // std::cout << res << " ";
        }
        // std::cout << "\n";
    }
    return num;
}
