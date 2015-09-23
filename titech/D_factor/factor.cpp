#include <iostream>
#include <algorithm>
#include <vector>
#include <stdint.h>
#include <math.h>

bool isfactor (int64_t num)
{
    for (int64_t i = 2; i*i < num; i++) {
        if ((num % i) == 0) {
            return false;
        }
    }
        return (num != 1);
}



 int main (void)
 {
    std::string str;
    std::cin >> str;
    char *str_c = new char[str.size()];

    char str_size = 0;
    for (int64_t i = 0; i < str.size(); i++) {
    int64_t j;
    for (j = 0; j < str_size; j++) {
    if (str_c[j] == str[i]) {
    break;
}
}
    if (j == str_size) {
    str_c[str_size++] = str[i];
}
}

    if (str_size > 5) {
        std::cout << "-1" << '\n';
        return 0;
    }

    int array[5] = {1, 3, 5, 7, 9};

    do {
        int64_t num = 0;
        for (int i = 0; i < str.size(); i++) {
            int j;
            for (j = 0; j < str_size; j++) {
                if (str_c[j] == str[i]) {
                break;
            }
            }
            num = num * 10 + array[j];
        }
        if (isfactor (num)) {
        std::cout << num << '\n';
        return 0;
    }
    } while (std::next_permutation (array, array + 5));

    std::cout << "-1\n";

}
