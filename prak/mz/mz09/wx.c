#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

int
main()
{
    char c;
    scanf("%c", &c);
    printf("%c", c);
    fprintf(stderr, "!!!");
    return 0;
}