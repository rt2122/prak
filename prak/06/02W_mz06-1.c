#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    unsigned long long sum = 0;
    for (int i = 1; i < argc; i++) {
        struct stat stb;
        if (lstat(argv[i], &stb) >= 0 && S_ISREG(stb.st_mode) && stb.st_nlink == 1) {
            sum += stb.st_size;
        }
    }
    printf("%llu\n", sum);
    return 0;
}
