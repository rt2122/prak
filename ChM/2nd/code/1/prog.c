#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include "methods.h"
#include <math.h>

int
main(void)
{
    double x0 = given.x0;
    double y0 = given.y0;
    int n = 5;
    double *x = make_x_grid(x0, 10, n);
    double *y = make_mem(n);
    y[0] = y0;
    double *u = make_grid_fun(n, x, given.u);
    rk2(n, x, y, given.f);
    for (int i = 0; i <= n; ++i) {
        printf("%lf %lf %lf\n", x[i], y[i], u[i]);
    }
    printf("%lf\n", dif_between_fun(n, x, y, u));
    print_res(n, x, y, "res.txt");
    print_res(n, x, u, "true.txt");
    free_mem();
}
