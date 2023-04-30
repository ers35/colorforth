// The author disclaims copyright to this source code.
#include "colorforth.h"
#include "cf-stdio.h"

#ifdef __MP_MATH

static char initialized = 0;

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
  stack->cells = cf_calloc(NULL, len, sizeof(mpz_t), MP_STACK_INIT_ERROR);
  for (int i = 0; i < MPSTACK_SIZE; i++)
  {
    mpz_init(stack->cells[i]);
  }
  stack->sp = 0;
  stack->lim = len - 1;
}

void
mpsize(struct state *s)
{
  PUSH1(sizeof(mpz_t));
}

static void
mdot_s(struct state *s)
{
  char *out;

  for (int i = 0, p = s->mpstack.sp + 1; i <= s->mpstack.lim; i++, p++)
  {
    if (p > s->mpstack.lim) p = 0;

    if (i <= s->mpstack.lim - 8) continue;

    gmp_asprintf(&out, "%Zd", s->mpstack.cells[p]);
    cf_printf(s, "%s ", out);
    free(out);
  }
  cf_printf(s, " <mtos\n");
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
  PUSH1(mpz_cmp(s_cells(s, psp), s_cells(s, s_sp(s))) > 0);
  s_sp(s) = get_psp(psp, s_lim(s));
}

void
minf(struct state *s)
{
  const int psp = get_s_psp(s);
  PUSH1(mpz_cmp(s_cells(s, psp), s_cells(s, s_sp(s))) < 0);
  s_sp(s) = get_psp(psp, s_lim(s));
}

void
mequal(struct state *s)
{
  const int psp = get_s_psp(s);
  PUSH1(mpz_cmp(s_cells(s, psp), s_cells(s, s_sp(s))) == 0);
  s_sp(s) = get_psp(psp, s_lim(s));
}

void
mload(struct state *s)
{
  POP1();
  s_sp(s) = get_s_nsp(s);
  mpz_set(s_cells(s, s_sp(s)), *(mpz_t*)p1);
}

void
mstore(struct state *s)
{
  POP1();
  mpz_t *ptr = (mpz_t*)p1;
  mpz_init(*ptr);
  mpz_set(*ptr, s_cells(s, s_sp(s)));
  s_sp(s) = get_s_psp(s);
}

void
mcompile_literal(struct state *s)
{
  mpz_t *ptr = cf_calloc(s, 1, sizeof(mpz_t), MP_LITERAL_ERROR);
  mpz_init(*ptr);
  mpz_set(*ptr, s_cells(s, s_sp(s)));;
  s_sp(s) = get_s_psp(s);

  compile_code(s, OP_NUMBER, (cell)ptr);
}

void
require_mp_math_fn(struct state *s)
{
  if (initialized) return;

  init_mpstack(&s->mpstack, MPSTACK_SIZE);
  define_prefix('#', define_mpz,  COLOR_YELLOW,     0);

  define_primitive_extension(s, MPZ_HASH,       ENTRY_NAME("mpz"), mpsize);

  define_primitive_extension(s, M_DOTS_HASH,    ENTRY_NAME("m.s"), mdot_s);
  define_primitive_extension(s, M_DOT_HASH,     ENTRY_NAME("m."), print_mtos);

  define_primitive_extension(s, MDROP_HASH,     ENTRY_NAME("mdrop"), mdrop);
  define_primitive_extension(s, MDUP_HASH,      ENTRY_NAME("mdup"), mdup);
  define_primitive_extension(s, MSWAP_HASH,     ENTRY_NAME("mswap"), mswap);
  define_primitive_extension(s, MOVER_HASH,     ENTRY_NAME("mover"), mover);
  define_primitive_extension(s, MROT_HASH,      ENTRY_NAME("mrot"), mrot);
  define_primitive_extension(s, M_SUBROT_HASH,  ENTRY_NAME("m-rot"), mminus_rot);
  define_primitive_extension(s, MNIP_HASH,      ENTRY_NAME("mnip"), mnip);

  define_primitive_extension(s, M_ADD_HASH,     ENTRY_NAME("m+"), madd);
  define_primitive_extension(s, M_SUB_HASH,     ENTRY_NAME("m-"), msub);
  define_primitive_extension(s, M_MUL_HASH,     ENTRY_NAME("m*"), mmul);
  define_primitive_extension(s, M_DIV_HASH,     ENTRY_NAME("m/"), mdiv);

  define_primitive_extension(s, M_TO__HASH,     ENTRY_NAME("m>"), msup);
  define_primitive_extension(s, M_FROM_HASH,    ENTRY_NAME("m<"), minf);
  define_primitive_extension(s, M_EQUAL_HASH,   ENTRY_NAME("m="), mequal);

  define_primitive_extension(s, M_LOAD_HASH,    ENTRY_NAME("m@"), mload);
  define_primitive_extension(s, M_STORE_HASH,   ENTRY_NAME("m!"), mstore);

  define_primitive_extension(s, M_INLINE_HASH,  ENTRY_NAME("[m,]"), mcompile_literal);

  initialized = 1;
}

#else
void
init_mp_math_utils(struct state *s)
{
}
#endif
