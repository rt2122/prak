#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>

int
main(void)
{
    int n, first_pros = 1;
    while (scanf("%d", &n) == 1) {
        int pid = fork();
        if (pid == -1) {
            printf("-1\n");
            fflush(stdout);
            return 1;
        }
        if (pid) {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) == 1) {
                if (first_pros) {
                    return 0;
                }
                return 1;
            }
            printf("%d\n", n);
            break;
        } else {
            first_pros = 0;
        }
    }
    return 0;
}