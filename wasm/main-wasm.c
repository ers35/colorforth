// The author disclaims copyright to this source code.
#include <string.h>
#include <colorforth.h>
#include <cf-stdio.h>

#include <emscripten/emscripten.h>

struct state *state;

EMSCRIPTEN_KEEPALIVE
void parse_cf(int c) {
  parse_colorforth(state, c);
}

int
main(int argc, char *argv[])
{
  state = colorforth_newstate();

  printf("colorForth loaded!\n");
}
