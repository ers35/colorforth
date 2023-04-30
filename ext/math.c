// The author disclaims copyright to this source code.
#include "colorforth.h"
#include "cf-stdio.h"

static char initialized = 0;

#define modulo(n2, n1) ((n2 < 0) ? (n2 % n1 + n1) : (n2 % n1))

void
div_fn(struct state *s)
{
  POP2();
  PUSH1(p2 / p1);
}

void
mod_fn(struct state *s)
{
  POP2();
  PUSH1(modulo(p2, p1));
}

void
slash_mod_fn(struct state *s)
{
  POP2();
  PUSH2(p2 / p1, modulo(p2, p1));
}

void
start_slash_fn(struct state *s)
{
  POP3();
  PUSH1(p3 * p2 / p1);
}

void
lshift_fn(struct state *s)
{
  POP2();
  PUSH1(p2 << p1);
}

void
rshift_fn(struct state *s)
{
  POP2();
  PUSH1(p2 >> p1);
}

void
sup_fn(struct state *s)
{
  POP2();
  PUSH1(p2 > p1);
}

void
inf_equal_fn(struct state *s)
{
  POP2();
  PUSH1(p2 <= p1);
}

void
sup_equal_fn(struct state *s)
{
  POP2();
  PUSH1(p2 >= p1);
}

void
not_equal_fn(struct state *s)
{
  POP2();
  PUSH1(p2 != p1);
}

void
or_fn(struct state *s)
{
  POP2();
  PUSH1(p2 || p1);
}

void
and_fn(struct state *s)
{
  POP2();
  PUSH1(p2 && p1);
}

void
min_fn(struct state *s)
{
  POP2();
  PUSH1(p1 < p2 ? p1 : p2);
}

void
max_fn(struct state *s)
{
  POP2();
  PUSH1(p1 > p2 ? p1 : p2);
}

void
abs_fn(struct state *s)
{
  POP1();
  PUSH1(labs(p1));
}

void
rand_fn(struct state *s)
{
  PUSH1(rand());
}

void
srand_fn(struct state *s)
{
  POP1();
  srand(p1);
}

void
random_fn(struct state *s)
{
  POP1();
  PUSH1(p1 != 0 ? rand() % p1 : 0);
}

void
require_math_fn(struct state *state)
{
  if (initialized) return;

  define_primitive_extension(state, DIV_HASH,       ENTRY_NAME("/"), div_fn);
  define_primitive_extension(state, MOD_HASH,       ENTRY_NAME("mod"), mod_fn);
  define_primitive_extension(state, DIVMOD_HASH,    ENTRY_NAME("/mod"), slash_mod_fn);
  define_primitive_extension(state, MUL_DIV_HASH,   ENTRY_NAME("*/"), start_slash_fn);
  define_primitive_extension(state, LSHIFT_HASH,    ENTRY_NAME("lshift"), lshift_fn);
  define_primitive_extension(state, RSHIFT_HASH,    ENTRY_NAME("rshift"), rshift_fn);
  define_primitive_extension(state, SUP_HASH,       ENTRY_NAME(">"), sup_fn);
  define_primitive_extension(state, INF_EQUAL_HASH, ENTRY_NAME("<="), inf_equal_fn);
  define_primitive_extension(state, SUP_EQUAL_HASH, ENTRY_NAME(">="), sup_equal_fn);
  define_primitive_extension(state, INFSUP_HASH,    ENTRY_NAME("<>"), not_equal_fn);
  define_primitive_extension(state, OR_HASH,        ENTRY_NAME("or"), or_fn);
  define_primitive_extension(state, AND_HASH,       ENTRY_NAME("and"), and_fn);
  define_primitive_extension(state, MIN_HASH,       ENTRY_NAME("min"), min_fn);
  define_primitive_extension(state, MAX_HASH,       ENTRY_NAME("max"), max_fn);
  define_primitive_extension(state, ABS_HASH,       ENTRY_NAME("abs"), abs_fn);
  define_primitive_extension(state, RAND_HASH,      ENTRY_NAME("rand"), rand_fn);
  define_primitive_extension(state, SRAND_HASH,     ENTRY_NAME("srand"), srand_fn);
  define_primitive_extension(state, RANDOM_HASH,    ENTRY_NAME("random"), random_fn);

  initialized = 1;
}
