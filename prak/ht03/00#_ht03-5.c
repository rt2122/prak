#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum
{
    M = 1024,
    MASK_SHORT = 0x80,
    MASK_LONG = 0xC0,
    DIVIDER = 0x20
};

typedef unsigned char uchar;

int
main(void)
{
    uchar buf[M], *max = NULL, *prev_word = NULL;
    int max_cp = 0, prev_cp = 0, prev_b = 0, len_buf;
    while ((len_buf = read(0, buf, sizeof(buf))) > 0) {
        int local_max_idx = -1, first_letter = -1, cur_cp = 0, local_max_b = 0;
        for (int idx = 0; idx < len_buf; ++idx) {
            if (!(buf[idx] & MASK_SHORT) && buf[idx] <= DIVIDER) {
                if (prev_cp) {
                    if (prev_cp + cur_cp > max_cp) {
                        if (max) {
                            free(max);
                        }
                        if (first_letter >= 0) {
                            prev_word = realloc(prev_word, (prev_b + idx - first_letter) * sizeof(buf[0]));
                            snprintf(prev_word + prev_b, idx - first_letter + 1, "%s", buf + first_letter);
                            max = prev_word;
                            max_cp = prev_cp + cur_cp;
                        } else {
                            max_cp = prev_cp;
                            max = prev_word;
                        }
                    } else {
                        if (prev_word) {
                            free(prev_word);
                        }
                    }
                    prev_cp = 0;
                    prev_b = 0;
                } else {
                    if (cur_cp > max_cp) {
                        max_cp = cur_cp;
                        local_max_idx = first_letter;
                        local_max_b = idx - first_letter;
                    }
                }
                cur_cp = 0;
                first_letter = -1;
            } else {
                if ((buf[idx] & MASK_LONG) != MASK_SHORT) {
                    ++cur_cp;
                }
                if (first_letter < 0) {
                    first_letter = idx;
                }
            }
        }
        if (local_max_idx >= 0) {
            if (max) {
                free(max);
            }
            max = calloc(local_max_b + 1, sizeof(buf[0]));
            snprintf(max, local_max_b + 1, "%s", buf + local_max_idx);
        }
        if (first_letter >= 0) {
            prev_word = realloc(prev_word, (prev_b + len_buf - first_letter) * sizeof(buf[0]));
            snprintf(prev_word + prev_b, len_buf - first_letter + 1, "%s", buf + first_letter);
            prev_b += len_buf - first_letter;
            prev_cp += cur_cp;
            if (prev_cp > max_cp && max) {
                free(max);
            }
        }
    }
    if (prev_cp > max_cp) {
        if (max) {
            free(max);
        }
        max_cp = prev_cp;
        max = prev_word;
    }
    printf("%d\n", max_cp);
    if (max_cp) {
        printf("%s\n", max);
        free(max);
    }
    return 0;
}