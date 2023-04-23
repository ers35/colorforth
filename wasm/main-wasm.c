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
  parse_from_string(state, "% cr \" This is a colorForth inspired implementation\" prn\n");
  parse_from_string(state, "\"   source code: https://github.com/pbrochard/colorforth-pbr\" prn\n");
  parse_from_string(state, "\" Try for example: 2 2 + .\" prn\n");
  parse_from_string(state, "cr \" Here are the defined words:\" prn\n\n");
  parse_from_string(state, "~ echo on words\n\n");
}
