/**
 * ultra64.h - Bridge header for PaperShip PC port
 *
 * The original decomp's ultra64.h includes all N64 SDK headers from PR/.
 * On PC, we redirect to libultraship which provides modern implementations
 * of all N64 types, GBI macros, and OS function declarations.
 *
 * This follows the same pattern used by Ghostship (SM64 PC port).
 *
 * IMPORTANT: Do NOT re-declare functions that libultraship already provides.
 * libultraship's os.h declares: osContInit, osContStartReadData, osContGetReadData,
 * osWritebackDCacheAll, osInvalDCache, osInvalICache, osWritebackDCache,
 * osPiStartDma, osViSwapBuffer, osViSetYScale, osViSetSpecialFeatures,
 * osViSetMode, osCreatePiManager, osSetTime, osGetTime, osGetCount,
 * osSetTimer, osAiSetFrequency, osCartRomInit, osEPiStartDma,
 * osAiSetNextBuffer, osAiGetLength, osEeprom*, osViSetEvent, osCreateViManager
 */
#ifndef _ULTRA64_H_
#define _ULTRA64_H_

// libultraship provides all N64 SDK types and GBI definitions:
// - Basic types: u8, u16, u32, u64, s8, s16, s32, s64, f32, f64
// - OS types: OSThread, OSMesg, OSMesgQueue, OSContPad, OSContStatus, etc.
// - OS functions: see list in libultraship/include/libultraship/libultra/os.h
// - GBI macros: gSPVertex, gDPSetTextureImage, gSPDisplayList, etc.
// - Math types: Mtx, MtxF, Vp, Vtx, etc.
// - VI types: OSViMode, etc.
#include <libultraship.h>
#include <libultra/gbi.h>

// libultraship defines GBI macros with __ prefix; alias to standard N64 SDK names
#ifndef gSPSegment
#define gSPSegment(pkt, segment, base) __gSPSegment(pkt, segment, base)
#endif
#ifndef gSPDisplayList
#define gSPDisplayList(pkt, dl) __gSPDisplayList(pkt, dl)
#endif
#ifndef gSPVertex
#define gSPVertex(pkt, v, n, v0) __gSPVertex(pkt, v, n, v0)
#endif
#ifndef gSPInvalidateTexCache
#define gSPInvalidateTexCache(pkt, texAddr) __gSPInvalidateTexCache(pkt, texAddr)
#endif

// Additional types/defines that libultraship may not provide but the decomp needs

// N64 memory mapping — on PC these are identity operations (no KSEG0 translation needed)
// The original N64 definitions truncate to 32-bit which destroys 64-bit pointers on PC.
#ifndef OS_K0_TO_PHYSICAL
#define OS_K0_TO_PHYSICAL(x) ((uintptr_t)(x))
#endif

#ifndef OS_PHYSICAL_TO_K0
#define OS_PHYSICAL_TO_K0(x) ((void*)(uintptr_t)(x))
#endif

// OS_TV types
#ifndef OS_TV_PAL
#define OS_TV_PAL    0
#define OS_TV_NTSC   1
#define OS_TV_MPAL   2
#endif

// VI special features flags
#ifndef OS_VI_GAMMA_OFF
#define OS_VI_GAMMA_ON          0x0001
#define OS_VI_GAMMA_OFF         0x0002
#define OS_VI_GAMMA_DITHER_ON   0x0004
#define OS_VI_GAMMA_DITHER_OFF  0x0008
#define OS_VI_DIVOT_ON          0x0010
#define OS_VI_DIVOT_OFF         0x0020
#define OS_VI_DITHER_FILTER_ON  0x0040
#define OS_VI_DITHER_FILTER_OFF 0x0080
#endif

// OS task flags
#ifndef OS_TASK_YIELEDE
#define OS_TASK_YIELEDE     0x0001
#define OS_TASK_DP_WAIT     0x0002
#define OS_TASK_LOADABLE    0x0004
#define OS_TASK_SP_ONLY     0x0008
#endif

// PFS (pak file system) defines
#ifndef PFS_READ
#define PFS_READ    0
#define PFS_WRITE   1
#endif

// OS yield data size
#ifndef OS_YIELD_DATA_SIZE
#define OS_YIELD_DATA_SIZE  0xC00
#endif

// RSP microcode data size
#ifndef SP_UCODE_DATA_SIZE
#define SP_UCODE_DATA_SIZE 2048
#endif

// GBPak power modes
#ifndef OS_GBPAK_POWER_OFF
#define OS_GBPAK_POWER_OFF  0
#define OS_GBPAK_POWER_ON   1
#endif

// OS read/write
#ifndef OS_READ
#define OS_READ     0
#define OS_WRITE    1
#endif

// MAXCONTROLLERS — libultraship os.h already defines this, guard it
#ifndef MAXCONTROLLERS
#define MAXCONTROLLERS  4
#endif

// Screen dimensions (N64 native)
#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   240
#endif

#ifdef __cplusplus
extern "C" {
#endif

// VI mode globals (stubs on PC, defined in os_stubs.c)
extern s32 osTvType;
extern OSViMode osViModeNtscLan1;
extern OSViMode osViModeMpalLan1;
extern OSViMode osViModeFpalLpn1;

// ============================================================
// Functions NOT provided by libultraship (declared & stubbed by us)
// ============================================================

void* osViGetCurrentFramebuffer(void);
void* osVirtualToPhysical(void* addr);

// Flash functions (Paper Mario specific — stubbed for PC)
void osFlashInit(void);
s32 osFlashReadArray(void* mb, s32 priority, u32 pageNum, void* dramAddr, u32 nPages, void* mq);
s32 osFlashWriteArray(u32 pageNum);
s32 osFlashWriteBuffer(void* mb, s32 priority, void* dramAddr, void* mq);
s32 osFlashSectorErase(u32 pageNum);
void osFlashClearStatus(void);
u32 osFlashGetAddr(s32 pageNum);
s32 osFlashReadId(u32* type, u32* maker);
s32 osFlashReadStatus(u8* flashStatus);
void osFlashAllErase(void);
void osFlashAllEraseThrough(void);
void osFlashSectorEraseThrough(s32 pageNum);

// AI status (not in libultraship)
u32 osAiGetStatus(void);

// Thread operations (not in libultraship os.h)
void osCreateThread(OSThread* thread, OSId id, void (*entry)(void*),
                    void* arg, void* sp, OSPri pri);
void osStartThread(OSThread* thread);
void osStopThread(OSThread* thread);
void osDestroyThread(OSThread* thread);
void osSetThreadPri(OSThread* thread, OSPri pri);
OSPri osGetThreadPri(OSThread* thread);
void osYieldThread(void);

// Message queue (not in libultraship os.h)
void osCreateMesgQueue(OSMesgQueue* mq, OSMesg* msg, s32 count);
s32 osSendMesg(OSMesgQueue* mq, OSMesg msg, s32 flag);
s32 osRecvMesg(OSMesgQueue* mq, OSMesg* msg, s32 flag);
s32 osJamMesg(OSMesgQueue* mq, OSMesg msg, s32 flag);

// Events (not in libultraship)
void osSetEventMesg(OSEvent e, OSMesgQueue* mq, OSMesg msg);

// SP (RSP) — not in libultraship os.h
void osSpTaskLoad(OSTask* tp);
void osSpTaskStartGo(OSTask* tp);
void osSpTaskYield(void);
OSYieldResult osSpTaskYielded(OSTask* tp);

// Interrupts
// Note: osSetIntMask is declared by libultraship's exception.h (do NOT re-declare here
// to avoid C/C++ linkage conflicts)
u32 __osDisableInt(void);
void __osRestoreInt(u32 mask);
u32 __osGetCause(void);

// SI raw DMA
s32 __osSiRawStartDma(s32 direction, void* dramAddr);

// N64 fixed-point trig functions (not in libultraship)
s16 sins(u16 angle);
s16 coss(u16 angle);

// EPI extra functions (osEPiReadIo, osEPiWriteIo not in libultraship)
s32 osEPiReadIo(OSPiHandle* pihandle, u32 devAddr, u32* data);
s32 osEPiWriteIo(OSPiHandle* pihandle, u32 devAddr, u32 data);

// N64 Voice Recognition types (not used by Paper Mario, needed by nusys.h)
typedef struct {
    OSMesgQueue* __mq;
    int __channel;
    s32 __mode;
    u8 cmd_status;
} OSVoiceHandle;

typedef struct {
    u16 warning;
    u16 answer_num;
    u16 voice_level;
    u16 voice_sn;
    u16 voice_time;
    u16 answer[5];
    u16 distance[5];
} OSVoiceData;

// GB Pak types (not used by Paper Mario, needed by nusys.h)
typedef struct {
    u16 fixed_code;
    u16 game_title[16];
    u16 company_code;
    u16 body_code;
    u8  cart_type;
    u8  rom_size;
    u8  ram_size;
    u8  country_code;
    u8  company_code_old;
    u8  version;
    u8  isum;
    u16 sum;
} OSGbpakId;

// TLB operations (not in libultraship)
typedef u32 OSPageMask;
#ifndef OS_PM_4K
#define OS_PM_4K    0x0000000
#define OS_PM_16K   0x0006000
#define OS_PM_64K   0x001E000
#define OS_PM_256K  0x007E000
#define OS_PM_1M    0x01FE000
#define OS_PM_4M    0x07FE000
#define OS_PM_16M   0x1FFE000
#endif
s32 osMapTLB(s32 index, OSPageMask pm, void* vaddr, u32 evenpaddr, u32 oddpaddr, s32 asid);
void osUnmapTLB(s32 index);
void osUnmapTLBAll(void);

#ifdef __cplusplus
}
#endif

#endif /* _ULTRA64_H_ */
