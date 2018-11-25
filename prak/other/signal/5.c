#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

enum { MAX_SIG = 4 };

long long prime = 0;
int n_sig = 0;

void handler(int sig) {
    if (sig == SIGINT && n_sig < MAX_SIG) {
        printf("%lld\n", prime);
        fflush(stdout);
    } else {
        exit(0);
    }
}

long long isprime(long long x) {
    for (int i = 2; i < x / 2; ++i) {
        if (x % i == 0) {
            return 0;
        }
    }
    return 1;
}

int
main(int argc, char **argv)
{
    long long low, high;
    struct sigaction ssi = { .sa_handler = handler, .sa_flags = SA_RESTART };
    sigaction(SIGINT, &ssi, NULL);
    sigaction(SIGTERM, &ssi, NULL);
    scanf("%lld%lld", &low, &high);
    printf("%d\n", getpid());
    fflush(stdout);
    for (long long i = low; i < high; ++i) {
        if (isprime(i)) {
            prime = i;
        }
    }
    printf("-1\n");
    fflush(stdout);
    exit(0);
}