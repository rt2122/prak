#include <stdlib.h>
#include <stdio.h>
enum
{
    MULT = 1103515245,
    INCR = 12345,
    MOD = 1U << 31
};

typedef struct RandomOperations
{
    int (*destroy)();
    int (*next)();
} RandomOperations;

typedef struct RandomGenerator
{
    unsigned cur;
    RandomOperations *ops;
} RandomGenerator;

int destroy_func(RandomGenerator *rr)
{
    if (rr) {
        free(rr);
    }
    return 0;
}

int next_func(RandomGenerator *rr)
{
    rr->cur = (int)(rr->cur * (long long) MULT + INCR) & (MOD - 1);
    return rr->cur;
}

RandomOperations opers = {destroy_func, next_func};

RandomGenerator *random_create(int seed)
{
    RandomGenerator *rr = calloc(1, sizeof(RandomGenerator));
    rr->ops = &opers;
    rr->cur = seed;
    return rr;
}

int
main(void)
{
    RandomGenerator *rr = random_create(1234);
    for (int j = 0; j < 100; ++j) {
        printf("%d\n", rr->ops->next(rr));
    }
    rr->ops->destroy(rr);
}