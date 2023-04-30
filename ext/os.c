// The author disclaims copyright to this source code.
#include "colorforth.h"
#include "cf-stdio.h"

static char initialized = 0;

void
system_fn(struct state *s)
{
  POP1();
  PUSH1(system((char*)p1));
}

void
c_alloc(struct state *s)
{
  POP1();
  const unsigned int size = (unsigned int)p1;
  PUSH1((cell)cf_calloc(s, 1, size, CALLOC_ERROR));
}

void
c_free(struct state *s)
{
  POP1();
  free((char *)p1);
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
