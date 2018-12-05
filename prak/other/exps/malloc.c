#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
    printf("#");
    int *mem = malloc(sizeof(int) * 10);
    scanf("%d", mem);
    free(mem);
    return 0;
}
