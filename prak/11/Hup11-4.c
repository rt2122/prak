#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>

enum 
{ 
    BORDER = 2,
    N_FD = 2,
    N_NUMS = 2
};

volatile int flag = 0;

void handler(int sig)
{
    flag = 1;
}

int main(int argc, char **argv)
{
    int max, num = 0;
    if (argc < 2) {
        return 1;
    }
    sscanf(argv[1], "%d", &max);
    if (max < BORDER) {
        printf("Done\n");
        return 0;
    }
    sigset_t mask, oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &mask, &oldmask);
    sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    int fd[N_FD];
    pipe(fd);
    FILE *r = fdopen(fd[0], "r");
    FILE *w = fdopen(fd[1], "a");
    int pid1, pid2;
    pid1 = fork();
    if (pid1) {
        pid2 = fork();
        if (pid2) {
            fprintf(w, "1 %d ", pid2);
            fflush(w);
            kill(pid1, SIGUSR1);
            while (wait(NULL) > 0 ) {}
            printf("Done\n");
            _exit(0);
        } else {
            num = 2;
        }
    } else {
        num = 1;
    }

    while (4) {
        while (!flag) {
            sigsuspend(&oldmask);
        }

        flag = 0;
        int x, pid, er = fscanf(r, "%d%d", &x, &pid);
        if (er != N_NUMS) {
            printf("%d\n", pid);
            kill(pid, SIGUSR1);
            _exit(0);
        }
        if (x == max) {
            fclose(r);
            fclose(w);
            kill(pid, SIGUSR1);
            _exit(0);
        }
        printf("%d %d\n", num, x);
        ++x;
        int mypid = getpid();
        fprintf(w,"%d %d ", x, mypid);
        fflush(w);
        if (x == max - 1 || max == BORDER) {
            fclose(w);
            fclose(r);
        }
        kill(pid, SIGUSR1);
    }
}
