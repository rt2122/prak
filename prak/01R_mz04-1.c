#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

int
main(void)
{
    int32_t n, w;
    scanf("%o%o", (int *) &n, (int *) &w);
    int32_t x = 1 << n;
    for (uint32_t i = 0; i < x; i++) {
        int32_t m = i < x / 2 ? i : -(i - x / 2);
        printf("|%*o|%*d|%*d|\n", (int) w, (int) i, (int) w,
                (int) i, (int) w, (int) m);
    }
    return 0;
}
