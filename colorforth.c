// The author disclaims copyright to this source code.

#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef int_fast32_t cell;
// circular stack
static cell stack[8];
// stack position
static uint32_t sp;

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
  OP_IF,
  OP_BYE,
  OP_WORDS,
  OP_RETURN,
  OP_TICK,
  OP_EMIT,
  OP_KEY,
  OP_LOAD,
  OP_STORE,
  OP_CELL,
  // call defined word
  OP_CALL,
  OP_TAIL_CALL,
  OP_NUMBER,
  OP_HERE,
};

// terminal input buffer
static struct tib
{
  char buf[8];
  uint32_t len;
} tib;
struct code;
struct entry;
static struct entry
{
  char name[sizeof(tib.buf)];
  cell name_len;
  struct entry *prev;
  cell code_len;
  struct code
  {
    enum opcode opcode;
    cell this;
  } code[];
} *dictionary = (void*)(char[4096]){0};
static struct entry *latest;
static void *here;

static void
push(const cell n)
{
  if (sp == (sizeof(stack) / sizeof(stack[0])) - 1)
  {
    sp = 0;
  }
  else
  {
    sp += 1;
  }
  stack[sp] = n;
}

static cell
pop(void)
{
  const cell n = stack[sp];
  if (sp == 0)
  {
    sp = (sizeof(stack) / sizeof(stack[0])) - 1;
  }
  else
  {
    sp -= 1;
  }
  return n;
}

static struct entry*
find_entry(void)
{
  for (struct entry *entry = latest; entry; entry = entry->prev)
  {
    if (entry->name_len == tib.len && memcmp(entry->name, tib.buf, tib.len) == 0)
    {
      return entry;
    }
  }
  return NULL;
}

// 'name' must be null-terminated.
static void
define_primitive(const char name[], const enum opcode opcode)
{
  struct entry *entry = here;
  entry->name_len = strlen(name);
  memcpy(entry->name, name, entry->name_len);
  entry->prev = latest;
  entry->code[0].opcode = opcode;
  entry->code[0].this = 0;
  entry->code_len = 1;
  here = &entry->code[1];
  latest = entry;
}

typedef void (*func)(void);

static void
define(void)
{
  struct entry *entry = here;
  entry->name_len = tib.len;
  memcpy(entry->name, tib.buf, tib.len);
  entry->prev = latest;
  here = &entry->code[0];
  entry->code_len = 0;
  latest = entry;
}

// TODO: use strtoul instead
// or, support signed with strtol
static bool
tib_to_number(cell *n_)
{
  cell n = 0;
  for (uint32_t i = 0; i < tib.len; ++i)
  {
    if (isdigit(tib.buf[i]))
    {
      n *= 10;
      n += tib.buf[i] - '0';
    }
    else
    {
      return 0;
    }
  }
  *n_ = n;
  return 1;
}

static void
compile(void)
{
  struct entry *entry = find_entry();
  struct code *code = &latest->code[latest->code_len];
  if (entry)
  {
    if (entry == latest)
    {
      code->opcode = OP_TAIL_CALL;
      code->this = 0;
    }
    // FIXME: why a special case
    else if (strcmp(entry->name, "if") == 0)
    {
      code->opcode = OP_IF;
      code->this = 0;
    }
    // FIXME: why a special case
    else if (strcmp(entry->name, ";") == 0)
    {
      code->opcode = OP_RETURN;
      code->this = 0;
    }
    else
    {
      code->opcode = OP_CALL;
      code->this = (cell)entry;
    }
    latest->code_len += 1;
  }
  else
  {
    // try to interpret as an unsigned decimal number
    cell n = 0;
    if (tib_to_number(&n))
    {
      // compile number
      code->opcode = OP_NUMBER;
      code->this = n;
      latest->code_len += 1;
    }
    else
    {
      printf("? ");
    }
  }
  here = &latest->code[latest->code_len];
}

static void
execute_(struct entry *entry)
{
  //~ printf("%s\t%li\n", entry->name, entry->code_len);
  for (struct code *pc = &entry->code[0]; pc < &entry->code[entry->code_len]; ++pc)
  {
    switch (pc->opcode)
    {
      case OP_PRINT_TOS:
      {
        printf("%"PRIiFAST32" ", pop());
        fflush(stdout);
        break;
      }
      
      case OP_DUP:
      {
        push(stack[sp]);
        break;
      }
      
      case OP_OVER:
      {
        const cell n1 = pop();
        const cell n2 = pop();
        push(n2);
        push(n1);
        push(n2);
        break;
      }
      
      case OP_SWAP:
      {
        const cell n1 = pop();
        const cell n2 = pop();
        push(n1);
        push(n2);
        break;
      }
      
      case OP_DROP:
      {
        pop();
        break;
      }
      
      case OP_ADD:
      {
        const cell n1 = pop();
        const cell n2 = pop();
        push(n1 + n2);
        break;
      }
      
      case OP_SUB:
      {
        const cell n1 = pop();
        const cell n2 = pop();
        push(n1 - n2);
        break;
      }
      
      case OP_MUL:
      {
        const cell n1 = pop();
        const cell n2 = pop();
        push(n1 * n2);
        break;
      }
      
      case OP_EQUAL:
      {
        const cell n1 = pop();
        const cell n2 = pop();
        push(n1 == n2);
        break;
      }
      
      case OP_LESS:
      {
        const cell n1 = pop();
        const cell n2 = pop();
        push(n2 < n1);
        break;
      }
      
      case OP_IF:
      {
        const cell n = pop();
        if (!n)
        {
          // doesn't work, because this is only causing if to return, not foo.
          // fixed with an exception in compile
          ++pc;
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
        for (struct entry *entry = latest; entry; entry = entry->prev)
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
      
      case OP_TICK:
      {
        break;
      }
      
      case OP_EMIT:
      {
        putchar((char)pop());
        break;
      }
      
      case OP_KEY:
      {
        push((char)getchar());
        break;
      }
      
      case OP_LOAD:
      {
        push(*(cell*)pop());
        break;
      }
      
      case OP_STORE:
      {
        cell *ptr = (cell*)pop();
        const cell n = pop();
        *ptr = n;
        break;
      }
      
      case OP_CELL:
      {
        push(sizeof(cell));
        break;
      }
      
      case OP_CALL:
      {
        struct entry *entry_ = (struct entry*)pc->this;
        // recursion
        execute_(entry_);
        break;
      }
      
      case OP_TAIL_CALL:
      {
        pc = &entry->code[-1];
        break;
      }
      
      case OP_NUMBER:
      {
        push(pc->this);
        break;
      }
      
      case OP_HERE:
      {
        push((cell)&here);
        break;
      }
      
      default:
      {
        puts("unknown opcode");
        exit(1);
      }
    }
  }
}

static void
execute(void)
{
  struct entry *entry = find_entry();
  if (entry)
  {
    execute_(entry);
  }
  else
  {
    // try to interpret as an unsigned decimal number
    cell n = 0;
    if (tib_to_number(&n))
    {
      push(n);
    }
    else
    {
      printf("? ");
    }
  }
}

static void
comment(void)
{
  
}

int
main(int argc, char *argv[])
{
  here = dictionary;
  static const struct primitive_map
  {
    const char *name;
    const enum opcode opcode;
  } primitive_map[] = 
  {
    {".", OP_PRINT_TOS},
    {"dup", OP_DUP},
    {"over", OP_OVER},
    {"swap", OP_SWAP},
    {"drop", OP_DROP},
    {"+", OP_ADD},
    {"-", OP_SUB},
    {"*", OP_MUL},
    {"=", OP_EQUAL},
    {"<", OP_LESS},
    {"if", OP_IF},
    {"bye", OP_BYE},
    {"words", OP_WORDS},
    {";", OP_RETURN},
    {"'", OP_TICK},
    {"emit", OP_EMIT},
    {"key", OP_KEY},
    {"@", OP_LOAD},
    {"!", OP_STORE},
    {"cell", OP_CELL},
    {"here", OP_HERE},
  };
  for (unsigned int i = 0; i < sizeof(primitive_map) / sizeof(primitive_map[0]); ++i)
  {
    define_primitive(primitive_map[i].name, primitive_map[i].opcode);
  }
  while (1)
  {
    func color = execute;
    tib.len = 0;
    bool reading_word = true;
    while (reading_word)
    {
      int c;
      switch (c = getchar())
      {
        case ':':
        {
          color = define;
          break;
        }
        
        case '^':
        {
          color = compile;
          break;
        }
        
        case '~':
        {
          color = execute;
          break;
        }
        
        case '(':
        {
          color = comment;
          break;
        }
        
        case '\n':
        case ' ':
        case '\t':
        {
          if (tib.len == 0)
          {
            // Strip leading whitespace.
          }
          else
          {
            // Have word.
            color();
            reading_word = false;
          }
          break;
        }
        
        case EOF:
        {
          exit(0);
          break;
        }
        
        default:
        {
          if (tib.len < sizeof(tib.buf))
          {
            tib.buf[tib.len++] = c;
          }
          break;
        }
      }
    }
  }
  return 0;
}
