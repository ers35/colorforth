// The author disclaims copyright to this source code.
#include <colorforth.h>

void
echo_set(struct state *s)
{
  s->echo_on = pop(s->stack);
}

void
file_length(struct state *s)
{
}

void
load_file(struct state *s)
{
}

void
save_file(struct state *s)
{
}

void
included_file(struct state *s)
{

}

void
init_io_utils(struct state *state)
{
  define_primitive_extension(state, "echo!", OP_ECHO_SET, echo_set);
  // define_primitive_extension(state, "load", OP_LOAD, load_file);
  // define_primitive_extension(state, "save", OP_SAVE, save_file);
  define_primitive_extension(state, "included", OP_INCLUDED, included_file);
}
