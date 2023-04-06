// Header file for all the rng related functions
#pragma once

#define SEEDN_MIN 0
#define SEEDN_MAX 25000 
#define two32 4294967296
#define two53 9007199254740992
#define RAND32MAX 0xffffffff
#define RAND64MAX 0xffffffffffffffff

#include <stdbool.h>
#include <stdint.h>

bool gen_seed(void);
bool seed_verifier(void);
void msws32_s(void);
void msws64_s(void);
uint32_t msws_uint(uint32_t min, uint32_t max);
uint64_t msws_ull(uint64_t min, uint64_t max);
long double msws_f32(void);
long double *msws_dualf32(void);
long double msws_f53(void);