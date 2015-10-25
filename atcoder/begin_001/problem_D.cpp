#include <iostream>
#include <stdio.h>
#include <stdlib.h>

int main ()
{
    int row_num;
    scanf ("%d", &row_num);

    int *start_time, *stop_time;
    start_time = static_cast<int *>(malloc (sizeof(int) * row_num));
    stop_time  = static_cast<int *>(malloc (sizeof(int) * row_num));

    int table_max = 0, old_table_max = -1;

    for (int idx = 0; idx < row_num; idx++) {
        int start, stop;
        scanf ("%d-%d", &start, &stop);
        int table_idx;
        for (table_idx = 0; table_idx < table_max; table_idx++) {
            if (start <= start_time[table_idx] &&
                start_time[table_idx] <= stop) {
                start_time[table_idx] = start;
                break;
            }
            if (start <= stop_time[table_idx] &&
                stop_time[table_idx] <= stop) {
                stop_time[table_idx] = stop;
                break;
            }
            if (start <= start_time[table_idx] &&
                stop_time[table_idx] <= stop) {
                start_time[table_idx] = start;
                stop_time[table_idx]  = stop;
                break;
            }
            if (start_time[table_idx] <= start &&
                stop <= stop_time[table_idx]) {
                break;
            }
        }
        if (table_idx == table_max) {
            start_time[table_max] = start;
            stop_time[table_max]  = stop;
            table_max++;
            printf ("Inserted %d-%d\n", start, stop);
        } else {
            printf ("Skipped %d-%d\n", start, stop);
        }
    }

    for (int idx=0; idx < table_max; idx++) {
        printf ("%04d-%04d\n", start_time[idx], stop_time[idx]);
    }

    while (old_table_max != table_max) {
    }
}
