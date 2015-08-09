#include <iostream>
#include <vector>
#include <random>

#define MAX_NUM (10)
int value[MAX_NUM];

void show_vector (std::vector<int> *vec_list);
bool find_part_sum (int total);
bool part_sum (int i, int sum, const int total, std::vector<int> *vec_list);

int main (void)
{
    std::random_device rnd;
    for (int i = 0; i < MAX_NUM; i++) {
        value[i] = rnd() % 100;
    }

    std::cout << "Array = ";
    for (int i = 0; i < MAX_NUM; i++) {
        std::cout << value[i] << ' ';
    }
    std::cout << '\n';

    bool ans = find_part_sum ((MAX_NUM/4)*100);

    if (ans == true) {
        std::cout << "Yes\n";
    } else {
        std::cout << "No\n";
    }
}

void show_vector (std::vector<int> *vec_list)
{
    int total = 0;
    while (!vec_list->empty()) {
        int elem = vec_list->back();
        total += elem;
        std::cout << elem << ", ";
        vec_list->pop_back();
    }
    std::cout << "TOTAL=" << total << '\n';
}


bool find_part_sum (int total)
{
    std::vector<int> *vec_list = new std::vector<int>();
    return part_sum (0, 0, total, vec_list);
}


bool part_sum (int i, int sum, const int total, std::vector<int> *vec_list)
{
    if (i > MAX_NUM) {
        return false;
    } else if (sum + value[i] == total) {
        vec_list->push_back (value[i]);
        show_vector (vec_list);
        return true;
    }

    vec_list->push_back (value[i]);
    if (part_sum (i + 1, sum + value[i], total, vec_list)) {
        return true;
    }
    vec_list->pop_back ();

    if (part_sum (i + 1, sum, total, vec_list)) {
        return true;
    }
    return false;
}
