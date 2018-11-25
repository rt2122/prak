#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

enum { N_FD = 2 };

int sys_check(int x) {
    if (x == -1) {
        kill(0, SIGKILL);
        _exit(1);
    }
    return x;
}
int
main(int argc, char **argv)
{
    if (argc < 2) {
        _exit(1);
    }
    int fd[N_FD];
    int i, pid;
    for (i = 1; i < argc - 1; i++) {
        sys_check(pipe(fd));
        pid = sys_check(fork());
        if (!pid) {
            sys_check(close(fd[0]));
            sys_check(dup2(fd[1], 1));
            sys_check(close(fd[1]));
            execlp(argv[i], argv[i], NULL);
            sys_check(-1);
        }
        sys_check(close(fd[1]));
        sys_check(dup2(fd[0], 0));
        sys_check(close(fd[0]));
    }
    pid = sys_check(fork());
    if (!pid) {
        execlp(argv[argc - 1], argv[argc - 1], NULL);
        sys_check(-1);
    }
    while (wait(NULL) != -1);
    _exit(0);
}