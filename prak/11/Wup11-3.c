#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>

int
main(int argc, char *argv[])
{
    int n, status, ans = 0;
    sscanf(argv[1], "%d", &n);
    for (int i = 2; i < argc; ++i) {
        FILE *f = fopen(argv[i], "r");
        char path[PATH_MAX];
        fscanf(f, "%s", path);
        fclose(f);
        if (!fork()) {
            execlp(path, path, NULL);
            _exit(1);
        }
        if (i >= n + 1 || i == argc - 1) {
            while (wait(&status) > 0) {
                if (WIFEXITED(status) && !WEXITSTATUS(status)) {
                    ++ans;
                }
            }
        }
    }
    printf("%d\n", ans);
    fflush(stdout);
    _exit(0);
}
