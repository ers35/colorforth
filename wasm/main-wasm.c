// The author disclaims copyright to this source code.
#include <string.h>
#include "colorforth.h"
#include "cf-stdio.h"

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
  parse_from_string(state, "( This is a colorForth inspired implementation )\n");
  parse_from_string(state, "( source code: https://github.com/pbrochard/colorforth-pbr )\n");
  parse_from_string(state, "( try for example: 2 2 + . )\n");
  parse_from_string(state, "( here are the defined words )\n\n");
  parse_from_string(state, "~ words\n\n");
}
