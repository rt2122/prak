#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

enum { N_FD = 2 };

int sys_check(int x, int *pids, int n_proc) {
    if (x == -1) {
        for (int i = 0; i < n_proc && pids[i]; ++i) {
            kill(pids[i], SIGKILL);
        }
        free(pids);
        _exit(1);
    }
    return x;
}
int
main(int argc, char **argv)
{
    if (argc < 2) {
        _exit(0);
    }
    int fd[N_FD];
    int i;
    int *pids = calloc(argc - 1, sizeof(int));
    for (i = 1; i < argc - 1; i++) {
        sys_check(pipe(fd), pids, argc - 1);
        pids[i - 1] = sys_check(fork(), pids, argc - 1);
        if (!pids[i - 1]) {
            close(fd[0]);
            dup2(fd[1], 1);
            close(fd[1]);
            execlp(argv[i], argv[i], NULL);
            _exit(1);
        }
        sys_check(close(fd[1]), pids, argc - 1);
        sys_check(dup2(fd[0], 0), pids, argc - 1);
        sys_check(close(fd[0]), pids, argc - 1);
    }
    pids[i] = sys_check(fork(), pids, argc - 1);
    if (!pids[i]) {
        execlp(argv[argc - 1], argv[argc - 1], NULL);
        _exit(1);
    }
    sys_check(close(0), pids, argc - 1);
    sys_check(close(1), pids, argc - 1);
    while (wait(NULL) != -1) {}
    free(pids);
    _exit(0);
}
