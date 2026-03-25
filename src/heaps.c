#include "common.h"
#include "audio.h"
#include "model.h"

typedef s8 TlbEntry[0x1000];
typedef TlbEntry TlbMappablePage[15];

BSS TlbMappablePage gEffectDataBuffer ALIGNED(0x1000);
BSS u8 gEffectGlobals[0x1000] ALIGNED(0x1000);
#if !VERSION_PAL
BSS u8 BattleEntityHeapBottom[0x3000] ALIGNED(0x1000);
#endif
BSS u8 AuHeapBase[AUDIO_HEAP_SIZE] ALIGNED(0x1000);
// On N64 these are contiguous at fixed addresses 0x80200000-0x8020FFFF.
// hud_element_set_aux_cache() in state_pause uses D_80200000 with size 0x38000,
// assuming the memory after it is available (model/shape data is cleared during pause).
// On PC, enlarge D_80200000 to the full size needed to prevent buffer overruns.
#ifdef PORT
BSS u8 D_80200000[0x38000] ALIGNED(0x1000);
#else
BSS u8 D_80200000[0x4000] ALIGNED(0x1000);
#endif
BSS u8 D_80204000[0x3000] ALIGNED(0x1000);
BSS u8 D_80207000[0x3000] ALIGNED(0x1000);
BSS u8 D_8020A000[0x6000] ALIGNED(0x1000);
BSS ShapeFile gMapShapeData;
