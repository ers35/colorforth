#include "colorforth.h"

void
echo_set(struct state *s)
{
  s->echo_on = pop(s->stack);
}

void
init_io_utils(struct state *state)
{
  define_primitive_extension(state, "echo!", OP_ECHO_SET, echo_set);
}
