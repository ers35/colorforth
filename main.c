#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#include "colorforth.h"

int
main(int argc, char *argv[])
{
#ifdef __ECHO_COLOR
  struct termios old_tio, new_tio;
  tcgetattr(STDIN_FILENO,&old_tio);
  new_tio=old_tio;
  new_tio.c_lflag &=(~ICANON & ~ISIG & ~ECHO);
  tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);
#endif

  struct state *state = colorforth_newstate();

  while (!state->done)
  {
    parse_colorforth(state, getchar());
  }

#ifdef __ECHO_COLOR
  tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);
#endif

  return 0;
}
