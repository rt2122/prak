#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char
*getline2(FILE *f)
{
    enum { BUF = 1024 };
    int max = 2, cur = 0;
    char *t = calloc(max * BUF, sizeof(char));

    if (fgets(t + cur * BUF, BUF + 1, f) == NULL) {
        printf("#");
        fflush(stdout);
        free(t);
        return NULL;
    }
    printf("%s\n", t);
    fflush(stdout);
    if (t[strlen(t) - 1] == '\n') {
        return t;
    }
    ++cur;
    ++max;
    t = realloc(t, (sizeof(char)) * max * BUF);
    while (fgets(t + cur * BUF, BUF + 1, f) != NULL) {
        ++cur;
        ++max;
        if (t[strlen(t) - 1] == '\n') {
            return t;
        }
        t = realloc(t, (sizeof(char)) * max * BUF);
    }
    return t;
}

int
main(void)
{
    FILE *f = fopen("test.txt", "r");
    printf("%s#\n", getline2(f));
    fclose(f);
    return 0;
}