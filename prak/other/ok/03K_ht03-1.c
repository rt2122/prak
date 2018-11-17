#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char **argv)
{
    if (argc < 5) {
        return 1;
    }
    int count, low, high;
    unsigned seed;
    sscanf(argv[1], "%d", &count);
    sscanf(argv[2], "%d", &low);
    sscanf(argv[3], "%d", &high);
    sscanf(argv[4], "%u", &seed);
    if (low >= high) {
        return 1;
    }
    srand(seed);
    for (int i = 0; i < count; i++) {
        double num = rand() / (RAND_MAX + 1.0);
        int ans = (int)(num * (high - low)) + low;
        printf("%d\n", ans);
    }
    return 0;
}
