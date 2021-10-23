// The author disclaims copyright to this source code.
#include <string.h>

#include <colorforth.h>
#include <cf-stdio.h>

extern struct code* return_fn (struct state *s, struct code *pc);
extern struct code* call_fn (struct state *s, struct code *pc);
extern struct code* tail_call_fn (struct state *s, struct code *pc);
extern struct code* number_fn (struct state *s, struct code *pc);
extern struct code* tick_number_fn (struct state *s, struct code *pc);
extern struct code* when_fn (struct state *s, struct code *pc);
extern struct code* unless_fn (struct state *s, struct code *pc);

void
see(struct state *s, struct entry *entry)
{
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
      struct entry *entry_ = (struct entry*) entry->code[i].this;

      if (entry->code[i].fn == return_fn)
      {
        cf_printf(s, "; ");
        if (!display_next_sc)
        {
          done = 1;
        }
      }
      else if (entry->code[i].fn == call_fn)
      {
        cf_printf(s, "%s ", entry_->name == NULL ? "???" : entry_->name);
      }
      else if (entry->code[i].fn == tail_call_fn)
      {
        cf_printf(s, "%s¬ ", entry->name == NULL ? "???" : entry_->name);
      }
      else if (entry->code[i].fn == tick_number_fn)
      {
        cf_printf(s, "`%s ", entry_->name);
      }
      else if (entry->code[i].fn == number_fn)
      {
        cf_printf(s, "%"CELL_FMT" ", entry->code[i].this);
      }
      else if (entry_)
      {
        cf_printf(s, "%s ", entry_->name);
      }

      display_next_sc = (entry->code[i].fn == when_fn || entry->code[i].fn == unless_fn) ? 1 : 0;
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
    if (entry->name_len == 0) continue;

    s->tib.len = entry->name_len;
    memcpy(s->tib.buf, entry->name, entry->name_len);
    struct entry *entry_ = find_entry(s, dict);
    see(s, entry_);
  }
}

struct code*
disassemble_fn(struct state *s, struct code *pc)
{
  cf_printf(s, "-------- Words ------------------------------------------\n");
  disassemble_dict(s, &s->dict);
  cf_printf(s, "--------Inlined-------------------------------------------\n");
  disassemble_dict(s, &s->inlined_dict);
  cf_printf(s, "---------------------------------------------------------\n");
  return pc;
}

struct code*
see_fn(struct state *s, struct code *pc)
{
    struct entry *entry_ = (struct entry*)pop(s->stack);
    see(s, entry_);
    return pc;
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

struct code*
fullroom_fn(struct state *s, struct code *pc)
{
  cf_printf(s, "-------- ROOM -------------------------------------------\n");
  cf_printf(s, "Cell size is %u bytes / %u bits\n", (unsigned int) sizeof(cell), (unsigned int) sizeof(cell) * 8);

  cf_printf(s, "The circular stack size is %d cells\n", s->stack->lim + 1);
  cf_printf(s, "The circular return stack size is %d cells\n", s->r_stack->lim + 1);
  cf_printf(s, "Maximum length of a word is %d chars\n", TIB_SIZE);

  cf_printf(s, "--\n");

  commonroom(s);
  return pc;
}

struct code*
room_fn(struct state *s, struct code *pc)
{
  cf_printf(s, "-------- ROOM -------------------------------------------\n");
  commonroom(s);
  return pc;
}

struct code*
shortroom_fn(struct state *s, struct code *pc)
{
  const unsigned int defined = s->dict.latest - s->dict.entries + 1;
  const unsigned int defined_inlined = s->inlined_dict.latest - s->inlined_dict.entries + 1;
  const unsigned int used = (char *)s->here - (char *)s->heap;
  cf_printf(s, "ROOM: Entries: %u / %d (%u%%) | Inlined: %u / %d (%u%%) | Heap (bytes): %u / %d (%u%%)\n",
            defined, DICT_SIZE, (defined*100/DICT_SIZE),
            defined_inlined, INLINED_DICT_SIZE, (defined_inlined*100/INLINED_DICT_SIZE),
            used, HEAP_SIZE,(used*100/HEAP_SIZE));
  return pc;
}

struct code*
is_fn (struct state *s, struct code *pc)
{
  struct entry *entry_to = (struct entry*)pop(s->stack);
  struct entry *entry_from = (struct entry*)pop(s->stack);

  entry_from->code = entry_to->code;
  return pc;
}

struct code*
hide_entry_fn (struct state *s, struct code *pc)
{
  struct entry *entry = (struct entry*)pop(s->stack);

  free(entry->name);
  entry->name = NULL;
  entry->name_len = 0;
  return pc;
}

void
init_dict_utils(struct state *state)
{
  define_primitive(state, "see", see_fn);
  define_primitive(state, "disassemble", disassemble_fn);
  define_primitive(state, "room", room_fn);
  define_primitive(state, "fullroom", fullroom_fn);
  define_primitive(state, "shortroom", shortroom_fn);
  define_primitive(state, "is", is_fn);
  define_primitive(state, "hide-entry", hide_entry_fn);
}
