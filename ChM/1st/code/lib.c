int n;

void
nn(void)
{
    printf("\n");
}

void
print_matr(double **matr, double *f)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; ++j) {
            printf("%.4lf ", matr[i][j]);
        }
        if (f) {
            printf("|%.4lf\n", f[i]);
        } else {
            nn();
        }
    }
    nn();
}

double 
**get_memory_matr(void)
{
    double **ret = (double **)calloc(n, sizeof(double *));
    for (int i = 0; i < n; ++i) {
        ret[i] = (double *)calloc(n, sizeof(double));
    }
    return ret;
}

void
free_memory(double **matr)
{
    for (int i = 0; i < n; ++i) {
        free(matr[i]);
    }
    free(matr);
}

double
**copy_matr(double **orig)
{
    double **copy = get_memory_matr();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            copy[i][j] = orig[i][j];
        }
    }
    return copy;
}

void
switch_lines(double **a, double **b)
{
    double *tmp = *a;
    *a = *b;
    *b = tmp;
}

void
switch_main(double **matr, double **reverse, int num_zeros)
{
    double max = fabs(matr[num_zeros][num_zeros]);
    int pos = num_zeros;
    for (int i = num_zeros + 1; i < n; i++) {
        if (fabs(matr[i][num_zeros]) > max) {
            pos = i;
            max = fabs(matr[i][num_zeros]);
        }
    }
    if (pos == num_zeros) {
        return;
    }
    switch_lines(&matr[pos],&matr[num_zeros]);
    switch_lines(&reverse[pos], &reverse[num_zeros]);
}

void
print_two_matr(double **a, double **b)
{
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%*lf ", 5, a[i][j]);
        }
        printf("|");
        for (int j = 0; j < n; ++j) {
            printf("%*lf ", 5, b[i][j]);
        }
        nn();
    }
    nn();
}

void
get_i_matr(double **ret)
{
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; j++) {
            ret[i][j] = (i == j);
        }
    }
}

void
print_vec(double *f, double mul)
{
    for (int i = 0; i < n; ++i) {
        printf("%lf ", f[i] * mul);
    }
    nn();
}


double
*get_vec(void)
{
    return calloc(n, sizeof(double));
}

void
read_matr(FILE *file, double **a, double *f)
{
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            fscanf(file, "%lf", &a[i][j]);
        }
        if (f) {
            fscanf(file, "%lf", &f[i]);
        }
    }
}