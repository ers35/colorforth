#include "colorforth.h"

void
system_func(struct state *s) {
  push(s, system((char*)pop(s)));
}
