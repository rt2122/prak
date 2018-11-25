#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum
{
    SUNDAY = 7,
    FIRST_YEAR = 1900,
    LAST_MON = 12
};

int
main(int argc, char **argv)
{
    if (argc < 3) {
        return 1;
    }
    int year, m_day, w_day;
    sscanf(argv[1], "%d", &year);
    sscanf(argv[2], "%d", &m_day);
    sscanf(argv[3], "%d", &w_day);
    w_day = w_day == SUNDAY ? 0 : w_day;
    year -= FIRST_YEAR;

    int ans = 0;
    for (int i = 0; i < LAST_MON; ++i) {
        struct tm stm = { .tm_year = year, .tm_mon = i, .tm_mday = m_day};
        mktime(&stm);
        if (stm.tm_wday == w_day) {
            ++ans;
        }
    }

    printf("%d\n", ans);
    return 0;
}