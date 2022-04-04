// The author disclaims copyright to this source code.
#include "colorforth.h"

extern void require_os_fn(struct state *s);
extern void require_dict_fn(struct state *s);
extern void require_io_fn(struct state *s);
extern void require_parser_fn(struct state *s);
extern void require_time_fn(struct state *s);
extern void require_math_fn(struct state *s);
extern void require_ext_math_fn(struct state *state);
extern void require_mp_math_fn(struct state *state);
extern void require_threads_fn(struct state *state);


void
load_extensions(struct state *state)
{
  define_primitive_extension(state, REQUIRE_OS_HASH,          ENTRY_NAME("require-os"), require_os_fn);
  define_primitive_extension(state, REQUIRE_DICT_HASH,        ENTRY_NAME("require-dict"), require_dict_fn);
  define_primitive_extension(state, REQUIRE_IO_HASH,          ENTRY_NAME("require-io"), require_io_fn);
  define_primitive_extension(state, REQUIRE_PARSER_HASH,      ENTRY_NAME("require-parser"), require_parser_fn);
  define_primitive_extension(state, REQUIRE_TIME_HASH,        ENTRY_NAME("require-time"), require_time_fn);
  define_primitive_extension(state, REQUIRE_MATH_HASH,        ENTRY_NAME("require-math"), require_math_fn);
  define_primitive_extension(state, REQUIRE_EXT_MATH_HASH,    ENTRY_NAME("require-ext-math"), require_ext_math_fn);
  define_primitive_extension(state, REQUIRE_MP_MATH_HASH,     ENTRY_NAME("require-mp-math"), require_mp_math_fn);
  define_primitive_extension(state, REQUIRE_THREADS_HASH,     ENTRY_NAME("require-threads"), require_threads_fn);
}
