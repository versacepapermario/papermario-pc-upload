/**
 * shape_swizzle.c - Convert N64 shape file binary data to PC structs
 *
 * Shape files are decompressed from ROM and contain N64 structs with 32-bit
 * big-endian pointers pre-resolved to 0x80210000 (the N64 address of gMapShapeData).
 * On 64-bit PC, these structs have different sizes (8-byte pointers, alignment padding).
 * This module rebuilds the model tree from N64 binary into proper PC structs.
 *
 * Conversions performed:
 *   - Display lists: N64 8-byte commands → PC 16-byte commands (uintptr_t words)
 *   - Vertices: N64 16-byte Vtx_t (s16 positions) → PC 24-byte Vtx_t (float positions)
 *   - Matrices: N64 fixed-point Mtx → PC float MtxF
 *   - All N64 VRAM pointers rebased to PC buffer addresses
 */

#include "common.h"
#include "model.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "port/endian.h"

// N64 address where gMapShapeData is loaded (from decomp's undefined_syms.txt)
#define N64_SHAPE_BASE 0x80210000

// N64 struct sizes (fixed binary layout from ROM)
#define N64_MODEL_NODE_SIZE    0x14
#define N64_GROUP_DATA_SIZE    0x14
#define N64_DISPLAY_DATA_SIZE  0x08
#define N64_PROPERTY_SIZE      0x0C
#define N64_VTX_SIZE           16   // s16 ob[3], u16 flag, s16 tc[2], u8 cn[4]
#define N64_GFX_SIZE           8    // u32 w0, u32 w1
#define N64_MTX_SIZE           64   // 16 x s32 (fixed-point format)

// F3DEX2 GBI opcodes (for parsing N64 display lists)
#define F3DEX2_G_VTX      0x01
#define F3DEX2_G_TRI1     0x05
#define F3DEX2_G_TRI2     0x06
#define F3DEX2_G_MTX      0xDA
#define F3DEX2_G_MOVEWORD 0xDB
#define F3DEX2_G_MOVEMEM  0xDC
#define F3DEX2_G_DL       0xDE
#define F3DEX2_G_ENDDL    0xDF
#define F3DEX2_G_SETTIMG  0xFD
#define F3DEX2_G_SETCIMG  0xFF

// Convert N64 virtual address to offset in buffer, return pointer into buffer
static inline void* n64_to_buf(u8* buf, u32 n64_addr) {
    if (n64_addr == 0) return NULL;
    u32 offset = n64_addr - N64_SHAPE_BASE;
    return buf + offset;
}

// Check if an N64 address is within the shape buffer
static inline int n64_addr_valid(u32 n64_addr, u32 bufSize) {
    if (n64_addr == 0) return 0;
    if (n64_addr < N64_SHAPE_BASE) return 0;
    u32 offset = n64_addr - N64_SHAPE_BASE;
    return offset < bufSize;
}

// ================================================================
// Vertex conversion: N64 Vtx_t (16 bytes) → PC Vtx (24 bytes with GBI_FLOATS)
// ================================================================

/**
 * Convert N64 big-endian vertex data to PC format.
 * N64 Vtx_t: s16 ob[3], u16 flag, s16 tc[2], u8 cn[4] = 16 bytes
 * PC Vtx_t (GBI_FLOATS): float ob[3], u16 flag, s16 tc[2], u8 cn[4] = 24 bytes
 */
static Vtx* convert_n64_vertices(u8* buf, u32 n64_addr, s32 count, u32 bufSize) {
    if (!n64_addr_valid(n64_addr, bufSize) || count <= 0) return NULL;

    u32 offset = n64_addr - N64_SHAPE_BASE;
    if (offset + count * N64_VTX_SIZE > bufSize) {
        fprintf(stderr, "[shape_swizzle] vtx offset 0x%X + %d verts exceeds buffer (0x%X)\n",
                offset, count, bufSize);
        return NULL;
    }

    Vtx* pcVerts = malloc(count * sizeof(Vtx));
    ASSERT(pcVerts != NULL);
    memset(pcVerts, 0, count * sizeof(Vtx));

    u8* src = buf + offset;
    for (s32 i = 0; i < count; i++) {
        u8* v = src + i * N64_VTX_SIZE;

        // Position: s16 big-endian → float
        s16 x = (s16)read_be_u16(v + 0);
        s16 y = (s16)read_be_u16(v + 2);
        s16 z = (s16)read_be_u16(v + 4);
        pcVerts[i].v.ob[0] = (float)x;
        pcVerts[i].v.ob[1] = (float)y;
        pcVerts[i].v.ob[2] = (float)z;

        // Flag: u16 big-endian
        pcVerts[i].v.flag = read_be_u16(v + 6);

        // Texture coords: s16 big-endian
        pcVerts[i].v.tc[0] = (s16)read_be_u16(v + 8);
        pcVerts[i].v.tc[1] = (s16)read_be_u16(v + 10);

        // Color/normal: 4 × u8 (no swap needed)
        pcVerts[i].v.cn[0] = v[12];
        pcVerts[i].v.cn[1] = v[13];
        pcVerts[i].v.cn[2] = v[14];
        pcVerts[i].v.cn[3] = v[15];
    }

    return pcVerts;
}

// ================================================================
// Matrix conversion: N64 fixed-point Mtx → PC float MtxF
// ================================================================

/**
 * Convert N64 big-endian fixed-point matrix to PC float matrix.
 * N64 MtxS: int m[4][4] where m[0..1] = integer parts, m[2..3] = fractional parts
 * PC MtxF (GBI_FLOATS): float mf[4][4]
 */
static Mtx* convert_n64_matrix(u8* buf, u32 n64_addr, u32 bufSize) {
    if (!n64_addr_valid(n64_addr, bufSize)) return NULL;

    u32 offset = n64_addr - N64_SHAPE_BASE;
    if (offset + N64_MTX_SIZE > bufSize) return NULL;

    u8* src = buf + offset;

    // Read 16 big-endian s32 values
    u32 raw[16];
    for (int i = 0; i < 16; i++) {
        raw[i] = read_be_u32(src + i * 4);
    }

    // Allocate PC float matrix
    Mtx* mtx = malloc(sizeof(Mtx));
    ASSERT(mtx != NULL);

    // Convert N64 fixed-point to float using same algorithm as guMtxL2F
    // m[0][0..3] and m[1][0..3] = integer part halves
    // m[2][0..3] and m[3][0..3] = fractional part halves
    u32* m1 = &raw[0];  // m[0] and m[1] — 8 values
    u32* m2 = &raw[8];  // m[2] and m[3] — 8 values
    float (*mf)[4] = (float (*)[4])mtx;

    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 2; c++) {
            u32 intPair = *m1;
            u32 fracPair = *m2;
            u32 tmp1 = (intPair & 0xffff0000) | ((fracPair >> 16) & 0xffff);
            u32 tmp2 = ((intPair << 16) & 0xffff0000) | (fracPair & 0xffff);
            mf[r][c * 2 + 0] = (s32)tmp1 / 65536.0f;
            mf[r][c * 2 + 1] = (s32)tmp2 / 65536.0f;
            m1++;
            m2++;
        }
    }

    return mtx;
}

// ================================================================
// Display list conversion: N64 8-byte Gfx → PC 16-byte Gfx
// ================================================================

// Track converted DLs and vertices to avoid double conversion
#define MAX_CONVERTED_DLS  2048
#define MAX_CONVERTED_VTXS 4096

static struct { u32 n64_addr; Gfx* pc_dl; } sConvertedDLs[MAX_CONVERTED_DLS];
static int sConvertedDLCount = 0;

static struct { u32 n64_addr; Vtx* pc_vtx; s32 count; } sConvertedVtxs[MAX_CONVERTED_VTXS];
static int sConvertedVtxCount = 0;

static Gfx* find_converted_dl(u32 n64_addr) {
    for (int i = 0; i < sConvertedDLCount; i++) {
        if (sConvertedDLs[i].n64_addr == n64_addr) return sConvertedDLs[i].pc_dl;
    }
    return NULL;
}

// Find cached vtx entry with the largest count for this address
static int find_converted_vtx_index(u32 n64_addr) {
    int bestIdx = -1;
    for (int i = 0; i < sConvertedVtxCount; i++) {
        if (sConvertedVtxs[i].n64_addr == n64_addr) {
            if (bestIdx < 0 || sConvertedVtxs[i].count > sConvertedVtxs[bestIdx].count)
                bestIdx = i;
        }
    }
    return bestIdx;
}

/**
 * Count commands in an N64 display list (terminated by G_ENDDL or gSPBranchList).
 * A gSPBranchList (DE00) replaces the current DL on the interpreter stack without
 * pushing, so nothing after it in this DL will ever execute. Treat it as a terminator
 * to avoid reading past the branch into non-DL data.
 * Returns -1 if list is malformed.
 */
static s32 count_n64_dl_commands(u8* buf, u32 n64_addr, u32 bufSize) {
    if (!n64_addr_valid(n64_addr, bufSize)) return -1;

    u32 offset = n64_addr - N64_SHAPE_BASE;
    s32 count = 0;
    s32 maxCmds = (bufSize - offset) / N64_GFX_SIZE;
    if (maxCmds > 4096) maxCmds = 4096; // safety limit

    for (s32 i = 0; i < maxCmds; i++) {
        u8* cmd = buf + offset + i * N64_GFX_SIZE;
        u32 w0 = read_be_u32(cmd);
        u8 opcode = (w0 >> 24) & 0xFF;
        count++;
        if (opcode == F3DEX2_G_ENDDL) break;
        // gSPBranchList (G_DL with nopush) replaces the current stack entry,
        // so the interpreter will never execute commands after it in this DL.
        // Stop counting to avoid including non-DL data in the allocation.
        if (opcode == F3DEX2_G_DL) {
            u8 pushFlag = (w0 >> 16) & 0xFF;
            if (pushFlag != 0) break; // G_DL_NOPUSH=1 = branch = effective terminator
        }
    }

    return count;
}

/**
 * Convert an N64 display list to PC format.
 * Recursively converts sub-DLs and vertex arrays.
 */
static Gfx* convert_n64_display_list(u8* buf, u32 n64_addr, u32 bufSize) {
    if (!n64_addr_valid(n64_addr, bufSize)) return NULL;

    // Check if already converted
    Gfx* existing = find_converted_dl(n64_addr);
    if (existing != NULL) return existing;

    // Count commands
    s32 cmdCount = count_n64_dl_commands(buf, n64_addr, bufSize);
    if (cmdCount <= 0) return NULL;

    // Allocate PC display list (+1 for safety G_ENDDL sentinel)
    Gfx* pcDL = malloc((cmdCount + 1) * sizeof(Gfx));
    ASSERT(pcDL != NULL);
    memset(pcDL, 0, (cmdCount + 1) * sizeof(Gfx));
    // Write G_ENDDL sentinel past the last command as a safety net.
    // If the interpreter somehow reads past the counted commands, it will
    // hit this terminator instead of heap garbage.
    pcDL[cmdCount].words.w0 = (uintptr_t)0xDF000000;
    pcDL[cmdCount].words.w1 = 0;

    // Register before converting (to handle recursive refs — prevents infinite recursion)
    if (sConvertedDLCount < MAX_CONVERTED_DLS) {
        sConvertedDLs[sConvertedDLCount].n64_addr = n64_addr;
        sConvertedDLs[sConvertedDLCount].pc_dl = pcDL;
        sConvertedDLCount++;
    } else {
        fprintf(stderr, "[shape_swizzle] WARNING: DL cache full (%d), addr 0x%08X not cached — risk of infinite recursion!\n",
                MAX_CONVERTED_DLS, n64_addr);
    }

    u32 offset = n64_addr - N64_SHAPE_BASE;

    for (s32 i = 0; i < cmdCount; i++) {
        u8* cmd = buf + offset + i * N64_GFX_SIZE;
        u32 w0 = read_be_u32(cmd);
        u32 w1 = read_be_u32(cmd + 4);
        u8 opcode = (w0 >> 24) & 0xFF;

        switch (opcode) {
            case F3DEX2_G_VTX: {
                // w0: 0x01 | (n << 12) | ((v0+n) << 1)
                // w1: N64 vertex address
                s32 numVerts = (w0 >> 12) & 0xFF;
                u32 n64_vtxAddr = w1;

                // Convert vertices — always allocate per-command to avoid cache
                // count mismatches where a smaller allocation gets reused for a
                // larger vertex load, causing heap overread.
                // Reuse only when the cached allocation has enough vertices.
                int vtxIdx = find_converted_vtx_index(n64_vtxAddr);
                Vtx* pcVtx = NULL;
                if (vtxIdx >= 0 && sConvertedVtxs[vtxIdx].count >= numVerts) {
                    pcVtx = sConvertedVtxs[vtxIdx].pc_vtx;
                } else {
                    pcVtx = convert_n64_vertices(buf, n64_vtxAddr, numVerts, bufSize);
                    if (pcVtx != NULL && sConvertedVtxCount < MAX_CONVERTED_VTXS) {
                        // Store as new entry (keep old entry alive for other DLs)
                        sConvertedVtxs[sConvertedVtxCount].n64_addr = n64_vtxAddr;
                        sConvertedVtxs[sConvertedVtxCount].pc_vtx = pcVtx;
                        sConvertedVtxs[sConvertedVtxCount].count = numVerts;
                        sConvertedVtxCount++;
                    }
                }

                pcDL[i].words.w0 = (uintptr_t)w0;
                pcDL[i].words.w1 = (uintptr_t)pcVtx;
                break;
            }

            case F3DEX2_G_MTX: {
                // w1: N64 matrix address → convert to PC float matrix
                u32 n64_mtxAddr = w1;
                Mtx* pcMtx = NULL;
                if (n64_addr_valid(n64_mtxAddr, bufSize)) {
                    pcMtx = convert_n64_matrix(buf, n64_mtxAddr, bufSize);
                }
                pcDL[i].words.w0 = (uintptr_t)w0;
                pcDL[i].words.w1 = (uintptr_t)pcMtx;
                break;
            }

            case F3DEX2_G_MOVEMEM: {
                // w1: N64 address of data (lights, viewport, etc.)
                u32 n64_dataAddr = w1;
                void* pcData = NULL;
                if (n64_addr_valid(n64_dataAddr, bufSize)) {
                    pcData = n64_to_buf(buf, n64_dataAddr);
                }
                pcDL[i].words.w0 = (uintptr_t)w0;
                pcDL[i].words.w1 = (uintptr_t)(pcData ? pcData : (void*)(uintptr_t)w1);
                break;
            }

            case F3DEX2_G_DL: {
                // w1: N64 sub-DL address
                u32 n64_subDL = w1;
                Gfx* pcSubDL = NULL;
                if (n64_addr_valid(n64_subDL, bufSize)) {
                    pcSubDL = convert_n64_display_list(buf, n64_subDL, bufSize);
                    if (pcSubDL != NULL) {
                        // Verify the converted DL doesn't point into the raw buffer
                        if ((u8*)pcSubDL >= buf && (u8*)pcSubDL < buf + bufSize) {
                            fprintf(stderr, "[shape_swizzle] BUG! G_DL converted sub-DL %p is inside raw buf [%p..%p] "
                                    "(n64=0x%08X offset=0x%X)\n",
                                    (void*)pcSubDL, (void*)buf, (void*)(buf+bufSize),
                                    n64_subDL, n64_subDL - N64_SHAPE_BASE);
                        }
                    }
                } else if (n64_subDL != 0) {
                    static int sOutOfBoundsDlCount = 0;
                    if (sOutOfBoundsDlCount < 10) {
                        u8 pushFlag = (w0 >> 16) & 0xFF;
                        fprintf(stderr, "[shape_swizzle] G_DL references addr 0x%08X outside shape buffer "
                                "(base=0x%08X size=0x%X push=%d) — setting to NULL\n",
                                n64_subDL, N64_SHAPE_BASE, bufSize, pushFlag);
                        sOutOfBoundsDlCount++;
                    }
                }
                pcDL[i].words.w0 = (uintptr_t)w0;
                pcDL[i].words.w1 = (uintptr_t)pcSubDL;
                break;
            }

            case F3DEX2_G_SETTIMG:
            case F3DEX2_G_SETCIMG: {
                // w1: N64 texture/color image address
                // Only rebase if it points within the shape buffer
                u32 n64_imgAddr = w1;
                if (n64_addr_valid(n64_imgAddr, bufSize)) {
                    void* pcImg = n64_to_buf(buf, n64_imgAddr);
                    pcDL[i].words.w0 = (uintptr_t)w0;
                    pcDL[i].words.w1 = (uintptr_t)pcImg;
                } else {
                    // Points outside shape buffer (texture loaded separately)
                    // Keep as-is — runtime code or texture system will set this
                    pcDL[i].words.w0 = (uintptr_t)w0;
                    pcDL[i].words.w1 = (uintptr_t)w1;
                }
                break;
            }

            default:
                // All other commands: copy w0/w1 as-is (no pointer rebasing needed)
                // This covers triangles, texture settings, render mode, etc.
                pcDL[i].words.w0 = (uintptr_t)w0;
                pcDL[i].words.w1 = (uintptr_t)w1;
                break;
        }
    }

    return pcDL;
}

// ================================================================
// Model tree swizzle (recursive)
// ================================================================

// Forward declarations
static ModelNode* swizzle_node(u8* buf, u32 n64_addr, u32 bufSize);

static ModelGroupData* swizzle_group_data(u8* buf, u32 n64_addr, u32 bufSize) {
    if (n64_addr == 0) return NULL;

    u32 offset = n64_addr - N64_SHAPE_BASE;
    if (offset + N64_GROUP_DATA_SIZE > bufSize) {
        fprintf(stderr, "[shape_swizzle] group data offset 0x%X out of bounds (size=0x%X)\n", offset, bufSize);
        return NULL;
    }

    u8* src = buf + offset;

    ModelGroupData* gd = malloc(sizeof(ModelGroupData));
    ASSERT(gd != NULL);

    u32 n64_transformMatrix = read_be_u32(src + 0x00);
    u32 n64_lightingGroup   = read_be_u32(src + 0x04);
    gd->numLights           = read_be_s32(src + 0x08);
    gd->numChildren         = read_be_s32(src + 0x0C);
    u32 n64_childList       = read_be_u32(src + 0x10);

    // Convert transform matrix from N64 fixed-point to PC float
    gd->transformMatrix = convert_n64_matrix(buf, n64_transformMatrix, bufSize);

    // Lighting group: all single-byte fields (col, dir) — no byte-swap needed
    // Just rebase the pointer into the buffer
    if (n64_lightingGroup != 0 && n64_addr_valid(n64_lightingGroup, bufSize)) {
        gd->lightingGroup = (Lightsn*)n64_to_buf(buf, n64_lightingGroup);
        // NOTE: Previously forced zero-ambient to white to prevent black models.
        // Removed — zero ambient is intentional for dark rooms (e.g. Dry Dry Ruins).
        // Maps that need white passthrough should use vertex colors with G_LIGHTING off.
    } else {
        if (n64_lightingGroup != 0) {
            fprintf(stderr, "[shape_swizzle] lightingGroup 0x%08X out of bounds (bufSize=0x%X), nLights=%d\n",
                    n64_lightingGroup, bufSize, gd->numLights);
        }
        gd->lightingGroup = NULL;
    }

    // Child list: array of N64 pointers to ModelNode
    if (n64_childList != 0 && gd->numChildren > 0 && n64_addr_valid(n64_childList, bufSize)) {
        u32 clOffset = n64_childList - N64_SHAPE_BASE;
        if (clOffset + gd->numChildren * 4 > bufSize) {
            fprintf(stderr, "[shape_swizzle] child list at 0x%X overflows buffer (need %d entries)\n",
                    clOffset, gd->numChildren);
            gd->childList = NULL;
        } else {
            gd->childList = malloc(gd->numChildren * sizeof(ModelNode*));
            ASSERT(gd->childList != NULL);

            u8* clSrc = buf + clOffset;
            for (s32 i = 0; i < gd->numChildren; i++) {
                u32 n64_child = read_be_u32(clSrc + i * 4);
                gd->childList[i] = swizzle_node(buf, n64_child, bufSize);
            }
        }
    } else {
        gd->childList = NULL;
    }

    return gd;
}

static ModelNode* swizzle_node(u8* buf, u32 n64_addr, u32 bufSize) {
    if (n64_addr == 0) return NULL;

    u32 offset = n64_addr - N64_SHAPE_BASE;
    if (offset + N64_MODEL_NODE_SIZE > bufSize) {
        fprintf(stderr, "[shape_swizzle] node offset 0x%X out of bounds (size=0x%X)\n", offset, bufSize);
        return NULL;
    }

    u8* src = buf + offset;

    ModelNode* node = malloc(sizeof(ModelNode));
    ASSERT(node != NULL);

    node->type          = read_be_s32(src + 0x00);
    u32 n64_displayData = read_be_u32(src + 0x04);
    node->numProperties = read_be_s32(src + 0x08);
    u32 n64_propertyList = read_be_u32(src + 0x0C);
    u32 n64_groupData    = read_be_u32(src + 0x10);

    // Display data: contains a Gfx* display list pointer + 4 bytes extra data
    if (n64_displayData != 0) {
        u32 ddOffset = n64_displayData - N64_SHAPE_BASE;
        if (ddOffset + N64_DISPLAY_DATA_SIZE <= bufSize) {
            u8* ddSrc = buf + ddOffset;
            ModelDisplayData* dd = malloc(sizeof(ModelDisplayData));
            ASSERT(dd != NULL);

            u32 n64_displayList = read_be_u32(ddSrc + 0x00);

            // Convert N64 display list to PC format
            dd->displayList = convert_n64_display_list(buf, n64_displayList, bufSize);

            memcpy(dd->unk_04, ddSrc + 0x04, 4);
            node->displayData = dd;
        } else {
            node->displayData = NULL;
        }
    } else {
        node->displayData = NULL;
    }

    // Property list: array of {key, dataType, data} — each 0x0C bytes in N64
    if (n64_propertyList != 0 && node->numProperties > 0 && n64_addr_valid(n64_propertyList, bufSize)) {
        u32 plOffset = n64_propertyList - N64_SHAPE_BASE;
        s32 numProps = node->numProperties;
        if (plOffset + numProps * N64_PROPERTY_SIZE > bufSize) {
            fprintf(stderr, "[shape_swizzle] property list at 0x%X overflows buffer (%d props)\n",
                    plOffset, numProps);
            node->propertyList = NULL;
        } else {
        ModelNodeProperty* props = malloc(numProps * sizeof(ModelNodeProperty));
        ASSERT(props != NULL);

        for (s32 i = 0; i < numProps; i++) {
            u8* pSrc = buf + plOffset + i * N64_PROPERTY_SIZE;
            props[i].key      = read_be_s32(pSrc + 0x00);
            props[i].dataType = read_be_s32(pSrc + 0x04);

            u32 rawData = read_be_u32(pSrc + 0x08);

            // Texture name property has a string pointer; others are scalar values
            if (props[i].key == MODEL_PROP_KEY_TEXTURE_NAME) {
                props[i].data.p = n64_to_buf(buf, rawData);
            } else {
                // For s32/f32 values, store the byte-swapped value.
                props[i].data.s = (s32)rawData;
            }
        }
        node->propertyList = props;
        }
    } else {
        node->propertyList = NULL;
    }

    // Group data (recursive — contains child list)
    node->groupData = swizzle_group_data(buf, n64_groupData, bufSize);

    return node;
}

// ================================================================
// Main entry point
// ================================================================

void shape_data_swizzle_n64_to_pc(
    void* rawBuf, u32 bufSize,
    ModelNode** outRoot,
    Vtx_t** outVtxTable,
    char*** outModelNames,
    char*** outColliderNames,
    char*** outZoneNames
) {
    u8* buf = (u8*)rawBuf;

    // Reset conversion tracking for this shape file
    sConvertedDLCount = 0;
    sConvertedVtxCount = 0;

    // Parse N64-layout header (5 x u32 at byte offsets 0x00-0x10)
    u32 n64_root          = read_be_u32(buf + 0x00);
    u32 n64_vertexTable   = read_be_u32(buf + 0x04);
    u32 n64_modelNames    = read_be_u32(buf + 0x08);
    u32 n64_colliderNames = read_be_u32(buf + 0x0C);
    u32 n64_zoneNames     = read_be_u32(buf + 0x10);

    // Vertex table pointer (not used directly for rendering, but store for reference)
    *outVtxTable = (Vtx_t*)n64_to_buf(buf, n64_vertexTable);

    // Name arrays: just rebase pointers (used for debug only)
    *outModelNames    = (char**)n64_to_buf(buf, n64_modelNames);
    *outColliderNames = (char**)n64_to_buf(buf, n64_colliderNames);
    *outZoneNames     = (char**)n64_to_buf(buf, n64_zoneNames);

    // Recursively build the PC model tree (converts DLs, vertices, matrices)
    *outRoot = swizzle_node(buf, n64_root, bufSize);

    fprintf(stderr, "[shape_swizzle] converted: %d DLs, %d vertex arrays (bufSize=0x%X)\n",
            sConvertedDLCount, sConvertedVtxCount, bufSize);

    if (*outRoot == NULL) {
        fprintf(stderr, "[shape_swizzle] WARNING: root model is NULL\n");
    }
}
