// The author disclaims copyright to this source code.
#ifndef __CONF_H
#define __CONF_H

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


// Various sizes
#define STACK_SIZE 30
#define R_STACK_SIZE 30

#define DICT_SIZE 200
#define INLINED_DICT_SIZE 100

#define HEAP_SIZE 40960

#define TIB_SIZE 32


// Extensions to load
struct state;

extern void init_os_utils(struct state *s);
extern void init_dict_utils(struct state *s);
extern void init_io_utils(struct state *s);
extern void init_parser_utils(struct state *s);
extern void init_math_utils(struct state *s);
extern void init_ext_math_utils(struct state *state);

#define LOAD_EXTENTIONS                         \
  init_os_utils(state);                         \
  init_dict_utils(state);                       \
  init_io_utils(state);                         \
  init_parser_utils(state);                     \
  init_math_utils(state);                       \
  init_ext_math_utils(state);


#endif /* __CONF_H */
