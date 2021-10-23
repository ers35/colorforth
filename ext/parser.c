// The author disclaims copyright to this source code.
#include <colorforth.h>

// Expose the parser to colorForth
// gniark gniark: parsing recursion from colorForth itself!!
struct code*
parse_fn(struct state *s, struct code *pc)
{
  const int c = pop(s->stack);
  parse_colorforth(s, c);

  return pc;
}

void
init_parser_utils(struct state *state)
{
  define_primitive(state, "parse", parse_fn);
  define_primitive(state, "clear-tib", clear_tib_fn);
}
