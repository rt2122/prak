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
    int num;
    while (scanf("%d", &num) == 1) {
        write(fd[1], &num, sizeof(num));
    }
    close(fd[1]);
    if (!fork()) {
        if (!fork()) {
            close(fd[1]);
            int sum = 0;
            while (read(fd[0], &num, sizeof(num)) == sizeof(num)) {
                sum += num;
            }
            printf("%d\n", sum);
            exit(0);
        }
        close(fd[1]);
        wait(NULL);
        exit(0);
    }
    wait(NULL);
    exit(0);
}