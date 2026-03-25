/**
 * PR/abi.h - Bridge stub for PaperShip PC port
 * Redirects to libultraship's ABI (Audio Binary Interface).
 */
#ifndef _ABI_H_
#define _ABI_H_

#include <libultra/abi.h>

// N64 SDK audio command aliases (libultraship uses different names)
#ifndef A_SETVOL
#define A_SETVOL        9   // libultraship: A_DUPLICATE
#endif
#ifndef A_POLEF
#define A_POLEF         14  // libultraship: A_HILOGAIN
#endif
#ifndef A_ENVMIXER_GE
#define A_ENVMIXER_GE   19
#endif

#endif /* _ABI_H_ */
