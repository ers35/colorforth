// The author disclaims copyright to this source code.
#ifndef __CF_STDIO_H
#define __CF_STDIO_H

#include <conf.h>
#include <stdio.h>

#define CF_EOF EOF

#ifdef __INTERNAL_STDIO

#define cf_putchar(c) putchar(c)
#define cf_getchar() getchar()

#define cf_printf(...) printf( __VA_ARGS__ )

#define cf_fflush() fflush(stdout)

#else

extern void cf_putchar(int c);
extern int cf_getchar();

extern void cf_printf(const char* format, ...);

extern void cf_fflush();

#endif /* __INTERNAL_STDIO */

#endif /* __CF_STDIO_H */
