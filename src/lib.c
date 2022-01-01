// The author disclaims copyright to this source code.
#include "colorforth.h"
#include "cf-stdio.h"

void
set_base (struct state *s)
{

  s->base = pop(s->stack);
}

void
fetch_base (struct state *s)
{

  push(s->stack, s->base);
}

void
is (struct state *s)
{
  struct entry *entry_from = (struct entry*)pop(s->stack);
  struct entry *entry_to = (struct entry*)pop(s->stack);

  entry_from->code = entry_to->code;
}

void
hide_entry (struct state *s)
{
  struct entry *entry = (struct entry*)pop(s->stack);

  entry->name_hash = 0;
#ifdef __KEEP_ENTRY_NAMES
  free(entry->name);
  entry->name = NULL;
  entry->name_len = 0;
#endif
}

void
room (struct state *s)
{
  const unsigned int defined = s->dict.latest - s->dict.entries + 1;
  const unsigned int defined_inlined = s->inlined_dict.latest - s->inlined_dict.entries + 1;
  const unsigned int used = (char *)s->here - (char *)s->heap;
  const unsigned int cell_bytes = sizeof(cell);
  const unsigned int cell_bits = sizeof(cell) * 8;

  cf_printf(s, "Entries: %u/%d %u%%|Inlined: %u/%d %u%%|Heap (bytes): %u/%d %u%%|Cell: %u bytes, %u bits\n",
            defined, DICT_SIZE, (defined*100/DICT_SIZE),
            defined_inlined, INLINED_DICT_SIZE, (defined_inlined*100/INLINED_DICT_SIZE),
            used, HEAP_SIZE,(used*100/HEAP_SIZE), cell_bytes, cell_bits);

  push(s->stack, cell_bits);
  push(s->stack, cell_bytes);
  push(s->stack, HEAP_SIZE);
  push(s->stack, used);
  push(s->stack, INLINED_DICT_SIZE);
  push(s->stack, defined_inlined);
  push(s->stack, DICT_SIZE);
  push(s->stack, defined);
}

void
init_lib(struct state *state)
{
  define_primitive_extension(state, BASE_STORE_HASH,     ENTRY_NAME("base!"), set_base);
  define_primitive_extension(state, BASE_LOAD_HASH,      ENTRY_NAME("base@"), fetch_base);
  define_primitive_extension(state, ENTRY_IS_HASH,       ENTRY_NAME("is"), is);
  define_primitive_extension(state, ENTRY_HIDE_HASH,     ENTRY_NAME("entry/hide"), hide_entry);
  define_primitive_extension(state, ROOM_HASH,           ENTRY_NAME("room"), room);
}
