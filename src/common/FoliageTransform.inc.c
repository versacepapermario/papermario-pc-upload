#include "common.h"
#include "npc.h"
#include "model.h"

typedef struct FoliageModelList {
    s32 count;
    s32 models[VLA];
} FoliageModelList;

typedef struct FoliageDropList {
    s32 count;
    struct {
        s32 itemID;
        Vec3i pos;
        s32 spawnMode;
#ifdef PORT
        // PORT: Must be s32 (not Bytecode) so struct layout matches N64's 4-byte stride.
        // BufRead reads this struct 4 bytes at a time via port_buf_read_next.
        // Values are flag indices (small integers), so s32 is sufficient.
        s32 pickupFlag;
        s32 spawnFlag;
#else
        Bytecode pickupFlag;
        Bytecode spawnFlag;
#endif
    } drops[VLA];
} FoliageDropList;

typedef struct FoliageVectorList {
    s32 count;
    Vec3i vectors[VLA];
} FoliageVectorList;

typedef struct SearchBushConfig {
    FoliageModelList* bush;
    FoliageDropList* drops;
    FoliageVectorList* vectors;
    EvtScript* callback;
} SearchBushConfig;

typedef struct ShakeTreeConfig {
    FoliageModelList* leaves;
    FoliageModelList* trunk;
    FoliageDropList* drops;
    FoliageVectorList* vectors;
    EvtScript* callback;
} ShakeTreeConfig;

#ifdef PORT
// PORT: SearchBushConfig and ShakeTreeConfig contain pointer fields (8 bytes on 64-bit).
// BufRead reads 4 bytes at a time (port_buf_read_next), which truncates pointers.
// These API callables correctly unpack the struct fields as full pointers.
API_CALLABLE(N(UnpackSearchBushConfig)) {
    Bytecode* args = script->ptrReadPos;
    SearchBushConfig* config = (SearchBushConfig*)(intptr_t)evt_get_variable(script, *args++);
    Bytecode outBush = *args++;
    Bytecode outDrops = *args++;
    Bytecode outVectors = *args++;
    Bytecode outCallback = *args++;
    evt_set_variable(script, outBush, (Bytecode)(intptr_t)config->bush);
    evt_set_variable(script, outDrops, (Bytecode)(intptr_t)config->drops);
    evt_set_variable(script, outVectors, (Bytecode)(intptr_t)config->vectors);
    evt_set_variable(script, outCallback, (Bytecode)(intptr_t)config->callback);
    return ApiStatus_DONE2;
}

API_CALLABLE(N(UnpackShakeTreeConfig)) {
    Bytecode* args = script->ptrReadPos;
    ShakeTreeConfig* config = (ShakeTreeConfig*)(intptr_t)evt_get_variable(script, *args++);
    Bytecode outLeaves = *args++;
    Bytecode outTrunk = *args++;
    Bytecode outDrops = *args++;
    Bytecode outVectors = *args++;
    Bytecode outCallback = *args++;
    evt_set_variable(script, outLeaves, (Bytecode)(intptr_t)config->leaves);
    evt_set_variable(script, outTrunk, (Bytecode)(intptr_t)config->trunk);
    evt_set_variable(script, outDrops, (Bytecode)(intptr_t)config->drops);
    evt_set_variable(script, outVectors, (Bytecode)(intptr_t)config->vectors);
    evt_set_variable(script, outCallback, (Bytecode)(intptr_t)config->callback);
    return ApiStatus_DONE2;
}
#endif

void N(foliage_setup_shear_mtx)(Matrix4f mtx, f32 scale, f32 xAmount, f32 zAmount) {
    guMtxIdentF(mtx);
    mtx[1][0] = scale * xAmount;
    mtx[1][1] = 1.0f;
    mtx[1][2] = scale * zAmount;
}

/// @apiparam int model ID
/// @apiparam float scale
/// @apiparam float dx
/// @apiparam float dy
/// @apiparam float dz
API_CALLABLE(N(TransformFoliage)) {
    Bytecode* args = script->ptrReadPos;
    s32 modelListIndex = get_model_list_index_from_tree_index(evt_get_variable(script, *args++));
    f32 scale = evt_get_float_variable(script, *args++);
    f32 dx = evt_get_float_variable(script, *args++);
    f32 dy = evt_get_float_variable(script, *args++);
    f32 dz = evt_get_float_variable(script, *args++);
    Model* model = get_model_from_list_index(modelListIndex);
    Matrix4f mtx;

    if (!(model->flags & MODEL_FLAG_HAS_TRANSFORM)) {
        guTranslateF(model->userTransformMtx, 0.0f, dy, 0.0f);
        N(foliage_setup_shear_mtx)(mtx, scale, dx, dz);
        guMtxCatF(mtx, model->userTransformMtx, model->userTransformMtx);
        guTranslateF(mtx, 0.0f, -dy, 0.0f);
        guMtxCatF(mtx, model->userTransformMtx, model->userTransformMtx);
        model->flags |= (MODEL_FLAG_HAS_TRANSFORM | MODEL_FLAG_MATRIX_DIRTY);
    } else {
        guTranslateF(mtx, 0.0f, dy, 0.0f);
        guMtxCatF(mtx, model->userTransformMtx, model->userTransformMtx);
        N(foliage_setup_shear_mtx)(mtx, scale, dx, dz);
        guMtxCatF(mtx, model->userTransformMtx, model->userTransformMtx);
        guTranslateF(mtx, 0.0f, -dy, 0.0f);
        guMtxCatF(mtx, model->userTransformMtx, model->userTransformMtx);
    }

    return ApiStatus_DONE2;
}
