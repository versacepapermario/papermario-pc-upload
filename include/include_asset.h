#ifndef _H_INCLUDE_ASSET
#define _H_INCLUDE_ASSET

// Need GBI types (Gfx, Vtx) for inline .gfx.inc.c and .vtx.inc.c stubs
#include "ultra64.h"

/**
 * include_asset.h - PC port version
 *
 * On N64, INCLUDE_IMG/PAL/RAW use __asm__ .incbin to embed binary asset data
 * directly into the compiled object files from the build directory.
 *
 * On PC, assets are loaded at runtime from O2R archives via libultraship's
 * ResourceManager. These macros become simple extern declarations so the
 * symbols can be referenced by game code. The actual data will be provided
 * by the asset loading system.
 *
 * 2,027 uses across 238 files depend on these macros.
 */

// two macros are needed for N() usage
#define _INCLUDE_IMG(FILENAME, SYMBOLNAME) \
    extern unsigned char SYMBOLNAME[];

#define INCLUDE_IMG(FILENAME, SYMBOLNAME) \
    _INCLUDE_IMG(FILENAME, SYMBOLNAME)

#define INCLUDE_PAL(FILENAME, SYMBOLNAME) \
    extern unsigned short SYMBOLNAME[];

#define INCLUDE_RAW(FILENAME, SYMBOLNAME) \
    extern unsigned char SYMBOLNAME[];

#endif // _H_INCLUDE_ASSET
