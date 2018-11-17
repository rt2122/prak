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
    int i, pid;
    for (i = 1; i < argc; i++) {
        if (i != argc - 1) {
            pipe(fd);
        }
        pid = fork();
        if (pid == -1) {
            exit(1);
        }
        if (!pid) {
            dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);
            execlp(argv[i], argv[i], NULL);
            _exit(1);
        }
        dup2(fd[0], 0);
        if (i != argc - 1) {
            close(fd[0]);
            close(fd[1]);
        }
        }
    int status;
    while (wait(&status) != -1) {
        if (WIFEXITED(status) && WEXITSTATUS(status) == 1) {
            while (!kill(-1, SIGKILL));
            exit(1);
        }
    }
    exit(0);
}