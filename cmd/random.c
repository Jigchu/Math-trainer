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

   fprintf(fp, "#pragma once\n\nconst uint64_t seed[] = {\n");

   for (i = SEEDN_MIN; i < SEEDN_MAX; i++) {
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

// Changes the seed for the msws32 rng
void msws32_s(void)
{
   uint8_t bit32 = rand() % 2;

   uint32_t new_seed;

   if (bit32)
   {
      jump_ahead(rand());

      new_seed = msws_uint(0, SEEDN_MAX - 1);
   }
   else if (!bit32)
   {
      jump_ahead2(rand());

      new_seed = msws_ull(0, SEEDN_MAX - 1);
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

      new_seed1 = msws_uint(0, SEEDN_MAX - 1);
      new_seed2 = msws_uint(0, SEEDN_MAX - 1);
   }
   else if (!bit32)
   {
      jump_ahead2(rand());

      new_seed1 = msws_ull(0, SEEDN_MAX - 1);
      new_seed2 = msws_ull(0, SEEDN_MAX - 1);
   }

   x1 = w1 = s1 = seed[new_seed1];
   x2 = w2 = s2 = seed[new_seed2];

   return;
}

// Generates a random unsigned int from min to max inclusive using mswsrng
uint32_t msws_uint(uint32_t min, uint32_t max)
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
      result = msws32() / divisor;
   } while (result > (range - 1));

   return result + min;
   
}

// Generates a random unsigned long long from 0 to max inclusive using mswsrng
uint64_t msws_ull(uint64_t min, uint64_t max)
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
      result = msws64() / divisor;
   } while (result > (range - 1));

   return result + min;
}

// Generates a 32 bit precise floating point number between 0 and 1
long double msws_f32(void)
{
   long double result;
   do
   {
      result = msws32() / (long double) two32;
   } while (result <= 0 || result >= 1);

   return result;
}

// Generates two 32-bit precise floating point numbers between 0 and 1 with msws64
long double *msws_dualf32(void)
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

   u.i64 = msws64();

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
long double msws_f53(void)
{
   long double result;
   
   do
   {
      result = (long double) msws64() / two53;
   } while (result <= 0 || result >= 1);
   

   return result;
}