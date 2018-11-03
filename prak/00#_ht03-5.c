#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum { M = 65534 };

int
main(void)
{
    unsigned char buf[M], *max = NULL, *prev_word = NULL;
    int len_max = 0, prev_len = 0;
    while (read(0, buf, M) > 0) {
        int len_buf = strlen(buf);
        int idx = -1, local_max_idx = -1;
        for (int i = 0; i < len_buf; i++ ) {
            if (buf[i] <= ' ') {
                if (prev_len) {
                    if (prev_len + i - idx > len_max) {
                        len_max = prev_len + i - idx;
                        if (max) {
                            free(max);
                        }
                        max = calloc(len_max + 1, sizeof(char));
                        snprintf(max, len_max + 1, "%s%s", prev_word, buf + idx);
                        free(prev_word);
                        prev_len = 0;
                    }
                } else {
                    if (idx >= 0) {
                        if (i - idx > len_max) {
                            len_max = i - idx;
                            local_max_idx = idx;
                        }
                    }
                }
                idx = -1;
            } else {
                if (idx < 0) {
                    idx = i;
                }
            }
        }
        if (local_max_idx >= 0) {
            max = realloc(max, len_max + 1);
            snprintf(max, len_max + 1, "%s", buf + local_max_idx);
        }
        if (idx >= 0) {
            prev_word = realloc(prev_word, (prev_len + len_buf - idx + 1) * sizeof(char));
            snprintf(prev_word + prev_len, prev_len + len_buf - idx + 1, "%s", buf + idx);
            prev_len += len_buf - idx;
        }
    }
    if (prev_len > len_max) {
        if (max) {
            free(max);
        }
        len_max = prev_len;
        max = prev_word;
    }
    printf("%d\n", len_max);
    if (len_max) {
        printf("%s\n", max);
        free(max);
    }
    return 0;
}