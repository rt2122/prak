#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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


enum
{
    NOT_NUM_ANS = 0xbedabeda,
    BLANK_LINE_ANS = 0x0bad1dea,
    NOT_NUM = 0,
    BLANK_LINE = 1,
    ALL_GOOD = 2
};

int
all_numbers(char *s) {
    while (*s) {
        if (!isspace(*s) && !isdigit(*s)) {
            return 0;
        }
        ++s;
    }
    return 1;
}

int
getnum(char *s, int pos_num, int *flag)
{
    *flag = ALL_GOOD;
    int x;
    sscanf(s, "%d", &x);
    if (x == -1 && mycmp(s, "-1") || x == 0 && mycmp(s, "0")) {
        x = pos_num;
        if (s[0] == '-') {
            x = -x;
        }
    }
    return x;
}

int
get_not_space(char *s, int cur, int *flag)
{
    int ans = cur;
    *flag = BLANK_LINE;
    while (s[ans] && !isspace(s[ans])) {
        *flag = ALL_GOOD;
        ++ans;
    }
    while (s[ans] && isspace(s[ans])) {
        ++ans;
    }
    return ans;
}

int
main(void)
{
    char *s;
    int num_line = 1;
    while ((s = getline2(stdin))) {
        int word_cur_pos = 1, sym_cur_pos = 0, sum = 0;
        int flag = ALL_GOOD;
        while (s[sym_cur_pos] && (flag == ALL_GOOD)) {
            sum += getnum(s + sym_cur_pos, word_cur_pos, &flag);
            sym_cur_pos = get_not_space(s, sym_cur_pos, &flag);
            ++word_cur_pos;
        }
        if (flag == NOT_NUM) {
            sum = NOT_NUM_ANS + num_line;
        }
        if (flag == BLANK_LINE) {
            sum = BLANK_LINE_ANS + num_line;
        }
        free(s);
        ++num_line;
        printf("%d\n", sum);
    }
    return 0;
}