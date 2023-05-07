// Header file for all the rng related functions
#pragma once

#define SEEDN_MAX 25000 
#define two32 0x100000000
#define two53 0x20000000000000
#define RAND32MAX 0xffffffff
#define RAND64MAX 0xffffffffffffffff

#include <stdbool.h>
#include <stdint.h>

#include <mswsrng/mswsrng.h>


typedef struct mswsrng
{
	rng_seed seed;

	uint32_t (*rand_uint)(rng_seed *, uint32_t, uint32_t);
	uint64_t (*rand_ullong)(rng_seed *, uint64_t, uint64_t);

	long double (*rand_f32)(rng_seed *);
	long double *(*rand_2f32)(rng_seed *);
	long double (*rand_f53)(rng_seed *)
} mswsrng;


bool gen_seed(void);
bool seed_verifier(void);
mswsrng msws_init(bool type);