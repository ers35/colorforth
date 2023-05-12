// The author disclaims copyright to this source code.
#ifndef __COLORFORTH_H
#define __COLORFORTH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include "conf.h"

#include "color.h"

#ifdef __EXTENDED_MATH
#include <ext-math.h>
#endif

#ifdef __MP_MATH
#include <mp-math.h>
#endif


#define MAX_PREFIX 10

#define CELLS s->stack->cells
// SP is the current cell in CELLS
#define SP s->stack->sp
#define LIM s->stack->lim

#define R_CELLS s->r_stack->cells
// R_SP is the current cell in R_CELLS
#define R_SP s->r_stack->sp
#define R_LIM s->r_stack->lim


#ifndef UNSAFE_MODE
#define ENSURE_STACK_MIN_GEN(x, sp, msg, action) if (sp < x - 1) { cf_printf(NULL, msg); action; }
#define ENSURE_STACK_MAX_GEN(x, sp, lim, msg, action) if (sp > lim - x) { cf_printf(NULL, msg);  action; }

#define ENSURE_STACK_MIN(x, action) ENSURE_STACK_MIN_GEN(x, SP, "ES<!\n", action)
#define ENSURE_STACK_MAX(x, action) ENSURE_STACK_MAX_GEN(x, SP, LIM, "ES>!\n", action)

#define ENSURE_R_STACK_MIN(x, action) ENSURE_STACK_MIN_GEN(x, R_SP, "ERS<!\n", action)
#define ENSURE_R_STACK_MAX(x, action) ENSURE_STACK_MAX_GEN(x, R_SP, R_LIM, "ERS>!\n", action)
#else
#define ENSURE_STACK_MIN(x, action)
#define ENSURE_STACK_MAX(x, action)

#define ENSURE_STACK_MIN(x, action)
#define ENSURE_STACK_MAX(x, action)
#endif

#define POP() CELLS[SP]; SP -= 1
#define PUSH(x) SP += 1; CELLS[SP] = (x)

#define R_POP() R_CELLS[R_SP]; R_SP -= 1
#define R_PUSH(x) R_SP += 1; R_CELLS[R_SP] = (x)

#define POP1() ENSURE_STACK_MIN(1, return); cell p1=POP()
#define POP2() ENSURE_STACK_MIN(2, return); cell p1=CELLS[SP], p2=CELLS[SP-1]; SP-=2
#define POP3() ENSURE_STACK_MIN(3, return); cell p1=CELLS[SP], p2=CELLS[SP-1], p3=CELLS[SP-2]; SP-=3
#define POP4() ENSURE_STACK_MIN(4, return); cell p1=CELLS[SP], p2=CELLS[SP-1], p3=CELLS[SP-2], p4=CELLS[SP-3]; SP-=4

#define PUSH1(p1) ENSURE_STACK_MAX(1, return); PUSH(p1)
#define PUSH2(p1, p2) ENSURE_STACK_MAX(2, return); SP+=2; CELLS[SP-1]=p1;  CELLS[SP]=p2
#define PUSH3(p1, p2, p3) ENSURE_STACK_MAX(3, return); SP+=3; CELLS[SP-2]=p1; CELLS[SP-1]=p2; CELLS[SP]=p3


#define define_register_OP(N) OP_##N##_LOAD, OP_##N##_STORE, OP_##N##_ADD, \
    OP_##N##_INC, OP_##N##_DEC, OP_##N##_R_PUSH, OP_##N##_R_POP

#define NON_NULL(x) ((x) ? (x) : "")

enum opcode
{
  OP_NOP,
  OP_FUNCTION_CALL,
  OP_PRINT_TOS,
  OP_DUP,
  OP_OVER,
  OP_SWAP,
  OP_DROP,
  OP_ROT,
  OP_MINUS_ROT,
  OP_NIP,
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
  OP_CODE_LEN,

  OP_BRANCH,
  OP_ZBRANCH,
  OP_NBRANCH,

  OP_IF,
  OP_IF_EXIT,
  OP_IF_NOT,
  OP_IF_NOT_EXIT,

  OP_IF_STAR,
  OP_IF_STAR_EXIT,
  OP_IF_NOT_STAR,
  OP_IF_NOT_STAR_EXIT,

  OP_IF_ELSE,

  OP_GET_CVA, // Code value address
  // call defined word
  OP_CALL,
  OP_TAIL_CALL,
  OP_GET_ENTRY_CODE,
  OP_EXECUTE,
  OP_EXECUTE_STAR,
  OP_COMPILE,
  OP_COMPILE_INLINE,
  OP_COMPILE_LITERAL,
  OP_NUMBER,
  OP_TICK_NUMBER,
  OP_HERE,
  OP_LATEST,
  OP_I_LATEST,
  OP_DOT_S,

  /* inlined */
  OP_RETURN,

  OP_R_PUSH,
  OP_R_POP,
  OP_R_FETCH,

  OP_CLEAR,

#ifdef __USE_REGISTER
  define_register_OP(A),
  define_register_OP(B),
  define_register_OP(C),
  define_register_OP(I),
  define_register_OP(J),
#endif

  /* Last primitive opcode - do not remove! */
  __LAST_PRIMITIVE_OP_CODE__
};

enum errors {
  STATE_ERROR                =   0,
  STACK_ERROR                =   1,
  STACK_INIT_ERROR           =   2,
  RSTACK_ERROR               =   3,
  RSTACK_INIT_ERROR          =   4,
  DICT_ERROR                 =   5,
  INLINE_DICT_ERROR          =   6,
  HEAP_ERROR                 =   7,

  PRIMITIVE_ERROR            =  11,
  DEFINE_ERROR               =  12,
  DUPLICATE_HASH_ERROR       =  13,

  CALLOC_ERROR               = 100,
  F_STACK_ERROR              = 120,

  F_LITERAL_ERROR            = 121,

  MP_STACK_INIT_ERROR        = 110,
  MP_LITERAL_ERROR           = 111,

  THREAD_CLONE_STATE_ERROR   = 130,
  THREAD_STACK_ERROR         = 131,
  THREAD_INIT_STACK_ERROR    = 132,
  THREAD_RSTACK_ERROR        = 133,
  THREAD_INIT_RSTACK_ERROR   = 134
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
  cell value;
};

struct entry;
struct entry
{
  hash_t name_hash;
#ifdef __KEEP_ENTRY_NAMES
  char *name;
  size_t name_len;
#endif
  struct code *code;
};

struct stack
{
  // stack
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
  void *heap;
  void *here;

  char base;

  char done;
  char echo_on;

  // streams
  char *str_stream;
  FILE *file_stream;

  // extended math extension
#ifdef __EXTENDED_MATH
  struct fstack fstack;
#endif

  // mp math extension
#ifdef __MP_MATH
  struct mpstack mpstack;
#endif
};

struct prefix_map
{
  char c;
  void (*fn)(struct state *state);
  char *color;
};

extern struct prefix_map prefix_map[];

extern hash_t hash(char *str);

extern void cf_print_cell(struct state *state, cell cell);

extern void push(struct stack *stack, const cell n);
extern cell pop(struct stack *stack);

extern struct entry* find_entry(struct state *state, struct dictionary *dict);
extern struct entry* find_entry_by_code(struct dictionary *dict, struct code *code);
extern struct entry* find_entry_by_fn(struct dictionary *dict, struct code *code);
extern void unknow_word (struct state *s);

extern void define_prefix(char c, void (*fn)(struct state *s), char * color, short reset);
extern void define_primitive_extension(struct state *s, hash_t hashed_name, char name[], void (*fn)(struct state *s));

extern void compile_code(struct state *s, enum opcode opcode, cell value);

extern void quit(struct state *state);
extern struct state* colorforth_newstate(void);
extern void free_state(struct state* state);

extern void parse_colorforth(struct state *state, int c);
extern void parse_from_string(struct state *s, char *str);
extern void parse_space(struct state *s);
extern void clear_tib (struct state *s);

extern void init_lib(struct state *state);

extern void *cf_calloc(struct state *state, size_t nmemb, size_t size, unsigned char id);

#define CFSTRING2C(str) ((char *)(str) + sizeof(cell))

#ifdef __cplusplus
}
#endif

#endif /* __COLORFORTH_H */
