#include <iostream>
#include <string>
#include <stdint.h>

int main ()
{
    int dic_num, start_num;
    std::string k;

    std::cin >> dic_num >> start_num;
    std::cin >> k;

    start_num--;
    int *word    = new int[dic_num];
    int *visited = new int[dic_num];

    for(int i=0; i < dic_num; i++) {
        std::cin >> word[i];
        word[i]--;
        visited[i] = 0;
    }

    int next_idx = start_num;
    int step = 1;
    while (visited[next_idx] == 0) {
        // std::cout << "+" << word[next_idx] << '\n';
        visited[next_idx] = step;
        next_idx = word[next_idx];
        step++;
    }
    int loop_num = step - visited[next_idx];
    // std::cout << "Loop_num = " << loop_num << '\n';


    int now = 0;
    bool check = false;
    for (int i = 0; i < k.length(); i++) {
        now = now * 10 + (k[i]-'0');
        if (now > dic_num) {
            now %= loop_num;
            check = true;
        }
    }
    if (check) {
        int add = 0;
        while (add < dic_num) {
            add += loop_num;
        }
        now += add;
    }
    // std::cout << "Now = " << now << '\n';

    next_idx = start_num;
    for (int i = 0; i < now; i++) {
        next_idx = word[next_idx];
    }

    std::cout << (next_idx + 1) << '\n';

    return 0;
}
