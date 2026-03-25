/**
 * PR/ultratypes.h - Bridge stub for PaperShip PC port
 * Redirects to libultraship's types (which use proper stdint.h widths).
 * The original N64 SDK version uses 'unsigned long' for u32 which is
 * 64-bit on modern platforms — libultraship's version is correct.
 */
#ifndef _ULTRATYPES_H_
#define _ULTRATYPES_H_

#include <libultra/types.h>

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifndef NULL
#define NULL    0
#endif

#endif /* _ULTRATYPES_H_ */
