#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

enum
{
    FIRST_YEAR = 1900,
    SUNDAY = 7
};


int
main(int argc, char **argv)
{
    long int year, day;
    sscanf(argv[1], "%ld", &year);
    sscanf(argv[2], "%ld", &day);
    struct tm stm;
    memset(&stm, 0, sizeof(stm));
    stm.tm_year = year - (long)FIRST_YEAR;
    stm.tm_mday = day;
    mktime(&stm);

    printf("%d %d %d %d\n", stm.tm_year + FIRST_YEAR, stm.tm_mon + 1, stm.tm_mday, stm.tm_wday ? stm.tm_wday : SUNDAY);
    return 0;
}
