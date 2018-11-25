#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

volatile sig_atomic_t square_mode = 0;

void
handler(int sig)
{
    if (sig == SIGUSR1) {
        square_mode = 0;
    } else {
        square_mode = 1;
    }
}

long long
f(long long x)
{
    if (square_mode) {
        return x * x;
    } else {
        return -x;
    }
}

int
main(void)
{
    printf("%d\n", getpid());
    struct sigaction ssi = { .sa_handler = handler, .sa_flags = SA_RESTART };
    sigaction(SIGUSR1, &ssi, NULL);
    sigaction(SIGUSR2, &ssi, NULL);
    long long x;
    while (scanf("%lld", &x) == 1) {
        printf("%lld", f(x));
        fflush(stdout);
    }
    exit(0);
}