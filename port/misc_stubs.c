/**
 * misc_stubs.c - Stub implementations for miscellaneous undefined symbols.
 *
 * These are functions/data that need real implementations eventually,
 * but are stubbed here to achieve a linkable binary.
 */

#include "common.h"

#include "nu/nusys.h"
#include "libc/xstdio.h"
#include "../port/endian.h"

/* ------------------------------------------------------------------ */
/* Crash screen — N64 debug feature, not needed on PC                  */
/* ------------------------------------------------------------------ */

void crash_screen_init(void) {
    /* N64 crash handler — unused on PC, we use standard crash handling */
}

void crash_screen_set_draw_info(u16* frameBufPtr, s16 width, s16 height) {
    (void)frameBufPtr; (void)width; (void)height;
}

/* ------------------------------------------------------------------ */
/* decode_yay0 — Yay0 decompression                                    */
/*                                                                      */
/* ROM data is big-endian. This decoder reads multi-byte values         */
/* explicitly in big-endian byte order so it works on any platform.     */
/* The decompressed output is raw bytes (no endianness issue).          */
/* ------------------------------------------------------------------ */

void decode_yay0(void* src, void* dst) {
    u8* srcBytes = (u8*)src;

    /* Header: 4 big-endian u32 values */
    /* u32 magic = read_be_u32(srcBytes + 0); */ /* "Yay0" = 0x59617930 */
    u32 decompSize = read_be_u32(srcBytes + 4);
    u32 linkOffset = read_be_u32(srcBytes + 8);
    u32 chunkOffset = read_be_u32(srcBytes + 12);

    /* Three separate data streams */
    u8* bitstreamPos = srcBytes + 16;     /* u32 words (big-endian), MSB first */
    u8* linkPos = srcBytes + linkOffset;  /* u16 entries (big-endian) */
    u8* chunkPos = srcBytes + chunkOffset; /* raw u8 bytes */

    u8* dstPos = (u8*)dst;
    u8* dstEnd = dstPos + decompSize;

    u32 bits = 0;
    s32 bitsLeft = 0;

    while (dstPos < dstEnd) {
        if (bitsLeft == 0) {
            bits = read_be_u32(bitstreamPos);
            bitstreamPos += 4;
            bitsLeft = 32;
        }

        if (bits & 0x80000000) {
            /* Bit=1: literal byte from chunk stream */
            *dstPos++ = *chunkPos++;
        } else {
            /* Bit=0: backreference from link stream */
            u16 link = read_be_u16(linkPos);
            linkPos += 2;

            s32 dist = (link & 0x0FFF) + 1;
            s32 count = link >> 12;

            if (count == 0) {
                /* Extended count: read extra byte from chunk stream */
                count = *chunkPos++ + 18;
            } else {
                count += 2;
            }

            u8* copyFrom = dstPos - dist;
            s32 i;
            for (i = 0; i < count; i++) {
                *dstPos++ = *copyFrom++;
            }
        }

        bits <<= 1;
        bitsLeft--;
    }
}

/* ------------------------------------------------------------------ */
/* Printf — N64 SDK printf (used by libc/xstdio.h)                    */
/* The real signature is: int _Printf(outfun, char*, const char*, va_list) */
/* ------------------------------------------------------------------ */

int _Printf(outfun prout, char *arg, const char *fmt, va_list args) {
    (void)prout; (void)arg; (void)fmt; (void)args;
    return 0;
}

/* ------------------------------------------------------------------ */
/* NuSystem scheduler / framebuffer globals                            */
/* ------------------------------------------------------------------ */

/* nuGfxCfbNum — number of frame buffers (declared as u32 in nusys.h) */
u32 nuGfxCfbNum = 2;

/* nusched — NuSystem scheduler (declared as NUSched in nusys.h) */
NUSched nusched = {0};

/* ------------------------------------------------------------------ */
/* osAiSetFrequency — Audio interface frequency                        */
/* libultraship declares it but may not define it                      */
/* ------------------------------------------------------------------ */

s32 osAiSetFrequency(u32 frequency) {
    (void)frequency;
    return (s32)frequency;
}

/* ------------------------------------------------------------------ */
/* Obfuscated function wrappers                                        */
/* In the original ROM, these are anti-tamper checksums.               */
/* In SHIFT/PORT mode, they just call through to the real functions.    */
/* ------------------------------------------------------------------ */

extern HeapNode* general_heap_create(void);
extern void create_audio_system(void);
extern s32 battle_heap_create(void);

void obfuscated_general_heap_create(void) {
    general_heap_create();
}

void obfuscated_create_audio_system(void) {
    create_audio_system();
}

void obfuscated_battle_heap_create(void) {
    battle_heap_create();
}

/* ------------------------------------------------------------------ */
/* func_8004D8E0 — Unknown function referenced by PiranhaPlantAI      */
/* Likely a NPC AI helper; stub for now                                */
/* ------------------------------------------------------------------ */

void func_8004D8E0(void) {
    /* TODO: Identify and implement */
}

/* ------------------------------------------------------------------ */
/* D_80* data symbols                                                  */
/* These are BSS/data addresses referenced by various subsystems       */
/* ------------------------------------------------------------------ */

u8 D_8021CF40_5A3620[0x100] = {0};  /* lava_piranha data */
u8 D_8024A200[512] = {0};            /* filemenu copyarrow: IA4 64x16 = 512 bytes, ROM 0x16AA60 */
u8 D_8024A400[4096] = {0};           /* filemenu corners_yellow: RGBA32 16x64 = 4096 bytes, ROM 0x16AC60 */
u8 D_8024B400[512] = {0};            /* filemenu corners_gray: IA8 16x32 = 512 bytes, ROM 0x16BC60 */
u8 D_802E9170[0x80] = {0};           /* Shadow entity data */
u8 D_802E91F0[0x80] = {0};           /* Shadow entity data */

/* n_aspMainTextStart and n_aspMainDataStart are in asset_data_stubs.c */
