/**
 * stdlib/stdarg.h - PC port redirect
 *
 * The original decomp had a custom GNU stdarg.h for MIPS/N64.
 * On PC, we just use the system's standard stdarg.h.
 */
#ifndef _PM64_STDLIB_STDARG_H_
#define _PM64_STDLIB_STDARG_H_

#include_next <stdarg.h>

#endif
