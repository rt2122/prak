#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

int
main(int argc, char **argv)
{
    int f = open(argv[1], O_RDWR, 0600);
    long long min = 0, cur = 0;
    int flag = 1;
    while (read(f, &cur, sizeof(cur)) > 0) {
        if (cur < min || flag) {
            min = cur;
            flag = 0;
        }
    }
    if (min == LLONG_MIN) {
        min++;
    }
    lseek(f, 0, 0);
    while (read(f, &cur, sizeof(cur)) > 0) {
        if (cur == min) {
            lseek(f, -(int) sizeof(cur), 1);
            min = -min;
            write(f, &min, sizeof(cur));
            break;
        }
    }
    close(f);
    return 0;
}
