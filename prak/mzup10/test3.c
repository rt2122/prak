#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
    int x;
    while (scanf("%d", &x) == 1) {
        if (x % 100 == 44) {
            printf("%d ", x);
        }
    }
    return 1;
}