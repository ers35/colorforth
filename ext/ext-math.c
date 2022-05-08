// The author disclaims copyright to this source code.
#include "colorforth.h"
#include "cf-stdio.h"

#ifdef __EXTENDED_MATH

static char initialized = 0;

void
init_fstack(struct fstack *stack, int len)
{
  stack->cells = cf_calloc(NULL, len, sizeof(number_t), F_STACK_ERROR);
  stack->sp = 0;
  stack->lim = len;
}

void
fsize(struct state *state)
{
  push(state->stack, sizeof(number_t));
}

static void
fdot_s(struct state *state)
{
  cf_printf(state, "[%d] ", state->fstack.sp);
  for (int i = 0; i < state->fstack.sp; i++)
  {
    cf_printf(state, "%lf ", state->fstack.cells[i]);
  }
  cf_printf(state, "<ftos\n");
}

void
fpush(struct fstack *stack, const number_t n)
{
#ifndef UNSAFE_MODE
  if (stack->sp >= stack->lim)
  {
    cf_printf(NULL, "ES>!\n");
    return;
  }
#endif

  stack->cells[stack->sp] = n;
  stack->sp += 1;
}

number_t
fpop(struct fstack *stack)
{
#ifndef UNSAFE_MODE
  if (stack->sp == 0)
  {
    cf_printf(NULL, "ES<!\n");
    return 0;
  }
#endif

  stack->sp -= 1;
  return stack->cells[stack->sp];
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
  fpush(&s->fstack, s->fstack.cells[s->fstack.sp - 1]);
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
require_ext_math_fn(struct state *state)
{
  if (initialized) return;

  init_fstack(&state->fstack, FSTACK_SIZE);
  define_prefix('$', define_float,  COLOR_YELLOW,     0);

  define_primitive_extension(state, FLOAT_HASH,      ENTRY_NAME("float"), fsize);

  define_primitive_extension(state, F_DOTS_HASH,     ENTRY_NAME("f.s"), fdot_s);
  define_primitive_extension(state, F_DOT_HASH,      ENTRY_NAME("f."), print_ftos);

  define_primitive_extension(state, FDROP_HASH,      ENTRY_NAME("fdrop"), fdrop);
  define_primitive_extension(state, FDUP_HASH,       ENTRY_NAME("fdup"), fdup);
  define_primitive_extension(state, FSWAP_HASH,      ENTRY_NAME("fswap"), fswap);
  define_primitive_extension(state, FOVER_HASH,      ENTRY_NAME("fover"), fover);
  define_primitive_extension(state, FROT_HASH,       ENTRY_NAME("frot"), frot);
  define_primitive_extension(state, F_SUBROT_HASH,   ENTRY_NAME("f-rot"), fminus_rot);
  define_primitive_extension(state, FNIP_HASH,       ENTRY_NAME("fnip"), fnip);

  define_primitive_extension(state, F_ADD_HASH,      ENTRY_NAME("f+"), fadd);
  define_primitive_extension(state, F_SUB_HASH,      ENTRY_NAME("f-"), fsub);
  define_primitive_extension(state, F_MUL_HASH,      ENTRY_NAME("f*"), fmul);
  define_primitive_extension(state, F_DIV_HASH,      ENTRY_NAME("f/"), fdiv);

  define_primitive_extension(state, F_TO__HASH,      ENTRY_NAME("f>"), fsup);
  define_primitive_extension(state, F_FROM_HASH,     ENTRY_NAME("f<"), finf);

  define_primitive_extension(state, F_LOAD_HASH,     ENTRY_NAME("f@"), fload);
  define_primitive_extension(state, F_STORE_HASH,    ENTRY_NAME("f!"), fstore);

  define_primitive_extension(state, F_INLINE_HASH,   ENTRY_NAME("[f,]"), fcompile_literal);

  initialized = 1;
}

#else
void
init_ext_math_utils(struct state *state)
{
}
#endif
