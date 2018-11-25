#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
    int n_proc, status, n_exited = 0;;
    sscanf(argv[1], "%d", &n_proc);
    for (int i = 2; i < argc; ++i) {
        int father = fork();
        if (!father) {
            execlp(argv[i], argv[i], NULL);
            _exit(1);
        }
        if (i == n_proc + 1 || i == argc - 1) {
            while (wait(&status) != 1) {
                if (WIFEXITED(status) && !WEXITSTATUS(status)) {
                    n_exited++;
                }
            }
        }
    }
    printf("%d\n", n_exited);
    fflush(stdout);
    exit(0);
}