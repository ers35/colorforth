// The author disclaims copyright to this source code.
#include "colorforth.h"

void
system_func(struct state *s)
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
  define_primitive_extension(state, "system", OP_SYSTEM, system_func);
  define_primitive_extension(state, "c-alloc", OP_C_ALLOC, c_alloc);
  define_primitive_extension(state, "c-free", OP_C_FREE, c_free);
}
