#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

enum
{
    A = 1103515245,
    C = 12345,
    M = 1U << 31,
    MAX_LEN = 32,
    MAX_SYM = 62,
    NUM = 10
};

char ascii[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
        'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
        'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z'};

unsigned char get_ascii(int x)
{
    return ascii[x];
}

int get_num(unsigned char c)
{
    int i = 0;
    while(ascii[i] != c) {
        i++;
    }
    return i;
}

int get_random(int k)
{
    return (int) ((k * (long long) A + C) & (M - 1));
}

unsigned char get_ascii_from_random(int x) {
    return get_ascii((int) (((double) x / M) * MAX_SYM));
}

int
main(void)
{
    char pass[MAX_LEN + 1];
    scanf("%32s", pass);
    if (pass[0] == '.') {
        printf("#\n");
        return 0;
    }
    int num = get_num(pass[0]);
    int start_of_bust = (int) round(((double) num / MAX_SYM) * M);
    int end_of_bust;
    if (++num == MAX_SYM) {
        end_of_bust = M - 1;
    } else {
        end_of_bust = (int) round(((double) num / MAX_SYM) * M);
    }

    int num_of_pass = 0, seed = 0;
    for (int i = start_of_bust; i < end_of_bust; i++) {
        unsigned k = get_random(i);
        int j = 1;
        while (pass[j]) {
            if (pass[j] != '.') {
                unsigned char c = get_ascii_from_random(k);
                if (pass[j] != c) {
                    break;
                }
            }
            k = get_random(k);
            j++;
        }
        if (!pass[j]) {
            num_of_pass++;
            if (num_of_pass > 1) {
                break;
            }
            seed = i;
        }
    }

    if (num_of_pass != 1) {
        printf("#\n");
        return 0;
    }
    int i = 0;
    while (pass[i]) {
        pass[i] = get_ascii_from_random(seed);
        seed = get_random(seed);
        i++;
    }
    printf("%s\n", pass);
    return 0;
}