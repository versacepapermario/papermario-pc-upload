#include "model.h"
#ifdef PORT
#include <stdio.h>
#include <string.h>

/* Golden copy of AnimatedModel pointers — saved at reset, used to detect BSS corruption */
static AnimatedModel* port_savedWorldPtrs[MAX_ANIMATED_MODELS];
static AnimatedModel* port_savedBattlePtrs[MAX_ANIMATED_MODELS];
static int port_worldPtrsSaved = 0;
static int port_battlePtrsSaved = 0;
static int port_uamCallCount = 0;
#endif

extern AnimatedModelList gBattleMeshAnimationList;
extern AnimatedModelList gWorldMeshAnimationList;
extern AnimatedModelList* gCurrentMeshAnimationListPtr;

void update_animated_models(void) {
    s32 i;

#ifdef PORT
    if (gCurrentMeshAnimationListPtr == NULL) {
        return;
    }

    /* Detect and repair BSS corruption by comparing against golden copy */
    AnimatedModel** savedPtrs = NULL;
    int hasSaved = 0;
    if (gCurrentMeshAnimationListPtr == &gWorldMeshAnimationList) {
        savedPtrs = port_savedWorldPtrs;
        hasSaved = port_worldPtrsSaved;
    } else if (gCurrentMeshAnimationListPtr == &gBattleMeshAnimationList) {
        savedPtrs = port_savedBattlePtrs;
        hasSaved = port_battlePtrsSaved;
    }

    if (hasSaved && savedPtrs) {
        for (i = 0; i < MAX_ANIMATED_MODELS; i++) {
            AnimatedModel* cur = (*gCurrentMeshAnimationListPtr)[i];
            if (cur != savedPtrs[i]) {
                fprintf(stderr, "[UAM] CORRUPTION DETECTED frame=%d entry[%d]: expected=%p got=%p — restoring\n",
                        port_uamCallCount, i, (void*)savedPtrs[i], (void*)cur);
                fflush(stderr);
                (*gCurrentMeshAnimationListPtr)[i] = savedPtrs[i];
            }
        }
    }
    port_uamCallCount++;
#endif

    for (i = 0; i < MAX_ANIMATED_MODELS; i++) {
        AnimatedModel* anim = (*gCurrentMeshAnimationListPtr)[i];

#ifdef PORT
        if (anim == NULL) {
            continue;
        }
#endif
        if (anim->animModelID >= 0) {
            update_model_animator_with_transform(anim->animModelID, &anim->mtx);
        }
    }
}

void render_animated_models(void) {
    Matrix4f sp18;
    Matrix4f sp58;
    Matrix4f sp98;
    Matrix4f spD8;
    Matrix4f sp118;
    Matrix4f sp158;
    Matrix4f sp198;
    Matrix4f sp1D8;
    s32 i = 0;
    Matrix4f* sp218 = &sp58;

#ifdef PORT
    if (gCurrentMeshAnimationListPtr == NULL) {
        return;
    }

    /* Repair BSS corruption using golden copy (same as update_animated_models) */
    {
        AnimatedModel** savedPtrs = NULL;
        int hasSaved = 0;
        if (gCurrentMeshAnimationListPtr == &gWorldMeshAnimationList) {
            savedPtrs = port_savedWorldPtrs;
            hasSaved = port_worldPtrsSaved;
        } else if (gCurrentMeshAnimationListPtr == &gBattleMeshAnimationList) {
            savedPtrs = port_savedBattlePtrs;
            hasSaved = port_battlePtrsSaved;
        }
        if (hasSaved && savedPtrs) {
            for (i = 0; i < MAX_ANIMATED_MODELS; i++) {
                AnimatedModel* cur = (*gCurrentMeshAnimationListPtr)[i];
                if (cur != savedPtrs[i]) {
                    (*gCurrentMeshAnimationListPtr)[i] = savedPtrs[i];
                }
            }
        }
        i = 0;
    }
#endif

    for (; i < MAX_ANIMATED_MODELS; i++) {
        AnimatedModel* model = (*gCurrentMeshAnimationListPtr)[i];

#ifdef PORT
        if (model == NULL) {
            continue;
        }
#endif
        if (model->animModelID >= 0) {
            guTranslateF(sp18, model->pos.x, model->pos.y, model->pos.z);
            guRotateF(*sp218, model->rot.x, 1.0f, 0.0f, 0.0f);
            guRotateF(sp98, model->rot.y, 0.0f, 1.0f, 0.0f);
            guRotateF(spD8, model->rot.z, 0.0f, 0.0f, 1.0f);
            guScaleF(sp1D8, model->scale.x, model->scale.y, model->scale.z);
            guMtxCatF(spD8, *sp218, sp158);
            guMtxCatF(sp158, sp98, sp118);
            guMtxCatF(sp1D8, sp118, sp158);
            guMtxCatF(sp158, sp18, sp198);
            guMtxF2L(sp198, &model->mtx);
            render_animated_model(model->animModelID, &model->mtx);
        }
    }
}

// split here?

API_CALLABLE(InitAnimatedModels) {
    if (gGameStatusPtr->context == CONTEXT_WORLD) {
        gCurrentMeshAnimationListPtr = &gWorldMeshAnimationList;
    } else {
        gCurrentMeshAnimationListPtr = &gBattleMeshAnimationList;
    }

    return ApiStatus_DONE2;
}

API_CALLABLE(LoadAnimatedModel) {
    Bytecode* args = script->ptrReadPos;
    s32 index = evt_get_variable(script, *args++);
    StaticAnimatorNode** var1 = (StaticAnimatorNode**) evt_get_variable(script, *args++);
    AnimatedModel* animModel = (*gCurrentMeshAnimationListPtr)[index];
    s32 animModelID = create_model_animator(0);

    load_model_animator_tree(animModelID, var1);
    animModel->animModelID = animModelID;
    animModel->pos.x = 0;
    animModel->pos.y = 0;
    animModel->pos.z = 0;
    animModel->rot.x = 0;
    animModel->rot.y = 0;
    animModel->rot.z = 0;
    animModel->scale.x = 1.0f;
    animModel->scale.y = 1.0f;
    animModel->scale.z = 1.0f;
    animModel->curAnimData = nullptr;
    guMtxIdent(&animModel->mtx);

    return ApiStatus_DONE2;
}

API_CALLABLE(LoadAnimatedMesh) {
    Bytecode* args = script->ptrReadPos;
    s32 index = evt_get_variable(script, *args++);
    StaticAnimatorNode** tree = (StaticAnimatorNode**) evt_get_variable(script, *args++);
    AnimatedModel* animModel = (*gCurrentMeshAnimationListPtr)[index];
    s32 animModelID = create_model_animator(0);

    load_mesh_animator_tree(animModelID, tree);
    animModel->animModelID = animModelID;
    animModel->pos.x = 0;
    animModel->pos.y = 0;
    animModel->pos.z = 0;
    animModel->rot.x = 0;
    animModel->rot.y = 0;
    animModel->rot.z = 0;
    animModel->scale.x = 1.0f;
    animModel->scale.y = 1.0f;
    animModel->scale.z = 1.0f;
    animModel->curAnimData = nullptr;
    guMtxIdent(&animModel->mtx);

    return ApiStatus_DONE2;
}

API_CALLABLE(PlayModelAnimation) {
    Bytecode* args = script->ptrReadPos;
    s32 index = evt_get_variable(script, *args++);
    s16* var2 = (s16*) evt_get_variable(script, *args++);
    AnimatedModel* model = (*gCurrentMeshAnimationListPtr)[index];

    model->curAnimData = var2;
    play_model_animation(model->animModelID, var2);

    return ApiStatus_DONE2;
}

API_CALLABLE(PlayModelAnimationStartingFrom) {
    Bytecode* args = script->ptrReadPos;
    s32 index = evt_get_variable(script, *args++);
    s16* var2 = (s16*) evt_get_variable(script, *args++);
    s32 var3 = evt_get_variable(script, *args++);
    AnimatedModel* model = (*gCurrentMeshAnimationListPtr)[index];

    model->curAnimData = var2;
    play_model_animation_starting_from(model->animModelID, var2, var3);

    return ApiStatus_DONE2;
}

API_CALLABLE(ChangeModelAnimation) {
    Bytecode* args = script->ptrReadPos;
    s32 index = evt_get_variable(script, *args++);
    s16* var2 = (s16*) evt_get_variable(script, *args++);
    AnimatedModel* model = (*gCurrentMeshAnimationListPtr)[index];

    if (model->curAnimData == var2) {
        return ApiStatus_DONE2;
    }

    model->curAnimData = var2;
    play_model_animation(model->animModelID, var2);
    return ApiStatus_DONE2;
}

API_CALLABLE(SetAnimatedModelRootPosition) {
    Bytecode* args = script->ptrReadPos;
    s32 index = evt_get_variable(script, *args++);
    f32 x = evt_get_float_variable(script, *args++);
    f32 y = evt_get_float_variable(script, *args++);
    f32 z = evt_get_float_variable(script, *args++);
    AnimatedModel* model = (*gCurrentMeshAnimationListPtr)[index];

    model->pos.x = x;
    model->pos.y = y;
    model->pos.z = z;

    return ApiStatus_DONE2;
}

API_CALLABLE(GetAnimatedModelRootPosition) {
    Bytecode* args = script->ptrReadPos;
    s32 index = evt_get_variable(script, *args++);
    s32 outX = *args++;
    s32 outY = *args++;
    s32 outZ = *args++;
    AnimatedModel* model = (*gCurrentMeshAnimationListPtr)[index];

    evt_set_float_variable(script, outX, model->pos.x);
    evt_set_float_variable(script, outY, model->pos.y);
    evt_set_float_variable(script, outZ, model->pos.z);

    return ApiStatus_DONE2;
}

API_CALLABLE(AddAnimatedModelRootPosition) {
    Bytecode* args = script->ptrReadPos;
    s32 index = evt_get_variable(script, *args++);
    f32 x = evt_get_float_variable(script, *args++);
    f32 y = evt_get_float_variable(script, *args++);
    f32 z = evt_get_float_variable(script, *args++);
    AnimatedModel* model = (*gCurrentMeshAnimationListPtr)[index];

    model->pos.x += x;
    model->pos.y += y;
    model->pos.z += z;

    return ApiStatus_DONE2;
}

API_CALLABLE(SetAnimatedModelRootRotation) {
    Bytecode* args = script->ptrReadPos;
    s32 index = evt_get_variable(script, *args++);
    f32 x = evt_get_float_variable(script, *args++);
    f32 y = evt_get_float_variable(script, *args++);
    f32 z = evt_get_float_variable(script, *args++);
    AnimatedModel* model = (*gCurrentMeshAnimationListPtr)[index];

    model->rot.x = x;
    model->rot.y = y;
    model->rot.z = z;

    return ApiStatus_DONE2;
}

API_CALLABLE(SetAnimatedModelRootScale) {
    Bytecode* args = script->ptrReadPos;
    s32 index = evt_get_variable(script, *args++);
    f32 x = evt_get_float_variable(script, *args++);
    f32 y = evt_get_float_variable(script, *args++);
    f32 z = evt_get_float_variable(script, *args++);
    AnimatedModel* model = (*gCurrentMeshAnimationListPtr)[index];

    model->scale.x = x;
    model->scale.y = y;
    model->scale.z = z;

    return ApiStatus_DONE2;
}

API_CALLABLE(SetAnimatedModelRenderMode) {
    Bytecode* args = script->ptrReadPos;
    s32 index = evt_get_variable(script, *args++);
    s32 renderMode = evt_get_float_variable(script, *args++);

    get_animator_by_index((*gCurrentMeshAnimationListPtr)[index]->animModelID)->renderMode = renderMode;
    return ApiStatus_DONE2;
}

API_CALLABLE(DeleteAnimatedModel) {
    AnimatedModel* model = (*gCurrentMeshAnimationListPtr)[evt_get_variable(script, *script->ptrReadPos)];

    delete_model_animator(get_animator_by_index(model->animModelID));
    model->animModelID = -1;
    return ApiStatus_DONE2;
}

API_CALLABLE(SetAnimatorFlags) {
    Bytecode* args = script->ptrReadPos;
    s32 index = evt_get_variable(script, *args++);
    s32 a1 = *args++;
    s32 enable = evt_get_variable(script, *args++);
    ModelAnimator* animMesh = get_animator_by_index((*gCurrentMeshAnimationListPtr)[index]->animModelID);

    if (enable) {
        animMesh->flags |= a1;
    } else {
        animMesh->flags &= ~a1;
    }

    return ApiStatus_DONE2;
}

void reset_model_animators(void) {
    s32 i;

    if (gGameStatusPtr->context == CONTEXT_WORLD ) {
        gCurrentMeshAnimationListPtr = &gWorldMeshAnimationList;
    } else {
        gCurrentMeshAnimationListPtr = &gBattleMeshAnimationList;
    }

    for (i = 0; i < MAX_ANIMATED_MODELS; i++) {
        AnimatedModel* model = heap_malloc(sizeof(*model));

        (*gCurrentMeshAnimationListPtr)[i] = model;
        ASSERT((*gCurrentMeshAnimationListPtr)[i] != nullptr);
        (*gCurrentMeshAnimationListPtr)[i]->animModelID = -1;
    }

#ifdef PORT
    /* Save golden copy of pointers for BSS corruption detection */
    if (gCurrentMeshAnimationListPtr == &gWorldMeshAnimationList) {
        memcpy(port_savedWorldPtrs, gWorldMeshAnimationList, sizeof(port_savedWorldPtrs));
        port_worldPtrsSaved = 1;
    } else {
        memcpy(port_savedBattlePtrs, gBattleMeshAnimationList, sizeof(port_savedBattlePtrs));
        port_battlePtrsSaved = 1;
    }
    port_uamCallCount = 0;
#endif

    create_worker_scene(update_animated_models, render_animated_models);
}

void init_model_animators(void) {
    if (gGameStatusPtr->context == CONTEXT_WORLD) {
        gCurrentMeshAnimationListPtr = &gWorldMeshAnimationList;
    } else {
        gCurrentMeshAnimationListPtr = &gBattleMeshAnimationList;
    }
}

API_CALLABLE(GetAnimatedNodePosition) {
    Bytecode* args = script->ptrReadPos;
    s32 listIndex = evt_get_variable(script, *args++);
    s32 treeIndex = evt_get_variable(script, *args++);
    s32 outX = *args++;
    s32 outY = *args++;
    s32 outZ = *args++;
    AnimatedModel* model = (*gCurrentMeshAnimationListPtr)[listIndex];
    AnimatorNode* node = get_animator_node_with_id(get_animator_by_index(model->animModelID), treeIndex);
    f32 x, y, z;

    guMtxXFML(&model->mtx, node->pos.x, node->pos.y, node->pos.z, &x, &y, &z);
    evt_set_variable(script, outX, x);
    evt_set_variable(script, outY, y);
    evt_set_variable(script, outZ, z);

    return ApiStatus_DONE2;
}

API_CALLABLE(GetAnimatedNodeRotation) {
    Bytecode* args = script->ptrReadPos;
    s32 listIndex = evt_get_variable(script, *args++);
    s32 treeIndex = evt_get_variable(script, *args++);
    s32 outX = *args++;
    s32 outY = *args++;
    s32 outZ = *args++;
    AnimatedModel* model = (*gCurrentMeshAnimationListPtr)[listIndex];
    AnimatorNode* node = get_animator_node_with_id(get_animator_by_index(model->animModelID), treeIndex);

    evt_set_variable(script, outX, node->rot.x);
    evt_set_variable(script, outY, node->rot.y);
    evt_set_variable(script, outZ, node->rot.z);

    return ApiStatus_DONE2;
}

API_CALLABLE(GetAnimatedPositionByTreeIndex) {
    Bytecode* args = script->ptrReadPos;
    s32 listIndex = evt_get_variable(script, *args++);
    s32 treeIndex = evt_get_variable(script, *args++);
    s32 outX = *args++;
    s32 outY = *args++;
    s32 outZ = *args++;
    AnimatedModel* model = (*gCurrentMeshAnimationListPtr)[listIndex];
    AnimatorNode* node = get_animator_node_for_tree_index(get_animator_by_index(model->animModelID), treeIndex);
    f32 x, y, z;

    guMtxXFML(&model->mtx, node->pos.x, node->pos.y, node->pos.z, &x, &y, &z);
    evt_set_variable(script, outX, x);
    evt_set_variable(script, outY, y);
    evt_set_variable(script, outZ, z);

    return ApiStatus_DONE2;
}

API_CALLABLE(GetAnimatedRotationByTreeIndex) {
    Bytecode* args = script->ptrReadPos;
    s32 listIndex = evt_get_variable(script, *args++);
    s32 treeIndex = evt_get_variable(script, *args++);
    s32 outX = *args++;
    s32 outY = *args++;
    s32 outZ = *args++;
    AnimatedModel* model = (*gCurrentMeshAnimationListPtr)[listIndex];
    AnimatorNode* node = get_animator_node_for_tree_index(get_animator_by_index(model->animModelID), treeIndex);

    evt_set_variable(script, outX, node->rot.x);
    evt_set_variable(script, outY, node->rot.y);
    evt_set_variable(script, outZ, node->rot.z);

    return ApiStatus_DONE2;
}

API_CALLABLE(SetAnimatedNodeFlags) {
    Bytecode* args = script->ptrReadPos;
    s32 listIndex = evt_get_variable(script, *args++);
    s32 id = evt_get_variable(script, *args++);
    s32 flags = *args++;
    s32 set = evt_get_variable(script, *args++);
    ModelAnimator* animator = get_animator_by_index((*gCurrentMeshAnimationListPtr)[listIndex]->animModelID);
    AnimatorNode* node = get_animator_node_with_id(animator, id);

    if (set) {
        node->flags |= flags;
    } else {
        node->flags &= ~flags;
    }

    return ApiStatus_DONE2;
}
