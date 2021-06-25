// The author disclaims copyright to this source code.
#include <colorforth.h>

#ifdef __ECHO_COLOR

#include <cf-stdio.h>
#include <unistd.h>
#include <termios.h>

struct termios old_tio;

void
echo_color(struct state *state, int c, char *color)
{
  if (state->echo_on)
  {
    if (color)
    {
      cf_printf(state, "%s%c", color, c);
    }
    else
    {
      cf_printf(state, "%c", c);
    }
  }
  else if (color)
  {
    cf_printf(state, "%s", color);
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
}

#endif /* __ECHO_COLOR */
