// The author disclaims copyright to this source code.
#include <string.h>

#include "colorforth.h"
#include "cf-stdio.h"

static char initialized = 0;

void
see(struct state *s, struct entry *entry)
{
#ifdef __KEEP_ENTRY_NAMES
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
          struct entry *entry_by_fn = find_entry_by_fn(&s->dict, &entry->code[i]);
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
          struct entry *entry_by_code = find_entry_by_code(&s->dict, &entry->code[i]);
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

      display_next_sc = (entry->code[i].opcode == OP_IF || entry->code[i].opcode == OP_IF_NOT) ? 1 : 0;
    }
    cf_printf(s, "\n");
  }
  else
  {
    unknow_word(s);
  }
#else
  cf_printf(s, "Hashed names. Nothing to see!\n");
#endif
}

void
disassemble_dict(struct state *s, struct dictionary *dict)
{
#ifdef __KEEP_ENTRY_NAMES
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

  for (size_t i = 0, done = 0; !done; i++)
  {
    switch(entry->code[i].opcode)
    {
      case OP_RETURN:
      {
        done = 1;
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
  }
}

#ifdef __CHECK_DICT
#ifdef __KEEP_ENTRY_NAMES
unsigned int
check_entry(struct state *s, struct entry *check_entry)
{
  unsigned int clash_found = 0;

  for (struct entry *entry = s->dict.latest; entry != s->dict.entries - 1; entry--)
  {
    if (entry->name_len == 0) continue;

    if (entry->name_hash == check_entry->name_hash &&
        entry->name_len == check_entry->name_len &&
        memcmp(entry->name, check_entry->name, entry->name_len))
    {
      clash_found += 1;
      cf_printf(s, "'%s'  0x%lX clash with '%s'  0x%lX\n", entry->name, entry->name_hash, check_entry->name, check_entry->name_hash);
    }
  }

  return clash_found;
}

void
display_clash_found(struct state *s, unsigned int clash_found)
{
  if (clash_found)
  {
    cf_printf(s, "\nWARNING: %d Clash found in the dictionary\n", clash_found);
  }
  else
  {
    cf_printf(s, "No clash found in the dictionary\n");
  }
}

void
check_dict(struct state *s)
{
  unsigned int clash_found = 0;

  for (struct entry *entry = s->dict.latest; entry != s->dict.entries - 1; entry--)
  {
    if (entry->name_len == 0) continue;

    if (check_entry(s, entry)) clash_found += 1;
  }

  clash_found = clash_found / 2;

  display_clash_found(s, clash_found);
}
#endif /* __KEEP_ENTRY_NAMES */
#endif /* __CHECK_DICT */

void
require_dict_fn(struct state *state)
{
  if (initialized) return;

  define_primitive_extension(state, SEE_HASH,           ENTRY_NAME("see"), see_fn);
  define_primitive_extension(state, DISASSEMBLE_HASH,   ENTRY_NAME("disassemble"), disassemble);
  define_primitive_extension(state, FULLROOM_HASH,      ENTRY_NAME("fullroom"), fullroom);
  define_primitive_extension(state, ENTRY__PATCH_HASH, ENTRY_NAME("entry/patch"), patch_entry);

#ifdef __CHECK_DICT
#ifdef __KEEP_ENTRY_NAMES
  define_primitive_extension(state, CHECK_DICT_HASH, ENTRY_NAME("check-dict"), check_dict);
#endif /* __KEEP_ENTRY_NAMES */
#endif /* __CHECK_DICT */

  initialized = 1;
}
