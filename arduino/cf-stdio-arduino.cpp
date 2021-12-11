// The author disclaims copyright to this source code.
#include "cf-stdio.h"
#include <stdarg.h>

#include <Arduino.h>
#include <HardwareSerial.h>

void
cf_putchar(struct state *s __attribute__((unused)), int c)
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

char str[BUF_SIZE];

void cf_printf(struct state *s, const char* format, ...)
{
  va_list arg;

  va_start (arg, format);
  vsnprintf(str, BUF_SIZE, format, arg);
  va_end (arg);

  for(int i = 0; i < BUF_SIZE && str[i]; i++) {
    cf_putchar(s, str[i]);
  }
}

void cf_fflush()
{
  delay(200);
  while (Serial.available()) {
    delay(200);  //delay to allow byte to arrive in input buffer
  }
}
