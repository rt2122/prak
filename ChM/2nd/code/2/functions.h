#include <stdio.h>
#include <stdlib.h>

typedef struct problem
{
    double (*p)(double x);
    double (*q)(double x);
    double (*f)(double x);
    double sig1, sig2;
    double gam1, gam2;
    double del1, del2;
    double x0, x1;
} problem;

double
given_p(double x)
{
    return - 3 * x;
}

double
given_q(double x)
{
    return 2;
}

double
given_f(double x)
{
    return 1.5;
}

problem given = { .p = &given_p, .q = given_q, .f = given_f, .sig1 = 0, .sig2 = 0.5, 
        .gam1 = 1, .gam2 = 1, .del1 = 1.3, .del2 = 2, .x0 = 0.7, .x1 = 1};
