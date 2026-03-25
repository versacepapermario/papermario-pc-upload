#ifndef __INCLUDE_ASM_H__
#define __INCLUDE_ASM_H__

/**
 * include_asm.h - PC port version
 *
 * The original decomp uses INCLUDE_ASM to embed MIPS assembly for functions
 * that haven't been decompiled yet. Since Paper Mario is fully decompiled,
 * no INCLUDE_ASM calls remain in the source. We define empty macros here
 * just in case any are referenced.
 */

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)

// All functions are decompiled - INCLUDE_ASM is never used
#define INCLUDE_ASM(TYPE, FOLDER, NAME, ARGS...)
#define INCLUDE_ASM_SHIFT(TYPE, FOLDER, NAME, ARGS...)

#endif
