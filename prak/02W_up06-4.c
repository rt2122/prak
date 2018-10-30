#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <linux/limits.h>
#include <ctype.h>
#include <string.h>

int 
main(int argc, char **argv)
{
    unsigned long long sum = 0;
    struct dirent *dd;
    DIR *d = opendir(argv[1]);
    char path[PATH_MAX];
    uid_t usr = getuid();
    while ((dd = readdir(d))) {
        struct stat stb;
        if (snprintf(path, sizeof(path), "%s/%s", argv[1], dd->d_name) > sizeof(path)) {
            continue;
        }
        if (stat(path, &stb) >= 0) {
            if (S_ISREG(stb.st_mode) && stb.st_uid == usr && isupper(dd->d_name[0])) {
                sum += stb.st_size;
            }
        }
    }
    printf("%llu\n", sum);
    return 0;
}