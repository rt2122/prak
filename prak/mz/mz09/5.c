#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <fcntl.h>

int
main(int argc, char **argv)
{
    int pid = fork();
    if (argc < 5) {
        return 1;
    }
    if (!pid) {
        int fd1, fd2, fd3;
        fd1 = open(argv[2], O_RDONLY);
        fd2 = open(argv[3], O_CREAT | O_WRONLY | O_APPEND, 0660);
        fd3 = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0660);
        if (fd1 < 0 || fd2 < 0 || fd3 < 0) {
            return 42;
        }
        if (dup2(fd1, 0) < 0 || dup2(fd2, 1) < 0 || dup2(fd3, 2) < 0) {
            return 42;
        }
        execlp(argv[1], argv[1], NULL);
        return 42;
    } else {
        int status;
        wait(&status);
        printf("%d\n", status);
    }
    return 0;
}