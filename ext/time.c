// The author disclaims copyright to this source code.
#include "colorforth.h"
#include "cf-stdio.h"
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

static char initialized = 0;

void
time_fn(struct state *s)
{
  time_t t;
  PUSH1((unsigned) time(&t));
}

void
utime_fn(struct state *s)
{
  struct timeval tv;
  gettimeofday(&tv,NULL);
  PUSH1(tv.tv_sec * 1000000 + tv.tv_usec);
}

void
sleep_fn(struct state *s)
{
  POP1();
  sleep(p1);
}

void
mssleep_fn(struct state *s)
{
  POP1();

  struct timespec ts;
  ts.tv_sec = p1 / 1000;
  ts.tv_nsec = (p1 % 1000) * 1000000;

  nanosleep(&ts, NULL);
}

void
require_time_fn(struct state *state)
{
  if (initialized) return;

  define_primitive_extension(state, TIME_HASH,    ENTRY_NAME("time"), time_fn);
  define_primitive_extension(state, UTIME_HASH,   ENTRY_NAME("utime"), utime_fn);
  define_primitive_extension(state, SLEEP_HASH,   ENTRY_NAME("sleep"), sleep_fn);
  define_primitive_extension(state, MSSLEEP_HASH, ENTRY_NAME("mssleep"), mssleep_fn);

  initialized = 1;
}
