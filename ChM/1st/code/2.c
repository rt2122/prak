#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lib.c"

const double eps = 0.0001;
int m = 4;
int
all_zero(double **matr, int num)
{
    double el = matr[num][num];
    for (int i = num + 1; i < n; ++i) {
        if (fabs(matr[i][num] - el) > eps) {
            return 0;
        }
    }
    return 1;
}

enum 
{
    MAIN_ELEM = 1,
    OTHER = 0,
};
double
forward_step(double **matr, double **reverse, int mode)
{
    double det = 1;
    for (int j = 0; j < n - 1; ++j) {
        if (all_zero(matr, j)) {
            printf("Ошибка: матрица вырожденная\n");
            return 0;
        }
        if (mode == MAIN_ELEM) {
            switch_main(matr, reverse, j);
        }
        double me = matr[j][j];
        matr[j][j] = 1;
        det *= me;
        double *ml = matr[j];
        double *rev = reverse[j];
        for (int s = j + 1; s < n; s++) {
            ml[s] /= me;
        }
        for (int s = 0; s < n; s++) {
            rev[s] /= me;
        }

        //для каждой i-ой строки обнуляем j-ый элемент
        for (int i = j + 1; i < n; ++i) {
            double mul = -matr[i][j];
            matr[i][j] = 0;
            for (int k = 0; k < n; ++k) {
                if (k > j) matr[i][k] += ml[k] * mul; 
                reverse[i][k] += rev[k] * mul;
            }
        }
    }
    double me = matr[n - 1][n - 1];
    matr[n - 1][n - 1] = 1;
    for (int i = 0; i < n; ++i) {
        reverse[n - 1][i] /= me;
    }
    return det;
}

void
back_step(double **a, double **rev, double *x)
{
    int pos = n - 1;
    for (int i = n - 1; i > 0; --i) {
        double me;
        for (int j = 0; j < i; ++j) {
            me = a[j][i];
            a[j][i] = 0;
            for (int k = 0; k < n; ++k) {
                rev[j][k] -= me;
            }
        }
    }
    if (!x) return;
    for (int i = 0; i < n; ++i) {
        x[i] = rev[i][0];
    }
}

void
get_ans(double **a, double *f, int mode, double *x) {
    double **sec = get_memory_matr();
    for (int i = 0; i < n; ++i) {
        sec[i][0] = f[i];
        for (int j = 1; j < n; ++j) {
            sec[i][j] = 0;
        }
    }
    forward_step(a, sec, mode);
    back_step(a, sec, x);
    for (int i = 0; i < n; ++i) {
        f[i] = sec[i][0];
    }
    free_memory(sec);
}


double
get_reverse_matr(double **aa, double **reverse, int mode) 
{
    get_i_matr(reverse);
    forward_step(aa, reverse, mode);
    back_step(aa, reverse, NULL);
    return 0;
}

void
gen_right(double *f, double x)
{
    for (int i = 0; i < n; ++i) {
        f[i] = n * exp(x / (i + 1)) * cos(x);
    }
}

void
gen_a(double **a)
{
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) {
                a[i][i] = 2 * (double)i / (double)(m + n);
            } else {
                a[i][j] = n + m * m + (j / (double)m) + (i /(double) n);
            }
        }
    }
}

double
max_mod(double **a)
{
    double ans = fabs(a[0][0]);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (fabs(a[i][j] > ans)) {
                ans = fabs(a[i][j]);
            }
        }
    }
    return ans;
}

double
cond_num(double **a, double **rev)
{
    return max_mod(a) * max_mod(rev);
}

double
*copy_vec(double *f)
{
    double *ans = get_vec();
    for (int i = 0; i < n; ++i) {
        ans[i] = f[i];
    }
    return ans;
}

int
main(int argc, char* argv[])
{  
    n = 10;
    double **a = get_memory_matr();
    double **rev = get_memory_matr();
    double *f = calloc(n, sizeof(double));
    double input;
    scanf("%lf", &input);
    gen_a(a);
    gen_right(f, input);
    

    double *x = get_vec();
    double **a1 = copy_matr(a);
    double *f1 = copy_vec(f);
    get_ans(a, f, OTHER, x);
    printf("Решение методом Гаусса:\n");
    print_vec(x, 1); //выводим ответ
    free_memory(a);
    a = a1;
    a1 = copy_matr(a);
    free(f);
    f = f1;
    get_ans(a, f, MAIN_ELEM, x);
    printf("Решение методом гаусса с выбором главного элемента:\n");
    print_vec(x, 1);
    free(f);
    free(x);
    
    a1 = copy_matr(a);
    get_reverse_matr(a, rev, MAIN_ELEM);
    free(a);
    printf("Обратная матрица:\n");
    print_matr(rev, NULL);

    printf("Число обусловленности:\n");
    printf("%lf\n", cond_num(a1, rev));
    free_memory(a1);
    free_memory(rev);
    return 0;
}



