#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>
int main()
{
    char s[PATH_MAX];
    unsigned long long sum = 0;
    while (fgets(s, PATH_MAX, stdin) != NULL) {
        if (!strcmp(s, "0\n")) {
            break;
        }
        unsigned long long a, b;
        sscanf(s, "%llx-%llx", &a, &b);
        sum += b - a;
        //printf("%llu\n", sum);
    }
    printf("%llu\n", sum);
    return 0;
}
