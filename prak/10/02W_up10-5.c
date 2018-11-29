#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

enum { N_FD = 2 };

int sys_check(int x) {
    if (x == -1) {
        _exit(1);
    }
    return x;
}

void work(FILE *w, FILE *r, int numproc, int max)
{
    int x;
    while (fscanf(r, "%d", &x) == 1 && x < max) {
        ++x;
        printf("%d %d\n", numproc, x - 1);
        fflush(stdout);
        fprintf(w, "%d ", x);
        fflush(w);
    }
    fclose(w);
    fclose(r);
}

void do_proc(FILE *cl1, FILE *cl2, FILE *w, FILE *r, int num, int max) {
    int son = sys_check(fork());
    if (!son) {
        fclose(cl1);
        fclose(cl2);
        work(w, r, num, max);
        _exit(0);
    }
}

int
main(int argc, char **argv)
{
    if (argc < 2) {
        return 1;
    }
    int p12[N_FD];
    int p21[N_FD];

    if (pipe(p12) < 0) {
        exit(1);
    }
    if (pipe(p21) < 0) {
        exit(1);
    }
    int number;
    sscanf(argv[1], "%d", &number);

    FILE *w1 = fdopen(p12[1], "w");
    FILE *r1 = fdopen(p21[0], "r");
    FILE *w2 = fdopen(p21[1], "w");
    FILE *r2 = fdopen(p12[0], "r");
    do_proc(w2, r2, w1, r1, 1, number);
    do_proc(w1, r1, w2, r2, 2, number);

    fprintf(w2, "1 ");
    fclose(w1);
    fclose(r1);
    fclose(w2);
    fclose(r2);
    while (wait(NULL) != -1);
    printf("Done\n");
    return 0;
}