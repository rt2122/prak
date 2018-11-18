#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

enum
{
    FIRST_YEAR = 1900,
    NUM_OF_PARAM = 6
};

int get_date(FILE *f, time_t *t) {
    int year, mon, day, hour, min, sec;
    if (fscanf(f, "%d/%d/%d %d:%d:%d", &year, &mon, &day, &hour, &min, &sec) == NUM_OF_PARAM) {
        struct tm stm;
        stm.tm_year = year - FIRST_YEAR;
        stm.tm_mon = mon - 1;
        stm.tm_mday = day;
        stm.tm_hour = hour;
        stm.tm_min = min;
        stm.tm_sec = sec;
        stm.tm_isdst = -1;
        *t = mktime(&stm);
        return 1;
    } else {
        return 0;
    }

}

int
main(int argc, char **argv)
{
    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        return 1;
    }
    time_t t, t1;
    get_date(f, &t);
    while (get_date(f, &t1)) {
        printf("%ld\n", t1 - t);
        t = t1;
    }
    fclose(f);
    return 0;
}
