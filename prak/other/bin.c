#include <stdio.h>

int
main(void)
{
    FILE *f = fopen("out.bin", "wb");
    int m[] = {7, 0, 6,
               10, 2, 4,
               9, 0, 0,
               40, 0, 0,
               30, 5, 3,
               15, 0, 0,
               8, 0, 1};
    fwrite(m, sizeof(int), sizeof(m) / sizeof(int), f);
    fclose(f);
    return 0;
}