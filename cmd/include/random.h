// Header file for all the rng related functions
#pragma once

#define SEEDN_MAX 25000 
#define two32 0x100000000
#define two53 0x20000000000000
#define RAND32MAX 0xffffffff
#define RAND64MAX 0xffffffffffffffff

#include <stdbool.h>
#include <stdint.h>

// A struct for the seed for the rng
typedef struct mswsrng rng;


rng *msws_init(bool u64);
void free_rng(rng *in);
uint32_t rand_u32(rng *seed, uint32_t min, uint32_t max);
uint64_t rand_u64(rng *seed, uint64_t min, uint64_t max);
long double rand_f32(rng *seed);
long double *rand_2f32(rng *seed);
long double rand_f53(rng *seed);
bool gen_seed(void);
bool seed_verifier(void);