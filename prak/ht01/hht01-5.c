#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>

char
*getline2(FILE *f);

enum
{
    NOT_NUM = 0xbedabeda,
    BLANK_LINE = 0x0bad1dea,
    BASIS = 10
};

int
all_numbers(char *s)
{
    while (*s) {
        if (!isspace(*s) && !isdigit(*s) && *s != '-' && *s != '+') {
            return 0;
        }
        ++s;
    }
    return 1;
}

int
blank_line(char *s)
{
    if (s == NULL) {
        return 1;
    }
    while (*s && isspace(*s)) {
        s++;
    }
    if (!*s) {
        return 1;
    }
    return 0;
}

int
main(void)
{
    char *s;
    int num_line = 1;
    while ((s = getline2(stdin))) {
        int word_cur_pos = 1;
        unsigned sum = 0;
        if (blank_line(s)) {
            sum = BLANK_LINE + num_line;
        } else {
            if (!all_numbers(s)) {
                sum = NOT_NUM + num_line;
            } else {
                char *sym_cur_pos = s;
                while (*sym_cur_pos) {
                    if (isspace(*sym_cur_pos)) {
                        ++sym_cur_pos;
                    }
                    long x = strtol(sym_cur_pos, &sym_cur_pos, BASIS);
                    if (errno == ERANGE) {
                        if (x == LONG_MIN) {
                            x = -word_cur_pos;
                        } else {
                            if (x == LONG_MAX) {
                                x = word_cur_pos;
                            }
                        }
                    } else {
                        if (x > INT_MAX) {
                            x = -word_cur_pos;
                        } else {
                            if (x < INT_MIN) {
                                x = word_cur_pos;
                            }
                        }
                    }
                    sum += x;
                    ++word_cur_pos;
                }
            }

        }
        if (s) {
            free(s);
        }
        ++num_line;
        printf("%d\n", sum);
    }
    return 0;
}
