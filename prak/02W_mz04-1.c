#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <limits.h>

enum
{
    MAX_LEN = 4,
    SHIFT = 3
};

int32_t str_to_int(unsigned char *s) {
    int32_t i = 0, ans = 0;
    while (s[i]) {
        ans <<= SHIFT;
        ans += s[i] - '0';
        i++;
    }
    return ans;
}

int
main(void)
{
    int32_t n, w;
    unsigned char s[MAX_LEN];
    scanf("%3s", s);
    n = str_to_int(s);
    scanf("%3s", s);
    w = str_to_int(s);
    int32_t x = 1 << n;

    for (uint32_t i = 0; i < x; i++) {
        int32_t m = i < x / 2 ? i : -(i - x / 2);
        printf("|%*o|%*d|%*d|\n", (int) w, (int) i, (int) w,
                (int) i, (int) w, (int) m);
    }
    return 0;
}
