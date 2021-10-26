// The author disclaims copyright to this source code.
#include <colorforth.h>
#include <cf-stdio.h>
#include <time.h>

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
  push(s->stack, rand() % n);
}

void
time_fn(struct state *s)
{
  time_t t;
  push(s->stack, (unsigned) time(&t));
}

void
init_math_utils(struct state *state)
{
  define_primitive_extension(state, "/", div_fn);
  define_primitive_extension(state, "mod", mod_fn);
  define_primitive_extension(state, "/mod", slash_mod_fn);
  define_primitive_extension(state, "*/", start_slash_fn);
  define_primitive_extension(state, "lshift", lshift_fn);
  define_primitive_extension(state, "rshift", rshift_fn);
  define_primitive_extension(state, ">", sup_fn);
  define_primitive_extension(state, "<=", inf_equal_fn);
  define_primitive_extension(state, ">=", sup_equal_fn);
  define_primitive_extension(state, "<>", not_equal_fn);
  define_primitive_extension(state, "or", or_fn);
  define_primitive_extension(state, "and", and_fn);
  define_primitive_extension(state, "min", min_fn);
  define_primitive_extension(state, "max", max_fn);
  define_primitive_extension(state, "rand", rand_fn);
  define_primitive_extension(state, "srand", srand_fn);
  define_primitive_extension(state, "random", random_fn);
  define_primitive_extension(state, "time", time_fn);
}
