#include <iostream>
#include <string>

int main (void)
{
    std::string card1, card2;
    std::cin >> card1;
    std::cin >> card2;

    bool win = true;

    for (int index = 0; index < card1.size(); index++) {
        if (card1[index] != card2[index]) {
            if (card1[index] == '@') {
                if ((card2[index] != 'a') &&
                    (card2[index] != 't') &&
                    (card2[index] != 'c') &&
                    (card2[index] != 'o') &&
                    (card2[index] != 'd') &&
                    (card2[index] != 'e') &&
                    (card2[index] != 'r')) {
                    win = false;
                    break;
                }
            } else if (card2[index] == '@') {
                if ((card1[index] != 'a') &&
                    (card1[index] != 't') &&
                    (card1[index] != 'c') &&
                    (card1[index] != 'o') &&
                    (card1[index] != 'd') &&
                    (card1[index] != 'e') &&
                    (card1[index] != 'r')) {
                    win = false;
                    break;
                }
            } else {
                win = false;
                break;
            }
        }
    }

    if (win == true) {
        std::cout << "You can win\n";
    } else {
        std::cout << "You will lose\n";
    }
    return 0;
}
