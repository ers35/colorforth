// The author disclaims copyright to this source code.
#ifndef __COLORFORTH_H
#define __COLORFORTH_H

#include <stdlib.h>
#include <conf.h>

#include <color.h>

typedef long cell;

#define CELL_FMT "ld"

enum opcode
{
  OP_PRINT_TOS,
  OP_DUP,
  OP_OVER,
  OP_SWAP,
  OP_DROP,
  OP_ADD,
  OP_SUB,
  OP_MUL,
  OP_EQUAL,
  OP_LESS,
  OP_BYE,
  OP_WORDS,
  OP_EMIT,
  OP_KEY,
  OP_LOAD,
  OP_STORE,
  OP_CLOAD,
  OP_CSTORE,
  OP_CELL,
  // call defined word
  OP_CALL,
  OP_TAIL_CALL,
  OP_EXECUTE,
  OP_NUMBER,
  OP_TICK_NUMBER,
  OP_HERE,
  OP_LATEST,
  OP_I_LATEST,
  OP_DOT_S,

  /* inlined */
  OP_RETURN,
  OP_WHEN,
  OP_UNLESS,

  OP_R_PUSH,
  OP_R_POP,
  OP_R_FETCH,

  /* Extensions */
  OP_SEE,
  OP_DISASSEMBLE_DICT,
  OP_ROOM,
  OP_FULLROOM,
  OP_SHORTROOM,
  OP_SYSTEM,
  OP_C_ALLOC,
  OP_C_FREE,
  OP_ECHO_SET,

  /* Last non opcode - do not remove! */
  __LAST_NOT_AN_OPCODE__,
};

// terminal input buffer
struct tib
{
  char buf[TIB_SIZE];
  size_t len;
};

struct code;
struct code
{
  enum opcode opcode;
  cell this;
};

struct entry;
struct entry
{
  char *name;
  size_t name_len;
  struct code *code;
};

struct stack
{
  // circular stack
  cell *cells;
  // stack position
  int sp;
  int lim;
};

struct dictionary
{
  struct entry *entries;
  struct entry *latest;
};

struct state
{
  void (*color)(struct state *s);
  struct stack *stack;
  struct stack *r_stack;
  struct tib tib;
  struct dictionary dict;
  struct dictionary inlined_dict;
  void *heap;
  void *here;
  // track stream position for debugging compilation
  unsigned int line, coll;
  int done;
  int echo_on;

  // streams
  char *str_stream;
};

struct primitive_map
{
  char *name;
  enum opcode opcode;
  void (*func)();
} primitive_map[__LAST_NOT_AN_OPCODE__];

extern void push(struct stack *stack, const cell n);
extern cell pop(struct stack *stack);

extern struct entry* find_entry(struct state *state, struct dictionary *dict);
extern void unknow_word (struct state *s, const char *msg);

extern void define_primitive_extension(struct state *s, char name[], const enum opcode opcode, void (*func)(struct state *s));

extern struct state* colorforth_newstate(void);
extern void parse_colorforth(struct state *state, int c);
extern void parse_from_string(struct state *s, char *str, unsigned int len);

#endif /* __COLORFORTH_H */
