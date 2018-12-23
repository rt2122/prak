#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//выделение памяти на матрицу
void
alloc_matrix(double ***a, int n)
{
    *a = calloc(n, sizeof(**a));
    for (int i = 0; i < n; i++) {
        (*a)[i] = calloc(n, sizeof(***a));
    }
}

//вывод ответа
void
print_solution(double *b, int n)
{
    printf("Ответ:\n");
    for (int i = 0; i < n; i++) {
        printf("x%d = %lf\n", i + 1, b[i]);
    }
}

//вывод матрицы
void
print_matrix(double **a, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%lf ", a[i][j]);
        }
        printf("\n");
    }
}

//заполнение единичной матрицы
void
iden_matrix(double **a, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                a[i][j] = 1.0;
            } else {
                a[i][j] = 0.0;
            }
        }
    }
}

//вычисление определителя верхней треугольной матрицы
double
determinant(double **a, int n)
{
    double det = 1;
    for (int i = 0; i < n; i++) {
        det *= a[i][i];
    }
    return det;
}

//вычисление обратной матрицы
void
inv_matrix(double **a, double **idm, int n)
{
    double f_el, del;
    for(int i = 0; i < n; i++) {
        del = a[i][i];
        for(int j = 0; j < n; j++) {
            a[i][j] /= del;
            idm[i][j] /= del;
        }
        for(int g = 0; g < n; g++) {
            if (g != i) {
                f_el = a[g][i];
                for(int j = 0; j < n; j++) {
                    a[g][j] -= a[i][j] * f_el;
                    idm[g][j] -= idm[i][j] * f_el;

                }
            }
        }
    }
}

//поиск главного элемента + поменять две строки местами
int
swap(double **a, double *b, int i, int n)
{
    int j, max_ind = -1;
    double max = 0, tmp;
    for (j = i; j != n; j++) {
        if (fabs(a[j][i]) > max) {
            max = fabs(a[j][i]);
            max_ind = j;
        }
    }
    if (max_ind == -1) {
        return -1;
    }
    if (max_ind == i) {
        return 0;
    }
    for (int g = 0; g < n; g++) {
        tmp = a[i][g];
        a[i][g] = a[max_ind][g];
        a[max_ind][g] = tmp;
    }
    tmp = b[i];
    b[i] = b[max_ind];
    b[max_ind] = tmp;
    return 1;
}

//прямой ход
double
forward(double **a, double *b, int n)
{
    double f_el;
    int sw;
    double det = 1;

    for (int i = 0; i < n - 1; i++) {
        sw = swap(a, b, i, n);
        if (sw == -1) {
            printf("Матрица вырожденная\n");
            return 0;
        }
        if (sw == 1) {
            det = -det;
        }
        for(int g = i + 1; g < n; g++) {
            f_el = a[g][i];
            for(int j = i; j < n; j++) {
                a[g][j] -= a[i][j] * f_el / a[i][i];
            }
            b[g] -= b[i] * f_el / a[i][i];
        }
    }
    return det;
}

//обратный ход
void
backward(double **a, double *b, int n)
{
    double l_el;
    for (int i = n - 1; i > 0; i--) {
        b[i] /= a[i][i];
        for (int g = i - 1; g >= 0; g--) {
            l_el = a[g][i];
            for (int j = n - 1; j >= i; j--) {
                a[g][j] -= a[i][j] * l_el / a[i][i];
            }
            b[g] -= b[i] * l_el;
        }
    }
}

//метод Гаусса
void
Gauss_elim(double **a, double *b, double **inv, int n)
{
    double det = forward(a, b, n, i);
    if (!det) {
        return;
    }
    double **idm;
    alloc_matrix(&idm, n);
    iden_matrix(idm, n);

    det *= determinant(a, n);
    backward(a, b, n);
    inv_matrix(inv, idm, n);
//вывод
    printf("Определитель: %lf\n", det);
    printf("Обратная матрица:\n");
    print_matrix(idm, n);
    print_solution(b, n);
}

void
matrix_filling(int n, int m, double **a, double *b)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                a[i][j] = n + m * m + j / m + i / n;
            } else {
                a[i][j] = (double)(i + j) / (m + n);
            }
        }
        b[i] = m * i + n;
    }
}

int
main(int argc, char **argv) // argv[1]: filename
{
    int n, fl;
    double **a, *b, **inv;

    printf("Введите подвариант\n");
    scanf("%d", &fl); // fl = 1, если задание элементов, fl = 2, если формулами

    if (fl == 1) {
        printf("Введите порядок матрицы\n");
        scanf("%d", &n);
        printf("\n");
        alloc_matrix(&a, n);
        alloc_matrix(&inv, n);
        b = calloc(n, sizeof(*b));

        //считывание элементов матрицы
        FILE *file = fopen(argv[1], "r");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                fscanf(file, "%lf", &a[i][j]);
                inv[i][j] = a[i][j];
            }
            fscanf(file, "%lf", &b[i]);
        }
        fclose(file);
        Gauss_elim(a, b, inv, n);
    } else {

        // задание эл-ов матрицы специальными формулами
        int m;
        scanf("%d", &n);
        scanf("%d", &m);
        alloc_matrix(&a, n);
	   alloc_matrix(&inv, n);
        b = calloc(n, sizeof(*b));
        matrix_filling(n, m, a, b);
	   for (int i = 0; i < n; i++) {
	       for (int j = 0; j < n; j++) {
    	           inv[i][j] = a[i][j];
            }
        }
        Gauss_elim(a, b, inv, n);
    }
    return 0;
}
