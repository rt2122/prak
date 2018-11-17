#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>

int
main(void)
{
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        if (i < n) {
            printf("%d ", i);
            fflush(stdout);
            if (fork()) {
                wait(NULL);
                return 0;
            }
        } else {
            printf("%d\n", n);
            fflush(stdout);
            wait(NULL);
        }
    }
    return 0;
}