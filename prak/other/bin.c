#include <stdio.h>

int
main(void)
{
    FILE *f = fopen("out.bin", "wb");
    char m[] = {0x20, 0x20, 0x20, 0xC2, 0XA2, 0x21, 0x21, 0x21, 0x20, 0x21, 0};
    int len = 0;
    while (m[len++]);
    fwrite(m, 1, len, f);
    fclose(f);
    return 0;
}