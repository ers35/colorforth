// The author disclaims copyright to this source code.
#ifndef __CF_STDIO_H
#define __CF_STDIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "conf.h"
#include "colorforth.h"

#define CF_EOF EOF

extern void cf_putchar(struct state *s, int c);
extern int cf_getchar(struct state *s);

extern void cf_printf(struct state *s, const char* format, ...);

extern void cf_fflush();

#ifdef __cplusplus
}
#endif

#endif /* __CF_STDIO_H */
