#include "colorforth.h"

void
system_func(struct state *s)
{
  push(s->stack, system((char*)pop(s->stack)));
}

void
init_os_utils(struct state *state)
{
  define_primitive_extension(state, "system", OP_SYSTEM, system_func);
}
