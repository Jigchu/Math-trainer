// Tests for random.c
#pragma once

#include <stdint.h>
#include "random.h"

extern rng *seed;

void seed_t(void);
void rng32inv_t(void);
void rng64inv_t(void);
void rng32_t(uint32_t min, uint32_t max);
void rng64_t(uint64_t min, uint64_t max);
void rngflt_t(void);