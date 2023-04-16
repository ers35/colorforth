// The author disclaims copyright to this source code.
#include <string.h>
#include "colorforth.h"
#include "cf-stdio.h"

extern void parse_from_file(struct state *s, char *filename);

void
parse_command_line(struct state *state, int argc, char *argv[])
{
  char nextIsEval = 0;
  for (int i = 1; i < argc; i++)
  {
    if (memcmp(argv[i], "-e", 2) == 0)
    {
      nextIsEval = 1;
      continue;
    }

    if (nextIsEval)
    {
      parse_from_string(state, argv[i]);
      nextIsEval = 0;
      continue;
    }

    parse_from_file(state, argv[i]);
  }

  parse_colorforth(state, '~');
}

int
main(int argc, char *argv[])
{
   if (argc ==3 && memcmp(argv[1], "--hash", 2) == 0) {
     printf("0x%lX", hash(argv[2]));
     return 0;
   }

  init_terminal();

  struct state *state = colorforth_newstate();

  parse_command_line(state, argc, argv);

  while (!state->done)
  {
    parse_colorforth(state, cf_getchar(state));
  }

  free_state(state);

  reset_terminal();

  return 0;
}
