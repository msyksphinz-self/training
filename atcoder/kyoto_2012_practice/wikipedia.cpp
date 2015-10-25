#include <iostream>
#include <vector>
#include <stdint.h>

int64_t A2(int m, int n)
{
    std::vector<int64_t> table; // 4MB
    int ntable = table.size ();

    table.push_back(m);
    table.push_back(n);

    while (table.size() >=2 ) {
        int n = table.back (); table.pop_back ();
        int m = table.back (); table.pop_back ();

        if (m == 0) {
            table.push_back (n+1);
        } else if (n == 0) {
            table.push_back (m-1);
            table.push_back (1);
        } else {
            table.push_back (m-1);
            table.push_back (m);
            table.push_back (n-1);
        }
    }
    return table.back ();
}


int64_t A3 (int m, int n)
{
    const int table_max = 10000000;
    int64_t *table = new int64_t[table_max];

    int table_idx = 0;
    table[table_idx++] = m;
    table[table_idx++] = n;

    while (table_idx > 1) {
        int m = table[table_idx-2];
        int n = table[table_idx-1];

        // std::cout << "Size = " << table_idx << '\n';

        if (m == 0) {
            table[table_idx-2] = n+1;
            table_idx--;
        } else if (n == 0) {
            table[table_idx-2] = m-1;
            table[table_idx-1] = 1;
        } else {
            table[table_idx-2] = m-1;
            table[table_idx-1] = m;
            table[table_idx+1] = n-1;
            table_idx++;
        }
    }
    return table[0];
}



int main ()
{
    int m, n;
    std::cin >> m >> n;
    std::cout << A3(m, n) << '\n';
}
