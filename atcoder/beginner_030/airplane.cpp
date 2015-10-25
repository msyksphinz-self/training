#include <iostream>

int main ()
{
    int plane_A, plane_B;
    int time_A,  time_B;

    std::cin >> plane_A >> plane_B;
    std::cin >> time_A  >> time_B;

    int *start_A = new int[plane_A];
    int *start_B = new int[plane_B];

    for (int i = 0; i < plane_A; i++) {
        std::cin >> start_A[i];
    }
    for (int i = 0; i < plane_B; i++) {
        std::cin >> start_B[i];
    }


    int idx_A = 0, idx_B = 0;
    int num = 0;
    while (idx_A < plane_A && idx_B < plane_B) {
        int arrive_time_B = start_A[idx_A] + time_A;
        // Search Start Time from B;
        while ((idx_B < plane_B) && (start_B[idx_B] < arrive_time_B)) {
            idx_B++;
        }
        if (idx_B >= plane_B) {
            break;
        }
        // std::cout << "ArriveB at " << arrive_time_B << '\n';
        int arrive_time_A = start_B[idx_B] + time_B;
        // Search Start Time from A
        while ((idx_A < plane_A) && (start_A[idx_A] < arrive_time_A)) {
            idx_A++;
        }
        // std::cout << "ArriveA at " << arrive_time_A << '\n';
        num ++;
    }

    std::cout << num << '\n';

}
