#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

enum { MAX_SIG = 4 };

long long prime = 0;
int n_sig = 0;

void handler(int sig) {
    if (sig == SIGINT && n_sig < MAX_SIG - 1) {
        printf("%lld\n", prime);
        ++n_sig;
        fflush(stdout);
    } else {
        exit(0);
    }
}

int isprime(long long x) {
    if (x == 0 || x == 1) {
        return 0;
    }
    long long max = (long long) sqrt(x) + 1;
    for (long long i = 2; i < max; ++i) {
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
    if (low >= high || high < 0) {
        return 1;
    }
    low = low < 0 ? 0 : low;
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
