#include <stdio.h>
#include <string.h>

#include "colorforth.h"


void
see(struct state *s, struct entry *entry)
{
  if (entry)
  {
    printf(":%.*s ", (int)entry->name_len, entry->name);
    for (size_t i = 0, done = 0; !done; i++)
    {
      struct entry *entry_ = (struct entry*) entry->code[i].this;
      switch(entry->code[i].opcode)
      {
        case OP_RETURN:
        {
          printf("; ");
          done = 1;
          break;
        }

        case OP_CALL:
        {
          printf("%s ", entry_->name);
          break;
        }
        case OP_TAIL_CALL:
        {
          printf("%s¬ ", entry->name);
          break;
        }

        case OP_TICK_NUMBER:
        {
          printf("'%s ", entry_->name);
          break;
        }

        case OP_NUMBER:
        {
          printf("%"CELL_FMT" ", entry->code[i].this);
          break;
        }

        default:
        {
          printf("%s ", primitive_map[entry->code[i].opcode].name);
        }
      }
    }
    printf("\n");
  }
  else
  {
    unknow_word(s, "with");
  }
}

void
disassemble_dict(struct state *s)
{
  printf("-------- Words ------------------------------------------\n");
  for (struct entry *entry = s->dict.latest - 1; entry != s->dict.entries - 1; entry--)
  {
    s->tib.len = entry->name_len;
    memcpy(s->tib.buf, entry->name, entry->name_len);
    struct entry *entry_ = find_entry(s);
    see(s, entry_);
  }
  printf("---------------------------------------------------------\n");
}

void
see_func(struct state *s)
{
    struct entry *entry_ = (struct entry*)pop(s->stack);
    see(s, entry_);
}

void
init_dict_utils(struct state *state)
{
  define_extension(state, "see", OP_SEE, see_func);
  define_extension(state, "disassemble", OP_DISASSEMBLE_DICT, disassemble_dict);
}
