#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

unsigned long long hash_function(unsigned long long);

int
main(int argc, char **argv)
{
    unsigned long long h;
    long long count;
    sscanf(argv[2], "%llx", &h);
    sscanf(argv[3], "%lld", &count);
    int f = open(argv[1], O_WRONLY | O_CREAT, 0660);
    if (!count) {
        close(f);
        return 0;
    }
    count--;
    long e = lseek(f, (long) sizeof(h) * count, SEEK_END);
    if (e < 0) {
        return 1;
    }

    if (write(f, &h, sizeof(h)) < 0) {
        return 1;
    }

    h = hash_function(h);

    for ( ; count; count--) {
        e = lseek(f, - (long) sizeof(h) * 2LL, SEEK_CUR);
        if (write(f, &h, sizeof(h)) < 0) {
            return 1;
        }
        h = hash_function(h);
        if (e < 0) {
            return 1;
        }
    }

    close(f);
    return 0;
}
