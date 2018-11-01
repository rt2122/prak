#include <stdlib.h>
#include <stdio.h>
enum
{
    MULT = 1103515245,
    INCR = 12345,
    MOD = (1U << 31) - 1
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
    free(rr->ops);
    free(rr);
    return 0;
}

int next_func(RandomGenerator *rr)
{
    rr->cur *= MULT;
    rr->cur += INCR;
    rr->cur &= MOD;
    return rr->cur;
}

RandomGenerator *random_create(int seed)
{
    RandomGenerator *rr = calloc(1, sizeof(RandomGenerator));
    rr->ops = calloc(1, sizeof(RandomOperations));
    rr->ops->destroy = destroy_func;
    rr->ops->next = next_func;
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