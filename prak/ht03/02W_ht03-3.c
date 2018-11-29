#include <stdlib.h>
#include <stdio.h>
enum
{
    MULT = 1103515245,
    INCR = 12345,
    MOD = 1U << 31,
    NUM_OF_STRUCTS = 1
};

typedef struct RandomOperations
{
    int (*destroy)();
    int (*next)();
} RandomOperations;

typedef struct RandomGenerator
{
    unsigned cur;
    const RandomOperations *ops;
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
    rr->cur = (int)(rr->cur * (unsigned) MULT + INCR) & (MOD - 1);
    return rr->cur;
}

const RandomOperations opers = { destroy_func, next_func };

RandomGenerator *random_create(int seed)
{
    RandomGenerator *rr;
    rr = calloc(NUM_OF_STRUCTS, sizeof(*rr));
    rr->ops = &opers;
    rr->cur = seed;
    return rr;
}
