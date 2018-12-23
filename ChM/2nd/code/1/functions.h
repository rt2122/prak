#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct problem
{
    double (*f)(double x, double y);
    double x0;
    double y0;
    double (*u)(double x);
} problem;

typedef struct sys_problem
{
    double (*f1)(double x, double u, double v);
    double (*f2)(double x, double u, double v);
    double x0;
    double y10;
    double y20;
    double (*u)(double x);
    double (*v)(double y);
} sys_problem;

double
given_f(double x, double y)
{
    return -y - x * x;
}

double
given_ans(double x)
{
    return -x * x - 2 * x - 2 + 12 * exp(-x); 
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

problem given = {.f = &given_f, .x0 = 0.0, .y0 = 10.0, .u = &given_ans};
sys_problem sys_given = {.f1 = &given_f1, .f2 = &given_f2, .x0 = 0.0, .y10 = 0.5, .y20 = 1.0};

