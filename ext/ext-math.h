// The author disclaims copyright to this source code.
#ifndef __EXT_MATH_H
#define  __EXT_MATH_H

#ifdef __EXTENDED_MATH

struct fstack
{
  // stack
  number_t *cells;
  // stack position
  int sp;
  int lim;
};

#endif

#endif /* __EXT_MATH_H */
