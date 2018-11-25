#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

int
main(int argc, char **argv)
{
    int f = open(argv[1], O_RDWR);
    long long min = 0, cur = 0, min_idx = 0, cur_idx = 0;
    int flag = 1;
    while (read(f, &cur, sizeof(cur)) == sizeof(cur)) {
        if (cur < min || flag) {
            min = cur;
            min_idx = cur_idx;
            flag = 0;
        }
        cur_idx += sizeof(cur);
    }
    if (min != LLONG_MIN) {
        min = -min;
    }
    if (!flag) {
        lseek(f, (long long) min_idx, SEEK_SET);
        write(f, &min, sizeof(min));
    }
    close(f);
    return 0;
}
