// A random number generator using the middle square weyl sequence RNG

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "random.h"

#include "mswsrngv5/init.h"
#include "mswsrngv5/seed.h"

#include "mswsrngv5/msws32.h"
#include "mswsrngv5/msws64.h"

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

   fprintf(fp, "#ifndef SEED_H\n\n#define SEED_H\n\n");
   fprintf(fp, "const uint64_t seed[] = {\n");

   for (i = SEEDN_MIN; i < SEEDN_MAX; i++) {
      fprintf(fp, "\t0x%16.16llx,\n", init_rand_digits(i));
   }

   fprintf(fp, "};\n\n#endif");

   fclose(fp);
 
   return true;
}

// Checks if seed is corrupted if corrupted returns true
bool seed_verifier(void)
{
   const long long char_count = 550075;

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

// Changes the seed for the msws32 rng
void msws32_s(void)
{
   uint8_t bit32 = rand() % 2;

   uint32_t new_seed;

   if (bit32)
   {
      jump_ahead(rand());

      new_seed = msws_uint(SEEDN_MAX);
   }
   else if (!bit32)
   {
      jump_ahead2(rand());

      new_seed = msws_ull(SEEDN_MAX);
   }

   x = w = s = seed[new_seed];

   return;
}

// Changes the seed for the msws64 rng
void msws64_s(void)
{
   uint8_t bit32 = rand() % 2;

   uint64_t new_seed1;
   uint64_t new_seed2;

   if (bit32)
   {
      jump_ahead(rand());

      new_seed1 = msws_uint(SEEDN_MAX);
      new_seed2 = msws_uint(SEEDN_MAX);
   }
   else if (!bit32)
   {
      jump_ahead2(rand());

      new_seed1 = msws_ull(SEEDN_MAX);
      new_seed2 = msws_ull(SEEDN_MAX);
   }

   x1 = w1 = s1 = seed[new_seed1];
   x2 = w2 = s2 = seed[new_seed2];

   return;
}

// Generates a random unsigned int from 0 to max inclusive using mswsrng
uint32_t msws_uint(uint32_t max)
{
   // Check for invalid input
   if (max >= RAND32MAX)
   {
      return RAND32MAX;
   }

   // So that there is more uniformity when generating from a range
   uint32_t divisor = RAND32MAX / (max * 1);
   uint32_t result;

   do
   {
      result = msws32() / divisor;
   } while (result > max);

   return result;
   
}

// Generates a random unsigned long long from 0 to max inclusive using mswsrng
uint64_t msws_ull(uint64_t max)
{
   // Check for invalid input
   if (max >= RAND64MAX)
   {
      return (uint64_t) RAND64MAX;
   }

   // So that there is more uniformity when generating from a range
   uint32_t divisor = RAND64MAX / (max * 1);
   uint32_t result;

   do
   {
      result = msws64() / divisor;
   } while (result > max);

   return result;
}