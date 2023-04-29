// The author disclaims copyright to this source code.

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "colorforth.h"
#include "cf-stdio.h"

#ifdef __EMBED_LIB_CF
#include <lib.cf.h>
#endif /* __EMBED_LIB_CF */

extern void load_extensions(struct state *s);

#ifdef __CHECK_DICT
extern char check_entry(struct state *s, struct entry *check_entry);
extern void display_clash_found(struct state *s, char clash_found);
#endif /* __CHECK_DICT */

struct prefix_map prefix_map[MAX_PREFIX];

#define define_register(N)                                              \
  case OP_##N##_STORE: { N = pop(s->stack); break; }                    \
  case OP_##N##_LOAD: { push(s->stack, N); break; }                     \
  case OP_##N##_ADD: { N += pop(s->stack); break; }                     \
  case OP_##N##_INC: { N += 1; break; }                                 \
  case OP_##N##_DEC: { N -= 1; break; }                                 \
  case OP_##N##_R_POP: { N = pop(s->r_stack); break; }                  \
  case OP_##N##_R_PUSH: { push(s->r_stack, N); break; }

#define define_register_primitive(N)                                    \
  define_primitive(s, REG_##N##_LOAD_HASH,        ENTRY_NAME(#N"@"), OP_##N##_LOAD); \
  define_primitive(s, REG_##N##_STORE_HASH,       ENTRY_NAME(#N"!"), OP_##N##_STORE); \
  define_primitive(s, REG_##N##_ADD_STORE_HASH,   ENTRY_NAME(#N"+!"), OP_##N##_ADD); \
  define_primitive(s, REG_##N##_ADD_ADD_HASH,     ENTRY_NAME(#N"++"), OP_##N##_INC); \
  define_primitive(s, REG_##N##_SUB_SUB_HASH,     ENTRY_NAME(#N"--"), OP_##N##_DEC); \
  define_primitive(s, REG_##N##_TO_R_HASH,        ENTRY_NAME(#N">R"), OP_##N##_R_PUSH); \
  define_primitive(s, REG_R_TO_##N##_HASH,        ENTRY_NAME("R>"#N), OP_##N##_R_POP);

void
cf_print_cell(struct state *s, cell cell)
{
  switch (s->base)
  {
    case 16:
    {
      cf_printf(s, "$%lX", cell);
      break;
    }
    case 2:
    {
      cf_putchar(s, '#');
      if (cell == 0)
      {
        cf_printf(s, "0 ");
      } else {
        int i = CHAR_BIT * sizeof cell;
        char output = 0;
        while(i--)
        {
          const char n = ((cell >> i) & 1);
          if (n) output = 1;
          if (output) cf_putchar(s, '0' + n);
        }
      }
      break;
    }
    default:
    {
      cf_printf(s, CELL_FMT, cell);
      break;
    }
  }
}

void
quit(struct state *s)
{
  s->done = 1;
  echo_color(s, ' ', COLOR_CLEAR);
  cf_printf(s, "\n");
}

void
cf_fatal_error(struct state *s, char id)
{
  cf_printf(s, "E%d\n", id);
  if (s)
  {
    echo_color(s, ' ', COLOR_CLEAR);
  }
  cf_fflush();
  reset_terminal();
  exit(1);
}

void *
cf_calloc(struct state *s, size_t nmemb, size_t size, unsigned char id)
{
  void *ptr = calloc(nmemb, size);
  if (!ptr) cf_fatal_error(s, id);

  return ptr;
}


#define FNV_32_PRIME ((hash_t)0x01000193)

hash_t
hash(char *str)
{
  if (!str) return 0;

  unsigned char *s = (unsigned char *)str;
  hash_t hval = 0;

  while (*s) {
    hval ^= (hash_t)*s++;

    /* multiply by the 32 bit FNV magic prime mod 2^32 */
    hval *= FNV_32_PRIME;
  }

  return hval;
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
  for (struct entry *entry = dict->latest; entry != dict->entries - 1; entry--)
  {
    if (entry->name != NULL) free(entry->name);
  }
#endif

  free(dict->entries);
}

static void
dot_s(struct state *s, struct stack *stack)
{
  cf_printf(s, "[%d] ", stack->sp + 1);
  for (int i = 0; i < stack->sp + 1; i++)
  {
    cf_print_cell(s, stack->cells[i]);
    cf_printf(s, " ");
  }
  cf_printf(s, "<tos\n");
}

inline void
push(struct stack *stack, const cell n)
{
#ifndef UNSAFE_MODE
  if (stack->sp >= stack->lim)
  {
    cf_printf(NULL, "ES>!\n");
    return;
  }
#endif

  stack->sp += 1;
  stack->cells[stack->sp] = n;
}

inline cell
pop(struct stack *stack)
{
#ifndef UNSAFE_MODE
  if (stack->sp < 0)
  {
    cf_printf(NULL, "ES<!\n");
    return 0;
  }
#endif

  const cell ret = stack->cells[stack->sp];
  stack->sp -= 1;
  return ret;
}

void
clear_tib (struct state *s){
  for(size_t i = 0; i < s->tib.len; i++)
  {
    s->tib.buf[i] = 0;
  }
  s->tib.len = 0;
}

#ifdef __KEEP_ENTRY_NAMES
void
dump_words(struct state *s, struct dictionary *dict)
{
  for (struct entry *entry = dict->latest; entry != dict->entries - 1; entry--)
  {
    if (entry->name == NULL) continue;

    s->tib.len = entry->name_len;
    memcpy(s->tib.buf, entry->name, entry->name_len);
    s->tib.buf[s->tib.len] = '\0';
    cf_printf(s, "%s ", s->tib.buf);
  }
}
#else
void
dump_words(struct state *s, struct dictionary *dict __attribute__((unused)))
{
  cf_printf(s, "...\n");
}
#endif


void
words(struct state *s)
{
  dump_words(s, &s->dict);
  cf_printf(s, "\n");
}

struct entry*
find_entry(struct state *s, struct dictionary *dict)
{
  s->tib.buf[s->tib.len] = '\0';
  const hash_t tib_hash = hash(s->tib.buf);

  for (struct entry *entry = dict->latest; entry != dict->entries - 1; entry--)
  {
    if (entry->name_hash == tib_hash)
    {
      return entry;
    }
  }
  return NULL;
}

struct entry*
find_entry_by_hash(struct dictionary *dict, hash_t name_hash)
{
  for (struct entry *entry = dict->latest; entry != dict->entries - 1; entry--)
  {
    if (entry->name_hash == name_hash)
    {
      return entry;
    }
  }
  return NULL;
}

struct entry*
find_entry_by_code(struct dictionary *dict, struct code *code)
{
  for (struct entry *entry = dict->latest; entry != dict->entries - 1; entry--)
  {
    if (entry->code->opcode == code->opcode)
    {
      return entry;
    }
  }
  return NULL;
}

struct entry*
find_entry_by_fn(struct dictionary *dict, struct code *code)
{
  for (struct entry *entry = dict->latest; entry != dict->entries - 1; entry--)
  {
    if (entry->code->value == code->value)
    {
      return entry;
    }
  }
  return NULL;
}

void
print_tib(struct state *s)
{
  for(size_t i = 0; i < s->tib.len; i++)
  {
    cf_putchar(s, s->tib.buf[i]);
  }
}

void
unknow_word (struct state *s)
{
  print_tib(s);
  cf_printf(s, "?\n");

  if (s->str_stream || s->file_stream)
  {
    cf_fatal_error(s, -1);
  }
}

// 'hashed_name' is 'hash(name)' or 0x0 if names are kept
// 'name' must be null-terminated.
static void
define_primitive_generic(struct state *s, struct dictionary *dict,
                         hash_t hashed_name, char name[] __attribute__((unused)),
                         const enum opcode opcode, void (*fn)(struct state *s))
{
  if (hashed_name)
  {
    struct entry *found_entry = find_entry_by_hash(dict, hashed_name);
    if (found_entry)
    {
#ifdef __KEEP_ENTRY_NAMES
      cf_printf(s, "'%s' clash with '%s'\n", name, found_entry->name);
#endif /* __KEEP_ENTRY_NAMES */

      cf_fatal_error(s, DUPLICATE_HASH_ERROR);
    }
  }

  dict->latest++;
  struct entry *entry = dict->latest;

  entry->name_hash = hashed_name;

#ifdef __KEEP_ENTRY_NAMES
  if (!entry->name_hash)
  {
    entry->name_hash = hash(name);
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

  cf_printf(s, "%20s <-> %-20lX   hashed_name=%-20lX | %20s_HASH %20lX\n", name, entry->name_hash, hashed_name, up_name, hash(name));

  free(up_name);
#else
  cf_printf(s, "%-20lX %lX\n", entry->name_hash, hashed_name);
#endif
#endif

  entry->code = s->here;
  entry->code->opcode = opcode;
  entry->code->value = (cell) fn;

  (entry->code + 1)->opcode = OP_RETURN;
  (entry->code + 1)->value = 0;

  s->here = (struct code *)s->here + 2;
}

static void
define_primitive(struct state *s, hash_t hashed_name, char name[], const enum opcode opcode)
{
  define_primitive_generic(s, &s->dict, hashed_name, name, opcode, NULL);
}

void
define_primitive_extension(struct state *s, hash_t hashed_name, char name[], void (*fn)(struct state *s))
{
  define_primitive_generic(s, &s->dict, hashed_name, name, OP_FUNCTION_CALL, fn);
}

static bool
tib_to_number(struct state *s, cell *n)
{
  char *endptr;
  s->tib.buf[s->tib.len] = '\0';
  *n = strtol(s->tib.buf, &endptr, s->base);
  return *s->tib.buf != 0 && *endptr == 0;
}

/**
 *
 * Color functions
 *
 **/
static void
define_generic(struct state *s, struct dictionary *dict)
{
  dict->latest++;
  struct entry *entry = dict->latest;

  entry->name_hash = hash(s->tib.buf);

#ifdef __KEEP_ENTRY_NAMES
  entry->name_len = s->tib.len;
  entry->name = cf_calloc(s, 1, entry->name_len, DEFINE_ERROR);
  memcpy(entry->name, s->tib.buf, s->tib.len);

#ifdef __LIVE_CHECK_DICT
  if (check_entry(s, entry)) display_clash_found(s, 1);
#endif /* __LIVE_CHECK_DICT */
#endif

  entry->code = s->here;
}

static void
define(struct state *s)
{
  define_generic(s, &s->dict);
}

inline void
compile_code(struct state *s, enum opcode opcode, cell value)
{
  struct code *code = (struct code *)s->here;
  code->opcode = opcode;
  code->value = value;
  s->here = (struct code *)s->here + 1;
}

static void
compile_entry(struct state *s, struct entry *entry)
{
  compile_code(s, entry == s->dict.latest ? OP_TAIL_CALL : OP_CALL, (cell)entry);
}

static void
inline_entry(struct state *s, struct entry *entry)
{
  for (size_t i = 0, done = 0; !done; i++)
  {
    // inline OP_RETURN in first position
    if (entry->code[i].opcode == OP_RETURN && i > 0)
    {
      break;
    }
    compile_code(s, entry->code[i].opcode, entry->code[i].value);
  }
}

static void
compile_literal(struct state *s, cell n)
{
  compile_code(s, OP_NUMBER, n);
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

static void
compile_inline(struct state *s)
{
  struct entry *entry = find_entry(s, &s->dict);
  if (entry)
  {
    inline_entry(s, entry);
  }
  else
  {
    unknow_word(s);
  }
}

void
execute_(struct state *s, struct entry *entry)
{
  // cf_printf(s, "-> %s\n", entry->name);
  struct code *pc = entry->code;

  push(s->r_stack, 0);

#ifdef __USE_REGISTER
  register cell A = 0;
  register cell B = 0;
  register cell C = 0;
  register cell I = 0;
  register cell J = 0;
#endif

  // don't forget to INLINE a return!!!!
  while(1)
  {
    switch (pc->opcode)
    {
      case OP_RETURN:
      {
        struct code *code = (struct code *)pop(s->r_stack);
        if (code)
        {
          pc = code;
        }
        else
        {
          return;
        }
        break;
      }

      case OP_R_PUSH:
      {
        push(s->r_stack, pop(s->stack));
        break;
      }

      case OP_R_POP:
      {
        push(s->stack, pop(s->r_stack));
        break;
      }

      case OP_R_FETCH:
      {
        push(s->stack, s->r_stack->cells[s->r_stack->sp]);
        break;
      }

#ifdef __USE_REGISTER
      define_register(A);
      define_register(B);
      define_register(C);
      define_register(I);
      define_register(J);
#endif

      case OP_DUP:
      {
        ENSURE_STACK_MIN(1);
        ENSURE_STACK_MAX(1);
        SP += 1;
        CELLS[SP] = CELLS[SP - 1];
        break;
      }

      case OP_DROP:
      {
        ENSURE_STACK_MIN(1);
        SP -= 1;
        break;
      }

      case OP_SWAP:
      {
        ENSURE_STACK_MIN(2);
        const cell n = CELLS[SP];
        CELLS[SP] = CELLS[SP - 1];
        CELLS[SP - 1] = n;
        break;
      }

      case OP_OVER:
      {
        ENSURE_STACK_MIN(2);
        ENSURE_STACK_MAX(1);
        SP += 1;
        CELLS[SP] = CELLS[SP - 2];
        break;
      }

      case OP_ROT:
      {
        ENSURE_STACK_MIN(3);
        const cell n = CELLS[SP - 2];
        CELLS[SP - 2] = CELLS[SP - 1];
        CELLS[SP - 1] = CELLS[SP];
        CELLS[SP] = n;
        break;
      }

      case OP_MINUS_ROT:
      {
        ENSURE_STACK_MIN(3);
        const cell n = CELLS[SP];
        CELLS[SP] = CELLS[SP - 1];
        CELLS[SP - 1] = CELLS[SP - 2];
        CELLS[SP - 2] = n;
        break;
      }

      case OP_NIP:
      {
        ENSURE_STACK_MIN(2);
        SP -= 1;
        CELLS[SP] = CELLS[SP + 1];
        break;
      }

      case OP_LOAD:
      {
        ENSURE_STACK_MIN(1);
        CELLS[SP] = *(cell*) CELLS[SP];
        break;
      }

      case OP_STORE:
      {
        ENSURE_STACK_MIN(2);
        cell *ptr = (cell*) CELLS[SP];
        *ptr = CELLS[SP - 1];
        SP -= 2;
        break;
      }

      case OP_CLOAD:
      {
        ENSURE_STACK_MIN(1);
        CELLS[SP] = *(char*) CELLS[SP];
        break;
      }

      case OP_CSTORE:
      {
        ENSURE_STACK_MIN(2);
        char *ptr = (char*) CELLS[SP];
        *ptr = CELLS[SP - 1];
        SP -= 2;
        break;
      }

      case OP_CALL:
      {
        struct entry *entry_ = (struct entry*)pc->value;
        push(s->r_stack, (cell)pc);
        pc = entry_->code - 1;
        break;
      }

      case OP_TAIL_CALL:
      {
        struct entry *entry_ = (struct entry*)pc->value;
        pc = entry_->code - 1;
        break;
      }

      case OP_FUNCTION_CALL:
      {
        // Call extension function
        ((void (*)(struct state *s)) pc->value)(s);
        break;
      }

      case OP_NUMBER:
      case OP_TICK_NUMBER:
      {
        ENSURE_STACK_MAX(1);
        SP += 1;
        CELLS[SP] = pc->value;
        break;
      }

      case OP_ADD:
      {
        ENSURE_STACK_MIN(2);
        SP -= 1;
        CELLS[SP] = CELLS[SP] + CELLS[SP + 1];
        break;
      }

      case OP_SUB:
      {
        ENSURE_STACK_MIN(2);
        SP -= 1;
        CELLS[SP] = CELLS[SP] - CELLS[SP + 1];
        break;
      }

      case OP_MUL:
      {
        ENSURE_STACK_MIN(2);
        SP -= 1;
        CELLS[SP] = CELLS[SP] * CELLS[SP + 1];
        break;
      }

      case OP_EQUAL:
      {
        ENSURE_STACK_MIN(2);
        SP -= 1;
        CELLS[SP] = CELLS[SP] == CELLS[SP + 1];
        break;
      }

      case OP_LESS:
      {
        ENSURE_STACK_MIN(2);
        SP -= 1;
        CELLS[SP] = CELLS[SP] < CELLS[SP + 1];
        break;
      }

      case OP_IF:
      {
        struct entry *entry_ = (struct entry*)pop(s->stack);
        const cell f = pop(s->stack);

        if (f) {
          push(s->r_stack, (cell)pc);
          pc = entry_->code - 1;
        }

        break;
      }

      case OP_IF_EXIT:
      {
        struct entry *entry_ = (struct entry*)pop(s->stack);
        const cell f = pop(s->stack);

        if (f) {
          push(s->r_stack, (cell)pc);
          pc = entry_->code - 1;
          pop(s->r_stack);
        }

        break;
      }

      case OP_IF_NOT:
      {
        struct entry *entry_ = (struct entry*)pop(s->stack);
        const cell f = pop(s->stack);

        if (f == 0) {
          push(s->r_stack, (cell)pc);
          pc = entry_->code - 1;
        }

        break;
      }

      case OP_IF_NOT_EXIT:
      {
        struct entry *entry_ = (struct entry*)pop(s->stack);
        const cell f = pop(s->stack);

        if (f == 0) {
          push(s->r_stack, (cell)pc);
          pc = entry_->code - 1;
          pop(s->r_stack);
        }

        break;
      }

    case OP_IF_STAR:
      {
        struct entry *entry_ = (struct entry*)pop(s->stack);
        const cell f = pop(s->stack);

        if (f) pc = entry_->code - 1;

        break;
      }

      case OP_IF_STAR_EXIT:
      {
        struct entry *entry_ = (struct entry*)pop(s->stack);
        const cell f = pop(s->stack);

        if (f) {
          pc = entry_->code - 1;
          pop(s->r_stack);
        }

        break;
      }

      case OP_IF_NOT_STAR:
      {
        struct entry *entry_ = (struct entry*)pop(s->stack);
        const cell f = pop(s->stack);

        if (f == 0) pc = entry_->code - 1;

        break;
      }

      case OP_IF_NOT_STAR_EXIT:
      {
        struct entry *entry_ = (struct entry*)pop(s->stack);
        const cell f = pop(s->stack);

        if (f == 0) {
          pc = entry_->code - 1;
          pop(s->r_stack);
        }

        break;
      }

      case OP_IF_ELSE:
      {
        struct entry *entry_false_ = (struct entry*)pop(s->stack);
        struct entry *entry_true_ = (struct entry*)pop(s->stack);
        const cell f = pop(s->stack);

        push(s->r_stack, (cell)pc);
        if (f) {
          pc = entry_true_->code - 1;
        }
        else {
          pc = entry_false_->code - 1;
        }

        break;
      }

      case OP_EMIT:
      {
        cf_putchar(s, (char)pop(s->stack));
        break;
      }

      case OP_KEY:
      {
        push(s->stack, (char)cf_getchar(s));
        break;
      }

      case OP_CELL:
      {
        push(s->stack, sizeof(cell));
        break;
      }

      case OP_CODE_LEN:
      {
        push(s->stack, sizeof(struct code));
        break;
      }

      case OP_GET_ENTRY_CODE:
      {
        struct entry *entry_ = (struct entry*)pop(s->stack);
        push(s->stack, (cell)entry_->code);
        break;
      }

      case OP_EXECUTE:
      {
        struct code *code_ = (struct code*)pop(s->stack);
        push(s->r_stack, (cell)pc);
        pc = code_ - 1;
        break;
      }

      // Like execute but leave xt on the stack
      case OP_EXECUTE_STAR:
      {
        push(s->r_stack, (cell)pc);
        pc = (struct code*)s->stack->cells[s->stack->sp - 1] - 1;
        break;
      }

      case OP_HERE:
      {
        push(s->stack, (cell)&s->here);
        break;
      }

      case OP_LATEST:
      {
        push(s->stack, (cell)&s->dict.latest);
        break;
      }

      case OP_GET_CVA: // Code value address
      {
        struct code *code = (struct code *) pop(s->stack);
        push(s->stack, (cell) &code->value);
        break;
      }

      case OP_COMPILE:
      {
        struct entry *entry_ = (struct entry*)pop(s->stack);
        compile_entry(s, entry_);
        break;
      }

      case OP_COMPILE_INLINE:
      {
        struct entry *entry_ = (struct entry*)pop(s->stack);
        inline_entry(s, entry_);
        break;
      }

      case OP_COMPILE_LITERAL:
      {
        cell n = pop(s->stack);
        compile_literal(s, n);
        break;
      }

      case OP_BYE:
      {
        quit(s);
        return;
      }

      case OP_WORDS:
      {
        words(s);
        break;
      }

      case OP_NOP: {
        break;
      }

      case OP_PRINT_TOS:
      {
        cf_print_cell(s,pop(s->stack));
        cf_printf(s, " ");
        cf_fflush();
        break;
      }

      case OP_DOT_S:
      {
        dot_s(s, s->stack);
        break;
      }

      case OP_CLEAR:
      {
        SP = 0;
        break;
      }

      default:
      {
        cf_printf(s, "??");
      }
    }
    pc++;
  }
  // cf_printf(s, "   %s(done) <-\n", entry->name);
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
      push(s->stack, n);
    }
    else
    {
      unknow_word(s);
    }
  }
}

static void
tick(struct state *s)
{
  struct entry *entry = find_entry(s, &s->dict);
  if (entry)
  {
    push(s->stack, (cell)entry);
  }
  else
  {
    unknow_word(s);
  }
}

static void
compile_tick(struct state *s)
{
  struct entry *entry = find_entry(s, &s->dict);
  if (entry)
  {
    compile_code(s, OP_TICK_NUMBER, (cell)entry);
  }
  else
  {
    unknow_word(s);
  }
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

void
parse_space(struct state *s)
{
  char *old_str_stream = s->str_stream;
  FILE *old_file_stream = s->file_stream;

  s->str_stream = NULL;
  s->file_stream = NULL;

  parse_colorforth(s, ' '); parse_colorforth(s, '~');

  s->str_stream = old_str_stream;
  s->file_stream = old_file_stream;
}

void
parse_from_string(struct state *s, char *str)
{
  char *old_stream = s->str_stream;

  s->str_stream = str;
  int c;
  while((c = (char) cf_getchar(s)) != 0 && !s->done)
  {
    parse_colorforth(s, c);
  }

  parse_colorforth(s, ' ');

  s->str_stream = old_stream;

  parse_space(s);
}

void
define_prefix(char c, void (*fn)(struct state *s), char * color, short reset)
{
  static int n_prefix = 0;

  if (reset) n_prefix = 0;

  if (n_prefix >= MAX_PREFIX)
  {
    cf_fatal_error(NULL, 7);
  }

  prefix_map[n_prefix].c = c;
  prefix_map[n_prefix].fn = fn;
  prefix_map[n_prefix].color = color;
  n_prefix += 1;
}

#ifdef __EMBED_LIB_CF
void
parse_from_embed_lib_cf(struct state *s)
{
  char* str = malloc(lib_cf_len + 1);
  if (str)
  {
    strncpy(str, (char *)lib_cf, lib_cf_len);
    str[lib_cf_len] = 0;
    parse_from_string(s, str);
    free(str);
  }
}
#endif /* __EMBED_LIB_CF */

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
  s->dict.latest = s->dict.entries - 1;

  s->heap = cf_calloc(s, 1, HEAP_SIZE, HEAP_ERROR);
  s->here = s->heap;

  s->done = 0;
  s->echo_on = 0;

  s->str_stream = NULL;
  s->file_stream = NULL;

  define_prefix(':', define,         COLOR_RED,     1);
  define_prefix('^', compile,        COLOR_GREEN,   0);
  define_prefix('~', execute,        COLOR_YELLOW,  0);
  define_prefix('\'', tick,          COLOR_BLUE,    0);
  define_prefix('`', compile_tick,   COLOR_BLUE,    0);
  define_prefix(',', compile_inline, COLOR_CYAN,    0);

  define_primitive(s, NOP_HASH,               ENTRY_NAME("nop"), OP_NOP);
  define_primitive(s, PRINT_TOS_HASH,         ENTRY_NAME("."), OP_PRINT_TOS);
  define_primitive(s, DUP_HASH,               ENTRY_NAME("dup"), OP_DUP);
  define_primitive(s, OVER_HASH,              ENTRY_NAME("over"), OP_OVER);
  define_primitive(s, SWAP_HASH,              ENTRY_NAME("swap"), OP_SWAP);
  define_primitive(s, DROP_HASH,              ENTRY_NAME("drop"), OP_DROP);
  define_primitive(s, ROT_HASH,               ENTRY_NAME("rot"), OP_ROT);
  define_primitive(s, MINUS_ROT_HASH,         ENTRY_NAME("-rot"), OP_MINUS_ROT);
  define_primitive(s, NIP_HASH,               ENTRY_NAME("nip"), OP_NIP);
  define_primitive(s, ADD_HASH,               ENTRY_NAME("+"), OP_ADD);
  define_primitive(s, SUB_HASH,               ENTRY_NAME("-"), OP_SUB);
  define_primitive(s, MUL_HASH,               ENTRY_NAME("*"), OP_MUL);
  define_primitive(s, EQUAL_HASH,             ENTRY_NAME("="), OP_EQUAL);
  define_primitive(s, LESS_HASH,              ENTRY_NAME("<"), OP_LESS);
  define_primitive(s, BYE_HASH,               ENTRY_NAME("bye"), OP_BYE);
  define_primitive(s, WORDS_HASH,             ENTRY_NAME("words"), OP_WORDS);
  define_primitive(s, EMIT_HASH,              ENTRY_NAME("emit"), OP_EMIT);
  define_primitive(s, KEY_HASH,               ENTRY_NAME("key"), OP_KEY);
  define_primitive(s, LOAD_HASH,              ENTRY_NAME("@"), OP_LOAD);
  define_primitive(s, STORE_HASH,             ENTRY_NAME("!"), OP_STORE);
  define_primitive(s, CLOAD_HASH,             ENTRY_NAME("c@"), OP_CLOAD);
  define_primitive(s, CSTORE__HASH,           ENTRY_NAME("c!"), OP_CSTORE);
  define_primitive(s, CELL_HASH,              ENTRY_NAME("cell"), OP_CELL);
  define_primitive(s, CODE_LEN_HASH,          ENTRY_NAME("#code"), OP_CODE_LEN);
  define_primitive(s, HERE_HASH,              ENTRY_NAME("here"), OP_HERE);
  define_primitive(s, LATEST_HASH,            ENTRY_NAME("latest"), OP_LATEST);

  define_primitive(s, COMPILE_HASH,           ENTRY_NAME("[^]"), OP_COMPILE);
  define_primitive(s, COMPILE_INLINE_HASH,    ENTRY_NAME("[`]"), OP_COMPILE_INLINE);
  define_primitive(s, COMPILE_LITERAL_HASH,   ENTRY_NAME("[,]"), OP_COMPILE_LITERAL);
  define_primitive(s, GET_ENTRY_CODE_HASH,    ENTRY_NAME("code>"), OP_GET_ENTRY_CODE);
  define_primitive(s, EXECUTE_HASH,           ENTRY_NAME("execute"), OP_EXECUTE);
  define_primitive(s, EXECUTE_STAR_HASH,      ENTRY_NAME("execute*"), OP_EXECUTE_STAR);
  define_primitive(s, GET_CVA_HASH,           ENTRY_NAME("cva>"), OP_GET_CVA);

  define_primitive(s, IF_HASH,                ENTRY_NAME("if"), OP_IF);
  define_primitive(s, IF_EXIT_HASH,           ENTRY_NAME("if;"), OP_IF_EXIT);
  define_primitive(s, IF_NOT_HASH,            ENTRY_NAME("if-not"), OP_IF_NOT);
  define_primitive(s, IF_NOT_EXIT_HASH,       ENTRY_NAME("if-not;"), OP_IF_NOT_EXIT);

  define_primitive(s, IF_STAR_HASH,           ENTRY_NAME("if*"), OP_IF_STAR);
  define_primitive(s, IF_EXIT_STAR_HASH,      ENTRY_NAME("if*;"), OP_IF_STAR_EXIT);
  define_primitive(s, IF_NOT_STAR_HASH,       ENTRY_NAME("if-not*"), OP_IF_NOT_STAR);
  define_primitive(s, IF_NOT_EXIT_STAR_HASH,  ENTRY_NAME("if-not*;"), OP_IF_NOT_STAR_EXIT);

  define_primitive(s, IF_ELSE_HASH,           ENTRY_NAME("if-else"), OP_IF_ELSE);

  define_primitive(s, DOT_S_HASH,             ENTRY_NAME(".s"), OP_DOT_S);

  define_primitive(s, RETURN_HASH,            ENTRY_NAME(";"), OP_RETURN);

  define_primitive(s, R_PUSH_HASH,            ENTRY_NAME(">R"), OP_R_PUSH);
  define_primitive(s, R_POP_HASH,             ENTRY_NAME("R>"), OP_R_POP);
  define_primitive(s, R_FETCH_HASH,           ENTRY_NAME("R@"), OP_R_FETCH);

  define_primitive(s, CLEAR_HASH,             ENTRY_NAME("clear"), OP_CLEAR);

  init_lib(s);

#ifdef __USE_REGISTER
  // A, B, C, I and J registers are state global
  define_register_primitive(A);
  define_register_primitive(B);
  define_register_primitive(C);
  define_register_primitive(I);
  define_register_primitive(J);
  // I, J, K counter
  // A, B, C, X, Y just register
#endif

#ifdef __USE_EXTENSIONS
  load_extensions(s);
#endif

#ifdef __EMBED_LIB_CF
  parse_from_embed_lib_cf(s);
#endif /* __EMBED_LIB_CF */

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
