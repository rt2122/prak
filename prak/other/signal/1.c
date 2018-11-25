#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

enum
{
    MAX = 5,
    BUF = 3
};

volatile sig_atomic_t flag = 1;
int i = 0;

void
handler(int signo) {
    if (i != MAX - 1) {
        char s[BUF];
        snprintf(s, BUF, "%d\n", i);
        write(1, s, sizeof(s) - 1);
        fflush(stdout);
    }
    flag = 1;
}

int
main(void)
{
    printf("%d\n", getpid());
    sigset_t mask, oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaction(SIGINT, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    while (i < MAX) {
        sigprocmask(SIG_BLOCK, &mask, &oldmask);
        while (!flag) {
            sigsuspend(&oldmask);
            ++i;
        }
        sigprocmask(SIG_UNBLOCK, &mask, NULL);
        flag = 0;

    }
    exit(0);
}