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

void
handler1(int sig)
{
    i += USR_ONE;
    printf("%d %d\n", sig, i);
    fflush(stdout);
}

void
handler2(int sig)
{
    i += USR_TWO;
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
    sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = handler1, .sa_flags = SA_RESTART }, NULL);    
    sigaction(SIGUSR2, &(struct sigaction) { .sa_handler = handler2, .sa_flags = SA_RESTART }, NULL);    
    printf("%d\n", getpid());
    fflush(stdout);

    while (1) {
        sigsuspend(&oldmask);
    }
    return 0;
}
