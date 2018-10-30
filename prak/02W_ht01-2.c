#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

enum
{
    NUM = 10
};

int
main(void)
{
    char s[PATH_MAX];
    long long mas[NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    if (fgets(s, sizeof(s), stdin) != NULL) {
        int len = strlen(s);
        if (s[len - 1] == '\n' || s[len - 1] == '\r') {
            s[len - 1] = 0;
            len--;
        }
        if (s[len - 1] == '\r') {
            s[len - 1] = 0;
        }

        const char *s1 = s;
        FILE *f = fopen(s1, "r");
        if (f) {
            int c;
            while ((c = getc_unlocked(f)) != EOF) {
                if (c <= '9' && c >= '0') {
                    mas[c - '0']++;
                }
            }
            fclose(f);
        }
    }

    for (int i = 0; i < NUM; i++) {
        printf("%d %lld\n", i, mas[i]);
    }
    return 0;
}
