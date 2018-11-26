#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>

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
    int n_proc, a, d, k;
    sscanf(argv[1], "%d", &n_proc);
    char *f = argv[2];
    sscanf(argv[3], "%d", &a);
    sscanf(argv[4], "%d", &d);
    sscanf(argv[5], "%d", &k);

    for (int i = 0; i < n_proc; ++i) {
        if (!sys_check(fork())) {
            int fd = open(f, O_WRONLY | O_CREAT, 0600);
            for (int j = 0; j < k; ++j) {
                lseek(fd, (i + n_proc * j) * sizeof(int), SEEK_SET);
                int x = a + d * (i + n_proc * j);
                write(fd, &x, sizeof(x));
            }
            close(fd);
            _exit(0);
        }
    }
    while (wait(NULL) != -1);
    return 0;
}