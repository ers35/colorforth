// The author disclaims copyright to this source code.

#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "colorforth.h"


extern void system_func(struct state *s);
extern void disassemble_dict(struct state *s);
extern void see_func(struct state *s);

void
push(struct state *s, const cell n)
{
  if (s->sp == (sizeof(s->stack) / sizeof(s->stack[0])) - 1)
  {
    s->sp = 0;
  }
  else
  {
    s->sp += 1;
  }
  s->stack[s->sp] = n;
}

cell
pop(struct state *s)
{
  const cell n = s->stack[s->sp];
  if (s->sp == 0)
  {
    s->sp = (sizeof(s->stack) / sizeof(s->stack[0])) - 1;
  }
  else
  {
    s->sp -= 1;
  }
  return n;
}

struct entry*
find_entry(struct state *s)
{
  for (struct entry *entry = s->latest; entry; entry = entry->prev)
  {
    if (entry->name_len == s->tib.len && memcmp(entry->name, s->tib.buf, s->tib.len) == 0)
    {
      return entry;
    }
  }
  return NULL;
}

void
unknow_word (struct state *s, const char *msg)
{
  printf("Error %s '", msg);
  for(size_t i = 0; i < s->tib.len; i++)
  {
    putchar(s->tib.buf[i]);
  }
  printf("': unknown word at line %ld, column %ld\n", s->line, s->coll);
}

// 'name' must be null-terminated.
static void
define_primitive(struct state *s, char name[], const enum opcode opcode)
{
  struct entry *entry = (struct entry*)&s->latest->code[s->latest->code_len];
  entry->name_len = strlen(name);
  memcpy(entry->name, name, entry->name_len);
  entry->prev = s->latest;
  entry->code[0].opcode = opcode;
  entry->code[0].this = 0;
  entry->code_len = 1;
  s->latest = entry;

  primitive_map[opcode].name = name;
  primitive_map[opcode].opcode = opcode;
  primitive_map[opcode].func = NULL;
}

static void
define_extension(struct state *s, char name[], const enum opcode opcode, void (*func)(struct state *s)) {
  define_primitive(s, name, opcode);
  primitive_map[opcode].func = func;
}

static bool
tib_to_number(struct state *s, cell *n)
{
  *n = strtol(s->tib.buf, NULL, 10);
  return 1;
}

/**
 *
 * Color functions
 *
 **/
static void
define(struct state *s)
{
  struct entry *entry = (struct entry*)&s->latest->code[s->latest->code_len];
  entry->name_len = s->tib.len;
  memcpy(entry->name, s->tib.buf, s->tib.len);
  entry->prev = s->latest;
  entry->code_len = 0;
  s->latest = entry;
}

static void
compile(struct state *s)
{
  struct entry *entry = find_entry(s);
  struct code *code = &s->latest->code[s->latest->code_len];
  if (entry)
  {
    if (entry == s->latest)
    {
      code->opcode = OP_TAIL_CALL;
      code->this = 0;
    }
    else
    {
      code->opcode = OP_CALL;
      code->this = (cell)entry;
    }
    s->latest->code_len += 1;
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
      s->latest->code_len += 1;
    }
    else
    {
      unknow_word(s, "compiling");
      exit(1);
    }
  }
}

static void
compile_inline(struct state *s)
{
  struct entry *entry = find_entry(s);
  if (entry)
  {
    const cell code_len = entry->code_len == 1 ? 1 : entry->code_len - 1;
    for (cell i = 0; i < code_len; i++)
    {
      struct code *code = &s->latest->code[s->latest->code_len];
      code->opcode = entry->code[i].opcode;
      code->this = entry->code[i].this;
      s->latest->code_len += 1;
    }
  }
  else
  {
    unknow_word(s, "inlining");
    exit(1);
  }
}

static void
execute_(struct state *s, struct entry *entry)
{
  // printf("-> %s\t%li\n", entry->name, entry->code_len);
  for (struct code *pc = &entry->code[0]; pc < &entry->code[entry->code_len]; ++pc)
  {
    switch (pc->opcode)
    {
      case OP_PRINT_TOS:
      {
        printf("%ld ", pop(s));
        fflush(stdout);
        break;
      }

      case OP_DUP:
      {
        push(s, s->stack[s->sp]);
        break;
      }

      case OP_OVER:
      {
        const cell n1 = pop(s);
        const cell n2 = pop(s);
        push(s, n2);
        push(s, n1);
        push(s, n2);
        break;
      }

      case OP_SWAP:
      {
        const cell n1 = pop(s);
        const cell n2 = pop(s);
        push(s, n1);
        push(s, n2);
        break;
      }

      case OP_DROP:
      {
        pop(s);
        break;
      }

      case OP_ADD:
      {
        const cell n1 = pop(s);
        const cell n2 = pop(s);
        push(s, n1 + n2);
        break;
      }

      case OP_SUB:
      {
        const cell n1 = pop(s);
        const cell n2 = pop(s);
        push(s, n2 - n1);
        break;
      }

      case OP_MUL:
      {
        const cell n1 = pop(s);
        const cell n2 = pop(s);
        push(s, n1 * n2);
        break;
      }

      case OP_EQUAL:
      {
        const cell n1 = pop(s);
        const cell n2 = pop(s);
        push(s, n1 == n2);
        break;
      }

      case OP_LESS:
      {
        const cell n1 = pop(s);
        const cell n2 = pop(s);
        push(s, n2 < n1);
        break;
      }

      case OP_WHEN:
      {
        struct entry *entry_ = (struct entry*)pop(s);
        const cell n = pop(s);
        if (n)
        {
          // OP_RETURN: leaving the current word
          if (entry_->code[0].opcode == OP_RETURN)
          {
            pc = &entry->code[entry->code_len];
          }
          // Call itself -> recurse
          else if (entry_ == entry)
          {
            pc = &entry->code[-1];
          }
          // Call entry on the stack
          else
          {
            execute_(s, entry_);
          }
        }
        break;
      }

      case OP_UNLESS:
      {
        struct entry *entry_ = (struct entry*)pop(s);
        const cell n = pop(s);
        if (!n)
        {
          // OP_RETURN: leaving the current word
          if (entry_->code[0].opcode == OP_RETURN)
          {
            pc = &entry->code[entry->code_len];
          }
          // Call itself -> recurse
          else if (entry_ == entry)
          {
            pc = &entry->code[-1];
          }
          // Call entry on the stack
          else
          {
            execute_(s, entry_);
          }
        }
        break;
      }

      case OP_CHOOSE:
      {
        struct entry *entry_false_ = (struct entry*)pop(s);
        struct entry *entry_true_ = (struct entry*)pop(s);
        const cell n = pop(s);
        struct entry *entry_ = n ? entry_true_ : entry_false_;
        if (entry_->code[0].opcode == OP_RETURN)
        {
          pc = &entry->code[entry->code_len];
        }
        // Call itself -> recurse
        else if (entry_ == entry)
        {
          pc = &entry->code[-1];
        }
        // Call entry on the stack
        else
        {
          execute_(s, entry_);
        }
        break;
      }

      case OP_BYE:
      {
        exit(0);
        break;
      }

      case OP_WORDS:
      {
        for (struct entry *entry = s->latest; entry; entry = entry->prev)
        {
          printf("%.*s ", (int)entry->name_len, entry->name);
        }
        break;
      }

      case OP_RETURN:
      {
        pc = &entry->code[entry->code_len];
        break;
      }

      case OP_EMIT:
      {
        putchar((char)pop(s));
        break;
      }

      case OP_KEY:
      {
        push(s, (char)getchar());
        break;
      }

      case OP_LOAD:
      {
        push(s, *(cell*)pop(s));
        break;
      }

      case OP_STORE:
      {
        cell *ptr = (cell*)pop(s);
        cell n = pop(s);
        *ptr = n;
        break;
      }

      case OP_CLOAD:
      {
        push(s, *(char*)pop(s));
        break;
      }

      case OP_CSTORE:
      {
        char *ptr = (char*)pop(s);
        char n = pop(s);
        *ptr = n;
        break;
      }

      case OP_CELL:
      {
        push(s, sizeof(cell));
        break;
      }

      case OP_CALL:
      {
        struct entry *entry_ = (struct entry*)pc->this;
        // recursion
        execute_(s, entry_);
        break;
      }

      case OP_TAIL_CALL:
      {
        pc = &entry->code[-1];
        break;
      }

      case OP_NUMBER:
      case OP_TICK_NUMBER:
      {
        push(s, pc->this);
        break;
      }

      case OP_EXECUTE:
      {
        struct entry *entry_ = (struct entry*)pop(s);
        execute_(s, entry_);
        break;
      }

      case OP_HERE:
      {
        push(s, (cell)&s->here);
        break;
      }

      default:
      {
        if (primitive_map[pc->opcode].func != NULL)
        {
          primitive_map[pc->opcode].func(s);
        }
        else {
          puts("unknown opcode");
          exit(1);
        }
      }
    }
  }
  // printf("   %s(done) <-\n", entry->name);
}

static void
execute(struct state *s)
{
  struct entry *entry = find_entry(s);
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
      push(s, n);
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
  struct entry *entry = find_entry(s);
  if (entry)
  {
    push(s, (cell)entry);
  }
  else
  {
    unknow_word(s, "ticking");
  }
}

static void
compile_tick(struct state *s)
{
  struct entry *entry = find_entry(s);
  if (entry)
  {
    struct code *code = &s->latest->code[s->latest->code_len];
    code->opcode = OP_TICK_NUMBER;
    code->this = (cell)entry;
    s->latest->code_len += 1;
  }
  else
  {
    unknow_word(s, "ticking");
    exit(1);
  }
}

static void
comment(struct state *s)
{

}

void
parse_colorforth(struct state *state, int c) {
  switch (c)
  {
    case ':':
    {
      state->color = define;
      break;
    }

    case '^':
    {
      if (state->color == execute)
      {
        struct code *code = &state->latest->code[state->latest->code_len];
        code->opcode = OP_NUMBER;
        code->this = pop(state);
        state->latest->code_len += 1;
      }
      state->color = compile;
      break;
    }

    case '~':
    {
      state->color = execute;
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
      break;
    }

    case '(':
    {
      state->color = comment;
      break;
    }

    case ',':
    {
      state->color = compile_inline;
      break;
    }

    case '\n':
    case ' ':
    case '\t':
    {
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
      break;
    }

    case EOF:
    {
      //~ exit(0);
      break;
    }

    default:
    {
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
  else
  {
    state->coll += 1;
  }
}

struct state*
colorforth_newstate(void)
{
  struct state *state = calloc(1, sizeof(*state));
  state->color = execute;
  state->dictionary = calloc(1, 4096);
  state->latest = state->dictionary;
  state->here = calloc(1, 4096);
  state->coll = 0; state->line = 1;

  define_primitive(state, ".", OP_PRINT_TOS);
  define_primitive(state, "dup", OP_DUP);
  define_primitive(state, "over", OP_OVER);
  define_primitive(state, "swap", OP_SWAP);
  define_primitive(state, "drop", OP_DROP);
  define_primitive(state, "+", OP_ADD);
  define_primitive(state, "-", OP_SUB);
  define_primitive(state, "*", OP_MUL);
  define_primitive(state, "=", OP_EQUAL);
  define_primitive(state, "<", OP_LESS);
  define_primitive(state, "when", OP_WHEN);
  define_primitive(state, "unless", OP_UNLESS);
  define_primitive(state, "choose", OP_CHOOSE);
  define_primitive(state, "bye", OP_BYE);
  define_primitive(state, "words", OP_WORDS);
  define_primitive(state, ";", OP_RETURN);
  define_primitive(state, "emit", OP_EMIT);
  define_primitive(state, "key", OP_KEY);
  define_primitive(state, "@", OP_LOAD);
  define_primitive(state, "!", OP_STORE);
  define_primitive(state, "c@", OP_CLOAD);
  define_primitive(state, "c!", OP_CSTORE);
  define_primitive(state, "cell", OP_CELL);
  define_primitive(state, "here", OP_HERE);
  define_primitive(state, "execute", OP_EXECUTE);

  define_extension(state, "system", OP_SYSTEM, system_func);

  define_extension(state, "see", OP_SEE, see_func);
  define_extension(state, "disassemble", OP_DISASSEMBLE_DICT, disassemble_dict);

  return state;
}

int
main(int argc, char *argv[])
{
  struct state *state = colorforth_newstate();
  while (1)
  {
    parse_colorforth(state, getchar());
  }
  return 0;
}
