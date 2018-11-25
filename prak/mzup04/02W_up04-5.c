#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

int print_tree(int fd, int pos) {
    struct Node elem;
    lseek(fd, pos * sizeof(elem), 0);
    if (read(fd, &elem, sizeof(elem)) < 0) {
        return 1;
    }
    if (elem.right_idx) {
        print_tree(fd, elem.right_idx);
    }
    printf("%d ", elem.key);
    if (elem.left_idx) {
        print_tree(fd, elem.left_idx);
    }
    return 0;
}

int
main(int argc, char **argv)
{
    if (argc < 1) {
        return 1;
    }
    int fd = open(argv[1], O_RDONLY, 0);
    if (fd == -1) {
        return 1;
    }
    print_tree(fd, 0);
    printf("\n");
    close(fd);
    return 0;

}