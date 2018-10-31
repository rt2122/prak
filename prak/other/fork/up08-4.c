#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>

int rec(int n, int max) {
    if (n > max) {
        //printf("\n");
        //fflush(stdout);
        return 0;
    }
    int pid = fork();
    if (pid) {
        //printf("%d %d %d\n", n, getpid(), getppid());
        if (n == max) {
             printf("%d");
        } else
             printf("%d ", n);
        fflush(stdout);
        wait(NULL);
        if (n == 1) {
            //printf("%d father is alive\n", n);
            printf("\n");
        }
        exit(0);
    } else {
        rec(n + 1, max);
    }
}

int main()
{
    //printf("%d\n", getpid());
    int n;
    scanf("%d", &n);
    rec(1, n);
    //printf("\n");
    //printf("%d\n", getpid());
    return 0;
}
