#include <unistd.h>
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
    //()
    if (!fork()) {
        //cmd1
        close(fd[0]);
        dup2(fd[1], 1);
        int f1 = open(file1, O_RDONLY, 0);
        dup2(f1, 0);
        execlp(cmd1, cmd1, NULL);
        _exit(0);
    } else {
        int status;
        wait(&status);
        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
            if (!fork()) {
                //cmd2
                close(fd[0]);
                dup2(fd[1], 1);
                execlp(cmd2, cmd2, NULL);
                _exit(0);
            }
        }
    }
    if (!fork()) {
        //cmd3
        close(fd[1]);
        dup2(fd[0], 0);
        int f2 = open(file2, O_WRONLY | O_APPEND | O_CREAT, 0660);
        dup2(f2, 1);
        execlp(cmd3, cmd3, NULL);
        _exit(0);
    }
    wait(NULL);
    exit(0);
}