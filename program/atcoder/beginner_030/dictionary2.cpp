#include <iostream>
#include <string>

int main ()
{

    int N, a;
    std::cin >> N >> a;
    a--;

    std::string k_str;
    std::cin >> k_str;
    int *b = new int[N];
    int *visited = new int[N];


    for (int i = 0; i < N; i++) {
        std::cin >> b[i];
        b[i]--;
        visited[i] = 0;
    }

    int loop;
    for (int start = a, count = 0; ;count++, start = b[start]) {
        if (visited[start] != 0) {
            loop = count - visited[start];
            break;
        }
        std::cout << "index " << start << '\n';
        visited[start] = count;
    }
    std::cout << "Loop = " << loop << '\n';
    int mod = 0;
    for (int i = 0; i < k_str.size(); i++) {
        mod = ((mod * 10) + (k_str[i]-'0')) % loop;
    }

    std::cout << (mod + 1) << '\n';

    return 0;
}
