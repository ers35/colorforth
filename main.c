// The author disclaims copyright to this source code.
#include <cf-stdio.h>

#include "colorforth.h"

int
main(int argc, char *argv[])
{
  init_terminal();

  struct state *state = colorforth_newstate();

  while (!state->done)
  {
    parse_colorforth(state, cf_getchar());
  }

  reset_terminal();

  return 0;
}
