#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

enum { N_FD = 2 };

int
main(int argc, char **argv)
{
    if (argc < 2) {
        return 0;
    }
    int fd[N_FD];
    int i;
    for (i = 1; i < argc - 1; i++) {
        pipe(fd);
        if (!fork()) {
            dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);
            execlp(argv[i], argv[i], NULL);
            _exit(1);
        }
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
    }
    if (!fork()) {
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        execlp(argv[i], argv[i], NULL);
        _exit(1);
    }
    close(fd[1]);
    close(fd[0]);
    while (wait(NULL) != -1);
    exit(0);
}