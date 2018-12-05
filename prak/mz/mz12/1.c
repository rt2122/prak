#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

STYPE bit_reverse(STYPE value)
{
    UTYPE tmp = (UTYPE) value, ans = 0;
    int bit_max = sizeof(STYPE) * CHAR_BIT;
    for (int i = 0; i < bit_max; ++i) {
        ans <<= 1;
        ans |= tmp & 1;
        tmp >>= 1;
    }
    return (STYPE) ans;
}

