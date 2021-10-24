// The author disclaims copyright to this source code.
#include <colorforth.h>

// Expose the parser to colorForth
// gniark gniark: parsing recursion from colorForth itself!!
void
parse(struct state *s)
{
  const int c = pop(s->stack);
  parse_colorforth(s, c);
}

void
init_parser_utils(struct state *state)
{
  define_primitive_extension(state, "parse", parse);
  define_primitive_extension(state, "clear-tib", clear_tib);
}
