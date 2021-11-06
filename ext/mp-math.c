// The author disclaims copyright to this source code.
#include <colorforth.h>
#include <cf-stdio.h>

#ifdef __MP_MATH

#define s_sp(s) s->mpstack.sp
#define s_lim(s) s->mpstack.lim
#define s_cells(s, sp) s->mpstack.cells[sp]

#define get_psp(sp, lim) (sp == 0 ? lim : sp - 1)
#define get_nsp(sp, lim) (sp == lim ? 0 : sp + 1)

#define get_s_psp(s) get_psp(s_sp(s), s_lim(s))
#define get_s_nsp(s) get_nsp(s_sp(s), s_lim(s))


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
  s_sp(s) = nsp;
  mpz_set_str(s_cells(s, s_sp(s)), s->tib.buf, 10);
}

void
print_mtos(struct state *s)
{
  char *out;
  const int psp = get_s_psp(s);
  gmp_asprintf(&out, "%Zd", s_cells(s, s_sp(s)));
  cf_printf(s, "%s ", out);
  cf_fflush();
  free(out);
  s_sp(s) = psp;
}

void
mdrop(struct state *s)
{
  const int psp = get_s_psp(s);
  s_sp(s) = psp;
}

void
mdup(struct state *s)
{
  const int nsp = get_s_nsp(s);
  mpz_set(s_cells(s, nsp), s_cells(s, s_sp(s)));
  s_sp(s) = nsp;
}

void
mswap(struct state *s)
{
  const int psp = get_s_psp(s);
  mpz_swap(s_cells(s, psp), s_cells(s, s_sp(s)));
}

void
mover(struct state *s)
{
  const int psp = get_s_psp(s);
  const int nsp = get_s_nsp(s);

  mpz_set(s_cells(s, nsp), s_cells(s, psp));

  s_sp(s) = nsp;
}

void
mrot(struct state *s)
{
  const int psp = get_s_psp(s);
  const int ppsp = get_psp(psp, s_lim(s));

  mpz_t n;
  mpz_init(n);

  mpz_set(n, s_cells(s, s_sp(s)));
  mpz_set(s_cells(s, s_sp(s)), s_cells(s, ppsp));
  mpz_set(s_cells(s, ppsp), s_cells(s, psp));
  mpz_set(s_cells(s, psp), n);

  mpz_clear(n);
}

void
mminus_rot(struct state *s)
{
  const int psp = get_s_psp(s);
  const int ppsp = get_psp(psp, s_lim(s));

  mpz_t n;
  mpz_init(n);

  mpz_set(n, s_cells(s, s_sp(s)));
  mpz_set(s_cells(s, s_sp(s)), s_cells(s, psp));
  mpz_set(s_cells(s, psp), s_cells(s, ppsp));
  mpz_set(s_cells(s, ppsp), n);

  mpz_clear(n);
}

void
mnip(struct state *s)
{
  const int psp = get_s_psp(s);
  mpz_set(s_cells(s, psp), s_cells(s, s_sp(s)));
  s_sp(s) = psp;
}

void
madd(struct state *s)
{
  const int psp = get_s_psp(s);

  mpz_add(s_cells(s, psp), s_cells(s, s_sp(s)), s_cells(s, psp));

  s_sp(s) = psp;
}

void
msub(struct state *s)
{
  const int psp = get_s_psp(s);

  mpz_sub(s_cells(s, psp), s_cells(s, psp), s_cells(s, s_sp(s)));

  s_sp(s) = psp;
}

void
mmul(struct state *s)
{
  const int psp = get_s_psp(s);

  mpz_mul(s_cells(s, psp), s_cells(s, psp), s_cells(s, s_sp(s)));

  s_sp(s) = psp;
}

void
mdiv(struct state *s)
{
  const int psp = get_s_psp(s);

  mpz_fdiv_q(s_cells(s, psp), s_cells(s, psp), s_cells(s, s_sp(s)));

  s_sp(s) = psp;
}

void
msup(struct state *s)
{
  const int psp = get_s_psp(s);
  push(s->stack, mpz_cmp(s_cells(s, psp), s_cells(s, s_sp(s))) > 0);
  s_sp(s) = get_psp(psp, s_lim(s));
}

void
minf(struct state *s)
{
  const int psp = get_s_psp(s);
  push(s->stack, mpz_cmp(s_cells(s, psp), s_cells(s, s_sp(s))) < 0);
  s_sp(s) = get_psp(psp, s_lim(s));
}

void
mload(struct state *s)
{
  s_sp(s) = get_s_nsp(s);
  mpz_set(s_cells(s, s_sp(s)), *(mpz_t*)pop(s->stack));
}

void
mstore(struct state *s)
{
  mpz_t *ptr = (mpz_t*)pop(s->stack);
  mpz_init(*ptr);
  mpz_set(*ptr, s_cells(s, s_sp(s)));
  s_sp(s) = get_s_psp(s);
}

void
mcompile_literal(struct state *s)
{
  mpz_t *ptr = calloc(1, sizeof(mpz_t));
  mpz_init(*ptr);
  mpz_set(*ptr, s_cells(s, s_sp(s)));;
  s_sp(s) = get_s_psp(s);

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
