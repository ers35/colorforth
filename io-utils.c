#include "colorforth.h"

void
echo_set(struct state *s)
{
  s->echo_on = pop(s);
}

void
init_io_utils(struct state *state)
{
  define_extension(state, "echo!", OP_ECHO_SET, echo_set);
}
