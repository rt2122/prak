#include <limits.h>

STYPE bit_reverse(STYPE value)
{
    enum { MAX_BIT = sizeof(STYPE) * CHAR_BIT };
    UTYPE tmp = (UTYPE) value, ans = 0;
    for (int i = 0; i < MAX_BIT; ++i) {
        ans <<= 1;
        ans |= tmp & 1;
        tmp >>= 1;
    }
    return (STYPE) ans;
}
