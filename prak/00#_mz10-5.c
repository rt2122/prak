#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>

enum { N_FD = 2 };

void work(int rfd, int wfd, int numproc, int max)
{
    int x;
    while (read(rfd, &x, sizeof(x)) == sizeof(x)) {
        ++x;
        printf("%d %d\n", numproc, x - 1);
        fflush(stdout);
        if (x == max) {
            break;
        }
        write(wfd, &x, sizeof(x));
    }
}

int do_proc(int cl1, int cl2, int w1, int w2, int num, int max) {
    int son = fork();
    if (son < 0) {
        exit(1);
    }
    if (!son) {
        close(cl1);
        close(cl2);
        work(w1, w2, num, max);
        _exit(1);
    }
}

int main()
{
    int p12[N_FD];
    int p21[N_FD];

    if (pipe(p12) < 0) {
        exit(1);
    }
    if (pipe(p21) < 0) {
        exit(1);
    }
    int son[N_FD];
    int number;
    do_proc(p21[1], p12[0], p21[0], p12[1], 1, number);
    do_proc(p12[1], p21[0], p12[0], p21[1], 2, number);
    scanf("%d", &number);
    int z = 1;
    write(p21[1], &z, sizeof(z));
    close(p12[0]); close(p12[1]);
    close(p21[0]); close(p21[1]);
    wait(NULL);
    wait(NULL);
    printf("Done\n");
}