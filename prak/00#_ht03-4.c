#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum
{
    A = 1103515245,
    C = 12345,
    M = 1U << 31,
    MAX_LEN = 32,
    MAX_SYM = 62
};

unsigned char get_ascii(int x)
{
    if (x < 10) {
        return x + '0';
    }
    x += 'A' - 10;
    if (x <= 'Z') {
        return x;
    }
    x += - 'Z' + 'a' - 1;
    return x;
}

int get_num(unsigned char c)
{
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 10;
    }
    return c - 'a' + 10 + 'Z' - 'A' + 1;
}

int
main()
{
    char pass[MAX_LEN];
    scanf("%32s", pass);
    int n = 1, len = strlen(pass);
    while (pass[n++] == '.');
    int k[len];
    memset(k, 0, sizeof(k));
    int k_min = get_num(pass[0]) * M / 62, k_max = (get_num(pass[0]) + 1) * M / 62;

    k[0] = k_min;
    int ready = 1, flag = 0;
    while (ready < len && k[0] < k_max) {
        printf("%d ", ready);
        k[ready] = (A * k[ready - 1] + C) % M;
        int c = ((float) k[ready] * MAX_SYM / M);
        if ((flag = pass[ready] != '.') && pass[ready] != get_ascii(c)) {
            ready = 1;
            k[0]++;
        } else {
            ready++;
        }
    }
    if (ready < len || !flag) {
        printf("#\n");
        return 0;
    }
    for (int i = 0; i < len; i++) {
        if (pass[i] == '.') {
            int c = (int) ((float) k[i] * MAX_SYM / M);
            pass[i] = get_ascii(c);
        }
    }
    printf("%s", pass);
    return 0;
}