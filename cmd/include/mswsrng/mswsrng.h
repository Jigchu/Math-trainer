// Implementation of Middle Square Weyl Sequence 
#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct rng_seed
{
    uint64_t x1;
    uint64_t w1;
    uint64_t s1;

    uint64_t x2;
    uint64_t w2;
    uint64_t s2;
} rng_seed;


inline static uint32_t msws32(rng_seed *seed)
{
    seed->x1 *= seed->x1; seed->x1 += (seed->w1 += seed->s1); return seed->x1 = (seed->x1>>32) | (seed->x1<<32);
}


inline static uint64_t msws64(rng_seed *seed) {
    uint64_t xx;

    seed->x1 *= seed->x1; xx = seed->x1 += (seed->w1 += seed->s1); seed->x1 = (seed->x1 >> 32) | (seed->x1 << 32);
    seed->x2 *= seed->x2;      seed->x2 += (seed->w2 += seed->s2); seed->x2 = (seed->x2 >> 32) | (seed->x2 << 32);

    return xx ^ seed->x2;
}


inline static void jump_ahead(rng_seed *seed, uint64_t m, bool msws64) {
    uint64_t y1;

    if (!msws64)
    {
        seed->x1 = seed->w1 += (m * seed->s1); y1 = seed->w1 + seed->s1;
        seed->x1 = seed->x1*seed->x1 + seed->w1; seed->x1 = (seed->x1>>32) | (seed->x1<<32);
        seed->x1 = seed->x1*seed->x1 + y1;       seed->x1 = (seed->x1>>32) | (seed->x1<<32);
        seed->x1 = seed->x1*seed->x1 + seed->w1; seed->x1 = (seed->x1>>32) | (seed->x1<<32);
    }
    else if (msws64)
    {
        uint64_t y2;

        seed->x1 = seed->w1 += (m * seed->s1); y1 = seed->w1 + seed->s1;
        seed->x2 = seed->w2 += (m * seed->s2); y2 = seed->w2 + seed->s2;
        seed->x1 = seed->x1*seed->x1 + seed->w1; seed->x1 = (seed->x1>>32) | (seed->x1<<32);
        seed->x2 = seed->x2*seed->x2 + seed->w2; seed->x2 = (seed->x2>>32) | (seed->x2<<32);
        seed->x1 = seed->x1*seed->x1 + y1;       seed->x1 = (seed->x1>>32) | (seed->x1<<32);
        seed->x2 = seed->x2*seed->x2 + y2;       seed->x2 = (seed->x2>>32) | (seed->x2<<32);
        seed->x1 = seed->x1*seed->x1 + seed->w1; seed->x1 = (seed->x1>>32) | (seed->x1<<32);
        seed->x2 = seed->x2*seed->x2 + seed->w2; seed->x2 = (seed->x2>>32) | (seed->x2<<32);
    }

    return;
}