// The author disclaims copyright to this source code.

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "colorforth.h"
#include "cf-stdio.h"

#ifdef __CHECK_DICT
extern char check_entry(struct state *s, struct entry *check_entry);
extern void display_clash_found(struct state *s, char clash_found);
#endif /* __CHECK_DICT */

struct prefix_map prefix_map[MAX_PREFIX];

#define define_register(N)                                                     \
  case OP_##N##_STORE: { ENSURE_STACK_MIN(1, break);  N = POP(); break; }      \
  case OP_##N##_LOAD: { ENSURE_STACK_MAX(1, break);   PUSH(N); break; }        \
  case OP_##N##_ADD: { ENSURE_STACK_MIN(1, break);    N += POP(); break; }     \
  case OP_##N##_INC: { N += 1; break; }                                        \
  case OP_##N##_DEC: { N -= 1; break; }                                        \
  case OP_##N##_R_POP: { ENSURE_R_STACK_MIN(1, break); N = R_POP(); break; }   \
  case OP_##N##_R_PUSH: { ENSURE_R_STACK_MAX(1, break); R_PUSH(N); break; }

#define define_register_primitive(N)                                    \
  define_primitive(s, REG_##N##_LOAD_HASH,        ENTRY_NAME(#N"@"), OP_##N##_LOAD); \
  define_primitive(s, REG_##N##_STORE_HASH,       ENTRY_NAME(#N"!"), OP_##N##_STORE); \
  define_primitive(s, REG_##N##_ADD_STORE_HASH,   ENTRY_NAME(#N"+!"), OP_##N##_ADD); \
  define_primitive(s, REG_##N##_ADD_ADD_HASH,     ENTRY_NAME(#N"++"), OP_##N##_INC); \
  define_primitive(s, REG_##N##_SUB_SUB_HASH,     ENTRY_NAME(#N"--"), OP_##N##_DEC); \
  define_primitive(s, REG_##N##_TO_R_HASH,        ENTRY_NAME(#N">R"), OP_##N##_R_PUSH); \
  define_primitive(s, REG_R_TO_##N##_HASH,        ENTRY_NAME("R>"#N), OP_##N##_R_POP);

#include "utils.c"

// 'hashed_name' is 'hash(name)' or 0x0 if names are kept
// 'name' must be null-terminated.
static void
define_primitive(struct state *s, char name[] __attribute__((unused)), hash_t hashed_name)
{
  struct entry *found_entry = find_entry_by_hash(&s->dict, hashed_name);
  if (found_entry)
  {
#ifdef __KEEP_ENTRY_NAMES
    cf_printf(s, "'%s' clash with '%s'\n", name, found_entry->name);
#endif /* __KEEP_ENTRY_NAMES */

    cf_fatal_error(s, DUPLICATE_HASH_ERROR);
  }

  s->dict.latest++;
  struct entry *entry = &s->dict.entries[s->dict.latest];

  entry->opcode = hashed_name;

#ifdef __KEEP_ENTRY_NAMES
  if (!entry->opcode)
  {
    entry->opcode = hash(name);
  }

  entry->name_len = strlen(NON_NULL(name));
  entry->name = cf_calloc(s, 1, entry->name_len, PRIMITIVE_ERROR);
  memcpy(entry->name, NON_NULL(name), entry->name_len);
#endif

#ifdef __SHOW_MISSING_HASH
#ifdef __KEEP_ENTRY_NAMES
  char *up_name = cf_calloc(s, 1, entry->name_len + 1, PRIMITIVE_ERROR);
  memcpy(up_name, name, entry->name_len + 1);

  char *p = up_name;
  while((*p=toupper(*p))) p++;

  cf_printf(s, "%20s <-> %-20lX   hashed_name=%-20lX | %20s_HASH %20lX\n", name, entry->opcode, hashed_name, up_name, hash(name));

  free(up_name);
#else
  cf_printf(s, "%-20lX %lX\n", entry->opcode, hashed_name);
#endif
#endif

  // The opcode will be overwritten at execution time
  entry->offset = 0;
}

/**
 *
 * Color functions
 *
 **/
static void
define(struct state *s)
{
  struct dictionary * dict = &s->dict;
  dict->latest++;
  struct entry *entry = &dict->entries[dict->latest];

  entry->opcode = hash(s->tib.buf);

#ifdef __KEEP_ENTRY_NAMES
  entry->name_len = s->tib.len;
  entry->name = cf_calloc(s, 1, entry->name_len, DEFINE_ERROR);
  memcpy(entry->name, s->tib.buf, s->tib.len);

#ifdef __LIVE_CHECK_DICT
  // TODO
  //if (check_entry(s, entry)) display_clash_found(s, 1);
#endif /* __LIVE_CHECK_DICT */
#endif

  entry->offset = s->here;
}

// inline void
// compile_code(struct state *s, cell code, cell value)
// {
//   ((cell *)s->heap)[s->here] = code;
//   s->here += 1;
//   // *((cell *) s->heap + s->here * sizeof(cell)) = code;
//   // s->here += 1;
//
//   ((cell *)s->heap)[s->here] = value;
//   s->here += 1;
// }

static void
compile_entry(struct state *s, struct entry *entry)
{
  if (entry->offset == 0) {
    STORE(entry->opcode, opcode_t);
  } else if (entry == &s->dict.entries[s->dict.latest]) {
    STORE(OP_TAIL_CALL, opcode_t);
    STORE(entry->offset, cell);
  } else {
    STORE(OP_CALL, opcode_t);
    STORE(entry->offset, cell);
  }
}

// static void
// inline_entry(struct state *s, struct entry *entry)
// {
//   for (size_t i = 0, done = 0; !done; i++)
//   {
//     // inline OP_RETURN in first position
//     if (entry->code[i].opcode == OP_RETURN && i > 0)
//     {
//       break;
//     }
//     compile_code(s, entry->code[i].opcode, entry->code[i].value);
//   }
// }

static void
compile_literal(struct state *s, cell n)
{
  //compile_code(s, OP_NUMBER, n);
  STORE(OP_NUMBER, opcode_t);
  STORE(n, cell);
}

static void
compile(struct state *s)
{
  struct entry *entry = find_entry(s, &s->dict);
  if (entry)
  {
    compile_entry(s, entry);
  }
  else
  {
    // try to interpret as an unsigned decimal number
    cell n = 0;
    if (tib_to_number(s, &n))
    {
      compile_literal(s, n);
    }
    else
    {
      unknow_word(s);
    }
  }
}

#define END break

void
execute_(struct state *s, struct entry *entry)
{
  // cf_printf(s, "-> %s offset=%d\n", entry->name, entry->offset);
  cell pc = entry->offset;

  if (entry->offset == 0) {
    HEAP(0, opcode_t) = (opcode_t) entry->opcode;
  }

  ENSURE_R_STACK_MAX(1, return);
  R_PUSH(0);

// #ifdef __USE_REGISTER
//   register cell A = 0;
//   register cell B = 0;
//   register cell C = 0;
//   register cell I = 0;
//   register cell J = 0;
// #endif
//

  // don't forget to COMPILE a return!!!!
  while(1)
  {
    switch (HEAP(pc, opcode_t))
    {
      case OP_RETURN:
      {
        ENSURE_R_STACK_MIN(1, END);
        cell offset = R_POP();
        if (offset == 0) {
          // cf_printf(s, "   %s(done) <-\n", entry->name);
          return ;
        }
        pc = offset;
        continue;
      }

#include "core-words.c"
      //#include "lib-words.c"
      //#include "extensions-words.c"

      default:
      {
        cf_printf(s, "??");
      }
    }
    pc += sizeof(opcode_t);
  }
}

void
execute(struct state *s)
{
  struct entry *entry = find_entry(s, &s->dict);
  if (entry)
  {
    execute_(s, entry);
  }
  else
  {
    // try to interpret as an unsigned decimal number
    cell n = 0;
    if (tib_to_number(s, &n))
    {
      ENSURE_STACK_MAX(1, return);
      PUSH(n);
    }
    else
    {
      unknow_word(s);
      return;
    }
  }
}

void
init_stack(struct stack *stack, int len, unsigned char id)
{
  stack->cells = cf_calloc(NULL, len, sizeof(cell), id);
  stack->sp = -1;
  stack->lim = len - 1;
}

void
free_stack(struct stack *stack)
{
  free(stack->cells);
  free(stack);
}

void
free_dictionary(struct dictionary *dict)
{
#ifdef __KEEP_ENTRY_NAMES
  for (int i = dict->latest; i >= 0; i--)
  {
    if (dict->entries[i].name != NULL) free(dict->entries[i].name);
  }
#endif

  free(dict->entries);
}

void
define_prefix(char c, void (*fn)(struct state *s), char * color, short reset)
{
  static int n_prefix = 0;

  if (reset) n_prefix = 0;

  if (n_prefix >= MAX_PREFIX) cf_fatal_error(NULL, 7);

  prefix_map[n_prefix].c = c;
  prefix_map[n_prefix].fn = fn;
  prefix_map[n_prefix].color = color;
  n_prefix += 1;
}

void
parse_colorforth(struct state *s, int c)
{
  if (s->tib.len == 0) {
    for (int i = 0; i < MAX_PREFIX; i++)
    {
      if (prefix_map[i].c == c) {
        s->color = prefix_map[i].fn;
        echo_color(s, c, prefix_map[i].color);
        return;
      }
    }
  }

  if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
  {
    echo_color(s, c, NULL);
    if (s->tib.len == 0)
    {
      // Strip leading whitespace.
    }
    else
    {
      // Have word.
      s->color(s);
      for(size_t i = 0; i < s->tib.len; i++)
      {
        s->tib.buf[i] = 0;
      }
      s->tib.len = 0;
    }

    return;
  }

  if (c == CF_EOF)
  {
    echo_color(s, c, COLOR_CLEAR);
    return;
  }

  if (c == '\b' || c == 127)
  {
    cf_printf(s, "\b \b");
    if (s->tib.len > 0)
    {
      s->tib.len -= 1;
    }
    return;
  }

  echo_color(s, c, NULL);
  if (s->tib.len < sizeof(s->tib.buf))
  {
    s->tib.buf[s->tib.len++] = c;
  }
}

struct state*
colorforth_newstate(void)
{
  struct state *s = cf_calloc(NULL, 1, sizeof(*s), STATE_ERROR);
  s->color = execute;

  s->base = 10;

  s->stack = cf_calloc(s, 1, sizeof(struct stack), STACK_ERROR);
  init_stack(s->stack, STACK_SIZE, STACK_INIT_ERROR);

  s->r_stack = cf_calloc(s, 1, sizeof(struct stack), RSTACK_ERROR);
  init_stack(s->r_stack, R_STACK_SIZE, RSTACK_INIT_ERROR);

  s->dict.entries = cf_calloc(s, DICT_SIZE, sizeof(struct entry), DICT_ERROR);
  s->dict.latest = -1;

  s->heap = cf_calloc(s, 1, HEAP_SIZE, HEAP_ERROR);
  s->here = 0;
  STORE(OP_NOP, opcode_t);
  STORE(OP_RETURN, opcode_t);

  s->done = 0;
  s->echo_on = 0;

  s->str_stream = NULL;
  s->file_stream = NULL;

#include "define-core.c"

  s->color = execute;
  echo_color(s, '~', COLOR_YELLOW);
  s->echo_on = 1;

  return s;
}

void
free_state(struct state *s)
{
  free(s->heap);

  free_dictionary(&s->dict);

  free_stack(s->r_stack);
  free_stack(s->stack);

#ifdef __EXTENDED_MATH
  free(s->fstack.cells);
#endif

#ifdef __MP_MATH
  free(s->mpstack.cells);
#endif

  free(s);
}
