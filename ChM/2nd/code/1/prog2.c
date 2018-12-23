#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include "methods.h"
#include <math.h>

int
main(int argc, char **argv)
{
    double x0 = sys_given.x0;
    double y10 = sys_given.y10;
    double y20 = sys_given.y20;
    int n;
    sscanf(argv[1], "%d", &n);
    double l;
    sscanf(argv[2], "%lf", &l);
    double *x = make_x_grid(x0, l, n);
    double *u = make_mem(n);
    double *v = make_mem(n);
    u[0] = y10;
    v[0] = y20;
    sys_rk4(n, x, u, v, sys_given.f, sys_given.g);
    for (int i = 0; i <= n; ++i) {
        printf("%lf %lf %lf\n", x[i], y[i], u[i]);
    }
    free_mem();
}
