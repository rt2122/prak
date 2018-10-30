#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>
int main()
{
    int pid;
    for (int i = 1; i <= 3; i++) {
        pid = fork();
        if (!pid) {
            int num;
            fflush(stdin);
            scanf("%d", &num);
            fflush(stdin);
            printf("%d %d\n", i, num * num);
            fflush(stdin);
            exit(0);
        } else {
            wait(NULL);
        }
    }
    if (pid) {
        wait(NULL);
    }
    return 0;
}
