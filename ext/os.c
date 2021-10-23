// The author disclaims copyright to this source code.
#include <colorforth.h>

struct code*
system_fn(struct state *s, struct code *pc)
{
  push(s->stack, system((char*)pop(s->stack)));
  return pc;
}

struct code*
c_alloc(struct state *s, struct code *pc)
{
  const unsigned int size = (unsigned int)pop(s->stack);
  push(s->stack, (cell)calloc(1, size));
  return pc;
}

struct code*
c_free(struct state *s, struct code *pc)
{
  free((char *)pop(s->stack));
  return pc;
}

void
init_os_utils(struct state *state)
{
  define_primitive(state, "system", system_fn);
  define_primitive(state, "c-alloc", c_alloc);
  define_primitive(state, "c-free", c_free);
}
