/**
 * PR/mbi.h - Bridge stub for PaperShip PC port
 * Redirects to libultraship's MBI (Media Binary Interface).
 */
#ifndef _MBI_H_
#define _MBI_H_

#include <libultra/mbi.h>

// _SHIFTL/_SHIFTR macros used throughout the decomp
#ifndef _SHIFTL
#define _SHIFTL(v, s, w) \
    ((unsigned int) (((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
#define _SHIFTR(v, s, w) \
    ((unsigned int)(((unsigned int)(v) >> (s)) & ((0x01 << (w)) - 1)))
#endif

#ifndef G_ON
#define G_ON  (1)
#define G_OFF (0)
#endif

#endif /* _MBI_H_ */
