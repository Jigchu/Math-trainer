#pragma once

/**************************************************************************\
*                                                                          *
*  Middle-Square Weyl Sequence Random Number Generator (32-bit output)     *
*                                                                          *
*  msws32() - returns a 32 bit unsigned int [0,0xffffffff]                 *
*                                                                          *
*  The state vector consists of three 64 bit words:  x, w, and s           * 
*                                                                          *
*  x - random output [0,0xffffffff]                                        *
*  w - Weyl sequence (period 2^64)                                         *
*  s - an odd constant                                                     *
*                                                                          *
*  The Weyl sequence is added to the square of x.  The middle is extracted *
*  by shifting right 32 bits:                                              *
*                                                                          *
*  x *= x; x += (w += s); return x = (x>>32) | (x<<32);                    *
*                                                                          *
*  The constant s should be set to a random 64-bit pattern.  The utility   *
*  init_rand_digits in init.h may be used to initialize the constant.      *
*  This utility generates constants with different hexadecimal digits.     *
*  This assures sufficient change in the Weyl sequence on each iteration   *
*  of the RNG.                                                             *
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

uint64_t x = 0, w = 0, s = 0;

inline static uint32_t msws32(void)
{
   x *= x; x += (w += s); return x = (x>>32) | (x<<32);

}

/**************************************************************************\
*                                                                          *
*  Jump ahead m elements in the Weyl sequence (32-bit output)              *
*                                                                          *
*  jump_ahead(m)                                                           *
*                                                                          *
*  The jump ahead is performed by setting w += (m * s);                    *
*                                                                          *
*  This allows for the creation of separate non-overlapping streams of     *
*  data.  One might call jump_ahead(n*trillion) for example to create a    *
*  stream for processor n. Also, x is randomized so that the streams       *
*  produced will be relatively random.  The first outputs of each stream   *
*  taken as a set should be random data.  We used the method from the      * 
*  counter-based squares RNG to do this.  Several rounds of squaring are   *
*  performed and the result is stored in x.                                *
*                                                                          *
\**************************************************************************/

inline static void jump_ahead(uint64_t m)
{
   uint64_t y;

   x = w += (m * s); y = w + s;
   x = x*x + w; x = (x>>32) | (x<<32);
   x = x*x + y; x = (x>>32) | (x<<32);
   x = x*x + w; x = (x>>32) | (x<<32);

}