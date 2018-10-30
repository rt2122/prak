#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

int
main(void)
{
    int n, w;
    scanf("%o%o", &n, &w);
    int32_t x = 1 << n;
    for (int32_t i = 0; i < x; i++) {
        int32_t m = i < x / 2 ? i : -(i - x / 2);
        printf("|%*o|%*d|%*d|\n", w, i, w, i, w, m);
    }
    return 0;
}
