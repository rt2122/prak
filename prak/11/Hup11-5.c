#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

enum { N_FD = 2 };
volatile sig_atomic_t flag = 0;

void handler(int sig)
{
    flag = 1;
}

int
main(int argc, char *argv[])
{
    sigset_t mask, oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &mask, &oldmask);
    sigaction(SIGUSR1, &(struct sigaction){ .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    int max;
    sscanf(argv[1], "%d", &max);
    int num = 0, pid1, pid2, n_sig = 2;
    int fd[N_FD];
    pipe(fd);
    FILE *r = fdopen(fd[0], "r");
    FILE *w = fdopen(fd[1], "w");
    pid1 = fork();
    if (!pid1) {
        num = 1;
    } else {
        pid2 = fork();
        if (!pid2) {
            num = 2;
        } else {
            fprintf(w, "%d %d ", pid2, 1);
            kill(pid1, SIGUSR1);
        }
    }
    while (!flag) {
        //while (!flag) {
            sigsuspend(&oldmask);
        //}
        sigprocmask(SIG_UNBLOCK, &oldmask, NULL);
        sigprocmask(SIG_BLOCK, &mask, &oldmask);
        flag = 0;
        if (num) {
            int pid, x;
            if (fscanf(r, "%d %d", &pid, &x) == -1 || x == max) {
                fclose(w);
                fclose(r);
                kill(getppid(), SIGUSR1);
                _exit(0);
            }
            printf("%d %d\n", num, x);
            ++x;
            fprintf(w, "%d %d ", getpid(), x);
            kill(pid, SIGUSR1);
        } else {
            n_sig--;
            if (!n_sig) {
                _exit(0);
            }
        }
    }
    _exit(0);
}
