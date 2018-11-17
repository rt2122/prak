#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

enum { N_FD = 2 };

int
main(void)
{
    int fd[N_FD];
    if (pipe(fd) < 0) {
        exit(1);
    }
    int son, grandson;
    son = fork();
    if (son < 0) {
        exit(1);
    }
    if (!son) {
        grandson = fork();
        if (grandson < 0) {
            exit(1);
        }
        if (!grandson) {
            close(fd[1]);
            long long int sum = 0;
            int num, r;
            while ((r = read(fd[0], &num, sizeof(num))) == sizeof(num)) {
                sum += num;
            }
            if (r == -1) {
                exit(1);
            }
            close(fd[0]);
            printf("%lld\n", sum);
            fflush(stdout);
            exit(0);
        } else {
            close(fd[0]);
            close(fd[1]);
            waitpid(grandson, NULL, 0);
            exit(0);
        }

    } else {
        close(fd[0]);
        int num;
        while (scanf("%d", &num) == 1) {
            write(fd[1], &num, sizeof(num));
        }
        close(fd[1]);
        waitpid(son, NULL, 0);
    }
    exit(0);
}