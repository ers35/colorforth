// The author disclaims copyright to this source code.
#include "colorforth.h"

static char initialized = 0;

void
system_fn(struct state *s)
{
  push(s->stack, system((char*)pop(s->stack)));
}

void
c_alloc(struct state *s)
{
  const unsigned int size = (unsigned int)pop(s->stack);
  push(s->stack, (cell)cf_calloc(s, 1, size, CALLOC_ERROR));
}

void
c_free(struct state *s)
{
  free((char *)pop(s->stack));
}

void
require_os_fn(struct state *state)
{
  if (initialized) return;

  define_primitive_extension(state, SYSTEM_HASH,      ENTRY_NAME("system"), system_fn);
  define_primitive_extension(state, C_SUBALLOC_HASH,  ENTRY_NAME("c-alloc"), c_alloc);
  define_primitive_extension(state, C_SUBFREE_HASH,   ENTRY_NAME("c-free"), c_free);

  initialized = 1;
}
