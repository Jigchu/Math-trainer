#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../src/random.c"

#include "mswsrng/mswsrng.h"

#include "tests/rng_test.h"

#include "colours.h"
#include "random.h" 

mswsrng rng;

// A test to check whether the functions generating and checking seed.h works
void seed_t(void)
{
    bool corrupted = seed_verifier();

    if (corrupted)
    {
        red();
        fprintf(stdout, ":( Seed verifier does not work\n");
        fprintf(stdout, "Cannot proceed with seed_t's subsequent tests\n");
        reset();

        return;
    }
    else
    {
        green();
        fprintf(stdout, ":) Seed verifier works\n");
        reset();
    }

    gen_seed();

    corrupted = seed_verifier();

    if (corrupted)
    {
        red();
        fprintf(stdout, ":( Seed generator does not work\n");
        reset();
    }
    else
    {
        green();
        fprintf(stdout, ":) Seed generator works\n");
        reset();
    }

    return;
}

// Makes sure invalid output cases work
void rng32inv_t(void)
{
    uint32_t result = rng.rand_uint(&rng.seed, 0, RAND32MAX);

    if (result == RAND32MAX)
    {
        green();
        fprintf(stdout, ":) msws32 RNG able to handle an invalid input in max\n");
        reset();
    }
    else
    {
        red();
        fprintf(stdout, ":( msws32 RNG unable to handle an invalid input. Output: %u\n", result);
        reset();
    }

    result = rng.rand_uint(&rng.seed, RAND32MAX, RAND32MAX);

    if (result == RAND32MAX)
    {
        green();
        fprintf(stdout, ":) msws32 RNG able to handle an invalid input in min\n");
        reset();
    }
    else
    {
        red();
        fprintf(stdout, ":( msws32 RNG unable to handle an invalid input. Output: %u\n", result);
        reset();
    }

    result = rng.rand_uint(&rng.seed, 1, 0);

    if (result == RAND32MAX)
    {
        green();
        fprintf(stdout, ":) msws32 RNG able to handle an invalid input when min is larger than max\n");
        reset();
    }
    else
    {
        red();
        fprintf(stdout, ":( msws32 RNG unable to handle an invalid input. Output: %u\n", result);
        reset();
    }

    return;
}

void rng64inv_t(void)
{
    uint64_t result = rng.rand_ullong(&rng.seed, 0, RAND64MAX);

    if (result == RAND64MAX)
    {
        green();
        fprintf(stdout, ":) msws64 RNG able to handle an invalid input in max\n");
        reset();
    }
    else
    {
        red();
        fprintf(stdout, ":( msws64 RNG unable to handle an invalid input. Output: %llu\n", result);
        reset();
    }

    result = rng.rand_ullong(&rng.seed, RAND64MAX, RAND64MAX);

    if (result == RAND64MAX)
    {
        green();
        fprintf(stdout, ":) msws64 RNG able to handle an invalid input in min\n");
        reset();
    }
    else
    {
        red();
        fprintf(stdout, ":( msws64 RNG unable to handle an invalid input. Output: %llu\n", result);
        reset();
    }

    result = rng.rand_ullong(&rng.seed, 1, 0);

    if (result == RAND64MAX)
    {
        green();
        fprintf(stdout, ":) msws64 RNG able to handle an invalid input when min is larger than max\n");
        reset();
    }
    else
    {
        red();
        fprintf(stdout, ":( msws64 RNG unable to handle an invalid input. Output: %llu\n", result);
        reset();
    }

    return;
}

// Makes sure the eng works
void rng32_t(uint32_t min, uint32_t max)
{
    uint32_t result = rng.rand_uint(&rng.seed, min, max);

    if (result >= min && result <= max)
    {
        green();
        fprintf(stdout, ":) msws32 RNG works. Result: %u\n", result);
        reset();
    }
    else
    {
        red();
        fprintf(stdout, ":( msws32 RNG does not work. max: %u, min: %u, result: %u\n", max, min, result);
        reset();
    }

    return;
}

void rng64_t(uint64_t min, uint64_t max)
{
    uint64_t result = rng.rand_ullong(&rng.seed, min, max);

    if (result >= min && result <= max)
    {
        green();
        fprintf(stdout, ":) msws64 RNG works. Result: %llu\n", result);
        reset();
    }
    else
    {
        red();
        fprintf(stdout, ":( msws64 RNG does not work. max: %llu, min: %llu, result: %llu\n", max, min, result);
        reset();
    }

    return;
}

void rngflt_t(void)
{
    long double result = rng.rand_f32(&rng.seed);

    if (result > 0 && result < 1)
    {
        green();
        fprintf(stdout, ":) msws_f32 float rng works. Result: %Lf\n", result);
        reset();
    }
    else
    {
        red();
        fprintf(stdout, ":( msws_f32 float rng does not work. Result: %Lf\n", result);
    }

    result = rng.rand_f53(&rng.seed);

    if (result > 0 && result < 1)
    {
        green();
        fprintf(stdout, ":) msws_f53 float rng works. Result: %Lf\n", result);
        reset();
    }
    else
    {
        red();
        fprintf(stdout, ":( msws_f53 float rng does not work. Result %Lf\n", result);
        reset();
    }

    long double *results = rng.rand_2f32(&rng.seed);

    if (results[0] > 0 && results[1] > 0 && results[0] < 1 && results[1] < 1)
    {
        green();
        fprintf(stdout, ":) msws_dualf32 float rng works. Results: %Lf, %Lf\n", results[0], results[1]);
        reset();
    }
    else
    {
        red();
        fprintf(stdout, ":( msws_dualf32 float rng does not work. Results: %Lf, %Lf\n", results[0], results[1]);
        reset();
    }

    return;
}