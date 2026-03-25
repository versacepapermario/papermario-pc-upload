#include "Merlar.h"

API_CALLABLE(N(AddMerlarHoverMotion)) {
    Npc* npc;

    if (isInitialCall) {
        (*(f32*)&script->functionTemp[1]) = 0.0f;
        script->functionTempPtr[2] = get_npc_safe(script->owner2.npcID);
    }
    npc = script->functionTempPtr[2];
    npc->verticalRenderOffset = sin_deg((*(f32*)&script->functionTemp[1])) * 5.0f;
    (*(f32*)&script->functionTemp[1]) = clamp_angle((*(f32*)&script->functionTemp[1]) + 4.5f);
    return ApiStatus_BLOCK;
}

EvtScript N(EVS_NpcAuxAI_Merlar) = {
    Return
    End
};

NpcSettings N(NpcSettings_Merlar) = {
    .height = 60,
    .radius = 60,
    .level = ACTOR_LEVEL_NONE,
    .otherAI = &N(EVS_NpcAuxAI_Merlar),
};

EvtScript N(EVS_NpcAux_Merlar) = {
    Return
    End
};

EvtScript N(EVS_NpcIdle_Merlar) = {
    Call(N(AddMerlarHoverMotion))
    Return
    End
};
