// The author disclaims copyright to this source code.
#include "colorforth.h"
#include "cf-stdio.h"

extern hash_t hash(char *str);

void
hash_fn (struct state *s)
{
  char *input = CFSTRING2C(pop(s->stack));
  push(s->stack, hash(input));
}

void
get_base_addr (struct state *s)
{

  push(s->stack, (cell) &s->base);
}

void
is (struct state *s)
{
  struct entry *entry_from = (struct entry*)pop(s->stack);
  struct entry *entry_to = (struct entry*)pop(s->stack);

  entry_from->code = entry_to->code;
}

void
c_is (struct state *s)
{
  struct entry *entry_from = (struct entry*)pop(s->stack);
  struct code *code_to = (struct code*)pop(s->stack);

  entry_from->code = code_to;
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
  const unsigned int used = (char *)s->here - (char *)s->heap;
  const unsigned int cell_bytes = sizeof(cell);
  const unsigned int cell_bits = sizeof(cell) * 8;

  cf_printf(s, "Entries: %u/%d %u%%|Heap (bytes): %u/%d %u%%|Cell: %u bytes, %u bits\n",
            defined, DICT_SIZE, (defined*100/DICT_SIZE),
            used, HEAP_SIZE,(used*100/HEAP_SIZE), cell_bytes, cell_bits);

  push(s->stack, cell_bits);
  push(s->stack, cell_bytes);
  push(s->stack, HEAP_SIZE);
  push(s->stack, used);
  push(s->stack, DICT_SIZE);
  push(s->stack, defined);
}

void
drop_room (struct state *s)
{
  for (int i = 0; i < 6; i++)
  {
    pop(s->stack);
  }
}

void
init_lib(struct state *state)
{
  define_primitive_extension(state, HASH_HASH,           ENTRY_NAME("hash"), hash_fn);
  define_primitive_extension(state, BASE_HASH,           ENTRY_NAME("base"), get_base_addr);
  define_primitive_extension(state, ENTRY_IS_HASH,       ENTRY_NAME("is"), is);
  define_primitive_extension(state, ENTRY_C_IS_HASH,     ENTRY_NAME("c>is"), c_is);
  define_primitive_extension(state, ENTRY_HIDE_HASH,     ENTRY_NAME("entry/hide"), hide_entry);
  define_primitive_extension(state, ROOM_HASH,           ENTRY_NAME("room"), room);
  define_primitive_extension(state, DROP_ROOM_HASH,      ENTRY_NAME("drop-room"), drop_room);
}
