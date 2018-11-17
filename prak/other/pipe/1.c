#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

enum
{
    N_FD = 2,
    BUF_SIZE = 6
};

int
main(void)
{
    int fd[N_FD];
    struct tm *stm;
    time_t t = time(NULL);
    pipe(fd);
    write(fd[1], &t, sizeof(t));
    close(fd[1]);
    if (!fork()) {
        //son
        if (!fork()) {
            //grandson
            if (!fork()) {
                //great-grandson
                read(fd[0], &t, sizeof(t));
                stm = localtime(&t);
                close(fd[0]);
                printf("S:%*d\n", N_FD, stm->tm_sec);
                exit(0);
            }
            wait(NULL);
            read(fd[0], &t, sizeof(t));
            stm = localtime(&t);
            close(fd[0]);
            printf("M:%*d\n", N_FD, stm->tm_min);
            exit(0);
        }
        wait(NULL);
        read(fd[0], &t, sizeof(t));
        stm = localtime(&t);
        close(fd[0]);
        printf("H:%*d\n", N_FD, stm->tm_hour);
        exit(0);

    }
    wait(NULL);
    return 0;
}