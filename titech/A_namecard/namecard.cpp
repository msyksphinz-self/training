#include <stdio.h>
#include <stdint.h>

int main ()
{
    char  card[9];
    fgets (card, 9, stdin);

    uint8_t year = (card[0]-'0') * 10 + (card[1] - '0');

    switch (card[2]) {
    case 'B' : printf ("Bachelor "); break;
    case 'M' : printf ("Master ");   break;
    case 'D' : printf ("Doctor ");   break;
    }

    printf ("%02d\n", year);

    return 0;
}
