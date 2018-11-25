#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int sys_check(int x) {
    if (x == -1) {
        _exit(1);
    }
    return x;
}

int
main(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i) {
        if (!sys_check(fork())) {
            execlp(argv[i], argv[i], NULL);
            _exit(1);
        }
        int status;
        wait(&status);
        if (WIFSIGNALED(status) || WIFEXITED(status) && WEXITSTATUS(status)) {
            _exit(1);
        }
    }
    _exit(0);
}