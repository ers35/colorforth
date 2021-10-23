// The author disclaims copyright to this source code.
#ifndef __COLORFORTH_H
#define __COLORFORTH_H

#include <stdlib.h>
#include <stdio.h>
#include <conf.h>

#include <color.h>

typedef long cell;

#define CELL_FMT "ld"

#define MAX_PREFIX 7
#define MAX_OP_CODE 100

// terminal input buffer
struct tib
{
  char buf[TIB_SIZE];
  size_t len;
};

struct state;

struct code;
struct code
{
  // enum opcode opcode;
  struct code * (*fn)(struct state *state, struct code *pc);
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

  short choose_state;

  // streams
  char *str_stream;
  FILE *file_stream;
};

struct prefix_map
{
  char c;
  void (*fn)(struct state *state);
  char *color;
};

extern struct prefix_map prefix_map[];

extern void push(struct stack *stack, const cell n);
extern cell pop(struct stack *stack);

extern struct entry* find_entry(struct state *state, struct dictionary *dict);
extern void unknow_word (struct state *s, const char *msg);

extern void define_primitive(struct state *s, char name[], struct code * (*fn)(struct state *state, struct code *pc));

extern void quit(struct state *state, char ask);
extern struct state* colorforth_newstate(void);
extern void parse_colorforth(struct state *state, int c);
extern void parse_from_string(struct state *s, char *str, unsigned int len);
extern void parse_space(struct state *s);
extern struct code* clear_tib_fn (struct state *s, struct code *pc);

#define CFSTRING2C(str) ((char *)(str) + sizeof(cell))

#endif /* __COLORFORTH_H */
