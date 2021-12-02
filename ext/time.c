// The author disclaims copyright to this source code.
#include "colorforth.h"
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

void
time_fn(struct state *s)
{
  time_t t;
  push(s->stack, (unsigned) time(&t));
}

void
utime_fn(struct state *s)
{
  struct timeval tv;
  gettimeofday(&tv,NULL);
  push(s->stack, tv.tv_sec * 1000000 + tv.tv_usec);
}

void
sleep_fn(struct state *s)
{
  cell sec = pop(s->stack);
  sleep(sec);
}

void
mssleep_fn(struct state *s)
{
  cell usec = pop(s->stack);

  struct timespec ts;
  ts.tv_sec = usec / 1000;
  ts.tv_nsec = (usec % 1000) * 1000000;

  nanosleep(&ts, NULL);
}

void
init_time_utils(struct state *state)
{
  define_primitive_extension(state, "time", time_fn);
  define_primitive_extension(state, "utime", utime_fn);
  define_primitive_extension(state, "sleep", sleep_fn);
  define_primitive_extension(state, "mssleep", mssleep_fn);
}
