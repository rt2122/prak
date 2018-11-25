#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>

volatile sig_atomic_t flag = 0;

void handler()
{
    flag = 1;
}

int main(int argc, char **argv)
{
    sigset_t s1, s2;
    sigemptyset(&s1);
    sigaddset(&s1, SIGUSR1);
    sigprocmask(SIG_BLOCK, &s1, &s2);
    sigaction(SIGUSR1, &(struct sigaction){ .sa_handler = handler }, NULL);
    int fd[2];
    pipe(fd);
    int x = 1, max;
    int pid = fork();
    if (!pid) {
        pid = getppid();
    } else {
        sscanf(argv[1], "%d", &max);
        write(fd[1], &x, sizeof(x));
        //close(fd[0]);
        //close(fd[1]);
        kill(pid, SIGUSR1);
        while (wait(NULL) != -1);
        exit(0);
    }
    while (x <= max) {
        while (!flag) {
            sigsuspend(&s2);
        }
        flag = 0;
        read(fd[0], &x, sizeof(x));
        printf("%d %d\n", getpid(), x);
        ++x;
        write(fd[1], &x, sizeof(x));
        kill(pid, SIGUSR1);
    }
    exit(0);
}