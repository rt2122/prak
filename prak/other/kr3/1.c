#include <stdio.h>
#include <stdlib.h>
struct RoundResult
{
    long long value;
    int status;
};

struct RoundResult iround(long long value, int prec)
{
    enum { STEP = 10 };
    struct RoundResult rr;
    if (prec == 0) {
        rr.value = value;
        rr.status = 0;
        return rr;
    }
    long long fine = 1;
    for (int i = 0; i < prec; ++i) {
        fine *= STEP;
    }
    value += value > 0 ? fine / 2 : - fine / 2 + 1;
    rr.value = fine * (value / fine);
    rr.status = 0;
    return rr;
}
int
main(int argc, char **argv)
{
    long long x;
    int y;
    sscanf(argv[1], "%lld", &x);
    sscanf(argv[2], "%d", &y);
    struct RoundResult rr = iround(x, y);
    printf("%lld %d\n", rr.value, rr.status);
    return 0;
}
