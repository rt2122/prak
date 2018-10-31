#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { BUF = 1024 };

char
*getline2(FILE *f)
{
    unsigned k = BUF, i = 0;
    char *t = calloc(k, sizeof(char));
    if (t == NULL) {
        return NULL;
    }
    if (fscanf(f, "%c", t) == EOF) {
        free(t);
        return 0;
    }
    do {
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
    } while (fscanf(f, "%c", t + i) == 1);
    t[i] = 0;
    return t;
}

int main(){
    char *s;
    FILE *f = fopen("a.txt", "r");
    s = getline2(f);
    printf("%s#\n", s);
    fclose(f);
    return 0;}
