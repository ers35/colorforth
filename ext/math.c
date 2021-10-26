// The author disclaims copyright to this source code.
#include <colorforth.h>
#include <cf-stdio.h>

void
div_fn(struct state *s)
{
  cell n1 = pop(s->stack);
  cell n2 = pop(s->stack);
  push(s->stack, n2 / n1);
}

void
mod_fn(struct state *s)
{
  cell n1 = pop(s->stack);
  cell n2 = pop(s->stack);
  push(s->stack, n2 % n1);
}

void
slash_mod_fn(struct state *s)
{
  cell n1 = pop(s->stack);
  cell n2 = pop(s->stack);
  push(s->stack, n2 / n1); push(s->stack, n2 % n1);
}

void
start_slash_fn(struct state *s)
{
  cell c = pop(s->stack);
  cell n1 = pop(s->stack);
  cell n2 = pop(s->stack);
  push(s->stack, n2 * n1 / c);
}

void
lshift_fn(struct state *s)
{
  cell n1 = pop(s->stack);
  cell n2 = pop(s->stack);
  push(s->stack, n2 << n1);
}

void
rshift_fn(struct state *s)
{
  cell n1 = pop(s->stack);
  cell n2 = pop(s->stack);
  push(s->stack, n2 >> n1);
}

void
or_fn(struct state *s)
{
  cell n1 = pop(s->stack);
  cell n2 = pop(s->stack);
  push(s->stack, n2 || n1);
}

void
and_fn(struct state *s)
{
  cell n1 = pop(s->stack);
  cell n2 = pop(s->stack);
  push(s->stack, n2 && n1);
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
  define_primitive_extension(state, "or", or_fn);
  define_primitive_extension(state, "and", and_fn);
}
