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

struct prefix_map prefix_map[MAX_PREFIX];


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

inline void
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

inline cell
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

struct code*
clear_tib_fn (struct state *s, struct code *pc){
  for(size_t i = 0; i < s->tib.len; i++)
  {
    s->tib.buf[i] = 0;
  }
  s->tib.len = 0;

  return pc;
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

struct code*
return_fn (struct state *s, struct code *pc)
{
  return pc;
}

struct code*
call_fn (struct state *s, struct code *pc)
{
  struct entry *entry_ = (struct entry*)pc->this;
  if (s->choose_state) {
    s->choose_state = 0;
    pc++;
  }

  push(s->r_stack, (cell)pc);
  return entry_->code - 1;
}

struct code*
tail_call_fn (struct state *s, struct code *pc)
{
  struct entry *entry_ = (struct entry*)pc->this;
  return entry_->code - 1;
}

struct code*
number_fn (struct state *s, struct code *pc)
{
  push(s->stack, pc->this);

  if (s->choose_state) {
    s->choose_state = 0;
    pc++;
  }
  return pc;
}

struct code*
tick_number_fn (struct state *s, struct code *pc)
{
  push(s->stack, pc->this);

  if (s->choose_state) {
    s->choose_state = 0;
    pc++;
  }
  return pc;
}


// 'name' must be null-terminated.
static void
define_primitive_generic(struct state *s, struct dictionary *dict, char name[],
                         struct code * (*fn)(struct state *state, struct code *pc))
{
  dict->latest++;
  struct entry *entry = dict->latest;
  entry->name_len = strlen(name);
  entry->name = calloc(1, entry->name_len);
  memcpy(entry->name, name, entry->name_len);
  entry->code = s->here;
  entry->code->fn = fn;
  entry->code->this = 0;

  (entry->code + 1)->fn = return_fn;
  (entry->code + 1)->this = 0;

  s->here = (struct code *)s->here + 2;
}

void
define_primitive(struct state *s, char name[], struct code * (*fn)(struct state *state, struct code *pc))
{
  define_primitive_generic(s, &s->dict, name, fn);
}

void
define_primitive_inlined(struct state *s, char name[], struct code * (*fn)(struct state *state, struct code *pc))
{
  define_primitive_generic(s, &s->inlined_dict, name, fn);
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

struct code*
nop_fn (struct state *s, struct code *pc)
{
  return pc;
}

struct code*
print_tos_fn (struct state *s, struct code *pc)
{
  cf_printf(s, "%"CELL_FMT" ", pop(s->stack));
  cf_fflush();
  return pc;
}

struct code*
dup_fn (struct state *s, struct code *pc)
{
  push(s->stack, s->stack->cells[s->stack->sp]);
  return pc;
}

struct code*
over_fn (struct state *s, struct code *pc)
{
  const cell n1 = pop(s->stack);
  const cell n2 = pop(s->stack);
  push(s->stack, n2);
  push(s->stack, n1);
  push(s->stack, n2);
  return pc;
}

struct code*
swap_fn (struct state *s, struct code *pc)
{
  const cell n1 = pop(s->stack);
  const cell n2 = pop(s->stack);
  push(s->stack, n1);
  push(s->stack, n2);
  return pc;
}

struct code*
drop_fn (struct state *s, struct code *pc)
{
  pop(s->stack);
  return pc;
}

struct code*
rot_fn (struct state *s, struct code *pc)
{
  const cell n3 = pop(s->stack);
  const cell n2 = pop(s->stack);
  const cell n1 = pop(s->stack);
  push(s->stack, n2);
  push(s->stack, n3);
  push(s->stack, n1);
  return pc;
}

struct code*
minus_rot_fn (struct state *s, struct code *pc)
{
  const cell n3 = pop(s->stack);
  const cell n2 = pop(s->stack);
  const cell n1 = pop(s->stack);
  push(s->stack, n3);
  push(s->stack, n1);
  push(s->stack, n2);
  return pc;
}

struct code*
nip_fn (struct state *s, struct code *pc)
{
  const cell n2 = pop(s->stack);
  pop(s->stack);
  push(s->stack, n2);
  return pc;
}

struct code*
add_fn (struct state *s, struct code *pc)
{
  const cell n1 = pop(s->stack);
  const cell n2 = pop(s->stack);
  push(s->stack, n1 + n2);
  return pc;
}

struct code*
sub_fn (struct state *s, struct code *pc)
{
  const cell n1 = pop(s->stack);
  const cell n2 = pop(s->stack);
  push(s->stack, n2 - n1);
  return pc;
}

struct code*
mul_fn (struct state *s, struct code *pc)
{
  const cell n1 = pop(s->stack);
  const cell n2 = pop(s->stack);
  push(s->stack, n1 * n2);
  return pc;
}

struct code*
equal_fn (struct state *s, struct code *pc)
{
  const cell n1 = pop(s->stack);
  const cell n2 = pop(s->stack);
  push(s->stack, n1 == n2);
  return pc;
}

struct code*
less_fn (struct state *s, struct code *pc)
{
  const cell n1 = pop(s->stack);
  const cell n2 = pop(s->stack);
  push(s->stack, n2 < n1);
  return pc;
}

struct code*
bye_fn (struct state *s, struct code *pc)
{
  quit(s, 0);
  return pc;
}

struct code*
words_fn (struct state *s, struct code *pc)
{
  words(s);
  return pc;
}

struct code*
emit_fn (struct state *s, struct code *pc)
{
  cf_putchar(s, (char)pop(s->stack));
  return pc;
}

struct code*
key_fn (struct state *s, struct code *pc)
{
  push(s->stack, (char)cf_getchar(s));
  return pc;
}

struct code*
load_fn (struct state *s, struct code *pc)
{
  push(s->stack, *(cell*)pop(s->stack));
  return pc;
}

struct code*
store_fn (struct state *s, struct code *pc)
{
  cell *ptr = (cell*)pop(s->stack);
  cell n = pop(s->stack);
  *ptr = n;
  return pc;
}

struct code*
cload_fn (struct state *s, struct code *pc)
{
  push(s->stack, *(char*)pop(s->stack));
  return pc;
}

struct code*
cstore_fn (struct state *s, struct code *pc)
{
  char *ptr = (char*)pop(s->stack);
  char n = pop(s->stack);
  *ptr = n;
  return pc;
}

struct code*
cell_fn (struct state *s, struct code *pc)
{
  push(s->stack, sizeof(cell));
  return pc;
}

struct code*
get_entry_code_fn (struct state *s, struct code *pc)
{
  struct entry *entry_ = (struct entry*)pop(s->stack);
  push(s->stack, (cell)entry_->code);
  return pc;
}

struct code*
execute_fn (struct state *s, struct code *pc)
{
  struct code *code_ = (struct code*)pop(s->stack);
  push(s->r_stack, (cell)pc);
  return code_ - 1;
}

struct code*
here_fn (struct state *s, struct code *pc)
{
  push(s->stack, (cell)&s->here);
  return pc;
}

struct code*
latest_fn (struct state *s, struct code *pc)
{
  push(s->stack, (cell)&s->dict.latest);
  return pc;
}

struct code*
i_latest_fn (struct state *s, struct code *pc)
{
  push(s->stack, (cell)&s->inlined_dict.latest);
  return pc;
}

struct code*
compile_literal_fn (struct state *s, struct code *pc)
{
  // compile_literal(s);
  struct code *code =  (struct code *)s->here;
  code->fn = number_fn;
  code->this = pop(s->stack);
  s->here = (struct code *)s->here + 1;
  return pc;
}

struct code*
dot_s_fn (struct state *s, struct code *pc)
{
    for (int i = 0, p = s->stack->sp + 1; i <= s->stack->lim; i++, p++)
  {
    if (p > s->stack->lim) p = 0;
    cf_printf(s, "%"CELL_FMT" ", s->stack->cells[p]);
  }
  cf_printf(s, " <tos\n");
  return pc;
}

struct code*
when_fn (struct state *s, struct code *pc)
{
  const cell n = pop(s->stack);
  return n ? pc : pc + 1;
}

struct code*
unless_fn (struct state *s, struct code *pc)
{
  const cell n = pop(s->stack);
  return n ? pc + 1 : pc;
}

struct code*
choose_fn (struct state *s, struct code *pc)
{
  const cell n = pop(s->stack);
  if (n)
  {
    s->choose_state = 1;
  }
  else
  {
    pc++;
  }
  return pc;
}

struct code*
r_push_fn (struct state *s, struct code *pc)
{
  push(s->r_stack, pop(s->stack));
  return pc;
}

struct code*
r_pop_fn (struct state *s, struct code *pc)
{
  push(s->stack, pop(s->r_stack));
  return pc;
}

struct code*
r_fetch_fn (struct state *s, struct code *pc)
{
  push(s->stack, s->r_stack->cells[s->r_stack->sp]);
  return pc;
}

static void
inline_entry(struct state *s, struct entry *entry)
{
  for (size_t i = 0, done = 0; !done; i++)
  {
    // inline the first OP_RETURN
    if (entry->code[i].fn == return_fn && i > 0)
    {
      break;
    }
    struct code *code = (struct code *)s->here;
    code->this = entry->code[i].this;
    code->fn = entry->code[i].fn;
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
    code->fn = entry == s->dict.latest ? tail_call_fn : call_fn;
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
      code->fn = number_fn;
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
execute_(struct state *s, struct entry *entry)
{
  // cf_printf(s, "-> %s\n", entry->name);
  register struct code *pc = entry->code;

  push(s->r_stack, 0);

  s->choose_state = 0;

  // don't forget to compile a return!!!!
  while(1)
  {
    if (pc->fn == return_fn)
    {
      struct code *code = (struct code *)pop(s->r_stack);
      if (code)
      {
        pc = code + 1;
      }
      else
      {
        return;
      }
    }
    else
    {
      pc = pc->fn(s, pc);
      pc++;
    }
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
    code->fn = tick_number_fn;
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

  if (c == ' ' || c == '\t' || c == '\n')
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

void
define_prefix(char c, void (*fn)(struct state *s), char * color, int n_prefix)
{
  prefix_map[n_prefix].c = c;
  prefix_map[n_prefix].fn = fn;
  prefix_map[n_prefix].color = color;
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

  state->coll = 0; state->line = 1;
  state->done = 0;
  state->echo_on = 0;

  state->str_stream = NULL;
  state->file_stream = NULL;

  int n_prefix=0;
  define_prefix(':', define,         COLOR_RED,     n_prefix++);
  define_prefix('|', define_inlined, COLOR_MAGENTA, n_prefix++);
  define_prefix('^', compile,        COLOR_GREEN,   n_prefix++);
  define_prefix('~', execute,        COLOR_YELLOW,  n_prefix++);
  define_prefix('\'', tick,          COLOR_BLUE,    n_prefix++);
  define_prefix('`', compile_tick,   COLOR_BLUE,    n_prefix++);
  define_prefix(',', compile_inline, COLOR_CYAN,    n_prefix++);

  define_primitive(state, "nop", nop_fn);
  define_primitive(state, ".", print_tos_fn);
  define_primitive(state, "dup", dup_fn);
  define_primitive(state, "over", over_fn);
  define_primitive(state, "swap", swap_fn);
  define_primitive(state, "drop", drop_fn);
  define_primitive(state, "rot", rot_fn);
  define_primitive(state, "-rot", minus_rot_fn);
  define_primitive(state, "nip", nip_fn);
  define_primitive(state, "+", add_fn);
  define_primitive(state, "-", sub_fn);
  define_primitive(state, "*", mul_fn);
  define_primitive(state, "=", equal_fn);
  define_primitive(state, "<", less_fn);
  define_primitive(state, "bye", bye_fn);
  define_primitive(state, "words", words_fn);
  define_primitive(state, "emit", emit_fn);
  define_primitive(state, "key", key_fn);
  define_primitive(state, "@", load_fn);
  define_primitive(state, "!", store_fn);
  define_primitive(state, "c@", cload_fn);
  define_primitive(state, "c!", cstore_fn);
  define_primitive(state, "cell", cell_fn);
  define_primitive(state, "here", here_fn);
  define_primitive(state, "latest", latest_fn);
  define_primitive(state, "i-latest", i_latest_fn);
  define_primitive(state, ">>", compile_literal_fn);
  define_primitive(state, "code>", get_entry_code_fn);
  define_primitive(state, "execute", execute_fn);
  define_primitive(state, ".s", dot_s_fn);

  define_primitive_inlined(state, ";", return_fn);
  define_primitive_inlined(state, "when", when_fn);
  define_primitive_inlined(state, "unless", unless_fn);
  define_primitive_inlined(state, "choose", choose_fn);

  define_primitive_inlined(state, ">R", r_push_fn);
  define_primitive_inlined(state, "R>", r_pop_fn);
  define_primitive_inlined(state, "R@", r_fetch_fn);

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
