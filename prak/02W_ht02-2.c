#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum
{
    ROUND = 10000, //10^(-4) - порядок округления
    FULL = 100 //100% - полная часть числа
};

int
main(int argc, char **argv)
{
    double res = atof(argv[1]);
    for (int i = 2; i < argc; i++) {
        double d = atof(argv[i]);
        res *= 1 + d / FULL;
        res = round(res * (double) ROUND) / (double) ROUND;
    }
    printf("%.4lf\n", res);
    return 0;
}
