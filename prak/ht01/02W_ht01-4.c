#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char
*getline2(FILE *f)
{
    enum { BUF = 1024, DIF = 2 };

    int cur = 0;
    char *t = calloc((cur + DIF) * BUF, sizeof(char));
    while (fgets(t + cur * BUF, BUF + 1, f) != NULL) {
        ++cur;
        if (t[strlen(t) - 1] == '\n') {
            return t;
        }
        t = realloc(t, (cur + DIF) * BUF * sizeof(char));
    }
    if (!cur) {
        free(t);
        return NULL;
    }
    return t;
}
