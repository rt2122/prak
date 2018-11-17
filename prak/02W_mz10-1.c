#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

enum
{
    N_FD = 2,
    BUF_SIZE = 6,
    FIRST_YEAR = 1900,
    TIMES = 3
};

int after_fork(int fd[N_FD], const char *fmt) {
    close(fd[1]);
    wait(NULL);
    time_t t;
    read(fd[0], &t, sizeof(t));
    struct tm *stm;
    stm = localtime(&t);
    if (fmt[0] == 'Y') {
        printf(fmt, stm->tm_year + FIRST_YEAR);
        exit(0);
    }
    char buf[BUF_SIZE];
    strftime(buf, BUF_SIZE, fmt, stm);
    close(fd[0]);
    printf("%s\n", buf);
    exit(0);
}

int
main(void)
{
    int fd[N_FD];
    pipe(fd);
    time_t t;
    if (!fork()) {
        if (!fork()) {
            if (!fork()) {
                t = time(NULL);
                for (int i = 0; i < TIMES; i++) {
                    write(fd[1], &t, sizeof(t));
                }
                close(fd[1]);
                close(fd[0]);
                exit(0);
            }
            after_fork(fd, "D:%d");
        }
        after_fork(fd, "M:%m");
    }
    after_fork(fd, "Y:%d\n");
    exit(0);
}