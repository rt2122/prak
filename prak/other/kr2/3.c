#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

enum
{
    BUF = 400,
    DELT = 40,
    N_RIGHTS = 3,
    ADDR_BEFORE = 2,
    ADDR_FOUND = 1,
    ADDR_AFTER = 0,
};

typedef struct Page
{
    unsigned long long st, en;
    int rights;
} Page;


int
get_rights(char *s)
{
    int ans = 0;
    for (int i = 0; i < N_RIGHTS; ++i) {
        ans <<= 1;
        ans |= s[i] != '-';
    }
    return ans;
}

int
main(int argc, char **argv)
{
    FILE *f = fopen(argv[1], "r");
    char buf[BUF];
    int cur = 0, max = DELT;
    Page *pages = calloc(max, sizeof(Page));
    while (fgets(buf, BUF, f)) {
        if (cur == max) {
            max += DELT;
            pages = realloc(pages, max * sizeof(pages[0]));
        }
        char r[N_RIGHTS + 1];
        sscanf(buf, "%llx-%llx%*c%3s", &(pages[cur].st), &(pages[cur].en), r);
        pages[cur].rights = get_rights(r);
        ++cur;
    }
    fclose(f);
    unsigned long long addr;
    int acc;
    while (scanf("%llx%d", &addr, &acc) == 2) {
        int flag = ADDR_AFTER, i;
        for (i = 0; i < cur && flag == ADDR_AFTER; ++i) {
            if (addr < pages[i].st) {
                flag = ADDR_BEFORE;
            } else {
                if (addr <= pages[i].en) {
                    flag = ADDR_FOUND;
                }
            }
        }
        if (flag == ADDR_BEFORE) {
            printf("0\n");
        }
        if (flag == ADDR_FOUND) {
            printf("%d\n", (pages[i].rights & acc) != 0 || !acc);
        }
    }
    free(pages);
    return 0;
}