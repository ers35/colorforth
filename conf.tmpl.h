// The author disclaims copyright to this source code.
#ifndef __CONF_H
#define __CONF_H

typedef long cell;
#define CELL_FMT "%ld"

// Max number of branch in a word
#define MAXBRANCH 100


// Uncomment for a minimal build with only mandatory extensions
// Build with:
//   make EXTRA_CFLAGS=-D__MINIMAL_BUILD EXT_SRC=
// #define __MINIMAL_BUILD

// Unsafe mode: disable stack underflow/overflow check
// #define UNSAFE_MODE

// Uncomment to display name <-> hash relationship
// #define __SHOW_MISSING_HASH

// Enable hashed names: reduce memory size but break words/see/disasemble
#define __KEEP_ENTRY_NAMES

#include "hash_def.h"

#ifndef __MINIMAL_BUILD

// Enable register variable
#define __USE_REGISTER

// Enable dictionary clash check
#define __CHECK_DICT

#ifdef __CHECK_DICT
#define __LIVE_CHECK_DICT
#endif /* __CHECK_DICT */

// Enable colors in the terminal
#define __ECHO_COLOR

// Embed lib.cf
#define __EMBED_LIB_CF

// Use extensions
#define __USE_EXTENSIONS

// Extended math
#define __EXTENDED_MATH

#endif /* __MINIMAL_BUILD */


#define FSTACK_SIZE 30
typedef double number_t;

// MP_MATH
#define __MP_MATH

#define MPSTACK_SIZE 30

// Threads
#define __THREADS

#define MAX_THREAD 10
#define MAX_LOCK 10

// NETWORK
#define __NETWORK

// Various sizes
#define STACK_SIZE 30
#define R_STACK_SIZE 30

#define DICT_SIZE 10000
#define INLINED_DICT_SIZE 1000

#define HEAP_SIZE 40960

#define TIB_SIZE 32

// Number of bits in a char
#define CHAR_BIT 8

struct state;

#endif /* __CONF_H */
