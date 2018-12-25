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
//int matr_size;
double **memory = NULL;
//double **matrix = NULL;

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
/*
double
**make_matr(int n)
{
    matr_size = n;
    matrix = calloc(n, sizeof(double *));
    for (int i = 0; i < n; ++i) {
        matrix[i] = calloc(n, sizeof(double));
    }
    return matrix;
}
*/
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
    /*
    if (matrix) {
        for (int i = 0; i < matr_size; ++i) {
            free(matrix[i]);
        }
        free(matrix);
    }
    */
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
*diag3(double *a, double *b, double *c, double *f, double q0, double qn)
{
    int n = matr_size + 1;
    double *alpha = calloc(n + 1, sizeof(double));
    double *beta = calloc(n + 1, sizeof(double));
    alpha[1] = 0;
    beta[1] = q0;
    for (int i = 1; i < n; ++i) {
        alpha[i + 1] = -b[i] / (a[i] * alpha[i] + c[i]);
        beta[i + 1] = (f[i] - a[i] * beta[i]) / (a[i] * alpha[i] + c[i]);
    }
    double *x = make_mem(n);
    x[n] = qn;
    
    for (int i = n - 1; i >= 0; --i) {
        x[i] = alpha[i + 1] * x[i + 1] + beta[i + 1];
    }

    free(alpha);
    free(beta);
    return x;
}
