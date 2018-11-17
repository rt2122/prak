#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

enum { N_FD = 2 };

int
main(int argc, char **argv)
{
    int fd[N_FD];
    char *cmd1 = argv[1], *cmd2 = argv[2], *cmd3 = argv[3];
    char *file1 = argv[4], *file2 = argv[5];
    pipe(fd);
    if (!fork()) {
        close(fd[0]);
        dup2(fd[1], 1);
        close(fd[1]);
        int f1 = open(file1, O_RDONLY, 0);
        dup2(f1, 0);
        close(f1);
        execlp(cmd1, cmd1, NULL);
        _exit(0);
    } else {
        int status;
        close(fd[0]);
        wait(&status);
        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
            if (!fork()) {
                dup2(fd[1], 1);
                close(fd[1]);
                execlp(cmd2, cmd2, NULL);
                _exit(0);
            }
        }
    }
    if (!fork()) {
        pipe(fd);
        close(fd[1]);
        dup2(fd[0], 0);
        close(fd[0]);
        int f2 = open(file2, O_WRONLY | O_APPEND | O_CREAT, 0660);
        dup2(f2, 1);
        close(f2);
        execlp(cmd3, cmd3, NULL);
        _exit(0);
    }
    close(fd[0]);
    close(fd[1]);
    while (wait(NULL) != -1);
    exit(0);
}