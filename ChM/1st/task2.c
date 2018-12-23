#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

const double eps = 1e-12;

void
alloc_matrix(double ***a, int n)
{
    *a = calloc(n, sizeof(**a));
    for (int i = 0; i < n; i++) {
        (*a)[i] = calloc(n, sizeof(***a));
    }
}

void
print_solution(double *b, int n)
{
    printf("Ответ:\n");
    for (int i = 0; i < n; i++) {
        printf("x%d = %lf\n", i + 1, b[i]);
    }
}

void
relaxation(double **a, double *b, int n)
{
    double s, w;
    double *prev = calloc(n, sizeof(*prev));
    double *cur = calloc(n, sizeof(*cur));
    int count = 0;
    for (w = 0.05; w < 2; w += 0.05) {
        for (int i = 0; i < n; i++) {
            prev[i] = cur[i] = 0;
        }
        do {
            count++;
            for (int i = 0; i < n; i++) {
                cur[i] = 0;
                for (int j = 0; j < i; j++) {
                    cur[i] += a[i][j] * cur[j];
                }
                for (int j = i; j < n; j++) {
                    cur[i] += a[i][j] * prev[j];
                }
                cur[i] = prev[i] + w / a[i][i] * (b[i] - cur[i]);
            }
            s = 0;
            for (int i = 0; i < n; i++) {
                s += (prev[i] - cur[i]) * (prev[i] - cur[i]);
            }
            for (int i = 0; i < n; i++) {
                prev[i] = cur[i];
            }
        } while (fabs(s) > eps);
        printf("w = %.2f\titer.count = %d\n", w, count);
        count = 0;
    }
    print_solution(cur, n);
    free(prev);
    free(cur);
}

void
matrix_filling(int n, int m, double **a, double *b)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                a[i][j] = n + m * m + (double)j / m + (double)i / n;
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

    printf("Введите режим\n");
    scanf("%d", &fl); 

    if (fl == 1) {
        printf("Введите порядок матрицы\n");
        scanf("%d", &n);
        printf("\n");
        alloc_matrix(&a, n);
        b = calloc(n, sizeof(*b));

        FILE *file = fopen(argv[1], "r");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                fscanf(file, "%lf", &a[i][j]);
            }
            fscanf(file, "%lf", &b[i]);
        }
        fclose(file);
        relaxation(a, b, n);
    } else {

        int m;
        scanf("%d", &n);
        scanf("%d", &m);
        alloc_matrix(&a, n);
        b = calloc(n, sizeof(*b));
        matrix_filling(n, m, a, b);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                   inv[i][j] = a[i][j];
            }
        }
        relaxation(a, b, n);
    }
    for (int i = 0; i < n; i++) {
        free(a[i]);
    }
    free(a);
    free(b);
    return 0;
}
