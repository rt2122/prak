#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

enum { FIRST_YEAR = 1900 };

int
main(int argc, char **argv)
{
    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        return 1;
    }

    int year, mon, day, hour, min, sec, year1, mon1, day1, hour1, min1, sec1;
    fscanf(f, "%d/%d/%d %d:%d:%d", &year, &mon, &day, &hour, &min, &sec);

    struct tm stm;
    stm.tm_year = year - FIRST_YEAR;
    stm.tm_mon = mon - 1;
    stm.tm_mday = day;
    stm.tm_hour = hour;
    stm.tm_min = min;
    stm.tm_sec = sec;
    stm.tm_isdst = -1;
    time_t t = mktime(&stm);

    while (fscanf(f, "%d/%d/%d %d:%d:%d", &year1, &mon1, &day1, &hour1, &min1, &sec1) == 6) {
        struct tm stm1;
        stm1.tm_year = year1 - FIRST_YEAR;
        stm1.tm_mon = mon1 - 1;
        stm1.tm_mday = day1;
        stm1.tm_hour = hour1;
        stm1.tm_min = min1;
        stm1.tm_sec = sec1;
        stm1.tm_isdst = -1;
        time_t t1 = mktime(&stm1);
        printf("%ld\n", t1 - t);
        t = t1;
    }

    fclose(f);
    return 0;
}
