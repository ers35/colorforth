// The author disclaims copyright to this source code.
#include <string.h>
#include "colorforth.h"
#include "cf-stdio.h"

extern void parse_from_file(struct state *s, char *filename);

void
parse_command_line(struct state *s, int argc, char *argv[])
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
      //parse_from_string(s, argv[i]);
      nextIsEval = 0;
      continue;
    }

    //parse_from_file(s, argv[i]);
  }

  //parse_colorforth(s, '~');
}



int
main_main(int argc, char *argv[])
{
  if (argc ==3 && memcmp(argv[1], "--hash", 2) == 0) {
    printf("0x%X", hash(argv[2]));
    return 0;
  }

  init_terminal();

  struct state *s = colorforth_newstate();

  parse_command_line(s, argc, argv);

  while (!s->done)
  {
    parse_colorforth(s, cf_getchar(s));
  }

  free_state(s);

  reset_terminal();

  return 0;
}

int
main_test(int argc, char *argv[])
{
  struct state *s = cf_calloc(NULL, 1, sizeof(*s), STATE_ERROR);
  s->heap = calloc(100, sizeof(uint8_t));

  HEAP(0, uint8_t) = 'a';
  HEAP(1, uint8_t) = 'b';
  HEAP(2, cell) = 0xFFFF;

  HEAP(10, uint32_t) = (uint32_t) 0xEEAACCDD99;
  HEAP(20, uint32_t) = (uint32_t) 0xEEAACCDD;
  HEAP(30, uint64_t) = (uint64_t) 0xEEAACCDD99887766;

  HEAP(40, uint8_t) = 'c';
  HEAP(41, uint16_t) = 0xAABB;
  HEAP(44, uint8_t) = 'd';

  s->here = 50;
  HEAP(s->here, uint8_t) = 'X'; s->here += sizeof(uint8_t);
  STORE('Y', uint8_t);
  STORE(0xAABB, uint16_t);
  STORE(0xCCDDEEFF, uint32_t);

  s->here = 60;
  STORE(0xAABBCCDDEEFF9988, uint64_t);
  STORE('A', uint8_t);
  STORE('B', uint8_t);

  printf("\n\n");

  for (int i = 0; i < 99; i++) {
    printf("%10X ", *((uint8_t *)s->heap + i));
    if ((i + 1) % 10 == 0) printf("\n");
  }

  return 0;
}

int
main(int argc, char *argv[])
{
  return main_main(argc, argv);
  // return main_test(argc, argv);
}
