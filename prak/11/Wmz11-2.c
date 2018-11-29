#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

enum
{
    MUL = 0,
    ADD = 1
};

volatile sig_atomic_t mode = ADD;

void
handlerint(int sig)
{
    mode = ADD;
}

void
handlerq(int sig)
{
    mode = MUL;
}

int
main(void)
{
    struct sigaction ssiint = { .sa_handler = handlerint, .sa_flags = SA_RESTART };
    struct sigaction ssiq = { .sa_handler = handlerq, .sa_flags = SA_RESTART };
    sigaction(SIGINT, &ssiint, NULL);
    sigaction(SIGQUIT, &ssiq, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    int full = 0;
    int x;
    while (scanf("%d", &x) == 1) {
        if (mode == MUL) {
            full *= (long long) x;
        } else {
            full += (long long) x;
        }
        printf("%d\n", full);
        fflush(stdout);
    }
    _exit(0);
}
