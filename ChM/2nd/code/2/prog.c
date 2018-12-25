#include <stdio.h>
#include <stdlib.h>
#include "methods.h"
#include "functions.h"

int
main(int argc, char **argv)
{
    int n;
    sscanf(argv[1], "%d", &n);

    problem prob = given;
    double sig1 = prob.sig1, sig2 = prob.sig2, gam1 = prob.gam1, gam2 = prob.gam2;
    double del1 = prob.del1, del2 = prob.del2, x0 = prob.x0, x1 = prob.x1;
    double *x = make_x_grid(x0, x1 - x0, n);
    double *y = prob_bor(n, x, prob.p, prob.q, prob.f, sig1, sig2, gam1, gam2, del1, del2);
    for (int i = 0; i <= n; ++i) {
        printf("%lf %lf\n", x[i], y[i]);
    }
    print_res(n, x, y, "ans.txt");
    free_mem();
}
