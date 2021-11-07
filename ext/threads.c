// The author disclaims copyright to this source code.
#include <colorforth.h>
#include <cf-stdio.h>

#ifdef __THREADS

#include <pthread.h>
#include <semaphore.h>

#ifdef __MP_MATH
extern void init_mpstack(struct mpstack *stack, int len);
#endif

struct thread_args
{
  pthread_t pthread;
  struct state *clone;
  struct entry *entry;
};

struct thread_args thread_args[MAX_THREAD];
sem_t locks[MAX_LOCK];

extern void init_stack(struct stack *stack, int len);
extern void execute(struct state *s);
extern void execute_(struct state *s, struct entry *entry);

struct state *
clone_state(struct state *state)
{
  struct state *clone = calloc(1, sizeof(*state));
  clone->color = execute;

  clone->dict.entries = state->dict.entries;
  clone->dict.latest = state->dict.latest;

  clone->inlined_dict.entries = state->inlined_dict.entries;
  clone->inlined_dict.latest = state->inlined_dict.latest;

  clone->stack = calloc(1, sizeof(struct stack));
  init_stack(clone->stack, STACK_SIZE);

  clone->r_stack = calloc(1, sizeof(struct stack));
  init_stack(clone->r_stack, R_STACK_SIZE);

  clone->heap = state->heap;
  clone->here = state->here;

  clone->done = 0;
  clone->echo_on = 0;

  clone->coll = 0; clone->line = 1;

  clone->str_stream = NULL;
  clone->file_stream = NULL;


#ifdef __MP_MATH
  init_mpstack(&clone->mpstack, MPSTACK_SIZE);
#endif

  return clone;
}

void
free_clone_state(struct state *clone)
{
  free(clone->stack);
  free(clone->r_stack);
  free(clone);
}

void *
perform_thread(void *arg)
{
  struct thread_args *thread_args = (struct thread_args *) arg;

  execute_(thread_args->clone, thread_args->entry);

  free_clone_state(thread_args->clone);

  return NULL;
}

void
thread_run(struct state *state)
{
  cell n = pop(state->stack);
  struct entry *entry = (struct entry *) pop(state->stack);

  if (n >= MAX_THREAD) {
    cf_printf(state, "Too many threads. At most %d allowed\n", MAX_THREAD);
    push(state->stack, -1);
    return;
  }

  struct state *clone = clone_state(state);

  thread_args[n].clone = clone;
  thread_args[n].entry = entry;

  pthread_create(&thread_args[n].pthread, NULL, perform_thread, (void *) &thread_args[n]);

  push(state->stack, n);
}

void
thread_join_all(struct state *state)
{
  for (int i = 0; i < MAX_THREAD; i++)
  {
    pthread_join(thread_args[i].pthread, NULL);
  }
}

void
thread_join(struct state *state)
{
  cell n = pop(state->stack);

  pthread_join(thread_args[n].pthread, NULL);
}

void
thread_cancel(struct state *state)
{
  cell n = pop(state->stack);

  pthread_cancel(thread_args[n].pthread);

  free_clone_state(thread_args[n].clone);
}

void
thread_lock(struct state *state)
{
  cell n = pop(state->stack);

  sem_wait(&locks[n]);
}

void
thread_unlock(struct state *state)
{
  cell n = pop(state->stack);

  sem_post(&locks[n]);
}

void
init_threads_utils(struct state *state)
{
  for (int i = 0; i < MAX_LOCK; i++)
  {
    sem_init(&locks[i], 0, 1);
  }

  define_primitive_extension(state, "thread/run", thread_run);
  define_primitive_extension(state, "thread/join-all", thread_join_all);
  define_primitive_extension(state, "thread/join", thread_join);
  define_primitive_extension(state, "thread/cancel", thread_cancel);
  define_primitive_extension(state, "thread/lock", thread_lock);
  define_primitive_extension(state, "thread/unlock", thread_unlock);
}

#else
void
init_threads_utils(struct state *state)
{
}
#endif
