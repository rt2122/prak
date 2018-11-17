#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>

int
make_proc(char *proc)
{
    if (!fork()) {
        execlp(proc, proc, NULL);
        _exit(1);
    }
    int status;
    wait(&status);
    if (WIFEXITED(status) && !WEXITSTATUS(status)) {
        return 1;
    }
    return 0;
}

int
main(int argc, char **argv)
{
    return (!make_proc(argv[1]) ? make_proc(argv[2]) : 1) ? !make_proc(argv[3]) : 1;
}