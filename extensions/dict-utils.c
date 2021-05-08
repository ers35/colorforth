#include <stdio.h>
#include <string.h>

#include "colorforth.h"

void
dump_words(struct dictionary *dict)
{
  for (struct entry *entry = dict->latest; entry != dict->entries - 1; entry--)
  {
    printf("%.*s ", (int)entry->name_len, entry->name);
  }
}

void
words(struct state *s)
{
  dump_words(&s->macro_dict);
  dump_words(&s->dict);
  printf("\n");
}

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
disassemble_dict(struct state *s, struct dictionary *dict)
{
  for (struct entry *entry = dict->latest; entry != dict->entries - 1; entry--)
  {
    s->tib.len = entry->name_len;
    memcpy(s->tib.buf, entry->name, entry->name_len);
    struct entry *entry_ = find_entry(s, dict);
    see(s, entry_);
  }
}

void
disassemble(struct state *s)
{
  printf("-------- Words ------------------------------------------\n");
  disassemble_dict(s, &s->dict);
  printf("--------Macros-------------------------------------------\n");
  disassemble_dict(s, &s->macro_dict);
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
  define_primitive_extension(state, "words", OP_WORDS, words);
  define_primitive_extension(state, "see", OP_SEE, see_func);
  define_primitive_extension(state, "disassemble", OP_DISASSEMBLE_DICT, disassemble);
}
