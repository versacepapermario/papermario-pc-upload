/**
 * endian.h - Big-endian to native byte-swap helpers for PC port
 *
 * .z64 ROM data is big-endian. On PC (little-endian), multi-byte integers
 * read from ROM buffers must be byte-swapped. Individual bytes don't need swapping.
 */

#ifndef PORT_ENDIAN_H
#define PORT_ENDIAN_H

#include "ultra64.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Read a big-endian u32 from a byte pointer */
static inline u32 read_be_u32(const void* p) {
    const u8* b = (const u8*)p;
    return ((u32)b[0] << 24) | ((u32)b[1] << 16) | ((u32)b[2] << 8) | (u32)b[3];
}

/** Read a big-endian s32 from a byte pointer */
static inline s32 read_be_s32(const void* p) {
    return (s32)read_be_u32(p);
}

/** Read a big-endian u16 from a byte pointer */
static inline u16 read_be_u16(const void* p) {
    const u8* b = (const u8*)p;
    return ((u16)b[0] << 8) | (u16)b[1];
}

/** Read a big-endian f32 from a byte pointer */
static inline f32 read_be_f32(const void* p) {
    u32 bits = read_be_u32(p);
    f32 result;
    memcpy(&result, &bits, sizeof(f32));
    return result;
}

/** Swap a native u32 from big-endian byte order */
static inline u32 bswap32(u32 val) {
    return ((val >> 24) & 0xFF) | ((val >> 8) & 0xFF00) |
           ((val << 8) & 0xFF0000) | ((val << 24) & 0xFF000000u);
}

/** Swap a native u16 from big-endian byte order */
static inline u16 bswap16(u16 val) {
    return (val >> 8) | (val << 8);
}

/**
 * Byte-swap an array of u32 values in-place (big-endian to native).
 * Use after nuPiReadRom when reading s32/u32 arrays or structs of s32 fields.
 */
static inline void bswap32_array(void* buf, u32 count) {
    u32* words = (u32*)buf;
    for (u32 i = 0; i < count; i++) {
        words[i] = bswap32(words[i]);
    }
}

#ifdef __cplusplus
}
#endif

#endif /* PORT_ENDIAN_H */
