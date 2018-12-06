#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

enum
{
    USR_ONE = 5,
    USR_TWO = -4
};

volatile int i = 0;
volatile int flag = 0;

void
handler(int sig)
{
    if (sig == SIGUSR1) {
        i += USR_ONE;
    }
    if (sig == SIGUSR2) {
        i += USR_TWO;
    }
    printf("%d %d\n", sig, i);
    fflush(stdout);
    if (i < 0) {
        exit(0);
    }
}

int
main(int argc, char *argv[])
{
    sigset_t mask, oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);

    sigprocmask(SIG_BLOCK, &mask, &oldmask);
    sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);    
    sigaction(SIGUSR2, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);    
    printf("%d\n", getpid());
    fflush(stdout);

    while (1) {
        while (!flag) {
            sigsuspend(&oldmask);
        }
        flag = 0;
    }
    return 0;
}
