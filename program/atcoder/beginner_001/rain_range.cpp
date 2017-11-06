#include <iostream>
#include <stdio.h>
int main ()
{
    int N;
    std::cin >> N;

    int time_array[24][12] = {0};

    int sh, sm, eh, em;
    for (int i = 0; i < N; i++) {
        scanf ("%02d%02d-%02d%02d", &sh, &sm, &eh, &em);
        // printf ("%02d%02d-%02d%02d\n", sh, sm, eh, em);
        sm /= 5;
        if (em == 0) {
            if (eh != 0) {
                eh--;
            }
            em = 60;
        }
        em = (em - 1) / 5;

        // printf ("%02d%02d-%02d%02d\n", sh, sm, eh, em);
        while (sh <= eh) {
            int end_minute;
            if (sh != eh) {
                end_minute = 11;
            } else {
                end_minute = em;
            }
            while (sm <= end_minute) {
                time_array[sh][sm] = 1;
                sm++;
            }
            sm = 0;
            sh++;
        }
    }

    bool rain_range = 0;
    // for (int hour = 0; hour < 24; hour++) {
    //     for (int minute = 0; minute < 12; minute++) {
    //         printf ("%d", time_array[hour][minute]);
    //     }
    //     printf ("\n");
    // }

    for (int hour = 0; hour < 24; hour++) {
        for (int minute = 0; minute < 12; minute++) {
            if ((time_array[hour][minute] == 1) && (rain_range == false)) {
                sh = hour; sm = minute;
                rain_range = true;
            } else if ((time_array[hour][minute] == 0) && (rain_range == true)) {
                eh = hour; em = minute;
                printf("%02d%02d-%02d%02d\n", sh, sm * 5, eh, em * 5);
                rain_range = false;
            }
        }
    }
    if (rain_range == true) {
        printf("%02d%02d-2400\n", sh, sm * 5);
    }
}
