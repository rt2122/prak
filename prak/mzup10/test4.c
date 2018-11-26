#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
    int x;
    while (scanf("%d", &x) == 1) {
        printf("%d ", x * x);
    }
    return 1;
}