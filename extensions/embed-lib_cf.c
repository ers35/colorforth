#include "colorforth.h"

#ifdef __EMBED_LIB_CF

#include "lib.cf.h"

void
embed_lib_cf (struct state *state)
{
  for(unsigned int i = 0; i < lib_cf_len; i++)
  {
    parse_colorforth(state, lib_cf[i]);
  }
}

#else
void
embed_lib_cf (struct state *state)
{
}

#endif /* __EMBED_LIB_CF */
