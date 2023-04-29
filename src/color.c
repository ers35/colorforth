// The author disclaims copyright to this source code.
#include "colorforth.h"

#ifdef __ECHO_COLOR

#include "cf-stdio.h"
#include <unistd.h>
#include <termios.h>

struct termios old_tio;

void
echo_color(struct state *s, int c, char *color)
{
  if (s->echo_on)
  {
    if (color)
    {
      cf_printf(s, "%s%c", color, c);
    }
    else
    {
      cf_printf(s, "%c", c);
    }
  }
  else if (color)
  {
    cf_printf(s, "%s", color);
  }
}

void
init_terminal()
{
  struct termios new_tio;
  tcgetattr(STDIN_FILENO,&old_tio);
  new_tio=old_tio;
  new_tio.c_lflag &=(~ICANON & ~ECHO);
  tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);
}

void
reset_terminal()
{
  tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);
  printf("%s", COLOR_CLEAR);
}

#endif /* __ECHO_COLOR */
