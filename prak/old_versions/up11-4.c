#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// классический пинг-понг между отцом и сыном с сигналами

enum { N_FD = 2 };

int fd[N_FD];

void
handler(int s)
{
    int x;
    read(fd[0], &x, sizeof(x));
    printf("%d %d\n", num, x);
    fflush(stdout);
    ++x;
    write(fd[1], &x, sizeof(x));
    kill(pid, SIGUSR1);
}

int
main(int argc, char **argv)
{
    pipe(fd);
    signal(SIGUSR1, handler);
    pid = fork();
    if (!pid) {
        pid = getppid();
        while (1) {
            pause();
        }
    } else {
        int z = 1;
        write(fd[1], &z, sizeof(z));
        kill(pid, SIGUSR1);
        while (1) {
            pause();
        }
    }
    exit(0);
}
