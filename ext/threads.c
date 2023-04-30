// The author disclaims copyright to this source code.
#include "colorforth.h"
#include "cf-stdio.h"

#ifdef __THREADS


#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <string.h>

static char initialized = 0;

#ifdef __EXTENDED_MATH
extern void init_fstack(struct fstack *stack, int len);
#endif

#ifdef __MP_MATH
extern void init_mpstack(struct mpstack *stack, int len, unsigned char id);
#endif

struct thread_args
{
  pthread_t pthread;
  struct state *clone;
  struct entry *entry;
};

struct thread_args thread_args[MAX_THREAD];
sem_t locks[MAX_LOCK];

extern void init_stack(struct stack *stack, int len, unsigned char id);
extern void execute(struct state *s);
extern void execute_(struct state *s, struct entry *entry);

struct state *
clone_state(struct state *s)
{
  struct state *clone = cf_calloc(s, 1, sizeof(*s), THREAD_CLONE_STATE_ERROR);
  clone->color = execute;

  clone->dict.entries = s->dict.entries;
  clone->dict.latest = s->dict.latest;

  clone->stack = cf_calloc(s, 1, sizeof(struct stack), THREAD_STACK_ERROR);
  init_stack(clone->stack, STACK_SIZE, THREAD_INIT_STACK_ERROR);

  clone->r_stack = cf_calloc(s, 1, sizeof(struct stack), THREAD_RSTACK_ERROR);
  init_stack(clone->r_stack, R_STACK_SIZE, THREAD_INIT_RSTACK_ERROR);

  clone->heap = s->heap;
  clone->here = s->here;

  clone->done = 0;
  clone->echo_on = 0;

  clone->str_stream = NULL;
  clone->file_stream = NULL;

#ifdef __EXTENDED_MATH
  init_fstack(&clone->fstack, FSTACK_SIZE);
#endif

#ifdef __MP_MATH
  init_mpstack(&clone->mpstack, MPSTACK_SIZE, 135);
#endif

  return clone;
}

void
free_clone_state(struct state *clone)
{
  free(clone->stack);
  free(clone->r_stack);

#ifdef __EXTENDED_MATH
  free(clone->fstack.cells);
#endif

#ifdef __MP_MATH
  free(clone->mpstack.cells);
#endif

  free(clone);
}

void
thread_exit_handler(int sig)
{
  pthread_exit(0);
}

void *
perform_thread(void *arg)
{
  struct thread_args *thread_args = (struct thread_args *) arg;

  struct sigaction actions;
  memset(&actions, 0, sizeof(actions));
  sigemptyset(&actions.sa_mask);
  actions.sa_flags = 0;
  actions.sa_handler = thread_exit_handler;
  sigaction(SIGUSR1,&actions,NULL);

  execute_(thread_args->clone, thread_args->entry);

  free_clone_state(thread_args->clone);

  return NULL;
}

void
thread_run(struct state *s)
{
  POP2();
  cell n = p1;
  struct entry *entry = (struct entry *) p2;

  if (n >= MAX_THREAD) {
    cf_printf(s, "Too many threads. At most %d allowed\n", MAX_THREAD);
    PUSH1(-1);
    return;
  }

  struct state *clone = clone_state(s);

  thread_args[p1].clone = clone;
  thread_args[p1].entry = entry;

  pthread_create(&thread_args[p1].pthread, NULL, perform_thread, (void *) &thread_args[p1]);

  PUSH1(n);
}

void
thread_join_all(struct state *s)
{
  for (int i = 0; i < MAX_THREAD; i++)
  {
    pthread_join(thread_args[i].pthread, NULL);
  }
}

void
thread_join(struct state *s)
{
  POP1();

  pthread_join(thread_args[p1].pthread, NULL);
}

void
thread_kill(struct state *s)
{
  POP1();

  pthread_kill(thread_args[p1].pthread, SIGUSR1);
  pthread_join(thread_args[p1].pthread, NULL);

  pthread_kill(thread_args[p1].pthread, 0);
  pthread_join(thread_args[p1].pthread, NULL);

  free_clone_state(thread_args[p1].clone);
}

void
thread_lock(struct state *s)
{
  POP1();

  sem_wait(&locks[p1]);
}

void
thread_unlock(struct state *s)
{
  POP1();

  sem_post(&locks[p1]);
}

void
require_threads_fn(struct state *s)
{
  if (initialized) return;

  for (int i = 0; i < MAX_LOCK; i++)
  {
    sem_init(&locks[i], 0, 1);
  }

  define_primitive_extension(s, THREAD__RUN_HASH,         ENTRY_NAME("thread/run"), thread_run);
  define_primitive_extension(s, THREAD__JOIN_SUBALL_HASH, ENTRY_NAME("thread/join-all"), thread_join_all);
  define_primitive_extension(s, THREAD__JOIN_HASH,        ENTRY_NAME("thread/join"), thread_join);
  define_primitive_extension(s, THREAD__KILL_HASH,        ENTRY_NAME("thread/kill"), thread_kill);
  define_primitive_extension(s, THREAD__LOCK_HASH,        ENTRY_NAME("thread/lock"), thread_lock);
  define_primitive_extension(s, THREAD__UNLOCK_HASH,      ENTRY_NAME("thread/unlock"), thread_unlock);

  initialized = 1;
}

#else
void
init_threads_utils(struct state *s)
{
}
#endif
