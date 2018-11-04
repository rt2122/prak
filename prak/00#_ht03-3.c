#include <stdlib.h>
#include <stdio.h>
enum
{
    MULT = 1103515245ULL,
    INCR = 12345ULL,
    MOD = (unsigned long long) (1U << 31) - 1
};

typedef struct RandomOperations
{
    int (*destroy)();
    int (*next)();
} RandomOperations;

typedef struct RandomGenerator
{
    int cur;
    RandomOperations *ops;
} RandomGenerator;


int destroy_func(RandomGenerator *rr)
{
    if (rr) {
        if (rr->ops) {
            free(rr->ops);
        }
        free(rr);
    }
    return 0;
}

int next_func(RandomGenerator *rr)
{
    rr->cur = (int)((unsigned long long)rr->cur * MULT + INCR) & MOD;
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
main()
{
    RandomGenerator *rr = random_create(1234);
    for (int j = 0; j < 100; ++j) {
        printf("%d\n", rr->ops->next(rr));
    }
    rr->ops->destroy(rr);
    return 0;
}