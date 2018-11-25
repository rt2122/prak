#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *S = "rwxrwxrwx";
enum { LEN_S = 9 };

int
main(int argc, char **argv)
{
    for (int i = 1; i < argc; i++) {
        int r;
        unsigned char cur_s[LEN_S + 1];
        sscanf(argv[i], "%o", &r);
        int z = 1 << (LEN_S - 1);
        for (int j = 0; j < LEN_S; j++) {
            cur_s[j] = z & r ? S[j] : '-';
            z >>= 1;
        }
        cur_s[LEN_S] = 0;
        printf("%s\n", cur_s);
    }
    return 0;
}