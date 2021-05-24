// The author disclaims copyright to this source code.
#ifndef __CF_STDIO_H
#define __CF_STDIO_H

#include <conf.h>
#include <stdio.h>
#include <colorforth.h>

#define CF_EOF EOF

extern void cf_putchar(struct state *s, int c);
extern int cf_getchar(struct state *s);

extern void cf_printf(struct state *s, const char* format, ...);

extern void cf_fflush();

#endif /* __CF_STDIO_H */
