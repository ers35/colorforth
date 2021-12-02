// The author disclaims copyright to this source code.
#include "colorforth.h"

void
system_fn(struct state *s)
{
  push(s->stack, system((char*)pop(s->stack)));
}

void
c_alloc(struct state *s)
{
  const unsigned int size = (unsigned int)pop(s->stack);
  push(s->stack, (cell)calloc(1, size));
}

void
c_free(struct state *s)
{
  free((char *)pop(s->stack));
}

void
init_os_utils(struct state *state)
{
  define_primitive_extension(state, "system", system_fn);
  define_primitive_extension(state, "c-alloc", c_alloc);
  define_primitive_extension(state, "c-free", c_free);
}
