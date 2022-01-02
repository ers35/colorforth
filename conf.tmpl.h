// The author disclaims copyright to this source code.
#ifndef __CONF_H
#define __CONF_H

typedef long cell;
#define CELL_FMT "%ld"

#define DEFAULT_BREAK_ON_UNKNOWN_WORD 0

// Enable hashed names: reduce memory size but break words/see/disasemble
#define __KEEP_ENTRY_NAMES

// Uncomment to display name <-> hash relationship
// #define __SHOW_MISSING_HASH

#include "hash_def.h"

// Enable register variable
#define __USE_REGISTER

// Enable colors in the terminal
#define __ECHO_COLOR

// Embed lib.cf
#define __EMBED_LIB_CF

// Extended math
#define __EXTENDED_MATH

#define FSTACK_SIZE 30
typedef double number_t;

// MP_MATH
#define __MP_MATH

#define MPSTACK_SIZE 30

// Threads
#define __THREADS

#define MAX_THREAD 10
#define MAX_LOCK 10


// Various sizes
#define STACK_SIZE 30
#define R_STACK_SIZE 30

#define DICT_SIZE 400
#define INLINED_DICT_SIZE 100

#define HEAP_SIZE 40960

#define TIB_SIZE 32

// Number of bits in a char
#define CHAR_BIT 8

// Extensions to load
struct state;

extern void init_os_utils(struct state *s);
extern void init_dict_utils(struct state *s);
extern void init_io_utils(struct state *s);
extern void init_parser_utils(struct state *s);
extern void init_time_utils(struct state *s);
extern void init_math_utils(struct state *s);
extern void init_ext_math_utils(struct state *state);
extern void init_mp_math_utils(struct state *state);
extern void init_threads_utils(struct state *state);

#define LOAD_EXTENTIONS                         \
  init_os_utils(state);                         \
  init_dict_utils(state);                       \
  init_io_utils(state);                         \
  init_parser_utils(state);                     \
  init_time_utils(state);                       \
  init_math_utils(state);                       \
  init_ext_math_utils(state);                   \
  init_mp_math_utils(state);                    \
  init_threads_utils(state);


#endif /* __CONF_H */
