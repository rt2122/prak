#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

int
main(int argc, char **argv)
{
    int f = open(argv[1], O_RDWR, 0);
    long long min = 0, cur = 0, minpos = 0, curpos = 0;
    int flag = 1;
    while (read(f, &cur, sizeof(cur)) > 0) {
        curpos++;
        if (cur < min || flag) {
            min = cur;
            flag = 0;
            minpos = curpos - 1;
        }
    }
    if (min == 0) {
        close(f);
        return 0;
    }
    if (lseek(f, minpos * sizeof(min), SEEK_SET) == -1) {
        close(f);
        return 1;
    }
    if (min == LLONG_MIN) {
        close(f);
        return 1;
    }
    min = -min;
    if (write(f, &min, sizeof(min)) == -1) {
        close(f);
        return 1;
    }
    close(f);
    return 0;
}
