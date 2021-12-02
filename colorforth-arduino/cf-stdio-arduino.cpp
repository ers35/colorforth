// The author disclaims copyright to this source code.
#include "cf-stdio.h"
#include <stdarg.h>

#include <Arduino.h>
#include <HardwareSerial.h>

void
cf_putchar(struct state *s, int c)
{
  Serial.print((char) c);
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

  while (!Serial.available()) {
    delay(2);  //delay to allow byte to arrive in input buffer
  }

  return Serial.read();
}

void cf_printf(struct state *s, const char* format, ...)
{
  va_list arg;

  char str[1024];

  va_start (arg, format);
  vsnprintf(str, 1024, format, arg);
  va_end (arg);

  for(int i = 0; i < 1024 && str[i]; i++) {
    cf_putchar(s, str[i]);
  }
}

void cf_fflush()
{
  fflush(stdout);
}
