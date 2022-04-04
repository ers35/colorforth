// The author disclaims copyright to this source code.
#ifndef __CONF_H
#define __CONF_H

typedef long cell;
#define CELL_FMT "%ld"

// Unsafe mode: disable stack underflow/overflow check
// #define UNSAFE_MODE

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

struct state;

#endif /* __CONF_H */
