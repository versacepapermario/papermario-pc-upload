/**
 * libc/xstdio.h - Bridge stub for PaperShip PC port
 *
 * The original decomp's xstdio.h defines _Pft and printf helpers.
 * libultraship provides these in libultra/printf.h.
 * We redirect to avoid duplicate type definitions.
 */
#ifndef _XSTDIO_H
#define _XSTDIO_H

#include <libultra/printf.h>

// FLAGS defines used by decomp printf code
#ifndef FLAGS_SPACE
#define FLAGS_SPACE 1
#define FLAGS_PLUS 2
#define FLAGS_MINUS 4
#define FLAGS_HASH 8
#define FLAGS_ZERO 16
#endif

// outfun typedef used by decomp printf code
typedef char *outfun(char*, const char*, size_t);

// Function declarations for decomp printf implementation
int _Printf(outfun prout, char *arg, const char *fmt, va_list args);
void _Litob(_Pft *args, char type);
void _Ldtob(_Pft* args, char type);

#endif
