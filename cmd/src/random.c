// A random number generator using the middle square weyl sequence RNG

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "random.h"

#include "mswsrng/init.h"
#include "mswsrng/seed.h"

#include "mswsrng/mswsrng.h"


// Generates a random unsigned int from min to max inclusive using mswsrng
uint32_t msws_uint(rng_seed *seed, uint32_t min, uint32_t max)
{
    // Check for invalid input
    if ((max >= RAND32MAX && min == 0) || min >= RAND32MAX || min > max)
    {
        return RAND32MAX;
    }

    // So that there is more uniformity when generating from a range
    uint32_t range = min == 0 ? max + 1 : max - (min - 1);
    uint32_t divisor = RAND32MAX / range;
    uint32_t result;

    do
    {
        result = msws32(seed) / divisor;
    } while (result > (range - 1));

    return result + min;
}


// Generates a random unsigned long long from 0 to max inclusive using mswsrng
uint64_t msws_ull(rng_seed *seed, uint64_t min, uint64_t max)
{
    // Check for invalid input
    if ((max >= RAND64MAX && min == 0) || min >= RAND64MAX || min > max)
    {
        return RAND64MAX;
    }

    // So that there is more uniformity when generating from a range
    uint64_t range = min == 0 ? max + 1 : max - (min - 1);
    uint64_t divisor = RAND64MAX / range;
    uint64_t result;

    do
    {
        result = msws64(seed) / divisor;
    } while (result > (range - 1));

    return result + min;
}


// Generates a 32 bit precise floating point number between 0 and 1
long double msws_f32(rng_seed *seed)
{
    long double result;
    do
    {
        result = msws32(seed) / (long double) two32;
    } while (result <= 0 || result >= 1);

    return result;
}


// Generates two 32-bit precise floating point numbers between 0 and 1 with msws64
long double *msws_dualf32(rng_seed *seed)
{
    long double *results = malloc(sizeof(long double) * 2);
    if (results == NULL)
    {
        return NULL;
    }

    union {
        uint64_t i64;
        uint32_t i32[2];
    } u;

    u.i64 = msws64(seed);

    for (int i = 0; i < 2; i++)
    {
        do
        {
            results[i] = u.i32[i] / (long double) two32;
        } while (results[i] <= 0 || results[i] >= 1);
    }

    return results;
}


// Generates a 53-bit precise floating point number between 0 and 1
long double msws_f53(rng_seed *seed)
{
    long double result;
    
    do
    {
        result = (long double) msws64(seed) / two53;
    } while (result <= 0 || result >= 1);
    

    return result;
}


// Generate seeds into seed.h in case it is corrupted, stolen from mswsrngv5 with a bit of minor tweaks
bool gen_seed(void) 
{
    FILE *fp;

    uint64_t i;

    /* create seed.h include file */

    fp = fopen("mswsrngv5/seed.h","w");
    if (fp == NULL) 
    {
        return false;
    }

    fprintf(fp, "#pragma once\n\nconst uint64_t seeds[] = {\n");

    for (i = 0; i < SEEDN_MAX; i++) {
        fprintf(fp, "\t0x%16.16llx,\n", init_rand_digits(i));
    }

    fprintf(fp, "};");

    fclose(fp);
 
    return true;
}


// Checks if seed is corrupted if corrupted returns true
bool seed_verifier(void)
{
    const long long char_count = 550045;

    // Checking file
    FILE *fp = fopen("mswsrngv5/seed.h", "r");
    if (fp == NULL) 
    {
        return false;
    }

    fseek(fp, 0, SEEK_END);
    long long offset = ftell(fp);

    return offset != char_count;
}


mswsrng msws_init(bool type)
{
    mswsrng retval;

    // Load the functions
    retval.rand_uint = &msws_uint;
    retval.rand_ullong = &msws_ull;

    retval.rand_f32 = &msws_f32;
    retval.rand_2f32 = &msws_dualf32;
    retval.rand_f53 = &msws_f53;

    // Set the seed
    retval.seed.x1 = retval.seed.w1 = retval.seed.s1 = seeds[msws_seed()];
    retval.seed.x2 = retval.seed.w2 = retval.seed.s2 = type ? seeds[msws_seed()] : 0;
    
    return retval;
}


// Generates one seed for mswsrng
int msws_seed()
{
    rng_seed seed = {
        seeds[14622], seeds[14622], seeds[14622],
        seeds[22730], seeds[22730], seeds[22730]
    };
    bool type = rand() % 2;

    jump_ahead(&seed, rand(), type);

    return type ? msws_ull(&seed, 0, SEEDN_MAX - 1) : msws_uint(&seed, 0, SEEDN_MAX - 1);
}
