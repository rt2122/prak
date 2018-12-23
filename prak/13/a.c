#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
    int x, y;
    sscanf(argv[1], "%d", &x);
    sscanf(argv[2], "%d", &y);
    printf("%d\n", x % y);
    return 0;
}
