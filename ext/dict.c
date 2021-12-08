// The author disclaims copyright to this source code.
#include <string.h>

#include "colorforth.h"
#include "cf-stdio.h"

void
see(struct state *s, struct entry *entry)
{
#ifndef __HASH_NAMES
  if (entry)
  {
    char display_next_sc = 0;

    if (!entry->code)
    {
      cf_printf(s, "<empty>\n");
      return;
    }

    cf_printf(s, ":%s ", entry->name == NULL ? "???" : entry->name);
    for (size_t i = 0, done = 0; !done; i++)
    {
      struct entry *entry_ = (struct entry*) entry->code[i].value;
      switch(entry->code[i].opcode)
      {
        case OP_RETURN:
        {
          cf_printf(s, "; ");
          if (!display_next_sc)
          {
            done = 1;
          }
          break;
        }

        case OP_CALL:
        {
          cf_printf(s, "%s ", entry_->name == NULL ? "???" : entry_->name);
          break;
        }
        case OP_TAIL_CALL:
        {
          cf_printf(s, "%s¬ ", entry->name == NULL ? "???" : entry_->name);
          break;
        }

        case OP_TICK_NUMBER:
        {
          cf_printf(s, "`%s ", entry_->name);
          break;
        }

        case OP_NUMBER:
        {
          cf_print_cell(s, entry->code[i].value);
          break;
        }

        case OP_FUNCTION_CALL:
        {
          struct entry *entry_by_fn = find_entry_by_fn(s, &s->dict, &entry->code[i]);
          if (entry_by_fn)
          {
            cf_printf(s, "%s ", entry_by_fn->name);
          }
          else
          {
            printf("\nError: Entry not found\n");
          }
          break;
        }

        default:
        {
          struct entry *entry_by_code = find_entry_by_code(s, &s->inlined_dict, &entry->code[i]);
          if (!entry_by_code)
          {
            entry_by_code = find_entry_by_code(s, &s->dict, &entry->code[i]);
          }

          if (entry_by_code)
          {
            cf_printf(s, "%s ", entry_by_code->name);
          }
          else
          {
            printf("\nError: Entry not found\n");
          }
        }
      }

      display_next_sc = (entry->code[i].opcode == OP_WHEN || entry->code[i].opcode == OP_UNLESS) ? 1 : 0;
    }
    cf_printf(s, "\n");
  }
  else
  {
    unknow_word(s, "with");
  }
#else
  cf_printf(s, "Hashed names. Nothing to see!\n");
#endif
}

void
disassemble_dict(struct state *s, struct dictionary *dict)
{
#ifndef __HASH_NAMES
  for (struct entry *entry = dict->latest; entry != dict->entries - 1; entry--)
  {
    if (entry->name_len == 0) continue;

    s->tib.len = entry->name_len;
    memcpy(s->tib.buf, entry->name, entry->name_len);
    struct entry *entry_ = find_entry(s, dict);
    see(s, entry_);
  }
#else
  cf_printf(s, "Hashed names. Nothing to see!\n");
#endif
}

void
disassemble(struct state *s)
{
  cf_printf(s, "-------- Words ------------------------------------------\n");
  disassemble_dict(s, &s->dict);
  cf_printf(s, "--------Inlined-------------------------------------------\n");
  disassemble_dict(s, &s->inlined_dict);
  cf_printf(s, "---------------------------------------------------------\n");
}

void
see_fn(struct state *s)
{
    struct entry *entry_ = (struct entry*)pop(s->stack);
    see(s, entry_);
}

void
fullroom(struct state *s)
{
  cf_printf(s, "-------- ROOM -------------------------------------------\n");
  cf_printf(s, "Cell size is %u bytes / %u bits\n", (unsigned int) sizeof(cell), (unsigned int) sizeof(cell) * 8);

  cf_printf(s, "The circular stack size is %d cells\n", s->stack->lim + 1);
  cf_printf(s, "The circular return stack size is %d cells\n", s->r_stack->lim + 1);
  cf_printf(s, "Maximm length of a word is %d chars\n", TIB_SIZE);

  cf_printf(s, "--\n");

  const unsigned int defined = s->dict.latest - s->dict.entries + 1;
  cf_printf(s, "There is %u / %d (%u%%) entries defined in the dictionary\n", defined, DICT_SIZE,
         (defined*100/DICT_SIZE));

  const unsigned int defined_inlined = s->inlined_dict.latest - s->inlined_dict.entries + 1;
  cf_printf(s, "There is %u / %d (%u%%) inlined defined in the inlined dictionary\n", defined_inlined, INLINED_DICT_SIZE,
         (defined_inlined*100/INLINED_DICT_SIZE));

  const unsigned int used = (char *)s->here - (char *)s->heap;
  cf_printf(s, "There is %u / %d (%u%%) bytes used on the heap\n", used, HEAP_SIZE,
         (used*100/HEAP_SIZE));
  cf_printf(s, "---------------------------------------------------------\n");
}

void
patch_entry (struct state *s)
{
  struct entry *entry = (struct entry*)pop(s->stack);
  struct entry *to = (struct entry*)pop(s->stack);
  struct entry *from = (struct entry*)pop(s->stack);

  char is_test = 0;

  for (size_t i = 0, done = 0; !done; i++)
  {
    switch(entry->code[i].opcode)
    {
      case OP_RETURN:
      {
        if (!is_test)
        {
          done = 1;
        }
        break;
      }

      case OP_CALL:
      {
        if ((struct entry *)entry->code[i].value == from)
        {
          entry->code[i].value = (cell) to;
        }
        break;
      }

      default:
      {
      }
    }

    is_test = (entry->code[i].opcode == OP_WHEN || entry->code[i].opcode == OP_UNLESS) ? 1 : 0;
  }
}

void
init_dict_utils(struct state *state)
{
  define_primitive_extension(state, "see", see_fn);
  define_primitive_extension(state, "disassemble", disassemble);
  define_primitive_extension(state, "fullroom", fullroom);
  define_primitive_extension(state, "entry/patch", patch_entry);
}
