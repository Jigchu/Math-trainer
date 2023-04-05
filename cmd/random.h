#ifndef RANDOM_H

#define RANDOM_H

#define SEEDN_MIN 0
#define SEEDN_MAX 25000 

#include <stdbool.h>
#include <stdint.h>

#include "mswsrngv5/seed.h"

const uint32_t RAND32MAX = 0xffffffff;
const uint64_t RAND64MAX = 0xffffffffffffffff;

bool gen_seed(void);
void msws32_s(void);
void msws64_s(void);
uint32_t msws_uint(uint32_t max);
uint64_t msws_ull(uint64_t max);

#endif