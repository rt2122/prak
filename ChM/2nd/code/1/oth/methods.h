#include <stdio.h>
#include <stdlib.h>
#include "methods.c"

double
dif_between_fun(int n, double x[], double y[], double (*u)(double x));

double
*make_mem(int n);

double
*make_x_grid(double x, double l, int n);

void
free_mem(void);

void
rk2(int n, double x[], double y[], double (*f)(double x, double y));

void
rk4(int n, double x[], double y[], double (*f)(double x, double y)); 

void
sys_rk4(int n, double x[], double u[], double v[], double (*f)(double x, double u, double v), 
        double (*g)(double x, double u, double v));

