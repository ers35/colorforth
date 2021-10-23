// The author disclaims copyright to this source code.
#include <colorforth.h>
#include <cf-stdio.h>

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

struct code*
echo_set_fn(struct state *s, struct code *pc)
{
  s->echo_on = pop(s->stack);
  return pc;
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

struct code*
file_size_fn(struct state *s, struct code *pc)
{
  char *filename = CFSTRING2C(pop(s->stack));

  int size = file_size(filename);

  if (size == -1) cf_printf(s, "Unable to read '%s'\n", filename);

  push(s->stack, file_size(filename));
  return pc;
}

struct code*
load_file_fn(struct state *s, struct code *pc)
{
  char *filename = CFSTRING2C(pop(s->stack));
  char *buf = (char *) (pop(s->stack));
  int size = file_size(filename);

  if (size == -1)
  {
    cf_printf(s, "Unable to read '%s'\n", filename);
    push(s->stack, -1);
    return pc;
  }

  FILE *fp = fopen(filename, "r");

  if (!fp)
  {
    cf_printf(s, "Unable to read '%s'\n", filename);
    push(s->stack, -1);
    return pc;
  }

  int len = fread(buf + sizeof(cell), 1, size, fp);
  if (len != size)
  {
    free(buf);
    cf_printf(s, "Unable to read '%s'\n", filename);
    push(s->stack, -1);
    return pc;
  }

  cell *cbuf = (cell *)buf;
  *cbuf = (cell)size;

  fclose(fp);

  return pc;
}

struct code*
save_file_fn(struct state *s, struct code *pc)
{
  char *filename = CFSTRING2C(pop(s->stack));
  cell value = pop(s->stack);
  char *str = CFSTRING2C(value);

  FILE *fp = fopen(filename, "w");

  if (!fp)
  {
    cf_printf(s, "Unable to read '%s'\n", filename);
    push(s->stack, -1);
    return pc;
  }

  fwrite(str, 1, *(cell *)value, fp);

  fclose(fp);

  return pc;
}

struct code*
included_file_fn(struct state *s, struct code *pc)
{
  char *filename = CFSTRING2C(pop(s->stack));
  parse_from_file(s, filename);

  return pc;
}

void
init_io_utils(struct state *state)
{
  define_primitive(state, "echo!", echo_set_fn);
  define_primitive(state, "file-size", file_size_fn);
  define_primitive(state, "load", load_file_fn);
  define_primitive(state, "save", save_file_fn);
  define_primitive(state, "included", included_file_fn);
}
