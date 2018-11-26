#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>


enum
{
    BUF = 4,
    ADDR_BEFORE = 2,
    ADDR_FOUND = 1,
    ADDR_AFTER = 0,
    ACC_POS = 26, //18s
    N_RIGHTS = 3

};

const int RWX[] = { 4, 2, 1 };

int
rights(char *s, int acc)
{
    for (int i = 0; i < N_RIGHTS; ++i) {
        if ((acc & RWX[i]) && s[i] == '-') {
            return 0;
        }
    }
    return 1;
}

int
access_to(char *s, unsigned long long addr, int acc, int *flag)
{
    unsigned long long starts, ends;
    sscanf(s, "%llx-%llx", &starts, &ends);
    if (addr < starts) {
        *flag = ADDR_BEFORE;
        return 0;
    } else {
        if (addr < ends) {
            *flag = ADDR_FOUND;
        } else {
            *flag = ADDR_AFTER;
            return 0;
        }
    }
    char *buf = s + ACC_POS;
    printf("%c%c%c%c\n", buf[0], buf[1], buf[2], buf[3]);
    return rights(buf, acc);
}

int
get_next_line(char *s, int off) {
    while (s[off] && s[off] != '\n') {
        ++off;
    }
    while (s[off] && s[off] == '\n') {
        ++off;
    }
    return off;
}

int
main(int argc, char **argv)
{
    int fd = open(argv[1], O_RDONLY, 0);
    struct stat ss;
    fstat(fd, &ss);
    char *ptr = mmap(NULL, ss.st_size, PROT_READ, MAP_SHARED, fd, 0);
    unsigned long long addr;
    int acc;
    unsigned long long off = 0;
    while (scanf("%llx %d", &addr, &acc) == 2) {
        int flag = ADDR_BEFORE, ans;
        off = 0;
        while (off < ss.st_size && flag == ADDR_BEFORE) {
            ans = access_to(ptr + off, addr, acc, &flag);
            off = get_next_line(ptr, off);
        }
        if (flag == ADDR_FOUND) {
            printf("%d\n", ans);
        } else {
            printf("0\n");
        }
    }
    close(fd);
    return 0;
}