// The author disclaims copyright to this source code.

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <colorforth.h>
#include <cf-stdio.h>

#ifdef __EMBED_LIB_CF
#include <lib.cf.h>
#endif /* __EMBED_LIB_CF */

struct primitive_map primitive_map[MAX_OP_CODE];


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
cf_fatal_error(struct state *state, const char* format, ...)
{
  va_list arg;

  va_start (arg, format);
  vfprintf(stdout, format, arg);
  va_end (arg);
  echo_color(state, ' ', COLOR_CLEAR);
  reset_terminal();
  exit(1);
}

void
init_stack(struct stack *stack, int len)
{
  stack->cells = calloc(len, sizeof(cell));
  stack->sp = 0;
  stack->lim = len - 1;
}

static void
dot_s(struct state *state, struct stack *stack)
{
  for (int i = 0, p = stack->sp + 1; i <= stack->lim; i++, p++)
  {
    if (p > stack->lim) p = 0;
    cf_printf(state, "%"CELL_FMT" ", stack->cells[p]);
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

    cf_printf(s, "%.*s ", (int)entry->name_len, entry->name);
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
define_primitive_generic(struct state *s, struct dictionary *dict, char name[], const enum opcode opcode)
{
  dict->latest++;
  struct entry *entry = dict->latest;
  entry->name_len = strlen(name);
  entry->name = calloc(1, entry->name_len);
  memcpy(entry->name, name, entry->name_len);
  entry->code = s->here;
  entry->code->opcode = opcode;
  entry->code->this = 0;

  (entry->code + 1)->opcode = OP_RETURN;
  (entry->code + 1)->this = 0;

  s->here = (struct code *)s->here + 2;

  primitive_map[opcode].name = name;
  primitive_map[opcode].opcode = opcode;
  primitive_map[opcode].func = NULL;
}

static void
define_primitive(struct state *s, char name[], const enum opcode opcode)
{
  define_primitive_generic(s, &s->dict, name, opcode);
}

static void
define_primitive_inlined(struct state *s, char name[], const enum opcode opcode)
{
  define_primitive_generic(s, &s->inlined_dict, name, opcode);
}

void
define_primitive_extension(struct state *s, char name[], void (*func)(struct state *s))
{
  if (s->current_opcode >= MAX_OP_CODE)
  {
    cf_fatal_error(s, "Too many opcode defined: %d\n", s->current_opcode);
  }
  define_primitive(s, name, s->current_opcode);
  primitive_map[s->current_opcode].func = func;
  s->current_opcode += 1;
}

static bool
tib_to_number(struct state *s, cell *n)
{
  char *endptr;
  s->tib.buf[s->tib.len] = '\0';
  *n = strtol(s->tib.buf, &endptr, 10);
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
  entry->name = calloc(1, entry->name_len);
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
    code->this = entry->code[i].this;
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
    code->this = (cell)entry;
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
      code->this = n;
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
  code->this = pop(s->stack);
  s->here = (struct code *)s->here + 1;
}

static void
execute_(struct state *s, struct entry *entry)
{
  // cf_printf(s, "-> %s\n", entry->name);
  struct code *pc = entry->code;

  push(s->r_stack, 0);

  // don't forget to compile a return!!!!
  while(1)
  {
    switch (pc->opcode)
    {
      case OP_NOP: {
        break;
      }

      case OP_PRINT_TOS:
      {
        cf_printf(s, "%"CELL_FMT" ", pop(s->stack));
        cf_fflush();
        break;
      }

      case OP_DUP:
      {
        push(s->stack, s->stack->cells[s->stack->sp]);
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

      case OP_SWAP:
      {
        const cell n1 = pop(s->stack);
        const cell n2 = pop(s->stack);
        push(s->stack, n1);
        push(s->stack, n2);
        break;
      }

      case OP_DROP:
      {
        pop(s->stack);
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
        pop(s->stack);
        const cell n2 = pop(s->stack);
        push(s->stack, n2);
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

      case OP_CELL:
      {
        push(s->stack, sizeof(cell));
        break;
      }

      case OP_CALL:
      {
        struct entry *entry_ = (struct entry*)pc->this;
        push(s->r_stack, (cell)pc);
        pc = entry_->code - 1;
        break;
      }

      case OP_TAIL_CALL:
      {
        struct entry *entry_ = (struct entry*)pc->this;
        pc = entry_->code - 1;
        break;
      }

      case OP_NUMBER:
      case OP_TICK_NUMBER:
      {
        push(s->stack, pc->this);
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

      case OP_DOT_S:
      {
        dot_s(s, s->stack);
        break;
      }

      default:
      {
        if (primitive_map[pc->opcode].func != NULL)
        {
          primitive_map[pc->opcode].func(s);
        }
        else
        {
          cf_printf(s, "unknown opcode");
          quit(s, 1);
        }
      }
    }
    pc++;
  }
  // cf_printf(s, "   %s(done) <-\n", entry->name);
}

static void
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
    code->this = (cell)entry;
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
  switch (c)
  {
    case ':':
    {
      state->color = define;
      echo_color(state, c, COLOR_RED);
      break;
    }

    case '|':
    {
      state->color = define_inlined;
      echo_color(state, c, COLOR_MAGENTA);
      break;
    }

    case '^':
    {
      state->color = compile;
      echo_color(state, c, COLOR_GREEN);
      break;
    }

    case '%':
    {
      state->echo_on = 0;
      state->color = execute;
      echo_color(state, c, COLOR_YELLOW);
      break;
    }

    case '~':
    {
      state->color = execute;
      echo_color(state, c, COLOR_YELLOW);
      break;
    }

    case '\'':
    {
      if (state->color == execute)
      {
        state->color = tick;
      }
      else
      {
        state->color = compile_tick;
      }
      echo_color(state, c, COLOR_BLUE);
      break;
    }

    case ',':
    {
      state->color = compile_inline;
      echo_color(state, c, COLOR_CYAN);
      break;
    }

    case '\n':
    case ' ':
    case '\t':
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
        clear_tib(state);
      }
      break;
    }

    case '\b':
    case 127:
    {
      cf_printf(state, "\b \b");
      if (state->tib.len > 0)
      {
        state->tib.len -= 1;
      }
      break;
    }

    case CF_EOF:
    {
      //~ exit(0);
      echo_color(state, c, COLOR_CLEAR);
      break;
    }

    default:
    {
      echo_color(state, c, NULL);
      if (state->tib.len < sizeof(state->tib.buf))
      {
        state->tib.buf[state->tib.len++] = c;
      }
      break;
    }
  }

  if (c == '\n')
  {
    state->coll = 0; state->line += 1;
  }
  else if (c == CF_EOF)
  {
    state->coll = 0; state->line = 1;
  }
  else
  {
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
parse_from_string(struct state *s, char *str, unsigned int len)
{
  if (!len) len = 0xFFFF;

  char *old_stream = s->str_stream;

  s->str_stream = str;
  int c;
  unsigned int i = 0;
  while((c = cf_getchar(s)) != 0 && i < len && !s->done)
  {
    parse_colorforth(s, c);
    i += 1;
  }

  s->str_stream = old_stream;

  parse_space(s);
}

struct state*
colorforth_newstate(void)
{
  struct state *state = calloc(1, sizeof(*state));
  state->color = execute;

  state->stack = calloc(1, sizeof(struct stack));
  init_stack(state->stack, STACK_SIZE);

  state->r_stack = calloc(1, sizeof(struct stack));
  init_stack(state->r_stack, R_STACK_SIZE);

  state->dict.entries = calloc(DICT_SIZE, sizeof(struct entry));
  state->dict.latest = state->dict.entries - 1;

  state->inlined_dict.entries = calloc(INLINED_DICT_SIZE, sizeof(struct entry));
  state->inlined_dict.latest = state->inlined_dict.entries - 1;

  state->heap = calloc(1, HEAP_SIZE);
  state->here = state->heap;

  state->current_opcode = __LAST_PRIMITIVE_OP_CODE__;

  state->coll = 0; state->line = 1;
  state->done = 0;
  state->echo_on = 0;

  state->str_stream = NULL;
  state->file_stream = NULL;

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
  define_primitive(state, ">_", OP_COMPILE_LITERAL);
  define_primitive(state, "code>", OP_GET_ENTRY_CODE);
  define_primitive(state, "execute", OP_EXECUTE);
  define_primitive(state, ".s", OP_DOT_S);

  define_primitive_inlined(state, ";", OP_RETURN);
  define_primitive_inlined(state, "when", OP_WHEN);
  define_primitive_inlined(state, "unless", OP_UNLESS);

  define_primitive_inlined(state, ">R", OP_R_PUSH);
  define_primitive_inlined(state, "R>", OP_R_POP);
  define_primitive_inlined(state, "R@", OP_R_FETCH);

  LOAD_EXTENTIONS;

#ifdef __EMBED_LIB_CF
  parse_from_string(state, (char *)src_lib_cf, src_lib_cf_len);
#endif /* __EMBED_LIB_CF */

  state->color = execute;
  echo_color(state, '~', COLOR_YELLOW);
  state->coll = 0; state->line = 1;
  state->echo_on = 1;

  return state;
}
