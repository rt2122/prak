#include <stdlib.h>
#include <string.h>

const char *S = "rwxrwxrwx";

int
parse_rwx_permissions(const char *str)
{
    if (!str) {
        return -1;
    }
    int LEN = strlen(S);
    if (strlen(str) != LEN) {
        return -1;
    }
    int ans = 0;
    for (int i = 0; i < LEN; i++) {
        ans <<= 1;
        if (str[i] == S[i]) {
            ans += 1;
        } else {
            if (str[i] != '-') {
                return -1;
            }
        }

    }
    return ans;
}
