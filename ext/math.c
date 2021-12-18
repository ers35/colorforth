// The author disclaims copyright to this source code.
#include "colorforth.h"
#include "cf-stdio.h"

void
div_fn(struct state *s)
{
  const cell n1 = pop(s->stack);
  const cell n2 = pop(s->stack);
  push(s->stack, n2 / n1);
}

void
mod_fn(struct state *s)
{
  const cell n1 = pop(s->stack);
  const cell n2 = pop(s->stack);
  push(s->stack, n2 % n1);
}

void
slash_mod_fn(struct state *s)
{
  const cell n1 = pop(s->stack);
  const cell n2 = pop(s->stack);
  push(s->stack, n2 / n1); push(s->stack, n2 % n1);
}

void
start_slash_fn(struct state *s)
{
  const cell c = pop(s->stack);
  const cell n1 = pop(s->stack);
  const cell n2 = pop(s->stack);
  push(s->stack, n2 * n1 / c);
}

void
lshift_fn(struct state *s)
{
  const cell n1 = pop(s->stack);
  const cell n2 = pop(s->stack);
  push(s->stack, n2 << n1);
}

void
rshift_fn(struct state *s)
{
  const cell n1 = pop(s->stack);
  const cell n2 = pop(s->stack);
  push(s->stack, n2 >> n1);
}

void
sup_fn(struct state *s)
{
  const cell n1 = pop(s->stack);
  const cell n2 = pop(s->stack);
  push(s->stack, n2 > n1);
}

void
inf_equal_fn(struct state *s)
{
  const cell n1 = pop(s->stack);
  const cell n2 = pop(s->stack);
  push(s->stack, n2 <= n1);
}

void
sup_equal_fn(struct state *s)
{
  const cell n1 = pop(s->stack);
  const cell n2 = pop(s->stack);
  push(s->stack, n2 >= n1);
}

void
not_equal_fn(struct state *s)
{
  const cell n1 = pop(s->stack);
  const cell n2 = pop(s->stack);
  push(s->stack, n2 != n1);
}

void
or_fn(struct state *s)
{
  const cell n1 = pop(s->stack);
  const cell n2 = pop(s->stack);
  push(s->stack, n2 || n1);
}

void
and_fn(struct state *s)
{
  const cell n1 = pop(s->stack);
  const cell n2 = pop(s->stack);
  push(s->stack, n2 && n1);
}

void
min_fn(struct state *s)
{
  const cell n1 = pop(s->stack);
  const cell n2 = pop(s->stack);
  push(s->stack, n1 < n2 ? n1 : n2);
}

void
max_fn(struct state *s)
{
  const cell n1 = pop(s->stack);
  const cell n2 = pop(s->stack);
  push(s->stack, n1 > n2 ? n1 : n2);
}

void
rand_fn(struct state *s)
{
  push(s->stack, rand());
}

void
srand_fn(struct state *s)
{
  const cell seed = pop(s->stack);
  srand(seed);
}

void
random_fn(struct state *s)
{
  const cell n = pop(s->stack);
  push(s->stack, n != 0 ? rand() % n : 0);
}

void
init_math_utils(struct state *state)
{
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
  define_primitive_extension(state, RAND_HASH,      ENTRY_NAME("rand"), rand_fn);
  define_primitive_extension(state, SRAND_HASH,     ENTRY_NAME("srand"), srand_fn);
  define_primitive_extension(state, RANDOM_HASH,    ENTRY_NAME("random"), random_fn);
}
