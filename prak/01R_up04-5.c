#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

enum
{
    BUF = 16,
    SIZE_OF_NODE = 3
};

typedef struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
} Tree;

void
print_tree(Tree *root, int32_t k)
{
    if ((root + k)->right_idx) {
        print_tree(root, (root + k)->right_idx);
    }
    printf("%d ", (root + k)->key);
    if ((root + k)->left_idx) {
        print_tree(root, (root + k)->left_idx);
    }
}

int
main(int argc, char **argv)
{
    if (argc < 1) {
        return -1;
    }
    int f = open(argv[1], O_RDONLY);
    if (!f) {
        return -1;
    }
    Tree *root = calloc(BUF, sizeof(Tree));
    if (root == NULL) {
        return -1;
    }
    int n, k = 0, mas[BUF * SIZE_OF_NODE];
    while ((n = read(f, mas, sizeof(mas))) > 0) {
        n /= sizeof(int);
        for (int i = 0; i < n; i += 3) {
            (root + k)->key = mas[i];
            (root + k)->left_idx = mas[i + 1];
            (root + k)->right_idx = mas[i + 2];
            k++;
        }
        if (n == BUF * SIZE_OF_NODE) {
            root = realloc(root, (k + BUF * SIZE_OF_NODE) * sizeof(Tree));
        } else break;
    }
    if (n == -1) {
        return -1;
    }
    print_tree(root, 0);
    printf("\n");
    close(f);
    free(root);
    return 0;
}
