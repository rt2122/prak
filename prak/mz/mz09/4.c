#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <fcntl.h>

int
mysys(const char *str)
{
    int pid = fork();
    if (pid < 0) {
        return -1;
    } else {
        if (!pid) {
            execlp("/bin/sh", "sh", "-c", str, NULL);
            _exit(127);
        } else {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                return WEXITSTATUS(status);
            } else {
                if (WIFSIGNALED(status)) {
                    return 128 + WTERMSIG(status);
                }
            }
        }
    }

    return 127;
}