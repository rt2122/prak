#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void
handler(int sig)
{
    printf("\nYOU SIGNALED ME????\n");
}

int
main(void)
{
    sigset_t mask, oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigprocmask(SIG_BLOCK, &mask, &oldmask);
    sigaction(SIGINT, &(struct sigaction){ .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    sleep(10);
    while (1) {
        sigsuspend(&oldmask);
    }
    exit(0);
}
