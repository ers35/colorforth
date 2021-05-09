#include "colorforth.h"

#ifdef __ECHO_COLOR

#include <stdio.h>
#include <unistd.h>
#include <termios.h>

struct termios old_tio;

void
echo_color(struct state *state, int c, char *color)
{
  printf("\b%s%c", color, c);
}

void
init_terminal()
{
  struct termios new_tio;
  tcgetattr(STDIN_FILENO,&old_tio);
  new_tio=old_tio;
  new_tio.c_lflag &=(~ICANON & ~ISIG & ~ECHO);
  tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);
}

void
reset_terminal()
{
  tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);
}

#endif /* __ECHO_COLOR */

