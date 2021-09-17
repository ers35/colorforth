// The author disclaims copyright to this source code.
#include <string.h>
#include <colorforth.h>
#include <cf-stdio.h>

extern void parse_from_file(struct state *s, char *filename);

void
parse_command_line(struct state *state, int argc, char *argv[])
{
  char nextIsFile = 0;
  for (int i = 1; i < argc; i++)
  {
    if (memcmp(argv[i], "-i", 2) == 0)
    {
      nextIsFile = 1;
      continue;
    }

    if (nextIsFile)
    {
      parse_from_file(state, argv[i]);
      nextIsFile = 0;
      continue;
    }

    parse_from_string(state, argv[i], 0);
  }

  if (argc == 1)
  {
    parse_colorforth(state, '~');
  }
}

int
main(int argc, char *argv[])
{
  init_terminal();

  struct state *state = colorforth_newstate();

  parse_command_line(state, argc, argv);

  while (!state->done)
  {
    parse_colorforth(state, cf_getchar(state));
  }

  reset_terminal();

  return 0;
}
