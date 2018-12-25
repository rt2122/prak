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
    return -x * x + 2 * x - 2 + 12 * exp(-x); 
}

double
sys_given_f1(double x, double u, double v)
{
    return (u - v) / x;
}

double
sys_given_f2(double x, double u, double v)
{
    return (u + v) / x;
}

double
sys_u(double x)
{
    return x * cos(log(x)) - x * sin(log(x));
}

double
sys_v(double x)
{
    return x * sin(log(x)) + x * cos(log(x));
}

problem given = {.f = &given_f, .x0 = 0.0, .y0 = 10.0, .u = &given_ans};
sys_problem sys_given = {.f1 = &sys_given_f1, .f2 = &sys_given_f2, .x0 = 1.0, 
        .y10 = 1.0, .y20 = 1.0, .u = sys_u, .v = sys_v };

double
test_f1(double x, double y)
{
    return 3 - y - x;
}

double
test_u1(double x)
{
    return 4 - x - 4 * exp(-x);
}

problem test1 = { .f = &test_f1, .x0 = 0, .y0 = 0, .u = test_u1 };

double
test_f2(double x, double y)
{
    return y - y * x;
}

double
test_u2(double x)
{
    return 5 * exp((-1.0/2.0) * x * (x - 2));
}

problem test2 = { .f = &test_f2, .x0 = 0, .y0 = 5.0, .u = test_u2 };

double
test_f3(double x, double y)
{
    return y + 2 * x - 3;
}

double
test_u3(double x)
{
    return 1 - 2 * x;
}

problem test3 = { .f = &test_f3, .x0 = 0, .y0 = 1, .u = test_u3 };


