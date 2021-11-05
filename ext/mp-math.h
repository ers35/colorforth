// The author disclaims copyright to this source code.
#ifndef __MP_MATH_H
#define  __MP_MATH_H

#ifdef __MP_MATH

#include <gmp.h>

struct mpstack
{
  // circular stack
  mpz_t *cells;
  // stack position
  int sp;
  int lim;
};

#endif

#endif /* __MP_MATH_H */
