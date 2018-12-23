#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double
given_f(double x, double y)
{
    return -y - x * x;
}

double
given_ans(double x)
{
    return -x * x + 2 * x - 2 + 12 * exp(-x);
}


double
given_f1(double x, double u, double v)
{
    return sqrt(x * x + 1.1 * u * u) + v;
}

double
given_f2(double x, double u, double v)
{
    return cos(2.1 * v) + u;
}


