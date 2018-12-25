#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double
dif_between_fun(int n, double x[], double y[], double u[])
{
    double dif_max = 0;
    for (int i = 0; i < n; ++i) {
        double dif = fabs(y[i] - u[i]);
        if (dif > dif_max) {
            dif_max = dif;
        }
    }
    return dif_max;
}

enum { BUF = 8 }; 
int mem_size = 0, mem_eng = 0;
double **memory = NULL;

double
*make_mem(int n)
{
    double *ret = calloc(n + 1, sizeof(double));
    if (mem_eng == mem_size) {
        mem_size += BUF;
        memory = realloc(memory, sizeof(double *) * mem_size);
    }
    memory[mem_eng++] = ret;
    return ret;
}

double
*make_x_grid(double x, double l, int n)
{
    double *ret = make_mem(n);
    double h = l / (double) n;
    for (int i = 0; i <= n; ++i) {
        ret[i] = x + i * h;
    }
    return ret;
}

double
*make_grid_fun(int n, double *x, double (*u)(double x)) 
{
    double *ret = make_mem(n);
    for (int i = 0; i <= n; ++i) {
        ret[i] = u(x[i]);
    }
    return ret;
}

void
free_mem(void)
{
    if (!memory) {
        return;
    }
    for (int i = 0; i < mem_eng; ++i) {
        free(memory[i]);
    }
    free(memory);
}

void
print_res(int n, double *x, double *y, const char *name)
{
    FILE *f = fopen(name, "w");
    for (int i = 0; i <= n; ++i) {
        fprintf(f, "%lf, %lf,\n", x[i], y[i]);
    }
    fclose(f);
}

double
*diag3(int n, double *a, double *b, double *c, double *f)
{
    for (int i = 1; i <= n; ++i) {
        c[i] -= b[i - 1] * a[i] / c[i - 1];
        f[i] -= f[i - 1] * a[i] / c[i - 1];
    }
    double *x = make_mem(n);
    x[n] = f[n] / c[n];
    for (int i = n - 1; i >= 0; --i) {
        x[i] = f[i] - x[i + 1] * b[i];
        x[i] /= c[i];
    }
    return x;
}

double
*prob_bor(int n, double x[], double (*p)(double x), double (*q)(double x), double (*f)(double x), 
        double sig1, double sig2, double gam1, double gam2, double del1, double del2)
{

    double *a = calloc(n + 1, sizeof(double));
    double *b = calloc(n + 1, sizeof(double));
    double *c = calloc(n + 1, sizeof(double));
    double *ff = calloc(n + 1, sizeof(double));
    
    double h = x[1] - x[0];
    a[0] = 0, b[0] = gam1 / h, c[0] = sig1 - gam1 / h;
    a[n] = gam2 / h, b[n] = 0, c[n] = sig2 - gam2 / h;
    for (int i = 1; i < n; ++i) {
        a[i] = 1 / (h * h) - p(x[i]) / (2 * h);
        b[i] = q(x[i]) - 2 / (h * h);
        c[i] = 1 / (h * h) + p(x[i]) / (2 * h);
        ff[i] = -f(x[i]);
    }
    double *y = diag3(n, a, b, c, ff);
    free(a);
    free(b);
    free(c);
    free(ff);
    return y;
}
