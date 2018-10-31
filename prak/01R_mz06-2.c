#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

enum
{
    B_YEAR = 1975,
    B_MON = 10,
    B_DAY = 7,
    ONE_DAY = 60 * 60 * 24,
    ONE_MONTH = 30,
    ONE_YEAR = 12
};

int main(void)
{
    long long year, mon, day;
    struct tm *t = calloc(1, sizeof(struct tm));
    t->tm_year = B_YEAR - 1900;
    t->tm_mon = B_MON - 1;
    t->tm_mday = B_DAY;
    long long bir = mktime(t);
    printf("%lld\n", bir);
    struct tm *tq = calloc(1, sizeof(struct tm));
    while (scanf("%lld%lld%lld", &year, &mon, &day) == 3) {
        tq->tm_year = year - 1900;
        tq->tm_mon = mon - 1;
        tq->tm_mday = day;
        long long tem = mktime(tq);
        printf("%lld\n", tem);
        long long delt = tem - bir;
        delt /= (long long)ONE_DAY;
        year = delt / (long long)(ONE_MONTH * ONE_YEAR) + 1;
        delt %= (long long)(ONE_MONTH * ONE_YEAR);
        mon = (long long)delt / (long long)ONE_MONTH + 1;
        day = delt % (long long)ONE_MONTH + 1;
        printf("%lld %lld %lld\n", year, mon, day);
    }
    return 0;
}
