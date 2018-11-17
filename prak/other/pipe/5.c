#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

enum { N_FD = 2 };

int
main(int argc, const char *argv[])
{
    int f_to_s[N_FD], s_to_f[N_FD];
    int max, cur;
    printf("@");
    fflush(stdout);
    sscanf(argv[1], "%d", &max);
    printf("#%d", max);
    fflush(stdout);
    pipe(f_to_s);
    pipe(s_to_f);
    if (!fork()) {
        //1
        int n_son = 1;
        FILE *sfr = fdopen(s_to_f[0], "r");
        FILE *fsw = fdopen(f_to_s[1], "w");
        close(s_to_f[1]);
        close(f_to_s[0]);
        while (fscanf(sfr, "%d", &cur) == 1 && cur < max) {
            fprintf(fsw, "%d ", cur + 1);
            fflush(fsw);
            printf("%d %d\n", n_son, cur);
    	    fflush(stdout);
        }
        close(s_to_f[0]);
        close(f_to_s[1]);
        exit(0);
    }
    if (!fork()) {
        //2
        int n_son = 2;
        FILE *fsr = fdopen(f_to_s[0], "r");
        FILE *sfw = fdopen(s_to_f[1], "w");
        close(s_to_f[0]);
        close(f_to_s[1]);
        while (fscanf(fsr, "%d", &cur) == 1 && cur < max) {
            fprintf(sfw, "%d ", cur + 1);
            
    	    fflush(sfw);
	    printf("%d %d\n", n_son, cur);
            fflush(stdout);
        }
        close(s_to_f[1]);
        close(f_to_s[0]);
        exit(0);
    }
    close(s_to_f[0]);
    close(s_to_f[1]);
    close(f_to_s[0]);
    close(f_to_s[1]);
    while (wait(NULL) != -1);
    exit(0);
}
