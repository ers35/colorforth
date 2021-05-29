// The author disclaims copyright to this source code.
#ifndef __CONF_H
#define __CONF_H

// Enable colors in the terminal
#define __ECHO_COLOR

// Embed lib.cf
#define __EMBED_LIB_CF

// Various sizes
#define STACK_SIZE 30
#define R_STACK_SIZE 30

#define DICT_SIZE 100
#define INLINED_DICT_SIZE 100

#define HEAP_SIZE 40960

#define TIB_SIZE 32


// Extensions to load
struct state;

extern void init_os_utils(struct state *s);
extern void init_dict_utils(struct state *s);
extern void init_io_utils(struct state *s);

#define LOAD_EXTENTIONS                         \
  init_os_utils(state);                         \
  init_dict_utils(state);                       \
  init_io_utils(state);


#endif /* __CONF_H */
