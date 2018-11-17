#include <stdio.h>

int
main(void)
{
    FILE *f = fopen("out.bin", "wb");
    int m[] = {3, 2, 3, 12, 5, 0, 1, 0, 0, 10, 4, 1, 8, 0, 0, 11, 0, 0};
    int len = 0;
    while (m[len++]);
    fwrite(m, sizeof(int), sizeof(m) / sizeof(int), f);
    fclose(f);
    return 0;
}