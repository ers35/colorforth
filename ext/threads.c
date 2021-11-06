// The author disclaims copyright to this source code.
#include <colorforth.h>
#include <cf-stdio.h>

#ifdef __THREADS

#include <pthread.h>

struct thread_args
{
  struct state *clone;
  struct entry *entry;
};

struct thread_args thread_args[MAX_THREAD];

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
    cf_printf(state, "Too many thread. At most %d allowed\n", MAX_THREAD);
    push(state->stack, -1);
    return;
  }

  pthread_t thread;
  struct state *clone = clone_state(state);

  thread_args[n].clone = clone;
  thread_args[n].entry = entry;

  pthread_create(&thread, NULL, perform_thread, (void *) &thread_args[n]);

  push(state->stack, n);
}

void
init_threads_utils(struct state *state)
{
  define_primitive_extension(state, "thread-run", thread_run);
}

#else
void
init_threads_utils(struct state *state)
{
}
#endif
