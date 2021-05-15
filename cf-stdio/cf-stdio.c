// The author disclaims copyright to this source code.
#include <cf-stdio.h>
#include <stdarg.h>

#ifndef __INTERNAL_STDIO

void
cf_putchar(int c)
{
  putchar(c);
}

int cf_getchar()
{
  return getchar();
}

void cf_printf(const char* format, ...)
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

#endif /* __INTERNAL_STDIO */
