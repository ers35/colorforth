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
  if (s->str_stream)
  {
    int c = *s->str_stream;
    s->str_stream += 1;
    return c;
  }

  if (s->file_stream)
  {
    char c;
    int len = fread(&c, 1, 1, s->file_stream);
    if (len != 1)
    {
      return CF_EOF;
    }
    return c;
  }

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
