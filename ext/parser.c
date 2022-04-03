// The author disclaims copyright to this source code.
#include "colorforth.h"

static char initialized = 0;

// Expose the parser to colorForth
// gniark gniark: parsing recursion from colorForth itself!!
void
parse(struct state *s)
{
  const int c = pop(s->stack);
  parse_colorforth(s, c);
}

void
require_parser_fn(struct state *state)
{
  if (initialized) return;

  define_primitive_extension(state, PARSE_HASH,        ENTRY_NAME("parse"), parse);
  define_primitive_extension(state, CLEAR_TIB_HASH,    ENTRY_NAME("clear-tib"), clear_tib);

  initialized = 1;
}
