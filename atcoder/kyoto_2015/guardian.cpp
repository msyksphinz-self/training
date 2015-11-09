#include <iostream>

void show_map (bool map[10][10])
{
    for (int y=0; y < 10; y++) {
        for (int x = 0; x < 10; x++ ) {
            if (map[y][x] == false) {
                std::cout << '.';
            } else {
                std::cout << 'C';
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

const int kusari_x[] = {-2,  0,  2, -1,  0,  1, -2, -1, 1, 2, -1,  0,  1, -2,  0,  2};
const int kusari_y[] = {-2, -2, -2, -1, -1, -1,  0,  0, 0, 0,  1,  1,  1,  2,  2,  2};

bool put_kusari (bool map[10][10], bool course[10][10], bool cover[10][10], int y, int x)
{
    cover[y][x] = true;
    bool newly_covered = false;

    // std::cout << "Try " << y << ", " << x << '\n';
    for (int i = 0; i < sizeof(kusari_x)/sizeof(int); i++) {
        int target_x = x + kusari_x[i];
        int target_y = y + kusari_y[i];
        if (0 <= target_x && target_x < 10 &&
            0 <= target_y && target_y < 10) {
            if (cover[target_y][target_x] == false) {
                newly_covered = true;
                cover[y][x] = true;
            }
            if (course[target_y][target_x] == true) {
                // std::cout << " " << target_y << ", " << target_x << "is out\n";
                return false;
            }
        }
    }
    if (newly_covered == true) {
        map[y][x] = true;
    }

    return true;
}


int main ()
{
    bool course[10][10] = {false};
    bool map[10][10]    = {false};
    bool cover[10][10]  = {false};

    for (int x = 0; x < 10; x++) {
        course[5][x] = true;
    }

    // show_map (course);

    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            put_kusari (map, course, cover, y, x);
        }
    }

    show_map (map);

    return 0;
}
