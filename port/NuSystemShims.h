#pragma once

/**
 * NuSystem Shim Layer for PaperShip
 *
 * Paper Mario uses NuSystem (nu/nusys.h), Nintendo's game development framework
 * that sits on top of the standard N64 OS. SM64 and OoT do NOT use NuSystem,
 * so this is unique to the Paper Mario port.
 *
 * These shims redirect NuSystem calls to libultraship equivalents or no-ops
 * where the functionality is handled by the PC runtime.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

// Forward declare the NuSystem callback types
typedef void (*NUGfxFunc)(u32);
typedef void (*NUGfxPreNMIFunc)(void);

// NuSystem state that game code reads
extern NUGfxFunc nuGfxFunc;             // Retrace callback (set by nuGfxFuncSet)
extern NUGfxPreNMIFunc nuGfxPreNMIFunc_stored; // PreNMI callback
extern u32 nuGfxDisplay;               // Display on/off flag
extern u16* nuGfxCfb_ptr;              // Current framebuffer pointer (for crash_screen)

#ifdef __cplusplus
}
#endif
