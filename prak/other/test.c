#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum
{
    M = 1024,
    MAX_CODE = 4,
    MEANING_PART = 1 << 6
};

typedef unsigned char uchar;
const uchar MASK[] = { 0x80, 0xC0, 0xE0, 0xF0, 0xF8 };

int
decode(uchar *buf, int idx, int *len_sym)
{
    if (!buf[idx]) {
        printf("!!");
        return -1;
    }
    printf("%x", buf[idx] & MASK[0]);
    if (!(buf[idx] & MASK[0])) {
        *len_sym = 1;
        return buf[idx];
    }
    for (int i = 1; i < MAX_CODE; ++i) {
        if (buf[idx] & MASK[i + 1] == MASK[i]) {
            *len_sym = i + 1;
            int out = buf[idx] & ~MASK[i];
            for (int j = 1; j < *len_sym; ++j) {
                out *= MEANING_PART;
                out += buf[idx + j] & ~MASK[0];
            }
            return out;
        }
    }
    printf("!");
    return -1;
}

int
main(void)
{
    uchar buf[] = {0x20, 0x20, 0x20, 0x20};
    int k;
    printf("%x", decode(buf, 0, &k));
    return 0;
}