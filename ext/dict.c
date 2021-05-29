// The author disclaims copyright to this source code.
#include <cf-stdio.h>
#include <string.h>

#include <colorforth.h>

void
dump_words(struct state *s, struct dictionary *dict)
{
  for (struct entry *entry = dict->latest; entry != dict->entries - 1; entry--)
  {
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

void
see(struct state *s, struct entry *entry)
{
  if (entry)
  {
    char display_next_sc = 0;
    cf_printf(s, ":%.*s ", (int)entry->name_len, entry->name);
    for (size_t i = 0, done = 0; !done; i++)
    {
      struct entry *entry_ = (struct entry*) entry->code[i].this;
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
          cf_printf(s, "%s ", entry_->name);
          break;
        }
        case OP_TAIL_CALL:
        {
          cf_printf(s, "%s¬ ", entry->name);
          break;
        }

        case OP_TICK_NUMBER:
        {
          cf_printf(s, "'%s ", entry_->name);
          break;
        }

        case OP_NUMBER:
        {
          cf_printf(s, "%"CELL_FMT" ", entry->code[i].this);
          break;
        }

        default:
        {
          cf_printf(s, "%s ", primitive_map[entry->code[i].opcode].name);
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
  cf_printf(s, "-------- Words ------------------------------------------\n");
  disassemble_dict(s, &s->dict);
  cf_printf(s, "--------Inlined-------------------------------------------\n");
  disassemble_dict(s, &s->inlined_dict);
  cf_printf(s, "---------------------------------------------------------\n");
}

void
see_func(struct state *s)
{
    struct entry *entry_ = (struct entry*)pop(s->stack);
    see(s, entry_);
}

void
commonroom(struct state *s)
{
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
fullroom(struct state *s)
{
  cf_printf(s, "-------- ROOM -------------------------------------------\n");
  cf_printf(s, "Cell size is %u bytes / %u bits\n", (unsigned int) sizeof(cell), (unsigned int) sizeof(cell) * 8);

  cf_printf(s, "The circular stack size is %d cells\n", s->stack->lim + 1);
  cf_printf(s, "The circular return stack size is %d cells\n", s->r_stack->lim + 1);
  cf_printf(s, "Maximm length of a word is %d chars\n", TIB_SIZE);

  cf_printf(s, "--\n");

  commonroom(s);
}

void
room(struct state *s)
{
  cf_printf(s, "-------- ROOM -------------------------------------------\n");
  commonroom(s);
}

void
shortroom(struct state *s)
{
  const unsigned int defined = s->dict.latest - s->dict.entries + 1;
  const unsigned int defined_inlined = s->inlined_dict.latest - s->inlined_dict.entries + 1;
  const unsigned int used = (char *)s->here - (char *)s->heap;
  cf_printf(s, "ROOM: Entries: %u / %d (%u%%) | Inlined: %u / %d (%u%%) | Heap (bytes): %u / %d (%u%%)\n",
            defined, DICT_SIZE, (defined*100/DICT_SIZE),
            defined_inlined, INLINED_DICT_SIZE, (defined_inlined*100/INLINED_DICT_SIZE),
            used, HEAP_SIZE,(used*100/HEAP_SIZE));
}

void
is (struct state *s)
{
  struct entry *entry_to = (struct entry*)pop(s->stack);
  struct entry *entry_from = (struct entry*)pop(s->stack);

  entry_from->code = entry_to->code;
}

void
init_dict_utils(struct state *state)
{
  define_primitive_extension(state, "words", OP_WORDS, words);
  define_primitive_extension(state, "see", OP_SEE, see_func);
  define_primitive_extension(state, "disassemble", OP_DISASSEMBLE_DICT, disassemble);
  define_primitive_extension(state, "room", OP_ROOM, room);
  define_primitive_extension(state, "fullroom", OP_FULLROOM, fullroom);
  define_primitive_extension(state, "shortroom", OP_SHORTROOM, shortroom);
  define_primitive_extension(state, "is", OP_IS, is);
}
