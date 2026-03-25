/**
 * gcc/stdarg.h - PC port redirect
 *
 * The original decomp had a MIPS-specific stdarg.h.
 * On PC, we just use the system's standard stdarg.h.
 */
#ifndef _PM64_GCC_STDARG_H_
#define _PM64_GCC_STDARG_H_

#include_next <stdarg.h>

#endif
