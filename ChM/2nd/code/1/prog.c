#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include "methods.h"
#include <math.h>

int
main(int argc, char **argv)
{
    problem prob;
    char mode = argv[1][0];
    if (mode == '0') {
        prob = given;
            }
    if (mode == '1') {
        char num_test = argv[1][1];
        switch (num_test) {
            case '1':
                prob = test1;
                break;
            case '2':
                prob = test2;
                break;
            case '3':
                prob = test3;
                break;
            default:
                break;
        }
    }
    double x0 = prob.x0;
    double y0 = prob.y0;
    int n;
    sscanf(argv[2], "%d", &n);
    double l;
    sscanf(argv[3], "%lf", &l);
    double *x = make_x_grid(x0, l, n);
    double *y = make_mem(n);
    y[0] = y0;
    double *u = make_grid_fun(n, x, prob.u);
    rk4(n, x, y, prob.f);
    for (int i = 0; i <= n; ++i) {
        printf("%lf %lf %lf\n", x[i], y[i], u[i]);
    }
    printf("%.16lf\n", dif_between_fun(n, y, u));
    print_res(n, x, y, "res0.txt");
    print_res(n, x, u, "true0.txt");
    free_mem();
    return 0;

}
