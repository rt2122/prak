#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

enum { N_FD = 2 };

void
push_sqr(int count, int mod)
{
    for (int i = 1; i <= count; ++i) {
        int tmp = (i * i) % mod;
        printf("%d\n", tmp);
        fflush(stdout);
    }
}

void
get_chars(void)
{
    char tmp;
    while (read(0, &tmp, sizeof(tmp)) == sizeof(tmp)) {
        tmp = tmp == ' ' ? '\n' : tmp;
        write(1, &tmp, sizeof(tmp));
        fflush(stdout);
    }
}

int
main(int argc, char *argv[])
{
    int count, mod;
    sscanf(argv[1], "%d", &count);
    sscanf(argv[2], "%d", &mod);
    char *prog = argv[3];
    
    int pipe12[N_FD], pipe24[N_FD];
    pipe(pipe24);

    int p1 = fork();
    if (!p1) {
        close(pipe24[0]);
        pipe(pipe12);
        int p2 = fork();
        if (!p2) {
            dup2(pipe24[1], 1);
            close(pipe24[1]);
            dup2(pipe12[0], 0);
            close(pipe12[0]);
            close(pipe12[1]);
            execlp(prog, prog, NULL);
            _exit(0);
        }
        dup2(pipe12[1], 1);
        close(pipe12[1]);
        close(pipe24[1]);
        close(pipe12[0]);
        push_sqr(count, mod);
        close(1);
        close(0);
        wait(NULL);
        _exit(0);
    }
    
    close(pipe24[1]);
    int p3 = fork();
    if (!p3) {
        int p4 = fork();
        if (!p4) {
            dup2(pipe24[0], 0);
            close(pipe24[0]);
            get_chars();
            _exit(0);
        }
        close(pipe24[0]);
        close(0);
        close(1);
        wait(NULL);
        _exit(0);
    }
    close(pipe24[0]);
    close(0);
    while (wait(NULL) > 0) {}
    printf("0\n");
    return 0;
}
