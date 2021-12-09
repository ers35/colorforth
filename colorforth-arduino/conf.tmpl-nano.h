// The author disclaims copyright to this source code.
#ifndef __CONF_H
#define __CONF_H

typedef int cell;
#define CELL_FMT "%d"

// Enable hashed names: reduce memory size but break words/see/disasemble
#define __HASH_NAMES

// Enable colors in the terminal
// #define __ECHO_COLOR

// Embed lib.cf
// #define __EMBED_LIB_CF

// Extended math
// #define __EXTENDED_MATH

// #define FSTACK_SIZE 30
// typedef double number_t;

// MP_MATH
// #define __MP_MATH

// #define MPSTACK_SIZE 30

// Threads
// #define __THREADS

// #define MAX_THREAD 10
// #define MAX_LOCK 10

// #define __USE_REGISTER

// Various sizes
#define STACK_SIZE 10
#define R_STACK_SIZE 10

#define DICT_SIZE 50
#define INLINED_DICT_SIZE 10

#define HEAP_SIZE 600

#define TIB_SIZE 32
#define BUF_SIZE 128

// Number of bits in a char
#define CHAR_BIT 8

// Extensions to load
struct state;

// extern void init_os_utils(struct state *s);
// extern void init_dict_utils(struct state *s);
// extern void init_io_utils(struct state *s);
// extern void init_parser_utils(struct state *s);
// extern void init_time_utils(struct state *s);
// extern void init_math_utils(struct state *s);
// extern void init_ext_math_utils(struct state *state);
// extern void init_mp_math_utils(struct state *state);
// extern void init_threads_utils(struct state *state);

#define LOAD_EXTENTIONS

//   init_dict_utils(state);
//   init_os_utils(state);                         \
//   init_io_utils(state);                         \
//   init_parser_utils(state);                     \
//   init_time_utils(state);                       \
//   init_math_utils(state);                       \
//   init_ext_math_utils(state);                   \
//   init_mp_math_utils(state);                    \
//   init_threads_utils(state);

#endif /* __CONF_H */
