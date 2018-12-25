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
rk2(int n, double x[], double y[], double (*f)(double x, double y))
{
    double tmp, h = x[1] - x[0];
    for (int i = 0; i < n; ++i) {
        tmp = y[i] + f(x[i], y[i]) * h;
        y[i + 1] = y[i] + (f(x[i], y[i]) + f(x[i + 1], tmp)) * h / 2.0;
    }
}

void
rk4(int n, double x[], double y[], double (*f)(double x, double y)) 
{
    double k1, k2, k3, k4, h = x[1] - x[0];
    for (int i = 0; i < n; ++i) {
        k1 = f(x[i], y[i]);
        k2 = f(x[i] + h / 2, y[i] + h * k1 / 2);
        k3 = f(x[i] + h / 2, y[i] + h * k2 / 2);
        k4 = f(x[i] + h, y[i] + h * k3);
        y[i + 1] = y[i] + h * (k1 + 2 * k2 + 3 * k3 + k4) / 6;
    }
}

void
sys_rk4(int n, double x[], double u[], double v[], double (*f)(double x, double u, double v), 
        double (*g)(double x, double u, double v))
{
    double h = x[1] - x[0], k1, k2, k3, k4, m1, m2, m3, m4;
    for (int i = 0; i < n; ++i) {
        k1 = f(x[i], u[i], v[i]);
        m1 = g(x[i], u[i], v[i]);
        k2 = f(x[i] + h / 2, u[i] + k1 / 2, v[i] + m1 / 2);
        m2 = g(x[i] + h / 2, u[i] + k1 / 2, v[i] + m1 / 2);
        k3 = f(x[i] + h / 2, u[i] + k2 / 2, v[i] + m2 / 2);
        m3 = g(x[i] + h / 2, u[i] + k2 / 2, v[i] + m2 / 2);
        k4 = f(x[i] + h, u[i] + k3, v[i] + m3);
        m4 = g(x[i] + h, u[i] + k3, v[i] + m3);
        u[i + 1] = u[i] + h * (k1 + 2 * k2 + 2 * k3 + k4) / 6;
        v[i + 1] = v[i] + h * (m1 + 2 * m2 + 2 * m3 + m4) / 6;
    }
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
