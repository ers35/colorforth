// The author disclaims copyright to this source code.
#include <colorforth.h>

void
parse_from_file(struct state *s, char *filename)
{
  s->tib.len = 0;
  FILE *old_stream = s->file_stream;
  s->file_stream = fopen(filename, "r");
  if (!s->file_stream)
  {
    s->file_stream = old_stream;
    cf_printf(s, "Unable to read '%s'\n", filename);
    quit(s, 1);
    return;
  }

  int c;
  while((c = cf_getchar(s)) != CF_EOF && !s->done)
  {
    parse_colorforth(s, c);
  }

  fclose(s->file_stream);
  s->file_stream = old_stream;

  parse_space(s);
}

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
  char *filename = CFSTRING2C(pop(s->stack));
  parse_from_file(s, filename);
}

void
init_io_utils(struct state *state)
{
  define_primitive_extension(state, "echo!", OP_ECHO_SET, echo_set);
  // define_primitive_extension(state, "load", OP_LOAD, load_file);
  // define_primitive_extension(state, "save", OP_SAVE, save_file);
  define_primitive_extension(state, "included", OP_INCLUDED, included_file);
}
