#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>

volatile int flag = 0;

void handler()
{
    flag = 1;
}

int main(int argc, char **argv)
{
    int max, num;
    sscanf(argv[1], "%d", &max);
    sigset_t s1, s2;
    sigemptyset(&s1);
    sigaddset(&s1, SIGUSR1);
    sigprocmask(SIG_BLOCK, &s1, &s2);
    sigaction(SIGUSR1, &(struct sigaction){ .sa_handler = handler }, NULL);
    int fd[2];
    pipe(fd);
    int pid[2];
    int pid1, pid2;
    pid1 = fork();
    if (pid1) {
        pid2 = fork();
        if (pid2) {
            int x = 1;
            write(fd[1], &x, sizeof(x));
            x = pid2;
            write(fd[1], &x, sizeof(x));
            x = 1;
            write(fd[1], &x, sizeof(x));
            kill(pid1, SIGUSR1);
            wait(NULL);
            wait(NULL);
            _exit(0);
        } else {
            num = 2;
        }
    } else {
        num = 1;
    }

    while (1) {
        while (!flag) {
            sigprocmask(SIG_SETMASK, &s2, NULL);
            pause();
            sigprocmask(SIG_BLOCK, &s1, NULL);
        }

        while (!flag) {
            sigsuspend(&s2);
        }

        flag = 0;
        int x, pid, num;
        read(fd[0], &x, sizeof(x));
        read(fd[0], &pid, sizeof(pid));
        read(fd[0], &num, sizeof(num));
        if (!x) {
            _exit(0);
        }
        printf("%d %d\n", num, x);
        ++x;
        if (x == max) {
            x = 0;
        }
        int mypid = getpid();
        num = 1 - num;
        write(fd[1], &x, sizeof(x));
        write(fd[1], &mypid, sizeof(mypid));
        write(fd[1], &num, sizeof(num));
        kill(pid, SIGUSR1);
        if (!x) {
            _exit(0);
        }
    }
}