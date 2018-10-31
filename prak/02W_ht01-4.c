#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { BUF = 1024 };

char
*getline2(FILE *f)
{
    unsigned k = BUF, i = 0;
    unsigned char *t = calloc(k, sizeof(char));
    if (t == NULL) {
        return NULL;
    }
    int c;
    if ((c = fgetc_unlocked(f)) == EOF) {
        free(t);
        return 0;
    }
    do {
        t[i] = c;
        if (t[i++] == '\n') {
            t[i] = 0;
            return t;
        }
        if (i == k - 1) {
            k += BUF;
            t = realloc(t, k * sizeof(char));
            if (t == NULL) {
                return NULL;
            }
        }
    } while ((c = fgetc_unlocked(f)) != EOF);
    t[i] = 0;
    return t;
}
