#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { BUF = 1024 };

char
*getline2(FILE *f)
{
    unsigned k = 2 * BUF;
    char *t = calloc(k, sizeof(char));
    if (t == NULL) {
        return NULL;
    }
    if (fscanf(f, "%1024s", t) == EOF) {
        free(t);
        return 0;
    }
    int del = 0;
    do {
        del = BUF - strlen(t + k - BUF);
        k += BUF;
        t = realloc(t, k * sizeof(char));
    } while (fscanf(f, "%1024s", t + k - BUF) == 1 && !del);
    return t;
}

int
main(void)
{
    char *s = getline2(stdin);
    printf("%s#", s);
    return 0;
}
