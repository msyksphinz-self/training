#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    std::string in_string, out_string;

    std::cin >> in_string;

    while (1) {
        int max_index = 0;
        int max_osize = 0;
        out_string = "";

        int size_str = in_string.size();

        for (int i = 0; i < size_str; i++) {
            int o_size = 0;
            int o_index = i;
            while (in_string[o_index] == 'o') {
                o_index++;
                o_size ++;
            }
            if ((in_string.substr (o_index, 6) == "kayama") &&
                o_size >= 3) {
                if (o_size > max_osize) {
                    max_index = o_index;
                    max_osize = o_size;
                }
            }
        }

        if (max_osize == 0) {
            out_string = in_string;
            break;
        }

        out_string.append (in_string.substr (0, max_index - max_osize));

        std::string target_o = in_string.substr(max_index - max_osize, size_str - max_index + max_osize);

        bool done;
        std::string tmp_string;
        do {
            done = false;
            tmp_string = "";
            if (target_o.substr(0, 4) == "oooo") {
                tmp_string.append ("o");
                tmp_string.append (target_o.substr(4, target_o.size()));
                done = true;
            } else if (target_o.substr(0, 3) == "ooo") {
                tmp_string.append ("Oo");
                tmp_string.append (target_o.substr(3, target_o.size()));
                done = true;
            } else if (target_o.substr(0, 2) == "oo") {
                tmp_string.append ("O");
                tmp_string.append (target_o.substr(2, target_o.size()));
                done = true;
            } else if (target_o.substr(0, 2) == "OO") {
                tmp_string.append ("o");
                tmp_string.append (target_o.substr(2, target_o.size()));
                done = true;
            }
            if (done == true) {
                target_o = tmp_string;
            }
        } while (done == true);

        out_string.append (target_o);
        in_string = out_string;
    }

    std::cout << out_string << std::endl;
}
