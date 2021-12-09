// The author disclaims copyright to this source code.
#include "colorforth.h"
#include "cf-stdio.h"

#ifdef __EXTENDED_MATH

void
init_fstack(struct fstack *stack, int len)
{
  stack->cells = cf_calloc(NULL, len, sizeof(number_t), FSTACK_ERROR);
  stack->sp = 0;
  stack->lim = len - 1;
}

void
fsize(struct state *state)
{
  push(state->stack, sizeof(number_t));
}

static void
fdot_s(struct state *state)
{
  for (int i = 0, p = state->fstack.sp + 1; i <= state->fstack.lim; i++, p++)
  {
    if (p > state->fstack.lim) p = 0;

    if (i <= state->fstack.lim - 8) continue;

    cf_printf(state, "%lf ", state->fstack.cells[p]);
  }
  cf_printf(state, " <ftos\n");
}

void
fpush(struct fstack *stack, const number_t n)
{
  if (stack->sp == stack->lim)
  {
    stack->sp = 0;
  }
  else
  {
    stack->sp += 1;
  }
  stack->cells[stack->sp] = n;
}

number_t
fpop(struct fstack *stack)
{
  const number_t n = stack->cells[stack->sp];
  if (stack->sp == 0)
  {
    stack->sp = stack->lim;
  }
  else
  {
    stack->sp -= 1;
  }

  return n;
}


void
define_float(struct state *s)
{
  char *ptr;
  const number_t n = strtod(s->tib.buf, &ptr);
  fpush(&s->fstack, n);
}

void
print_ftos(struct state *s)
{
  cf_printf(s, "%lf ", fpop(&s->fstack));
  cf_fflush();
}

void
fdrop(struct state *s)
{
  fpop(&s->fstack);
}

void
fdup(struct state *s)
{
  fpush(&s->fstack, s->fstack.cells[s->fstack.sp]);
}

void
fswap(struct state *s)
{
  const number_t n1 = fpop(&s->fstack);
  const number_t n2 = fpop(&s->fstack);
  fpush(&s->fstack, n1);
  fpush(&s->fstack, n2);
}

void
fover(struct state *s)
{
  const number_t n1 = fpop(&s->fstack);
  const number_t n2 = fpop(&s->fstack);
  fpush(&s->fstack, n2);
  fpush(&s->fstack, n1);
  fpush(&s->fstack, n2);
}

void
frot(struct state *s)
{
  const cell n3 = fpop(&s->fstack);
  const cell n2 = fpop(&s->fstack);
  const cell n1 = fpop(&s->fstack);
  fpush(&s->fstack, n2);
  fpush(&s->fstack, n3);
  fpush(&s->fstack, n1);
}

void
fminus_rot(struct state *s)
{
  const cell n3 = fpop(&s->fstack);
  const cell n2 = fpop(&s->fstack);
  const cell n1 = fpop(&s->fstack);
  fpush(&s->fstack, n3);
  fpush(&s->fstack, n1);
  fpush(&s->fstack, n2);
}

void
fnip(struct state *s)
{
  const number_t n2 = fpop(&s->fstack);
  fpop(&s->fstack);
  fpush(&s->fstack, n2);
}

void
fadd(struct state *s)
{
  const number_t n1 = fpop(&s->fstack);
  const number_t n2 = fpop(&s->fstack);

  fpush(&s->fstack, n1 + n2);
}

void
fsub(struct state *s)
{
  const number_t n1 = fpop(&s->fstack);
  const number_t n2 = fpop(&s->fstack);

  fpush(&s->fstack, n2 - n1);
}

void
fmul(struct state *s)
{
  const number_t n1 = fpop(&s->fstack);
  const number_t n2 = fpop(&s->fstack);

  fpush(&s->fstack, n1 * n2);
}

void
fdiv(struct state *s)
{
  const number_t n1 = fpop(&s->fstack);
  const number_t n2 = fpop(&s->fstack);

  fpush(&s->fstack, n2 / n1);
}

void
fsup(struct state *s)
{
  const number_t n1 = fpop(&s->fstack);
  const number_t n2 = fpop(&s->fstack);

  push(s->stack, n2 > n1);
}

void
finf(struct state *s)
{
  const number_t n1 = fpop(&s->fstack);
  const number_t n2 = fpop(&s->fstack);

  push(s->stack, n2 < n1);
}

void
fload(struct state *s)
{
  fpush(&s->fstack, *(number_t*)pop(s->stack));
}

void
fstore(struct state *s)
{
  number_t *ptr = (number_t*)pop(s->stack);
  number_t n = fpop(&s->fstack);
  *ptr = n;
}

void
fcompile_literal(struct state *s)
{
  number_t *ptr = cf_calloc(s, 1, sizeof(number_t), F_LITERAL_ERROR);
  *ptr = fpop(&s->fstack);

  struct code *code =  (struct code *)s->here;
  code->opcode = OP_NUMBER;
  code->value = (cell)ptr;
  s->here = (struct code *)s->here + 1;
}

void
init_ext_math_utils(struct state *state)
{
  init_fstack(&state->fstack, FSTACK_SIZE);
  define_prefix('$', define_float,  COLOR_YELLOW,     0);

  define_primitive_extension(state, "float", fsize);

  define_primitive_extension(state, "f.s", fdot_s);
  define_primitive_extension(state, "f.", print_ftos);

  define_primitive_extension(state, "fdrop", fdrop);
  define_primitive_extension(state, "fdup", fdup);
  define_primitive_extension(state, "fswap", fswap);
  define_primitive_extension(state, "fover", fover);
  define_primitive_extension(state, "frot", frot);
  define_primitive_extension(state, "f-rot", fminus_rot);
  define_primitive_extension(state, "fnip", fnip);

  define_primitive_extension(state, "f+", fadd);
  define_primitive_extension(state, "f-", fsub);
  define_primitive_extension(state, "f*", fmul);
  define_primitive_extension(state, "f/", fdiv);

  define_primitive_extension(state, "f>", fsup);
  define_primitive_extension(state, "f<", finf);

  define_primitive_extension(state, "f@", fload);
  define_primitive_extension(state, "f!", fstore);

  define_primitive_extension(state, "f>>", fcompile_literal);
}

#else
void
init_ext_math_utils(struct state *state)
{
}
#endif
