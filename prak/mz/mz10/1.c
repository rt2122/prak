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

int
main(void)
{
    char buf[BUF_SIZE];
    int fd[N_FD];
    struct tm *stm;
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
            close(fd[1]);
            wait(NULL);
            read(fd[0], &t, sizeof(t));
            stm = localtime(&t);
            strftime(buf, BUF_SIZE, "D:%d", stm);
            close(fd[0]);
            printf("%s\n", buf);
            exit(0);
        }
        close(fd[1]);
        wait(NULL);
        read(fd[0], &t, sizeof(t));
        stm = localtime(&t);
        strftime(buf, BUF_SIZE, "M:%m", stm);
        close(fd[0]);
        printf("%s\n", buf);
        exit(0);

    }
    close(fd[1]);
    wait(NULL);
    read(fd[0], &t, sizeof(t));
    stm = localtime(&t);
    close(fd[0]);
    printf("%Y:%d\n", stm->tm_year);
    exit(0);
}