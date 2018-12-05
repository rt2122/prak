#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    int ss, gs;
    gs = fork();
    if (!gs) {
        
        _exit(0);
    }
    ss = fork();
    if (!ss) {

        _exit(0);
    }
    _exit(0);
}
