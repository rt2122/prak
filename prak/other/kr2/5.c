#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>

enum { N_FD = 2 };

int
sys_check(int x)
{
    if (x == -1) {
        _exit(1);
    }
    return x;
}

int
main(int argc, char **argv)
{
    int fd[N_FD];
    sys_check(pipe(fd));
    if (!sys_check(fork())) {
        sys_check(close(fd[0]));
        sys_check(dup2(fd[1], 1));
        sys_check(close(fd[1]));
        execlp(argv[1], argv[1], NULL);
        _exit(1);
    }
    close(fd[1]);
    int file = open(argv[4], O_WRONLY | O_CREAT, 0666);
    int pid;
    if (!sys_check(pid = fork())) {
        sys_check(dup2(fd[0], 0));
        sys_check(close(fd[0]));
        sys_check(dup2(file, 1));
        sys_check(close(file));
        execlp(argv[2], argv[2], NULL);
        _exit(1);
    }
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status) && !WEXITSTATUS(status)) {
        if (!sys_check(pid = fork())) {
            sys_check(dup2(fd[0], 0));
            sys_check(close(fd[0]));
            sys_check(dup2(file, 1));
            sys_check(close(file));
            execlp(argv[3], argv[3], NULL);
            _exit(1);
        }
    }
    close(fd[0]);
    close(file);
    while (wait(NULL) != -1);
    return 0;
}