#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum
{
    M = 65000,
    MAX_CODE = 4,
    MEANING_PART = 1 << 6
};

typedef unsigned char uchar;
const uchar MASK[] = { 0x80, 0xC0, 0xE0, 0xF0, 0xF8 };

int
decode(char *buf, int idx, int *len_sym)
{
    if (!buf[idx]) {
        return 0;
    }
    if (!(buf[idx] & MASK[0])) {
        *len_sym = 1;
        return buf[idx];
    }
    for (int i = 1; i < MAX_CODE; ++i) {
        if ((buf[idx] & MASK[i + 1]) == MASK[i]) {
            *len_sym = i + 1;
            int out = buf[idx] & ~MASK[i];
            for (int j = 1; j < *len_sym; ++j) {
                out *= MEANING_PART;
                out += buf[idx + j] & ~MASK[0];
            }
            return out;
        }
    }
    return 0;
}

unsigned
do_i_need_more_bytes(uchar c[])
{
    if ((c[MAX_CODE - 1] & MASK[0]) == 0) {
        return 0;
    }
    for (int i = 0; i < MAX_CODE - 1; ++i) {
        for (int ret = 0; ret < MAX_CODE - 1; ++ret) {
            if (i + ret < MAX_CODE && (c[i + ret] & MASK[1]) != MASK[0] &&
                    (c[i + ret] & MASK[MAX_CODE - i]) == MASK[MAX_CODE - 1 - i]) {
                return ret;
            }
        }
    }
    return  0;
}

int
main(void)
{
    uchar buf[M + MAX_CODE], *max = NULL, *prev_word = NULL;
    int len_maxcp = 0, prev_lencp = 0, len_buf, len_max = 0, prev_len = 0;
    while ((len_buf = fread(buf, sizeof(uchar), M, stdin))) {
        int add_buf = do_i_need_more_bytes(buf + len_buf - MAX_CODE);
        if (add_buf) {
            fread(buf + len_buf, sizeof(uchar), add_buf, stdin);
            len_buf += add_buf;
        }
        int cur_idx = 0, len_sym = 0, cur_len_wordcp = 0, first_letter = -1;
        unsigned cur_sym;
        int local_max_idx = -1;
        while ((cur_sym = decode(buf, cur_idx, &len_sym))) {
            if (cur_sym <= 0x20) {
                if (prev_lencp) {
                    if (prev_lencp + cur_len_wordcp > len_maxcp) {
                        len_maxcp = prev_lencp + cur_len_wordcp;
                        if (max) {
                            free(max);
                        }
                        len_max = prev_len + cur_idx;
                        max = calloc(len_max + 1, sizeof(char));
                        snprintf(max, len_max + 1, "%s%s", prev_word, buf);
                        prev_lencp = 0;
                        prev_len = 0;
                        free(prev_word);
                    }
                } else {
                    if (first_letter >= 0 && cur_len_wordcp > len_maxcp) {
                        local_max_idx = first_letter;
                        len_maxcp = cur_len_wordcp;
                        len_max = cur_idx - first_letter;
                    }
                }
                first_letter = -1;
                cur_len_wordcp = 0;
            } else {
                if (first_letter < 0) {
                    first_letter = cur_idx;
                }
                cur_len_wordcp++;
            }
            cur_idx += len_sym;
        }
        if (local_max_idx >= 0) {
            if (max) {
                free(max);
            }
            max = calloc(len_max + 1, sizeof(char));
            snprintf(max, len_max + 1, "%s", buf + local_max_idx);
        }
        if (first_letter >= 0) {
            prev_word = realloc(prev_word, (prev_len + len_buf - first_letter + 1) * sizeof(char));
            snprintf(prev_word + prev_len, len_buf - first_letter + 1, "%s", buf + first_letter);
            prev_len += len_buf - first_letter;
            prev_lencp += cur_len_wordcp;
        }
    }
    if (prev_lencp > len_maxcp) {
        if (max) {
            free(max);
        }
        len_maxcp = prev_lencp;
        len_max = prev_len;
    }
    printf("%d\n", len_maxcp);
    if (len_maxcp) {
        printf("%s\n", max);
    }
    return 0;
}