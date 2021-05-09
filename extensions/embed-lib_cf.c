#include "lib.cf.h"
#include "colorforth.h"

void
embed_lib_cf (struct state *state)
{
  for(unsigned int i = 0; i < lib_cf_len; i++)
  {
    parse_colorforth(state, lib_cf[i]);
  }
}
