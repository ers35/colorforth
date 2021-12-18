// The author disclaims copyright to this source code.
#include "colorforth.h"
#include "cf-stdio.h"

void
parse_from_file(struct state *s, char *filename)
{
  s->tib.len = 0;
  FILE *old_stream = s->file_stream;
  s->file_stream = fopen(filename, "r");
  if (!s->file_stream)
  {
    s->file_stream = old_stream;
    cf_printf(s, "Unable to read '%s'\n", filename);
    quit(s, 1);
    return;
  }

  int c;
  while((c = cf_getchar(s)) != CF_EOF && !s->done)
  {
    parse_colorforth(s, c);
  }

  fclose(s->file_stream);
  s->file_stream = old_stream;

  parse_space(s);
}

void
echo_set(struct state *s)
{
  s->echo_on = pop(s->stack);
}

int
file_size(char *filename)
{
  FILE *fp = fopen(filename, "r");

  if (!fp) return -1;

  fseek(fp, 0L, SEEK_END);
  int size=ftell(fp);
  fclose(fp);

  return size;
}

void
file_size_fn(struct state *s)
{
  char *filename = CFSTRING2C(pop(s->stack));

  int size = file_size(filename);

  if (size == -1) cf_printf(s, "Unable to read '%s'\n", filename);

  push(s->stack, file_size(filename));
}

void
load_file(struct state *s)
{
  char *filename = CFSTRING2C(pop(s->stack));
  char *buf = (char *) (pop(s->stack));
  int size = file_size(filename);

  if (size == -1)
  {
    cf_printf(s, "Unable to read '%s'\n", filename);
    push(s->stack, -1);
    return;
  }

  FILE *fp = fopen(filename, "r");

  if (!fp)
  {
    cf_printf(s, "Unable to read '%s'\n", filename);
    push(s->stack, -1);
    return;
  }

  int len = fread(buf + sizeof(cell), 1, size, fp);
  if (len != size)
  {
    free(buf);
    cf_printf(s, "Unable to read '%s'\n", filename);
    push(s->stack, -1);
    return;
  }

  cell *cbuf = (cell *)buf;
  *cbuf = (cell)size;

  fclose(fp);
}

void
save_file(struct state *s)
{
  char *filename = CFSTRING2C(pop(s->stack));
  cell value = pop(s->stack);
  char *str = CFSTRING2C(value);

  FILE *fp = fopen(filename, "w");

  if (!fp)
  {
    cf_printf(s, "Unable to read '%s'\n", filename);
    push(s->stack, -1);
    return;
  }

  fwrite(str, 1, *(cell *)value, fp);

  fclose(fp);
}

void
included_file(struct state *s)
{
  char *filename = CFSTRING2C(pop(s->stack));
  parse_from_file(s, filename);
}

void
init_io_utils(struct state *state)
{
  define_primitive_extension(state, ECHO_STORE_HASH,     ENTRY_NAME("echo!"), echo_set);
  define_primitive_extension(state, FILE_SUBSIZE_HASH,   ENTRY_NAME("file-size"), file_size_fn);
  define_primitive_extension(state, FILE_LOAD_HASH,      ENTRY_NAME("load"), load_file);
  define_primitive_extension(state, FILE_SAVE_HASH,      ENTRY_NAME("save"), save_file);
  define_primitive_extension(state, INCLUDED_HASH,       ENTRY_NAME("included"), included_file);
}
