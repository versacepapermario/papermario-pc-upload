/**
 * entity_anim_swizzle.c - Convert N64 entity animation data to PC format
 *
 * Entity animated models (doors, save blocks, switches, etc.) have two DMA buffers:
 *   - gfx buffer (dmaList[0]): Display lists + vertex data (N64 format)
 *   - anim buffer (dmaList[1]): StaticAnimatorNode structs + pointer table + anim script
 *
 * On N64, StaticAnimatorNode is 0x2C bytes (4-byte pointers). On 64-bit PC it's
 * ~72 bytes (8-byte pointers). This module converts the node data to PC format
 * and the display list/vertex data to PC GBI format.
 */

#include "common.h"
#include "entity.h"
#include "model.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "port/endian.h"

// N64 StaticAnimatorNode layout (0x2C = 44 bytes)
#define N64_STATIC_ANIM_NODE_SIZE 0x2C

// Field offsets within N64 StaticAnimatorNode
#define SAN_OFF_DISPLAY_LIST    0x00
#define SAN_OFF_ROT             0x04
#define SAN_OFF_POS             0x0C
#define SAN_OFF_SIBLING         0x18
#define SAN_OFF_CHILD           0x1C
#define SAN_OFF_VTX_START       0x20
#define SAN_OFF_VTX_LIST        0x24
#define SAN_OFF_MODEL_ID        0x28

// F3DEX2 opcodes
#define ENT_G_VTX      0x01
#define ENT_G_DL       0xDE
#define ENT_G_ENDDL    0xDF
#define ENT_G_MTX      0xDA
#define ENT_G_MOVEMEM  0xDC
#define ENT_G_SETTIMG  0xFD
#define ENT_G_SETCIMG  0xFF

#define N64_GFX_CMD_SIZE 8
#define N64_VTX_SIZE     16

// ================================================================
// Conversion tracking (per entity load)
// ================================================================

typedef struct { u32 offset; Vtx* vtx; s32 count; } VtxCacheEntry;
typedef struct { u32 offset; Gfx* dl; } DLCacheEntry;

#define MAX_VTX_CACHE 256
#define MAX_DL_CACHE  128

static VtxCacheEntry sVtxCache[MAX_VTX_CACHE];
static int sVtxCacheCount;

static DLCacheEntry sDLCache[MAX_DL_CACHE];
static int sDLCacheCount;

// Find cached vtx entry with the largest count for this offset
static int find_cached_vtx_index(u32 offset) {
    int bestIdx = -1;
    for (int i = 0; i < sVtxCacheCount; i++) {
        if (sVtxCache[i].offset == offset) {
            if (bestIdx < 0 || sVtxCache[i].count > sVtxCache[bestIdx].count)
                bestIdx = i;
        }
    }
    return bestIdx;
}

static Gfx* find_cached_dl(u32 offset) {
    for (int i = 0; i < sDLCacheCount; i++)
        if (sDLCache[i].offset == offset) return sDLCache[i].dl;
    return NULL;
}

// ================================================================
// Vertex conversion: N64 s16 → PC float (GBI_FLOATS)
// ================================================================

static Vtx* convert_entity_vtx(u8* gfxBuf, u32 gfxSize, u32 offset, s32 count) {
    if (offset + count * N64_VTX_SIZE > gfxSize || count <= 0) return NULL;

    // Reuse cached entry only if it has enough vertices
    int vtxIdx = find_cached_vtx_index(offset);
    if (vtxIdx >= 0 && sVtxCache[vtxIdx].count >= count) return sVtxCache[vtxIdx].vtx;

    Vtx* pc = malloc(count * sizeof(Vtx));
    if (!pc) return NULL;
    memset(pc, 0, count * sizeof(Vtx));

    u8* src = gfxBuf + offset;
    for (s32 i = 0; i < count; i++) {
        u8* v = src + i * N64_VTX_SIZE;
        pc[i].v.ob[0] = (float)(s16)read_be_u16(v + 0);
        pc[i].v.ob[1] = (float)(s16)read_be_u16(v + 2);
        pc[i].v.ob[2] = (float)(s16)read_be_u16(v + 4);
        pc[i].v.flag   = read_be_u16(v + 6);
        pc[i].v.tc[0]  = (s16)read_be_u16(v + 8);
        pc[i].v.tc[1]  = (s16)read_be_u16(v + 10);
        pc[i].v.cn[0]  = v[12];
        pc[i].v.cn[1]  = v[13];
        pc[i].v.cn[2]  = v[14];
        pc[i].v.cn[3]  = v[15];
    }

    if (sVtxCacheCount < MAX_VTX_CACHE) {
        sVtxCache[sVtxCacheCount].offset = offset;
        sVtxCache[sVtxCacheCount].vtx = pc;
        sVtxCache[sVtxCacheCount].count = count;
        sVtxCacheCount++;
    }
    return pc;
}

// ================================================================
// Display list conversion: N64 8-byte → PC 16-byte Gfx
// ================================================================

static s32 count_dl_cmds(u8* gfxBuf, u32 gfxSize, u32 offset) {
    s32 count = 0;
    s32 max = (gfxSize - offset) / N64_GFX_CMD_SIZE;
    if (max > 4096) max = 4096;
    for (s32 i = 0; i < max; i++) {
        u32 w0 = read_be_u32(gfxBuf + offset + i * N64_GFX_CMD_SIZE);
        u8 op = (w0 >> 24) & 0xFF;
        count++;
        if (op == ENT_G_ENDDL) break;
        // gSPBranchList (G_DL nopush=1) terminates the DL — nothing after executes
        // G_DL_PUSH=0 means call (returns here), G_DL_NOPUSH=1 means branch (no return)
        if (op == ENT_G_DL && ((w0 >> 16) & 0xFF) != 0) break;
    }
    return count;
}

static Gfx* convert_entity_dl(u8* gfxBuf, u32 gfxSize, u32 dlOffset) {
    if (dlOffset >= gfxSize) return NULL;

    Gfx* existing = find_cached_dl(dlOffset);
    if (existing) return existing;

    s32 cmdCount = count_dl_cmds(gfxBuf, gfxSize, dlOffset);
    if (cmdCount <= 0) return NULL;

    Gfx* pc = malloc((cmdCount + 1) * sizeof(Gfx));
    if (!pc) return NULL;
    memset(pc, 0, (cmdCount + 1) * sizeof(Gfx));
    // G_ENDDL sentinel past last command to prevent reading heap garbage
    pc[cmdCount].words.w0 = (uintptr_t)0xDF000000;
    pc[cmdCount].words.w1 = 0;

    // Register early for recursive refs
    if (sDLCacheCount < MAX_DL_CACHE) {
        sDLCache[sDLCacheCount].offset = dlOffset;
        sDLCache[sDLCacheCount].dl = pc;
        sDLCacheCount++;
    }

    for (s32 i = 0; i < cmdCount; i++) {
        u8* cmd = gfxBuf + dlOffset + i * N64_GFX_CMD_SIZE;
        u32 w0 = read_be_u32(cmd);
        u32 w1 = read_be_u32(cmd + 4);
        u8 op = (w0 >> 24) & 0xFF;

        switch (op) {
            case ENT_G_VTX: {
                s32 numVerts = (w0 >> 12) & 0xFF;
                u32 vtxOffset = w1 & 0x00FFFFFF;
                Vtx* vtx = convert_entity_vtx(gfxBuf, gfxSize, vtxOffset, numVerts);
                if (!vtx) {
                    fprintf(stderr, "[entity_anim_dl] G_VTX FAILED: vtxOffset=0x%X numVerts=%d gfxSize=0x%X w0=0x%08X w1=0x%08X\n",
                            vtxOffset, numVerts, gfxSize, w0, w1);
                }
                pc[i].words.w0 = (uintptr_t)w0;
                pc[i].words.w1 = (uintptr_t)vtx;
                break;
            }
            case ENT_G_DL: {
                u32 subOffset = w1 & 0x00FFFFFF;
                Gfx* subDL = (subOffset < gfxSize) ?
                    convert_entity_dl(gfxBuf, gfxSize, subOffset) : NULL;
                pc[i].words.w0 = (uintptr_t)w0;
                pc[i].words.w1 = (uintptr_t)subDL;
                break;
            }
            case ENT_G_SETTIMG:
            case ENT_G_SETCIMG: {
                u32 imgOffset = w1 & 0x00FFFFFF;
                if ((w1 >> 24) == 0x0A && imgOffset < gfxSize) {
                    pc[i].words.w0 = (uintptr_t)w0;
                    pc[i].words.w1 = (uintptr_t)(gfxBuf + imgOffset);
                } else {
                    pc[i].words.w0 = (uintptr_t)w0;
                    pc[i].words.w1 = (uintptr_t)w1;
                }
                break;
            }
            case ENT_G_MTX: {
                u32 mtxOffset = w1 & 0x00FFFFFF;
                if ((w1 >> 24) == 0x0A && mtxOffset + 64 <= gfxSize) {
                    u8* ms = gfxBuf + mtxOffset;
                    Mtx* mtx = malloc(sizeof(Mtx));
                    u32 raw[16];
                    for (int j = 0; j < 16; j++) raw[j] = read_be_u32(ms + j * 4);
                    u32* m1 = &raw[0]; u32* m2 = &raw[8];
                    float (*mf)[4] = (float (*)[4])mtx;
                    for (int r = 0; r < 4; r++) {
                        for (int c = 0; c < 2; c++) {
                            u32 ip = *m1, fp = *m2;
                            u32 t1 = (ip & 0xffff0000) | ((fp >> 16) & 0xffff);
                            u32 t2 = ((ip << 16) & 0xffff0000) | (fp & 0xffff);
                            mf[r][c*2+0] = (s32)t1 / 65536.0f;
                            mf[r][c*2+1] = (s32)t2 / 65536.0f;
                            m1++; m2++;
                        }
                    }
                    pc[i].words.w0 = (uintptr_t)w0;
                    pc[i].words.w1 = (uintptr_t)mtx;
                } else {
                    pc[i].words.w0 = (uintptr_t)w0;
                    pc[i].words.w1 = (uintptr_t)w1;
                }
                break;
            }
            case ENT_G_MOVEMEM: {
                u32 dataOffset = w1 & 0x00FFFFFF;
                if ((w1 >> 24) == 0x0A && dataOffset < gfxSize) {
                    pc[i].words.w0 = (uintptr_t)w0;
                    pc[i].words.w1 = (uintptr_t)(gfxBuf + dataOffset);
                } else {
                    pc[i].words.w0 = (uintptr_t)w0;
                    pc[i].words.w1 = (uintptr_t)w1;
                }
                break;
            }
            default:
                pc[i].words.w0 = (uintptr_t)w0;
                pc[i].words.w1 = (uintptr_t)w1;
                break;
        }
    }
    return pc;
}

// ================================================================
// StaticAnimatorNode conversion: N64 0x2C → PC struct
// ================================================================

static void read_n64_node(u8* src, StaticAnimatorNode* dst) {
    dst->displayList = (void*)(uintptr_t)read_be_u32(src + SAN_OFF_DISPLAY_LIST);
    dst->rot.x = (s16)read_be_u16(src + SAN_OFF_ROT + 0);
    dst->rot.y = (s16)read_be_u16(src + SAN_OFF_ROT + 2);
    dst->rot.z = (s16)read_be_u16(src + SAN_OFF_ROT + 4);
    dst->pos.x = read_be_f32(src + SAN_OFF_POS + 0);
    dst->pos.y = read_be_f32(src + SAN_OFF_POS + 4);
    dst->pos.z = read_be_f32(src + SAN_OFF_POS + 8);
    dst->sibling = (StaticAnimatorNode*)(uintptr_t)read_be_u32(src + SAN_OFF_SIBLING);
    dst->child = (StaticAnimatorNode*)(uintptr_t)read_be_u32(src + SAN_OFF_CHILD);
    dst->vertexStartOffset = (s16)read_be_u16(src + SAN_OFF_VTX_START);
    dst->vtxList = (Vtx*)(uintptr_t)read_be_u32(src + SAN_OFF_VTX_LIST);
    dst->modelID = (s16)read_be_u16(src + SAN_OFF_MODEL_ID);
}

// ================================================================
// Main entry point
// ================================================================

/**
 * Convert entity animated model data from N64 to PC format.
 *
 * gfxBuf/gfxSize is the COMBINED [gfx][anim] buffer (contiguous, like N64 segment 0x0A).
 * animBuf/animSize is the anim portion within gfxBuf (animBuf = gfxBuf + gfxDataSize).
 * DL/vertex/texture offsets resolve from gfxBuf base; node offsets are anim-local.
 *
 * outPtrTable receives a malloc'd array of StaticAnimatorNode* (NULL-terminated).
 * Returns node count, or 0 on failure.
 */
s32 entity_convert_anim_data_port(
    EntityBlueprint* entityData,
    u8* animBuf, u32 animSize,
    u8* gfxBuf, u32 gfxSize,
    StaticAnimatorNode*** outPtrTable
) {
    // Reset caches
    sVtxCacheCount = 0;
    sDLCacheCount = 0;

    // ----------------------------------------------------------------
    // Step 1: Get node count from the compiled-in Mesh array.
    // On PORT, entityData->modelAnimationNodes points to the C-compiled
    // mesh array (e.g. Entity_HitYellowBlock_Mesh[]), which is valid.
    // This avoids unreliable heuristic scanning of the ROM data.
    // ----------------------------------------------------------------
    StaticAnimatorNode** compiledMesh = (StaticAnimatorNode**)entityData->modelAnimationNodes;
    if (compiledMesh == NULL) {
        fprintf(stderr, "[entity_anim] ERROR: modelAnimationNodes is NULL (type=%d)\n",
                entityData->entityType);
        *outPtrTable = NULL;
        return 0;
    }

    // ENTITY_ANIM_NULL = (void*)(0xFFFFFFFF) → 0x00000000FFFFFFFF on 64-bit
    s32 nodeCount = 0;
    while (nodeCount < 64) {
        uintptr_t entryVal = (uintptr_t)compiledMesh[nodeCount];
        if (entryVal == 0 || entryVal == (uintptr_t)0xFFFFFFFF) break;
        nodeCount++;
    }

    if (nodeCount == 0) {
        fprintf(stderr, "[entity_anim] WARNING: zero nodes in compiled mesh (type=%d)\n",
                entityData->entityType);
        *outPtrTable = NULL;
        return 0;
    }

    // ----------------------------------------------------------------
    // Step 2: Find the pointer table in the ROM anim buffer.
    // We know exactly nodeCount entries + 0xFFFFFFFF terminator.
    // Entries are anim-local offsets (plain u32 values relative to the
    // start of the anim buffer). They must be 0x2C-aligned (same
    // remainder mod 0x2C), distinct, and fit within the anim buffer.
    // Entry value 0 is valid (first node can be at offset 0).
    // ----------------------------------------------------------------
    u32 tableBytes = (nodeCount + 1) * 4;
    u32 ptrTableOff = 0xFFFFFFFF;

    for (u32 scan = 0; scan + tableBytes <= animSize; scan += 4) {
        // Check terminator first (cheap filter)
        u32 term = read_be_u32(animBuf + scan + nodeCount * 4);
        if (term != 0xFFFFFFFF) continue;

        // Validate all nodeCount entries
        s32 valid = 1;
        u32 alignRem = 0;
        s32 alignSet = 0;
        u32 seen[64];
        s32 nSeen = 0;

        for (s32 i = 0; i < nodeCount; i++) {
            u32 val = read_be_u32(animBuf + scan + i * 4);

            // Entry must not be terminator sentinel
            if (val == 0xFFFFFFFF) { valid = 0; break; }

            // Must be a valid offset within the anim buffer
            if (val + N64_STATIC_ANIM_NODE_SIZE > animSize) { valid = 0; break; }

            // All entries must be congruent mod 0x2C
            u32 rem = val % N64_STATIC_ANIM_NODE_SIZE;
            if (!alignSet) { alignRem = rem; alignSet = 1; }
            else if (rem != alignRem) { valid = 0; break; }

            // All entries must be distinct (prevents false positives from repeated data)
            s32 dup = 0;
            for (s32 j = 0; j < nSeen; j++) {
                if (seen[j] == val) { dup = 1; break; }
            }
            if (dup) { valid = 0; break; }
            if (nSeen < 64) seen[nSeen++] = val;
        }

        if (!valid) continue;

        ptrTableOff = scan;
        break;
    }

    if (ptrTableOff >= animSize) {
        fprintf(stderr, "[entity_anim] ERROR: pointer table not found "
                "(type=%d nodes=%d animSize=0x%X gfxSize=0x%X)\n",
                entityData->entityType, nodeCount, animSize, gfxSize);
        *outPtrTable = NULL;
        return 0;
    }

    fprintf(stderr, "[entity_anim] type=%d nodes=%d ptrTableOff=0x%X animSize=0x%X gfxSize=0x%X\n",
            entityData->entityType, nodeCount, ptrTableOff, animSize, gfxSize);

    // ----------------------------------------------------------------
    // Step 3: Read nodes from the anim buffer using pointer table entries.
    // Each entry is an anim-local offset (& 0xFFFF). The N64 swizzle
    // resolves as: baseAnim + (entry & 0xFFFF). Store offsets for
    // sibling/child resolution (they use the same encoding).
    // ----------------------------------------------------------------
    StaticAnimatorNode* pcNodes = malloc(nodeCount * sizeof(StaticAnimatorNode));
    if (!pcNodes) {
        *outPtrTable = NULL;
        return 0;
    }
    memset(pcNodes, 0, nodeCount * sizeof(StaticAnimatorNode));

    u32* nodeAnimOff = malloc(nodeCount * sizeof(u32));

    for (s32 i = 0; i < nodeCount; i++) {
        u32 rawEntry = read_be_u32(animBuf + ptrTableOff + i * 4);
        u32 animOff = rawEntry & 0xFFFF;
        nodeAnimOff[i] = animOff;

        read_n64_node(animBuf + animOff, &pcNodes[i]);
    }

    // ----------------------------------------------------------------
    // Step 4: Fix up node pointers (N64 VRAM → PC pointers).
    // Display lists and vertices use lo16 as offset into gfx buffer.
    // Sibling/child use lo16 matched against nodeLo16[] entries.
    // ----------------------------------------------------------------
    for (s32 i = 0; i < nodeCount; i++) {
        // displayList → converted PC display list (offset from combined gfx+anim base)
        u32 dlVal = (u32)(uintptr_t)pcNodes[i].displayList;
        if (dlVal != 0 && dlVal != 0xFFFFFFFF) {
            u32 dlOff = dlVal & 0xFFFF;
            pcNodes[i].displayList = (dlOff < gfxSize) ?
                (void*)convert_entity_dl(gfxBuf, gfxSize, dlOff) : NULL;
        } else {
            pcNodes[i].displayList = NULL;
        }

        // sibling/child: ROM data has these as 0xFFFFFFFF (not preserved in DMA overlay).
        // Reconstruct from the C-compiled mesh array which has valid sibling/child pointers.
        pcNodes[i].sibling = NULL;
        pcNodes[i].child = NULL;
        if (compiledMesh[i] != NULL) {
            StaticAnimatorNode* srcSib = compiledMesh[i]->sibling;
            StaticAnimatorNode* srcChild = compiledMesh[i]->child;

            // Map C-compiled sibling to converted node by finding its index in compiledMesh
            if (srcSib != NULL && (uintptr_t)srcSib != (uintptr_t)0xFFFFFFFF) {
                for (s32 j = 0; j < nodeCount; j++) {
                    if (compiledMesh[j] == srcSib) { pcNodes[i].sibling = &pcNodes[j]; break; }
                }
            }
            if (srcChild != NULL && (uintptr_t)srcChild != (uintptr_t)0xFFFFFFFF) {
                for (s32 j = 0; j < nodeCount; j++) {
                    if (compiledMesh[j] == srcChild) { pcNodes[i].child = &pcNodes[j]; break; }
                }
            }
        }

        // vtxList → converted PC vertices (& 0xFFFFF = 20-bit mask, matches N64 swizzle)
        u32 vtxVal = (u32)(uintptr_t)pcNodes[i].vtxList;
        if (vtxVal != 0 && vtxVal != 0xFFFFFFFF) {
            u32 vtxOff = vtxVal & 0xFFFFF;
            int cachedIdx = find_cached_vtx_index(vtxOff);
            pcNodes[i].vtxList = (cachedIdx >= 0) ? sVtxCache[cachedIdx].vtx : NULL;
            if (pcNodes[i].vtxList == NULL && vtxOff < gfxSize) {
                // Not found in DL cache — convert directly.
                // Estimate count from available space (capped at 256 vertices).
                s32 maxVtx = (gfxSize - vtxOff) / N64_VTX_SIZE;
                if (maxVtx > 256) maxVtx = 256;
                if (maxVtx > 0) {
                    pcNodes[i].vtxList = convert_entity_vtx(gfxBuf, gfxSize, vtxOff, maxVtx);
                    fprintf(stderr, "[entity_anim] vtxList[%d]: cache miss, converted %d vtx at offset 0x%X\n",
                            i, maxVtx, vtxOff);
                }
            }
        } else {
            pcNodes[i].vtxList = NULL;
        }
    }

    // Build pointer table (array of StaticAnimatorNode* for PC)
    StaticAnimatorNode** ptrTable = malloc((nodeCount + 1) * sizeof(StaticAnimatorNode*));
    for (s32 i = 0; i < nodeCount; i++) {
        ptrTable[i] = &pcNodes[i];
    }
    ptrTable[nodeCount] = NULL;

    free(nodeAnimOff);

    // Byte-swap the anim buffer in place: bswap16 makes all s16 values native-endian.
    // Needed for animation script commands (s16 opcodes) read by the animator.
    // Node data was already extracted above, so this doesn't affect it.
    {
        u16* words = (u16*)animBuf;
        u32 count16 = animSize / 2;
        for (u32 i = 0; i < count16; i++) {
            words[i] = bswap16(words[i]);
        }
    }

    fprintf(stderr, "[entity_anim] converted %d nodes, %d DLs, %d vtx arrays\n",
            nodeCount, sDLCacheCount, sVtxCacheCount);

    *outPtrTable = ptrTable;
    return nodeCount;
}

