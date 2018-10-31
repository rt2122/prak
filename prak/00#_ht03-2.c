#include <stdio.h>
#include <stdlib.h>

enum { MAX_PERCENT = 100 };

int main()
{
    int n, count, seed, sum = 0;
    scanf("%d", &n);
    int table[MAX_PERCENT];
int check[n];
for (int i = 0; i < n; i++)
    check[i] = 0;
int nums[n];
    for (int i = 0; i < n; i++) {
        int p, val;
        scanf("%d%d", &val, &p);
nums[i] = val;
        for (int j = sum; j < sum + p; j++) {
            table[j] = val;
        }
        sum += p;
    }
    scanf("%d%d", &count, &seed);
    srand(seed);
    for (int i = 0; i < count; i++) {
        double num = (double) rand() / ((double) RAND_MAX + (double) 1.0);
        int ans = (int)((double) num * (double) MAX_PERCENT);
check[nums[table[ans]]]++;
        printf("%d\n", table[ans]);
    }
for (int i = 0; i < n; i++) {
    printf("%lf\n", (double) check[i] / (double) n);
}
    return 0;
}
