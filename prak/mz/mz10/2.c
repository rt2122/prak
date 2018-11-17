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
    pipe(fd);
    int son, grandson;
    if (!(son = fork())) {
        if (!(grandson = fork())) {
            close(fd[1]);
            long long int sum = 0;
            int num, r;
            while ((r = read(fd[0], &num, sizeof(num))) != -1 && r != 0) {
                sum += num;
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