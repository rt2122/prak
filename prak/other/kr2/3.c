#include <stdio.h>
#include <stdlb.h>
#include <fcntl.h>
#include <sys/mman.h>

enum
{
    BUF = 4,
    ADDR_FOUND = 1,
    ADDR_NOT_FOUND = 0,
    ACC_POS = sizeof(int) * 4 + 2,
    READ = 4,
    WRITE = 2,
    XEC = 1,
    EXIST = 0,
    EX_POS =
};

int
rights(char *s, int acc)
{

}

int
access_to(char *s, int addr, int acc, int *flag)
{
    unsigned int starts, ends;
    sscanf(s, "%x-%x", &starts, &ends);
    if (starts <= addr && addr <= ends) {
        *flag = ADDR_FOUND;
    } else {
        *flag = ADDR_NOT_FOUND;
        return 0;
    }
    char buf[BUF];
    snprintf(buf, BUF + 1, "%s", s + ACC_POS);

}

int
main(int argc, char **argv)
{
    int fd = open(argv[1], O_RDONLY, 0);
    struct stat ss;
    fstat(fd, &ss);
    char *ptr = mmap(NULL, ss->st_size, PROT_READ, MAP_SHARED, fd, 0);
    int addr, acc;
    while (scanf("%x %d", &addr, &acc) == 2) {

    }
    close(fd);
    return 0;
}