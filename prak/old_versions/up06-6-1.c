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

int compar(const void *a, const void *b)
{
    return strcasecmp(*((const char **)a), *((const char **)b));
}

int traverse(char *dir, int lenn, int len)
{
    DIR *d = opendir(dir);
    if (!d) {
        return -1;
    }
    if (lenn) {
        printf("cd %s\n", dir + lenn);
    }
    struct stat stb;
    struct dirent *dd;
    char **vv = calloc(PATH_MAX, sizeof(char *));
    int num = 0, maxlen = 1;
    while ((dd = readdir(d))) {
        if (strcmp(dd->d_name, ".") && strcmp(dd->d_name, "..")) {
            snprintf(dir + len, PATH_MAX, "/%s", dd->d_name);
            if (lstat(dir, &stb) >= 0) {

                if (S_ISDIR(stb.st_mode)) {
                    if (num == maxlen) {
                        maxlen *= 2;
                        vv = realloc(vv, maxlen * sizeof(char *));
                    }
                    vv[num] = calloc(PATH_MAX, sizeof(char));
                    snprintf(vv[num], PATH_MAX, "%s", dir);
                    num++;
                }
            }
        }

    }
    closedir(d);
    qsort(vv, num, sizeof(*vv), compar);
    int k = 0;
    for (k = 0; k < num; k++) {
        traverse(vv[k], len + 1, strlen(dir));
        free(vv[k]);
    }
    free(vv);
    if (lenn) {
        printf("cd ..\n");
    }
    return 0;
}

int main(int argc, char *argv[])
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "%s", argv[1]);
    traverse(path, 0, strlen(path));
    return 0;
}
