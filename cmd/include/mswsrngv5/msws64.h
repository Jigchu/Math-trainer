#pragma once
/**************************************************************************\
*                                                                          *
*  Middle-Square Weyl Sequence Random Number Generator (64-bit output)     *
*                                                                          *
*  msws64() - returns a 64 bit unsigned int [0,0xffffffffffffffff]         *
*                                                                          *
*  This 64-bit version performs the same computations as the 32-bit        *
*  version.  Half of the computations are on x1, w1, and s1.  The          *
*  rest are on x2, w2, and s2.  Instruction-level parallelism (ILP)        *
*  on modern CPUs makes it possible to perform some of the computations    *
*  in parallel.  For this reason, this version is faster than calling      *
*  the 32-bit version twice.                                               *
*                                                                          *
*  The constants s1 and s2 should be set to a random 64-bit patterns.      *
*  The utility init_rand_digits in init.h may be used to initialize the    *
*  constants.  This utility generates constants with different hexadecimal *
*  digits.  This assures sufficient change in the Weyl sequence on each    *
*  iteration of the RNG.                                                   *
*                                                                          *
*  Note:  This version of the RNG includes an idea proposed by             *
*  Richard P. Brent (creator of the xorgens RNG).  Brent suggested         *
*  adding the Weyl sequence after squaring instead of before squaring.     *
*  This provides a basis for uniformity in the output.                     *
*                                                                          *
*  Copyright (c) 2014-2022 Bernard Widynski                                *
*                                                                          *
*  This code can be used under the terms of the GNU General Public License *
*  as published by the Free Software Foundation, either version 3 of the   *
*  License, or any later version. See the GPL license at URL               *
*  http://www.gnu.org/licenses                                             *
*                                                                          *
\**************************************************************************/

#include <stdint.h>

uint64_t x1 = 0, w1 = 0, s1 = 0;
uint64_t x2 = 0, w2 = 0, s2 = 0;


inline static uint64_t msws64() {

   uint64_t xx;

   x1 *= x1; xx = x1 += (w1 += s1); x1 = (x1 >> 32) | (x1 << 32);
   x2 *= x2;      x2 += (w2 += s2); x2 = (x2 >> 32) | (x2 << 32);

   return xx ^ x2;

}

/**************************************************************************\
*                                                                          *
*  Jump ahead m elements in the Weyl sequence (64-bit output)              *
*                                                                          *
*  jump_ahead2(m)                                                          *
*                                                                          *
*  The jump ahead is performed by setting w1 += (m * s1);                  *
*                                         w2 += (m * s2);                  *
*                                                                          *
*  This allows for the creation of separate non-overlapping streams of     *
*  data.  One might call jump_ahead2(n*trillion) for example to create a   *
*  stream for processor n. Also, x1 and x2 are randomized so that the      *
*  streams produced will be relatively random.  The first outputs of each  *
*  stream taken as a set should be random data.  We used the method from   * 
*  the counter-based squares RNG to do this.  Several rounds of squaring   *
*  are performed and the results is stored in x1 and x2.                   *
*                                                                          *
\**************************************************************************/

inline static void jump_ahead2(uint64_t m) {

   uint64_t y1, y2;

   x1 = w1 += (m * s1); y1 = w1 + s1;
   x2 = w2 += (m * s2); y2 = w2 + s2;
   x1 = x1*x1 + w1; x1 = (x1>>32) | (x1<<32);
   x2 = x2*x2 + w2; x2 = (x2>>32) | (x2<<32);
   x1 = x1*x1 + y1; x1 = (x1>>32) | (x1<<32);
   x2 = x2*x2 + y2; x2 = (x2>>32) | (x2<<32);
   x1 = x1*x1 + w1; x1 = (x1>>32) | (x1<<32);
   x2 = x2*x2 + w2; x2 = (x2>>32) | (x2<<32);

}