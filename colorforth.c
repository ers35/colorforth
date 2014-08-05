// The author disclaims copyright to this source code.

#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef int_fast32_t cell;
// circular stack
static cell stack[8];
// stack position
static uint32_t sp;
static struct tib
{
	char buf[8];
	uint32_t len;
} tib;
typedef void (*func)(void);
struct entry;
static struct entry
{
	char name[sizeof(tib.buf)];
	cell name_len;
	struct entry *prev;
	cell code_len;
	// could put a "do this" func here instead of requiring a type below
	struct code
	{
		enum { FUNC, NUMBER } type;
		union
		{
			func func; // primitive
			cell number;
		} data;
	} code[];
} *dictionary = (void*)(char[4096]){};
static struct entry *latest;
static void *here;
static struct code *pc = (void*)INT_FAST32_MAX;

static void
push(const cell n)
{
	if (sp == (sizeof(stack) / sizeof(stack[0])) - 1)
	{
		sp = 0;
	}
	else
	{
		sp += 1;
	}
	stack[sp] = n;
}

static cell
pop(void)
{
	const cell n = stack[sp];
	if (sp == 0)
	{
		sp = (sizeof(stack) / sizeof(stack[0])) - 1;
	}
	else
	{
		sp -= 1;
	}
	return n;
}

static void
dup(void)
{
	push(stack[sp]);
}

static void
swap(void)
{
	const cell n1 = pop();
	const cell n2 = pop();
	push(n1);
	push(n2);
}

static void
return_from_subroutine(void)
{
	pc = (void*)INT_FAST32_MAX;
}

static void
add(void)
{
	const cell n1 = pop();
	const cell n2 = pop();
	push(n1 + n2);
}

static void
subtract(void)
{
	const cell n1 = pop();
	const cell n2 = pop();
	push(n1 - n2);
}

static void
multiply(void)
{
	const cell n1 = pop();
	const cell n2 = pop();
	push(n1 * n2);
}

static void
equals(void)
{
	const cell n1 = pop();
	const cell n2 = pop();
	push(n1 == n2);
}

static void
less_than(void)
{
	const cell n1 = pop();
	const cell n2 = pop();
	push(n2 < n1);
}

static void
if_(void)
{
	const cell n = pop();
	if (!n)
	{
		pc += 1;
	}
}

static void
tail_call(void)
{
	pc = &latest->code[-1];
}

static void
dump_words(void)
{
	for (struct entry *entry = latest; entry; entry = entry->prev)
	{
		printf("%.*s ", (int)entry->name_len, entry->name);
	}
	putchar('\n');
}

static void
print_top_of_stack(void)
{
	printf("%"PRIiFAST32" ", pop());
	fflush(stdout);
}

static struct entry*
find_entry(void)
{
	for (struct entry *entry = latest; entry; entry = entry->prev)
	{
		if (entry->name_len == tib.len && memcmp(entry->name, tib.buf, tib.len) == 0)
		{
			return entry;
		}
	}
	return NULL;
}

static void
tick(void)
{
	//stack[grow_stack()] = (cell)find_entry(tib.buf, tib.len);
}

// 'name' must be null-terminated.
static void
define_primitive(const char name[], const func func)
{
	struct entry *entry = here;
	entry->name_len = strlen(name);
	entry->prev = latest;
	memcpy(entry->name, name, entry->name_len);
	entry->code_len = 1;
	entry->code[0].type = FUNC;
	entry->code[0].data.func = func;
	here = &entry->code[1];
	latest = entry;
	assert(entry->prev < entry);
}

static void
define(void)
{
	struct entry *entry = here;
	entry->name_len = tib.len;
	entry->prev = latest;
	memcpy(entry->name, tib.buf, tib.len);
	entry->code_len = 0;
	here = &entry->code[1];
	latest = entry;
	assert(entry->prev < entry);
}

// TODO: use strtoul instead
// or, support signed with strtol
static bool
tib_to_number(cell *n_)
{
	cell n = 0;
	for (uint32_t i = 0; i < tib.len; ++i)
	{
		if (isdigit(tib.buf[i]))
		{
			n *= 10;
			n += tib.buf[i] - '0';
		}
		else
		{
			//printf("%.*s?\n", tib.len, tib.buf);
			printf("? ");
			return 0;
		}
	}
	*n_ = n;
	return 1;
}

static void
execute(void)
{
	struct entry *entry = find_entry();
	if (entry)
	{
		//printf("[%.*s]\n", (int)entry->name_len, entry->name);
		for (pc = entry->code; pc < &entry->code[entry->code_len]; ++pc)
		{
			if (pc->type == FUNC)
			{
				pc->data.func();
			}
			else if (pc->type == NUMBER)
			{
				push(pc->data.number);
			}
		}
	}
	else
	{
		// try to interpret as an unsigned decimal number
		cell n = 0;
		if (tib_to_number(&n))
		{
			push(n);
		}
	}
}

static void
compile(void)
{
	struct entry *entry = find_entry();
	if (entry)
	{
		if (entry == latest)
		{
			// recursive tail call
			latest->code[latest->code_len].type = FUNC;
			latest->code[latest->code_len].data.func = tail_call;
			latest->code_len += 1;
		}
		else
		{
			//printf("[%.*s]\n", (int)entry->name_len, entry->name);
			// inline all of the entry's code
			memcpy(&latest->code[latest->code_len], entry->code, entry->code_len * sizeof(entry->code[0]));
			latest->code_len += entry->code_len;
		}
		here = &latest->code[latest->code_len];
	}
	else
	{
		// try to interpret as an unsigned decimal number
		cell n = 0;
		if (tib_to_number(&n))
		{
			// compile number
			latest->code[latest->code_len].type = NUMBER;
			latest->code[latest->code_len].data.number = n;
			++latest->code_len;
			here = &latest->code[latest->code_len];
		}
	}
}

static void
comment(void)
{
	
}

static void
bye(void)
{
	exit(0);
}

static void
emit(void)
{
	putchar((char)pop());
}

int
main(int argc, char *argv[])
{
	here = dictionary;
	define_primitive(".", print_top_of_stack);
	define_primitive("dup", dup);
	define_primitive("swap", swap);
	define_primitive("+", add);
	define_primitive("-", subtract);
	define_primitive("*", multiply);
	define_primitive("=", equals);
	define_primitive("<", less_than);
	define_primitive("if", if_);
	define_primitive("bye", bye);
	define_primitive("words", dump_words);
	define_primitive(";", return_from_subroutine);
	define_primitive("'", tick);
	define_primitive("emit", emit);
	while (1)
	{
		func color = execute;
		tib.len = 0;
		bool reading_word = true;
		while (reading_word)
		{
			int c;
			switch (c = getchar())
			{
				case ':':
				{
					color = define;
					break;
				}
				
				case '^':
				{
					color = compile;
					break;
				}
				
				case '~':
				{
					color = execute;
					break;
				}
				
				case '(':
				{
					color = comment;
					break;
				}
				
				case '\n':
				case ' ':
				case '\t':
				{
					if (tib.len == 0)
					{
						// Strip leading whitespace.
					}
					else
					{
						// Have word.
						color();
						//~ printf("[%.*s]\n", tib.len, tib.buf);
						reading_word = false;
					}
					break;
				}
				
				case EOF:
				{
					bye();
					break;
				}
				
				default:
				{
					if (tib.len < sizeof(tib.buf))
					{
						tib.buf[tib.len++] = c;
					}
					break;
				}
			}
		}
	}
	return 0;
}
