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

struct prefix_map prefix_map[MAX_PREFIX];

#define define_register(N) case OP_##N##_STORE: { N = pop(s->stack); break; } \
  case OP_##N##_LOAD: { push(s->stack, N); break; }                     \
  case OP_##N##_ADD: { N += pop(s->stack); break; }                     \
  case OP_##N##_INC: { N += 1; break; }                                 \
  case OP_##N##_DEC: { N -= 1; break; }                                 \
  case OP_##N##_R_POP: { N = pop(s->r_stack); break; }                  \
  case OP_##N##_R_PUSH: { push(s->r_stack, N); break; }

#define define_register_primitive(N) define_primitive_inlined(state, #N"@", OP_##N##_LOAD); \
  define_primitive_inlined(state, #N"!", OP_##N##_STORE);               \
  define_primitive_inlined(state, #N"+!", OP_##N##_ADD);                \
  define_primitive_inlined(state, #N"++", OP_##N##_INC);                \
  define_primitive_inlined(state, #N"--", OP_##N##_DEC);                \
  define_primitive_inlined(state, #N">R", OP_##N##_R_PUSH);             \
  define_primitive_inlined(state, "R>"#N, OP_##N##_R_POP);

void
cf_print_cell(struct state *state, cell cell)
{
  switch (state->base)
  {
    case 16:
    {
      cf_printf(state, "$%X ", cell);
      break;
    }
    case 2:
    {
      cf_putchar(state, '#');
      if (cell == 0)
      {
        cf_printf(state, "0 ");
      } else {
        int i = CHAR_BIT * sizeof cell;
        char output = 0;
        while(i--)
        {
          const char n = ((cell >> i) & 1);
          if (n) output = 1;
          if (output) cf_putchar(state, '0' + n);
        }
        cf_putchar(state, ' ');
      }
      break;
    }
    default:
    {
      cf_printf(state, "%ld ", cell);
      break;
    }
  }
}

void
quit(struct state *state, char ask)
{
  char c = 'n';
  if (ask)
  {
    cf_printf(state, "Continue? (y/n) ");
    c = cf_getchar(state);
  }
  if (c == 'n')
  {
    state->done = 1;
    echo_color(state, ' ', COLOR_CLEAR);
  }
  cf_printf(state, "\n");
}

void
cf_fatal_error(struct state *state, char id)
{
  cf_printf(state, "E%d\n", id);
  if (state)
  {
    echo_color(state, ' ', COLOR_CLEAR);
  }
  cf_fflush();
  reset_terminal();
  exit(1);
}

void *
cf_calloc(struct state *state, size_t nmemb, size_t size, unsigned char id)
{
  void *ptr = calloc(nmemb, size);
  if (!ptr) cf_fatal_error(state, id);

  return ptr;
}

void
init_stack(struct stack *stack, int len, unsigned char id)
{
  stack->cells = cf_calloc(NULL, len, sizeof(cell), id);
  stack->sp = 0;
  stack->lim = len - 1;
}

static void
dot_s(struct state *state, struct stack *stack)
{
  for (int i = 0, p = stack->sp + 1; i <= stack->lim; i++, p++)
  {
    if (p > stack->lim) p = 0;
    cf_print_cell(state, stack->cells[p]);
  }
  cf_printf(state, " <tos\n");
}

void
push(struct stack *stack, const cell n)
{
  if (stack->sp == stack->lim)
  {
    stack->sp = 0;
  }
  else
  {
    stack->sp += 1;
  }
  stack->cells[stack->sp] = n;
}

cell
pop(struct stack *stack)
{
  const cell n = stack->cells[stack->sp];
  if (stack->sp == 0)
  {
    stack->sp = stack->lim;
  }
  else
  {
    stack->sp -= 1;
  }

  return n;
}

void
clear_tib (struct state *s){
  for(size_t i = 0; i < s->tib.len; i++)
  {
    s->tib.buf[i] = 0;
  }
  s->tib.len = 0;
}

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

void
words(struct state *s)
{
  dump_words(s, &s->inlined_dict);
  dump_words(s, &s->dict);
  cf_printf(s, "\n");
}

struct entry*
find_entry(struct state *s, struct dictionary *dict)
{
  for (struct entry *entry = dict->latest; entry != dict->entries - 1; entry--)
  {
    if (entry->name_len == s->tib.len && memcmp(entry->name, s->tib.buf, s->tib.len) == 0)
    {
      return entry;
    }
  }
  return NULL;
}

struct entry*
find_entry_by_code(struct state *s, struct dictionary *dict, struct code *code)
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
find_entry_by_fn(struct state *s, struct dictionary *dict, struct code *code)
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
unknow_word (struct state *s, const char *msg)
{
  cf_printf(s, "Error %s '", msg);
  print_tib(s);
  cf_printf(s, "': unknown word at line %u, column %u\n", s->line, s->coll);
}

// 'name' must be null-terminated.
static void
define_primitive_generic(struct state *s, struct dictionary *dict, char name[],
                         const enum opcode opcode, void (*fn)(struct state *s))
{
  dict->latest++;
  struct entry *entry = dict->latest;
  entry->name_len = strlen(name);
  entry->name = cf_calloc(s, 1, entry->name_len, 11);
  memcpy(entry->name, name, entry->name_len);
  entry->code = s->here;
  entry->code->opcode = opcode;
  entry->code->value = (cell) fn;

  (entry->code + 1)->opcode = OP_RETURN;
  (entry->code + 1)->value = 0;

  s->here = (struct code *)s->here + 2;
}

static void
define_primitive(struct state *s, char name[], const enum opcode opcode)
{
  define_primitive_generic(s, &s->dict, name, opcode, NULL);
}

static void
define_primitive_inlined(struct state *s, char name[], const enum opcode opcode)
{
  define_primitive_generic(s, &s->inlined_dict, name, opcode, NULL);
}

void
define_primitive_extension(struct state *s, char name[], void (*fn)(struct state *s))
{
  define_primitive_generic(s, &s->dict, name, OP_FUNCTION_CALL, fn);
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
  entry->name_len = s->tib.len;
  entry->name = cf_calloc(s, 1, entry->name_len, 12);
  memcpy(entry->name, s->tib.buf, s->tib.len);
  entry->code = s->here;
}

static void
define(struct state *s)
{
  define_generic(s, &s->dict);
}

static void
define_inlined(struct state *s)
{
  define_generic(s, &s->inlined_dict);
}

static void
inline_entry(struct state *s, struct entry *entry)
{
  for (size_t i = 0, done = 0; !done; i++)
  {
    // inline the first OP_RETURN
    if (entry->code[i].opcode == OP_RETURN && i > 0)
    {
      break;
    }
    struct code *code = (struct code *)s->here;
    code->opcode = entry->code[i].opcode;
    code->value = entry->code[i].value;
    s->here = (struct code *)s->here + 1;
  }
}

static void
compile(struct state *s)
{
  struct entry *inlined_entry = find_entry(s, &s->inlined_dict);
  if (inlined_entry)
  {
    inline_entry(s, inlined_entry);
    return;
  }

  struct entry *entry = find_entry(s, &s->dict);
  struct code *code = (struct code *)s->here;
  if (entry)
  {
    code->opcode = entry == s->dict.latest ? OP_TAIL_CALL : OP_CALL;
    code->value = (cell)entry;
    s->here = (struct code *)s->here + 1;
  }
  else
  {
    // try to interpret as an unsigned decimal number
    cell n = 0;
    if (tib_to_number(s, &n))
    {
      // compile number
      code->opcode = OP_NUMBER;
      code->value = n;
      s->here = (struct code *)s->here + 1;
    }
    else
    {
      unknow_word(s, "compiling");
      quit(s, 1);
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
    unknow_word(s, "inlining");
    quit(s, 1);
  }
}

static void
compile_literal(struct state *s)
{
  struct code *code =  (struct code *)s->here;
  code->opcode = OP_NUMBER;
  code->value = pop(s->stack);
  s->here = (struct code *)s->here + 1;
}

void
execute_(struct state *s, struct entry *entry)
{
  // cf_printf(s, "-> %s\n", entry->name);
  struct code *pc = entry->code;

  push(s->r_stack, 0);

  short choose_state = 0;

#ifdef __USE_REGISTER
  register cell A = 0;
  register cell B = 0;
  register cell C = 0;
  register cell I = 0;
  register cell J = 0;
#endif

  // don't forget to compile a return!!!!
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
        push(s->stack, s->stack->cells[s->stack->sp]);
        break;
      }

      case OP_DROP:
      {
        pop(s->stack);
        break;
      }

      case OP_SWAP:
      {
        const cell n1 = pop(s->stack);
        const cell n2 = pop(s->stack);
        push(s->stack, n1);
        push(s->stack, n2);
        break;
      }

      case OP_OVER:
      {
        const cell n1 = pop(s->stack);
        const cell n2 = pop(s->stack);
        push(s->stack, n2);
        push(s->stack, n1);
        push(s->stack, n2);
        break;
      }

      case OP_ROT:
      {
        const cell n3 = pop(s->stack);
        const cell n2 = pop(s->stack);
        const cell n1 = pop(s->stack);
        push(s->stack, n2);
        push(s->stack, n3);
        push(s->stack, n1);
        break;
      }

      case OP_MINUS_ROT:
      {
        const cell n3 = pop(s->stack);
        const cell n2 = pop(s->stack);
        const cell n1 = pop(s->stack);
        push(s->stack, n3);
        push(s->stack, n1);
        push(s->stack, n2);
        break;
      }

      case OP_NIP:
      {
        const cell n2 = pop(s->stack);
        pop(s->stack);
        push(s->stack, n2);
        break;
      }

      case OP_LOAD:
      {
        push(s->stack, *(cell*)pop(s->stack));
        break;
      }

      case OP_STORE:
      {
        cell *ptr = (cell*)pop(s->stack);
        cell n = pop(s->stack);
        *ptr = n;
        break;
      }

      case OP_CLOAD:
      {
        push(s->stack, *(char*)pop(s->stack));
        break;
      }

      case OP_CSTORE:
      {
        char *ptr = (char*)pop(s->stack);
        char n = pop(s->stack);
        *ptr = n;
        break;
      }

      case OP_CALL:
      {
        struct entry *entry_ = (struct entry*)pc->value;
        if (choose_state) {
          choose_state = 0;
          pc++;
        }

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
        push(s->stack, pc->value);

        if (choose_state) {
          choose_state = 0;
          pc++;
        }
        break;
      }

      case OP_ADD:
      {
        const cell n1 = pop(s->stack);
        const cell n2 = pop(s->stack);
        push(s->stack, n1 + n2);
        break;
      }

      case OP_SUB:
      {
        const cell n1 = pop(s->stack);
        const cell n2 = pop(s->stack);
        push(s->stack, n2 - n1);
        break;
      }

      case OP_MUL:
      {
        const cell n1 = pop(s->stack);
        const cell n2 = pop(s->stack);
        push(s->stack, n1 * n2);
        break;
      }

      case OP_EQUAL:
      {
        const cell n1 = pop(s->stack);
        const cell n2 = pop(s->stack);
        push(s->stack, n1 == n2);
        break;
      }

      case OP_LESS:
      {
        const cell n1 = pop(s->stack);
        const cell n2 = pop(s->stack);
        push(s->stack, n2 < n1);
        break;
      }

      case OP_WHEN:
      {
        const cell n = pop(s->stack);
        if (!n) pc++;
        break;
      }

      case OP_UNLESS:
      {
        const cell n = pop(s->stack);
        if (n) pc++;
        break;
      }

      case OP_CHOOSE:
      {
        const cell n = pop(s->stack);
        if (n)
        {
          choose_state = 1;
        }
        else
        {
          pc++;
        }
        break;
      }

      case OP_COMPILE_LITERAL:
      {
        compile_literal(s);
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

      case OP_I_LATEST:
      {
        push(s->stack, (cell)&s->inlined_dict.latest);
        break;
      }

      case OP_BYE:
      {
        quit(s, 0);
        break;
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
        cf_fflush();
        break;
      }

      case OP_DOT_S:
      {
        dot_s(s, s->stack);
        break;
      }

      default:
      {
        cf_printf(s, "unknown opcode");
        quit(s, 1);
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
      unknow_word(s, "executing");
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
    unknow_word(s, "ticking");
  }
}

static void
compile_tick(struct state *s)
{
  struct entry *entry = find_entry(s, &s->dict);
  if (entry)
  {
    struct code *code = (struct code *)s->here;
    code->opcode = OP_TICK_NUMBER;
    code->value = (cell)entry;
    s->here = (struct code *)s->here + 1;
  }
  else
  {
    unknow_word(s, "ticking");
    quit(s, 1);
  }
}

void
parse_colorforth(struct state *state, int c)
{
  if (state->tib.len == 0) {
    for (int i = 0; i < MAX_PREFIX; i++)
    {
      if (prefix_map[i].c == c) {
        state->color = prefix_map[i].fn;
        echo_color(state, c, prefix_map[i].color);
        state->coll += 1;
        return;
      }
    }
  }

  if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
  {
    echo_color(state, c, NULL);
    if (state->tib.len == 0)
    {
      // Strip leading whitespace.
    }
    else
    {
      // Have word.
      state->color(state);
      for(size_t i = 0; i < state->tib.len; i++)
      {
        state->tib.buf[i] = 0;
      }
      state->tib.len = 0;
    }

    if (c == '\n') {
      state->coll = 0; state->line += 1;
    } else {
      state->coll += 1;
    }

    return;
  }

  if (c == CF_EOF)
  {
    state->coll = 0; state->line = 1;
    echo_color(state, c, COLOR_CLEAR);
    return;
  }

  if (c == '\b' || c == 127)
  {
    cf_printf(state, "\b \b");
    if (state->tib.len > 0)
    {
      state->tib.len -= 1;
      state->coll -= 1;
    }
    return;
  }


  echo_color(state, c, NULL);
  if (state->tib.len < sizeof(state->tib.buf))
  {
    state->tib.buf[state->tib.len++] = c;
    state->coll += 1;
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
parse_from_embed_lib_cf(struct state *state)
{
  char* str = malloc(src_lib_cf_len + 1);
  if (str)
  {
    strncpy(str, (char *)src_lib_cf, src_lib_cf_len);
    str[src_lib_cf_len] = 0;
    parse_from_string(state, str);
    free(str);
  }
}
#endif /* __EMBED_LIB_CF */

struct state*
colorforth_newstate(void)
{
  struct state *state = cf_calloc(NULL, 1, sizeof(*state), 0);
  state->color = execute;

  state->base = 10;

  state->stack = cf_calloc(state, 1, sizeof(struct stack), 1);
  init_stack(state->stack, STACK_SIZE, 2);

  state->r_stack = cf_calloc(state, 1, sizeof(struct stack), 3);
  init_stack(state->r_stack, R_STACK_SIZE, 4);

  state->dict.entries = cf_calloc(state, DICT_SIZE, sizeof(struct entry), 5);
  state->dict.latest = state->dict.entries - 1;

  state->inlined_dict.entries = cf_calloc(state, INLINED_DICT_SIZE, sizeof(struct entry), 6);
  state->inlined_dict.latest = state->inlined_dict.entries - 1;

  state->heap = cf_calloc(state, 1, HEAP_SIZE, 7);
  state->here = state->heap;

  state->coll = 0; state->line = 1;
  state->done = 0;
  state->echo_on = 0;

  state->str_stream = NULL;
  state->file_stream = NULL;

  define_prefix(':', define,         COLOR_RED,     1);
  define_prefix('|', define_inlined, COLOR_MAGENTA, 0);
  define_prefix('^', compile,        COLOR_GREEN,   0);
  define_prefix('~', execute,        COLOR_YELLOW,  0);
  define_prefix('\'', tick,          COLOR_BLUE,    0);
  define_prefix('`', compile_tick,   COLOR_BLUE,    0);
  define_prefix(',', compile_inline, COLOR_CYAN,    0);

  define_primitive(state, "nop", OP_NOP);
  define_primitive(state, ".", OP_PRINT_TOS);
  define_primitive(state, "dup", OP_DUP);
  define_primitive(state, "over", OP_OVER);
  define_primitive(state, "swap", OP_SWAP);
  define_primitive(state, "drop", OP_DROP);
  define_primitive(state, "rot", OP_ROT);
  define_primitive(state, "-rot", OP_MINUS_ROT);
  define_primitive(state, "nip", OP_NIP);
  define_primitive(state, "+", OP_ADD);
  define_primitive(state, "-", OP_SUB);
  define_primitive(state, "*", OP_MUL);
  define_primitive(state, "=", OP_EQUAL);
  define_primitive(state, "<", OP_LESS);
  define_primitive(state, "bye", OP_BYE);
  define_primitive(state, "words", OP_WORDS);
  define_primitive(state, "emit", OP_EMIT);
  define_primitive(state, "key", OP_KEY);
  define_primitive(state, "@", OP_LOAD);
  define_primitive(state, "!", OP_STORE);
  define_primitive(state, "c@", OP_CLOAD);
  define_primitive(state, "c!", OP_CSTORE);
  define_primitive(state, "cell", OP_CELL);
  define_primitive(state, "here", OP_HERE);
  define_primitive(state, "latest", OP_LATEST);
  define_primitive(state, "i-latest", OP_I_LATEST);
  define_primitive(state, ">>", OP_COMPILE_LITERAL);
  define_primitive(state, "code>", OP_GET_ENTRY_CODE);
  define_primitive(state, "execute", OP_EXECUTE);
  define_primitive(state, ".s", OP_DOT_S);

  define_primitive_inlined(state, ";", OP_RETURN);
  define_primitive_inlined(state, "when", OP_WHEN);
  define_primitive_inlined(state, "unless", OP_UNLESS);
  define_primitive_inlined(state, "choose", OP_CHOOSE);

  define_primitive_inlined(state, ">R", OP_R_PUSH);
  define_primitive_inlined(state, "R>", OP_R_POP);
  define_primitive_inlined(state, "R@", OP_R_FETCH);

  init_lib(state);

#ifdef __USE_REGISTER
  // A, B, C, I and J registers are state global
  define_register_primitive(A);
  define_register_primitive(B);
  define_register_primitive(C);
  define_register_primitive(I);
  define_register_primitive(J);
#endif

  LOAD_EXTENTIONS;

#ifdef __EMBED_LIB_CF
  parse_from_embed_lib_cf(state);
#endif /* __EMBED_LIB_CF */

  state->color = execute;
  echo_color(state, '~', COLOR_YELLOW);
  state->coll = 0; state->line = 1;
  state->echo_on = 1;

  return state;
}

void
free_state(struct state* state)
{
  free(state->heap);
  free(state->inlined_dict.entries);
  free(state->dict.entries);
  free(state->r_stack);
  free(state->stack);
  free(state);
}
