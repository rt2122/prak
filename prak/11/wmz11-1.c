#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

enum { MAX = 6 };

volatile int i = 0;

void
handler(int signo) {
    if (i != MAX - 1) {
        printf("%d\n", i);
        fflush(stdout);
        ++i;
    } else {
        _exit(0);
    }
}

int
main(void)
{
    sigset_t mask, oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGHUP);
    sigprocmask(SIG_BLOCK, &mask, &oldmask);
    sigaction(SIGHUP, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    while (1) {
        sigsuspend(&oldmask);
    }
    _exit(0);
}
