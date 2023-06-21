// The author disclaims copyright to this source code.

void
cf_print_cell(struct state *s, cell cell)
{
  switch (s->base)
  {
    case 16:
    {
      cf_printf(s, "$%lX", cell);
      break;
    }
    case 2:
    {
      cf_putchar(s, '#');
      if (cell == 0)
      {
        cf_printf(s, "0 ");
      } else {
        int i = CHAR_BIT * sizeof cell;
        char output = 0;
        while(i--)
        {
          const char n = ((cell >> i) & 1);
          if (n) output = 1;
          if (output) cf_putchar(s, '0' + n);
        }
      }
      break;
    }
    default:
    {
      cf_printf(s, CELL_FMT, cell);
      break;
    }
  }
}

void
quit(struct state *s)
{
  s->done = 1;
  echo_color(s, ' ', COLOR_CLEAR);
}

void
cf_fatal_error(struct state *s, char id)
{
  cf_printf(s, "E%d\n", id);
  if (s)
  {
    echo_color(s, ' ', COLOR_CLEAR);
  }
  cf_fflush();
  reset_terminal();
  exit(1);
}

void *
cf_calloc(struct state *s, size_t nmemb, size_t size, unsigned char id)
{
  void *ptr = calloc(nmemb, size);
  if (!ptr) cf_fatal_error(s, id);

  return ptr;
}


#define FNV_32_PRIME ((hash_t)0x01000193)

hash_t
hash(char *str)
{
  if (!str) return 0;

  unsigned char *s = (unsigned char *)str;
  hash_t hval = 0;

  while (*s) {
    hval ^= (hash_t)*s++;

    /* multiply by the 32 bit FNV magic prime mod 2^32 */
    hval *= FNV_32_PRIME;
  }

  return hval;
}

static void
dot_s(struct state *s, struct stack *stack)
{
  cf_printf(s, "[%d] ", stack->sp + 1);
  for (int i = 0; i < stack->sp + 1; i++)
  {
    cf_print_cell(s, stack->cells[i]);
    cf_printf(s, " ");
  }
  cf_printf(s, "<tos\n");
}

void
clear_tib (struct state *s){
  for(size_t i = 0; i < s->tib.len; i++)
  {
    s->tib.buf[i] = 0;
  }
  s->tib.len = 0;
}

#ifdef __KEEP_ENTRY_NAMES
void
dump_words(struct state *s, struct dictionary *dict)
{
  //for (struct entry *entry = dict->latest; entry != dict->entries - 1; entry--)
  for (int i = dict->latest; i > 0; i--)
  {
    struct entry entry = dict->entries[i];

    if (entry.name == NULL) continue;

    s->tib.len = entry.name_len;
    memcpy(s->tib.buf, entry.name, entry.name_len);
    s->tib.buf[s->tib.len] = '\0';
    cf_printf(s, "%s ", s->tib.buf);
  }
}
#else
void
dump_words(struct state *s, struct dictionary *dict __attribute__((unused)))
{
  cf_printf(s, "...\n");
}
#endif


void
words(struct state *s)
{
  dump_words(s, &s->dict);
  cf_printf(s, "\n");
}

cell
find_entry(struct state *s, struct dictionary *dict)
{
  s->tib.buf[s->tib.len] = '\0';
  const hash_t tib_opcode = hash(s->tib.buf);

  for (int i = dict->latest; i >= 0; i--)
  {
    if (dict->entries[i].opcode == tib_opcode)
    {
      return i;
    }
  }
  return -1;
}

cell
find_entry_by_hash(struct dictionary *dict, hash_t opcode)
{
  for (int i = dict->latest; i >= 0; i--)
  {
    if (dict->entries[i].opcode == opcode)
    {
      return i;
    }
  }
  return -1;
}

// struct entry*
// find_entry_by_code(struct dictionary *dict, struct code *code)
// {
//   for (struct entry *entry = dict->latest; entry != dict->entries - 1; entry--)
//   {
//     if (entry->code->opcode == code->opcode)
//     {
//       return entry;
//     }
//   }
//   return NULL;
// }
//
// struct entry*
// find_entry_by_fn(struct dictionary *dict, struct code *code)
// {
//   for (struct entry *entry = dict->latest; entry != dict->entries - 1; entry--)
//   {
//     if (entry->code->value == code->value)
//     {
//       return entry;
//     }
//   }
//   return NULL;
// }

void
print_tib(struct state *s)
{
  for(size_t i = 0; i < s->tib.len; i++)
  {
    cf_putchar(s, s->tib.buf[i]);
  }
}

void
unknow_word (struct state *s)
{
  print_tib(s);
  cf_printf(s, "?\n");

  if (s->str_stream || s->file_stream)
  {
    cf_fatal_error(s, -1);
  }
}

static bool
tib_to_number(struct state *s, cell *n)
{
  char *endptr;
  s->tib.buf[s->tib.len] = '\0';
  *n = strtol(s->tib.buf, &endptr, s->base);
  return *s->tib.buf != 0 && *endptr == 0;
}
