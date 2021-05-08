#ifndef __CONF_H
#define __CONF_H

// Embed the lib.cf inside colorforth binary
#define __EMBED_LIB

// Enable colors in the terminal
#define __ECHO_COLOR


// Various sizes
#define STACK_SIZE 30
#define R_STACK_SIZE 30

#define DICT_SIZE 100
#define MACRO_DICT_SIZE 20

#define HEAP_SIZE 40960

#define TIB_SIZE 32

// Extensions to load
#define LOAD_EXTENTIONS                         \
  init_os_utils(state);                         \
  init_dict_utils(state);                       \
  init_io_utils(state);


#endif /* __CONF_H */
