#include "colorforth.h"

void
system_func(struct state *s)
{
  push(s, system((char*)pop(s)));
}

void
init_os_utils(struct state *state)
{
  define_extension(state, "system", OP_SYSTEM, system_func);
}
