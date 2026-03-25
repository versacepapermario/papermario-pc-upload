/**
 * shape_swizzle.h - Convert N64 shape file data to PC structs
 */
#ifndef PORT_SHAPE_SWIZZLE_H
#define PORT_SHAPE_SWIZZLE_H

#include "common.h"
#include "model.h"

/**
 * Convert N64 binary shape data to PC-native model tree.
 * Call after decode_yay0() on shape data.
 */
void shape_data_swizzle_n64_to_pc(
    void* rawBuf, u32 bufSize,
    ModelNode** outRoot,
    Vtx_t** outVtxTable,
    char*** outModelNames,
    char*** outColliderNames,
    char*** outZoneNames
);

#endif
