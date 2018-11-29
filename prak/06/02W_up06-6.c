#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <strings.h>

enum { MAX = 128 };

int compar(const void *a, const void *b)
{
    return strcasecmp(*((const char **)a), *((const char **)b));
}

int rec_dir(char *path, int index, int name_len)
{
    DIR *d = opendir(path);
    if (!d) {
        return 0;
    }
    if (index) {
        printf("cd %s\n", path + index + 1);
    }
    struct stat stb;
    struct dirent *dd;
    int num = 0, maxlen = MAX;
    char **cc = calloc(maxlen, sizeof(char *));
    while ((dd = readdir(d))) {
        if (strcmp(dd->d_name, ".") && strcmp(dd->d_name, "..")) {
            int slen = snprintf(path + name_len, PATH_MAX, "/%s", dd->d_name);
            if (slen + name_len < PATH_MAX && lstat(path, &stb) >= 0) {
                if (S_ISDIR(stb.st_mode)) {
                    if (num == maxlen) {
                        maxlen += MAX;
                        cc = realloc(cc, maxlen * sizeof(char *));
                    }
                    cc[num++] = strdup(dd->d_name);
                }
            }
        }
    }
    closedir(d);
    if (num) {
        qsort(cc, num, sizeof(*cc), compar);
    }
    for (int i = 0; i < num; i++) {
        int slen = snprintf(path + name_len, PATH_MAX, "/%s", cc[i]);
        rec_dir(path, name_len, name_len + slen);
        free(cc[i]);
    }
    free(cc);
    if (index) {
        printf("cd ..\n");
    }

    return 0;
}

int main(int argc, char *argv[])
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "%s", argv[1]);
    rec_dir(path, 0, strlen(path));
    return 0;
}
