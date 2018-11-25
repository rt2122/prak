#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

enum { N_FD = 2 };

int sys_check(int x) {
    if (x == -1) {
        _exit(1);
    }
    return x;
}

int
main(int argc, char **argv)
{
    if (argc < 5) {
        _exit(1);
    }
    char *cmd[] = { argv[1], argv[2], argv[3] };
    char *file[] = { argv[4], argv[5] };

    int f1 = sys_check(open(file[0], O_RDONLY, 0));
    int f2 = sys_check(open(file[1], O_WRONLY | O_APPEND | O_CREAT, 0600));
    int fd[N_FD];
    sys_check(pipe(fd));


    if (!sys_check(fork())) {
        sys_check(close(fd[0]));
        int br1 = sys_check(fork());
        if (!br1) {
            sys_check(dup2(fd[1], 1));
            sys_check(close(fd[1]));
            sys_check(dup2(f1, 0));
            sys_check(close(f1));
            execlp(cmd[0], cmd[0], NULL);
            _exit(1);
        }
        int status;
        waitpid(br1, &status, 0);
        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
            br1 = sys_check(fork());
            if (!br1) {
                sys_check(dup2(fd[1], 1));
                sys_check(close(fd[1]));
                execlp(cmd[1], cmd[1], NULL);
                _exit(1);
            } else {
                wait(NULL);
            }
        }
        _exit(0);
    }
    sys_check(close(fd[1]));
    int br2 = sys_check(fork());
    if (!br2) {
        sys_check(dup2(fd[0], 0));
        sys_check(close(fd[0]));
        sys_check(dup2(f2, 1));
        sys_check(close(f2));
        execlp(cmd[2], cmd[2], NULL);
        _exit(1);
    }
    sys_check(close(fd[0]));
    while (wait(NULL) != -1);
    _exit(0);
}