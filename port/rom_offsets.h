/**
 * rom_offsets.h - ROM offset lookup and byte-swap utilities for PC port
 *
 * On N64, ROM segment addresses (logos_ROM_START, etc.) are linker-generated
 * and contain actual ROM offsets. On PC, they're zero-length stub arrays.
 * This module provides a runtime lookup: given a stub address, returns
 * the real ROM offset from the US ROM.
 *
 * Also provides byte-swap macros for big-endian ROM data on little-endian PC.
 */

#ifndef ROM_OFFSETS_H
#define ROM_OFFSETS_H

#ifdef PORT

#include "ultra64.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Resolve a linker stub address to its actual ROM offset.
 * Returns the ROM offset, or 0xFFFFFFFF if not found.
 */
u32 resolve_rom_offset(const void* stub_addr);

/**
 * Byte-swap macros for big-endian ROM data on little-endian PC.
 * N64 ROM data is big-endian. On PC (little-endian), multi-byte fields
 * read from ROM must be byte-swapped.
 */
#if defined(__GNUC__) || defined(__clang__)
#define ROM_BSWAP32(x) __builtin_bswap32(x)
#define ROM_BSWAP16(x) __builtin_bswap16(x)
#else
#define ROM_BSWAP32(x) (((x) >> 24) | (((x) >> 8) & 0xFF00) | (((x) << 8) & 0xFF0000) | ((x) << 24))
#define ROM_BSWAP16(x) (((x) >> 8) | ((x) << 8))
#endif

#ifdef __cplusplus
}
#endif

#endif // PORT
#endif // ROM_OFFSETS_H
