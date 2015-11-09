#include <iostream>
#include <string.h>

int main ()
{
    int string_array_num;
    std::cin >> string_array_num;

    for (int array_idx = 0; array_idx < string_array_num; array_idx++) {
        char str[200];
        std::cin >> str;

        int sep_num = 0;
        int size_str = strlen(str);
        if (strlen(str) < 5) {
            size_str = 0;
        }
        for (int i = 0; i < size_str; i++) {
            if (!strncmp (&str[i], "tokyo", 5)) {
                sep_num++;
                i += 5;
            }
            if (!strncmp (&str[i], "kyoto", 5)) {
                sep_num++;
                i += 5;
            }
        }

        std::cout << sep_num << '\n';
    }
}
