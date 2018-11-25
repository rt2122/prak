#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

volatile sig_atomic_t mul_mode = 0;

void
handlerint(int sig)
{
    mul_mode = 0;
}

void
handlerq(int sig)
{
    mul_mode = 1;
}

int
main(void)
{
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);
    struct sigaction ssiint = { .sa_handler = handlerint, .sa_flags = SA_RESTART };
    struct sigaction ssiq = { .sa_handler = handlerq, .sa_flags = SA_RESTART };
    sigaction(SIGINT, &ssiint, NULL);
    sigaction(SIGQUIT, &ssiq, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    int full = 0;
    int x;
    while (scanf("%d", &x) == 1) {
        sigprocmask(SIG_BLOCK, &mask, NULL);
        if (mul_mode) {
            full *= (long long) x;
        } else {
            full += (long long) x;
        }
        printf("%d\n", full);
        fflush(stdout);
        sigprocmask(SIG_UNBLOCK, &mask, NULL);
    }
    _exit(0);
}