// The author disclaims copyright to this source code.
#include "colorforth.h"

void
set_base (struct state *s)
{

  s->base = pop(s->stack);
}

void
fetch_base (struct state *s)
{

  push(s->stack, s->base);
}

void
is (struct state *s)
{
  struct entry *entry_to = (struct entry*)pop(s->stack);
  struct entry *entry_from = (struct entry*)pop(s->stack);

  entry_from->code = entry_to->code;
}

void
hide_entry (struct state *s)
{
  struct entry *entry = (struct entry*)pop(s->stack);

  free(entry->name);
  entry->name = NULL;
  entry->name_len = 0;
}

void
init_lib(struct state *state)
{
  define_primitive_extension(state, "base!", set_base);
  define_primitive_extension(state, "base@", fetch_base);
  define_primitive_extension(state, "entry/is", is);
  define_primitive_extension(state, "entry/hide", hide_entry);
}
