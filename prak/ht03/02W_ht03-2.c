#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

enum { MAX_PERCENT = 100 };

int main()
{
    int n;
    scanf("%d", &n);
    int val, prob_val, cur = 0, prob[MAX_PERCENT] = { 0 };
    for (int i = 0 ; i < n; i++) {
        scanf("%d%d", &val, &prob_val);
        for (int j = cur; j < cur + prob_val; j++) {
            if (j == MAX_PERCENT) {
                return 1;
            }
            prob[j] = val;
        }
        cur += prob_val;
    }
    int m, seed;
    scanf("%d%u", &m, &seed);
    srand(seed);
    for (int i = 0; i < m; i++) {
        double num = rand() / (RAND_MAX + 1.0);
        int ans = prob[(int)(num * MAX_PERCENT)];
        printf("%d\n", ans);
    }
    return 0;
}
