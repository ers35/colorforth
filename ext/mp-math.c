// The author disclaims copyright to this source code.
#include <colorforth.h>
#include <cf-stdio.h>

#ifdef __MP_MATH

#define get_psp(sp, lim) (sp == 0 ? lim : sp - 1)
#define get_nsp(sp, lim) (sp == lim ? 0 : sp + 1)

#define get_s_psp(s) get_psp(s->mpstack.sp, s->mpstack.lim)
#define get_s_nsp(s) get_nsp(s->mpstack.sp, s->mpstack.lim)


 void
 init_mpstack(struct mpstack *stack, int len)
 {
   stack->cells = calloc(len, sizeof(mpz_t));
   for (int i = 0; i < MPSTACK_SIZE; i++)
   {
     mpz_init(stack->cells[i]);
   }
   stack->sp = 0;
   stack->lim = len - 1;
 }

void
mpsize(struct state *state)
{
  push(state->stack, sizeof(mpz_t));
}

static void
mdot_s(struct state *state)
{
  char *out;

  for (int i = 0, p = state->mpstack.sp + 1; i <= state->mpstack.lim; i++, p++)
  {
    if (p > state->mpstack.lim) p = 0;

    if (i <= state->mpstack.lim - 8) continue;

    gmp_asprintf(&out, "%Zd", state->mpstack.cells[p]);
    cf_printf(state, "%s ", out);
    free(out);
  }
  cf_printf(state, " <mtos\n");
}

void
define_mpz(struct state *s)
{
  const int nsp = get_s_nsp(s);
  s->mpstack.sp = nsp;
  mpz_set_str(s->mpstack.cells[s->mpstack.sp], s->tib.buf, 10);
}

void
print_mtos(struct state *s)
{
  char *out;
  const int psp = get_s_psp(s);
  gmp_asprintf(&out, "%Zd", s->mpstack.cells[s->mpstack.sp]);
  cf_printf(s, "%s ", out);
  cf_fflush();
  free(out);
  s->mpstack.sp = psp;
}

void
mdrop(struct state *s)
{
  const int psp = get_s_psp(s);
  s->mpstack.sp = psp;
}

void
mdup(struct state *s)
{
  const int nsp = get_s_nsp(s);
  mpz_set(s->mpstack.cells[nsp], s->mpstack.cells[s->mpstack.sp]);
  s->mpstack.sp = nsp;
}

void
mswap(struct state *s)
{
  const int psp = get_s_psp(s);
  mpz_swap(s->mpstack.cells[psp], s->mpstack.cells[s->mpstack.sp]);
}

void
mover(struct state *s)
{
  const int psp = get_s_psp(s);
  const int nsp = get_s_nsp(s);

  mpz_set(s->mpstack.cells[nsp], s->mpstack.cells[psp]);

  s->mpstack.sp = nsp;
}

void
mrot(struct state *s)
{
  const int psp = get_s_psp(s);
  const int ppsp = get_psp(psp, s->mpstack.lim);

  mpz_t n;
  mpz_init(n);

  mpz_set(n, s->mpstack.cells[s->mpstack.sp]);
  mpz_set(s->mpstack.cells[s->mpstack.sp], s->mpstack.cells[ppsp]);
  mpz_set(s->mpstack.cells[ppsp], s->mpstack.cells[psp]);
  mpz_set(s->mpstack.cells[psp], n);

  mpz_clear(n);
}

void
mminus_rot(struct state *s)
{
  const int psp = get_s_psp(s);
  const int ppsp = get_psp(psp, s->mpstack.lim);

  mpz_t n;
  mpz_init(n);

  mpz_set(n, s->mpstack.cells[s->mpstack.sp]);
  mpz_set(s->mpstack.cells[s->mpstack.sp], s->mpstack.cells[psp]);
  mpz_set(s->mpstack.cells[psp], s->mpstack.cells[ppsp]);
  mpz_set(s->mpstack.cells[ppsp], n);

  mpz_clear(n);
}

void
mnip(struct state *s)
{
  const int psp = get_s_psp(s);
  mpz_set(s->mpstack.cells[psp], s->mpstack.cells[s->mpstack.sp]);
  s->mpstack.sp = psp;
}

void
madd(struct state *s)
{
  const int psp = get_s_psp(s);

  mpz_add(s->mpstack.cells[psp], s->mpstack.cells[s->mpstack.sp], s->mpstack.cells[psp]);

  s->mpstack.sp = psp;
}

void
msub(struct state *s)
{
  const int psp = get_s_psp(s);

  mpz_sub(s->mpstack.cells[psp], s->mpstack.cells[psp], s->mpstack.cells[s->mpstack.sp]);

  s->mpstack.sp = psp;
}

void
mmul(struct state *s)
{
  const int psp = get_s_psp(s);

  mpz_mul(s->mpstack.cells[psp], s->mpstack.cells[psp], s->mpstack.cells[s->mpstack.sp]);

  s->mpstack.sp = psp;
}

void
mdiv(struct state *s)
{
  const int psp = get_s_psp(s);

  mpz_fdiv_q(s->mpstack.cells[psp], s->mpstack.cells[psp], s->mpstack.cells[s->mpstack.sp]);

  s->mpstack.sp = psp;
}

void
msup(struct state *s)
{
  const int psp = get_s_psp(s);
  push(s->stack, mpz_cmp(s->mpstack.cells[psp], s->mpstack.cells[s->mpstack.sp]) > 0);
  s->mpstack.sp = get_psp(psp, s->mpstack.lim);
}

void
minf(struct state *s)
{
  const int psp = get_s_psp(s);
  push(s->stack, mpz_cmp(s->mpstack.cells[psp], s->mpstack.cells[s->mpstack.sp]) < 0);
  s->mpstack.sp = get_psp(psp, s->mpstack.lim);
}

void
mload(struct state *s)
{
  s->mpstack.sp = get_s_nsp(s);
  mpz_set(s->mpstack.cells[s->mpstack.sp], *(mpz_t*)pop(s->stack));
}

void
mstore(struct state *s)
{
  mpz_t *ptr = (mpz_t*)pop(s->stack);
  mpz_init(*ptr);
  mpz_set(*ptr, s->mpstack.cells[s->mpstack.sp]);
  s->mpstack.sp = get_s_psp(s);
}

void
mcompile_literal(struct state *s)
{
  mpz_t *ptr = calloc(1, sizeof(mpz_t));
  mpz_init(*ptr);
  mpz_set(*ptr, s->mpstack.cells[s->mpstack.sp]);;
  s->mpstack.sp = get_s_psp(s);

  struct code *code =  (struct code *)s->here;
  code->opcode = OP_NUMBER;
  code->this = (cell)ptr;
  s->here = (struct code *)s->here + 1;
}

void
init_mp_math_utils(struct state *state)
{
  init_mpstack(&state->mpstack, MPSTACK_SIZE);
  define_prefix('#', define_mpz,  COLOR_YELLOW,     0);

  define_primitive_extension(state, "mpz", mpsize);

  define_primitive_extension(state, "m.s", mdot_s);
  define_primitive_extension(state, "m.", print_mtos);

  define_primitive_extension(state, "mdrop", mdrop);
  define_primitive_extension(state, "mdup", mdup);
  define_primitive_extension(state, "mswap", mswap);
  define_primitive_extension(state, "mover", mover);
  define_primitive_extension(state, "mrot", mrot);
  define_primitive_extension(state, "m-rot", mminus_rot);
  define_primitive_extension(state, "mnip", mnip);

  define_primitive_extension(state, "m+", madd);
  define_primitive_extension(state, "m-", msub);
  define_primitive_extension(state, "m*", mmul);
  define_primitive_extension(state, "m/", mdiv);

  define_primitive_extension(state, "m>", msup);
  define_primitive_extension(state, "m<", minf);

  define_primitive_extension(state, "m@", mload);
  define_primitive_extension(state, "m!", mstore);

  define_primitive_extension(state, "m>>", mcompile_literal);
}

#else
void
init_mp_math_utils(struct state *state)
{
}
#endif
