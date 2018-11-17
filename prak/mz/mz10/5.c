#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

enum { N_FD = 2 };

int
main(int argc, const char *argv[])
{
    int fs[N_FD], sf[N_FD];
    int max, cur;
    fflush(stdout);
    sscanf(argv[1], "%d", &max);
    printf("#%d\n", max);
    fflush(stdout);
    pipe(fs);
    pipe(sf);
    int pid1 = fork();
    if (pid1 == -1) {
        printf("-1\n");
        exit(1);
    }
    if (!pid1) {
        //1
        printf("#1\n");
        int n_son = 1;
        FILE *sfr = fdopen(sf[0], "r");
        FILE *fsw = fdopen(fs[1], "w");
        printf("%d %d\n", sfr, fsw);
        close(sf[1]);
        close(fs[0]);
        while (fscanf(sfr, "%d", &cur) == 1 && cur < max) {
            printf("#");
            fprintf(fsw, "%d ", cur + 1);
            fflush(fsw);
            printf("%d %d\n", n_son, cur);
            fflush(stdout);
        }
        fclose(fsw);
        fclose(sfr);
        close(sf[0]);
        close(fs[1]);
        exit(0);
    } else {
        int pid2 = fork();
        if (pid2 == -1) {
            printf("-1\n");
            exit(1);
        }
        if (!pid2) {
            //2
            printf("#2\n");
            int n_son = 2;
            FILE *fsr = fdopen(fs[0], "r");
            FILE *sfw = fdopen(sf[1], "w");
            close(sf[0]);
            close(fs[1]);
            while (fscanf(fsr, "%d", &cur) == 1 && cur < max) {
                fprintf(sfw, "%d ", cur + 1);
                fflush(sfw);
                printf("%d %d\n", n_son, cur);
                fflush(stdout);
            }
            fclose(sfw);
            fclose(fsr);
            close(sf[1]);
            close(fs[0]);
            exit(0);
        } else {
            close(sf[0]);
            close(sf[1]);
            close(fs[0]);
            close(fs[1]);
            wait(NULL);
            exit(0);
        }
        close(sf[0]);
        close(sf[1]);
        close(fs[0]);
        close(fs[1]);
        wait(NULL);
    }
    exit(0);
}
