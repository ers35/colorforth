// The author disclaims copyright to this source code.
#include "colorforth.h"

extern void require_io_fn(struct state *s);
extern void require_parser_fn(struct state *s);

#ifndef __MINIMAL_BUILD
extern void require_os_fn(struct state *s);
extern void require_dict_fn(struct state *s);
extern void require_time_fn(struct state *s);
extern void require_math_fn(struct state *s);
extern void require_ext_math_fn(struct state *state);
extern void require_mp_math_fn(struct state *state);
extern void require_threads_fn(struct state *state);
extern void require_network_fn(struct state *state);
extern void require_sdl_fn(struct state *state);
#endif /* __MINIMAL_BUILD */

void
load_extensions(struct state *state)
{
  define_primitive_extension(state, REQUIRE_IO_HASH,          ENTRY_NAME("require-io"), require_io_fn);
  define_primitive_extension(state, REQUIRE_PARSER_HASH,      ENTRY_NAME("require-parser"), require_parser_fn);

#ifndef __MINIMAL_BUILD

  define_primitive_extension(state, REQUIRE_OS_HASH,          ENTRY_NAME("require-os"), require_os_fn);
  define_primitive_extension(state, REQUIRE_DICT_HASH,        ENTRY_NAME("require-dict"), require_dict_fn);
  define_primitive_extension(state, REQUIRE_TIME_HASH,        ENTRY_NAME("require-time"), require_time_fn);
  define_primitive_extension(state, REQUIRE_MATH_HASH,        ENTRY_NAME("require-math"), require_math_fn);
#ifdef __EXTENDED_MATH
  define_primitive_extension(state, REQUIRE_EXT_MATH_HASH,    ENTRY_NAME("require-ext-math"), require_ext_math_fn);
#endif
#ifdef __MP_MATH
  define_primitive_extension(state, REQUIRE_MP_MATH_HASH,     ENTRY_NAME("require-mp-math"), require_mp_math_fn);
#endif
#ifdef __THREADS
  define_primitive_extension(state, REQUIRE_THREADS_HASH,     ENTRY_NAME("require-threads"), require_threads_fn);
#endif
#ifdef __NETWORK
  define_primitive_extension(state, REQUIRE_NETWORK_HASH,     ENTRY_NAME("require-network"), require_network_fn);
#endif

#ifdef __SDL
  define_primitive_extension(state, REQUIRE_SDL_HASH,         ENTRY_NAME("require-sdl"), require_sdl_fn);
#endif

#endif /* __MINIMAL_BUILD */
}
