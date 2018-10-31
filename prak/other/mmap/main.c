#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
    long a, b;
    sscanf(argv[2], "%ld", &a);
    sscanf(argv[3], "%ld", &b);
    struct stat stb;
    int fd = open(argv[1], O_RDONLY, 0);

    fstat(fd, &stb);
    unsigned char *ptr = mmap(NULL, stb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    int num_of_line = 1, ent = 0;
    for (int i = 0; ptr[i]; i++) {
        if (ptr[i] == '\n') {
            if (num_of_line <= b && num_of_line >= a) {
                printf("[%d]%*s", i - ent, i - ent, ptr + ent);
            }
            num_of_line++;
            ent = i;
        }
    }
    close(fd);
    return 0;
}
