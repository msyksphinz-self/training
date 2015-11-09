#include <iostream>
#include <sstream>
#include <string>

int main ()
{
    std::string w_str;
    std::cin >> w_str;
    std::stringstream w_ans;

    for (int i = 0; i < w_str.length(); i++) {
        if (w_str[i] != 'a' &&
            w_str[i] != 'e' &&
            w_str[i] != 'i' &&
            w_str[i] != 'o' &&
            w_str[i] != 'u') {
            w_ans << w_str[i];
        }
    }

    std::cout << w_ans.str() << '\n';
}
