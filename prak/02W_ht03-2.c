#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

enum { MAX_PERCENT = 100 };

int main()
{
    int n;
    scanf("%d", &n);
    int num[n][2];
    for (int i = 0 ; i < n; i++) {
        scanf("%d%d", &num[i][0], &num[i][1]);
    }
    int m, seed;
    scanf("%d%u", &m, &seed);
    srand(seed);

    int prob[MAX_PERCENT] = { 0 };
    int cur = 0;
    for (int i = 0; i < n; i++) {
        for (int j = cur; j < cur + num[i][1]; j++) {
            prob[j] = num[i][0];
        }
        cur += num[i][1];
    }
    /*
    for (int i = 0; i < 100; i++) {
        if (i % 20 == 0) {
            printf("\n");
        }
        printf("%d ", prob[i]);
    }
    */

    for (int i = 0; i < m; i++) {
        double num = rand() / (RAND_MAX + 1.0);
        int ans = prob[(int)(num * MAX_PERCENT)];
        printf("%d\n", ans);
    }
    return 0;
}
