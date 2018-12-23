#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lib.c"
double eps = 0.0001;
double shift = 0.237;

double 
*solve(double **matr, double *f)
{
    double *cur = calloc(n, sizeof(*cur));
    double curSolution = 0;

    for (double w = shift; w < 2; w += shift) {
        double *prev = calloc(n, sizeof(*prev));
        for (int i = 0; i < n; ++i) {
            cur[i] = 0;
        }
        int count = 0;
        do {
            ++count;
            double *tmp = cur;
            cur = prev;
            prev = tmp;
            for (int i = 0; i < n; ++i) {
                cur[i] = mat[i][n] * w + mat[i][i] * prev[i] * (1.0 - w);
                for (int j = 0; j < i; ++j) {
                    cur[i] -= mat[i][j] * cur[j] * w;
                }
                for (int j = i + 1; j < n; ++j) {
                    cur[i] -= mat[i][j] * prev[j] * w;
                }
                cur[i] /= mat[i][i];
            }
            curSolution = 0;
            for (int i = 0; i < n; i++) {
                curSolution += fabs(cur[i] - prev[i]);
                prev[i] = cur[i];
            }
        } while (fabs(curSolution) > eps);
        print_vec(cur, 1);
        free(prev);
    }
    return cur;
}


int main() {

        n = 100, m = 4;
        double **mat = get_memory_matr();
        double *f = get_vec();
        double *ans = solve(a, f);

        print_vec(ans);
        free(ans);
        free_memory(a);
        free(f);
    return 0;
}
