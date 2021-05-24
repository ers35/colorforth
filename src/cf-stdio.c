// The author disclaims copyright to this source code.
#include <cf-stdio.h>
#include <stdarg.h>

void
cf_putchar(struct state *s, int c)
{
  putchar(c);
}

int cf_getchar(struct state *s)
{
  return getchar();
}

void cf_printf(struct state *s, const char* format, ...)
{
  va_list arg;

  va_start (arg, format);
  vfprintf(stdout, format, arg);
  va_end (arg);
}

void cf_fflush()
{
  fflush(stdout);
}
