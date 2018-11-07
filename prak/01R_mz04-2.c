#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *S = "rwxrwxrwx";

int
main(int argc, char **argv)
{
    int len = strlen(S);
    for (int i = 1; i < argc; i++) {
        int r;
        sscanf(argv[i], "%o", &r);
        int z = 1 << (len - 1);
        for (int j = 0; j < len; j++) {
            char c = z & r ? S[j] : '-';
            printf("%c", c);
            z >>= 1;
        }
        printf("\n");
    }
    return 0;
}
