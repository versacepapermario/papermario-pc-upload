#include "common.h"
#include "effects_internal.h"
#include "nu/nusys.h"
#ifdef PORT
#include <stdio.h>
#include <string.h>
#endif

typedef struct FlamePreset {
    /* 0x00 */ Color_RGB8 colorScale;
    /* 0x03 */ s8 keyCenter;
    /* 0x04 */ Color_RGB8 envColor;
    /* 0x07 */ s8 primIntensity;
    /* 0x08 */ u8 sizeScale;
    /* 0x0C */ Gfx* dlist;
} FlamePreset; // size = 0x10

extern Gfx D_09000800_3543B0[];
extern Gfx D_090008F8_3544A8[]; //TODO rename EffectGfx_Flame_DrawQuad
extern Gfx D_09000918_3544C8[];
extern Gfx D_090009E0_354590[];

#ifdef PORT
extern unsigned char D_800B32E0[];
extern unsigned char D_09000000_353BB0[];
extern unsigned char D_09000200_353DB0[];
#endif

FlamePreset FlamePresets[] = {
    [FX_FLAME_BLUE] {
        .colorScale = { 255, 109, 255 },
        .keyCenter = 92,
        .envColor = { 102, 191, 255 },
        .primIntensity = 75,
        .sizeScale = 50,
        .dlist = D_09000800_3543B0,
    },
    [FX_FLAME_RED] {
        .colorScale = { 255, 255, 255 },
        .keyCenter = 0,
        .envColor = { 255, 32, 0 },
        .primIntensity = 75,
        .sizeScale = 100,
        .dlist = D_09000800_3543B0,
    },
    [FX_FLAME_SMALL_BLUE] {
        .colorScale = { 255, 0, 255 },
        .keyCenter = 92,
        .envColor = { 107, 168, 255 },
        .primIntensity = 75,
        .sizeScale = 10,
        .dlist = D_09000800_3543B0,
    },
    [FX_FLAME_PINK] {
        .colorScale = { 255, 255, 255 },
        .keyCenter = 244,
        .envColor = { 247, 175, 175 },
        .primIntensity = 22,
        .sizeScale = 30,
        .dlist = D_09000800_3543B0,
    },
};

s32 LastFlameRenderFrame = 0;

void flame_init(EffectInstance* effect);
void flame_update(EffectInstance* effect);
void flame_render(EffectInstance* effect);
void flame_appendGfx(void* effect);

void flame_main(
    s32 type,
    f32 arg1,
    f32 arg2,
    f32 arg3,
    f32 arg4,
    EffectInstance** outEffect
) {
    EffectBlueprint bp;
    EffectBlueprint* bpPtr = &bp;
    EffectInstance* effect;
    FlameFXData* data;
    s32 numParts = 1;

    if (type > FX_FLAME_PINK) {
        type = FX_FLAME_PINK;
    }

    bpPtr->init = flame_init;
    bpPtr->update = flame_update;
    bpPtr->renderScene = flame_render;
    bpPtr->unk_00 = 0;
    bpPtr->renderUI = nullptr;
    bpPtr->effectID = EFFECT_FLAME;

    effect = create_effect_instance(bpPtr);
    effect->numParts = numParts;
    data = effect->data.flame = general_heap_malloc(numParts * sizeof(*data));
    ASSERT(effect->data.flame != nullptr);

    data->type = type;
    data->unk_18 = 0;
    data->pos.x = arg1;
    data->pos.y = arg2;
    data->pos.z = arg3;
    data->baseScale = arg4 * FlamePresets[type].sizeScale * 0.01;
    data->unk_1C = 0;
    data->unk_24 = 0;
    data->unk_20 = 0;
    data->scaleW = 1.0f;
    data->scaleH = 1.0f;
    data->unk_28 = 1.0f;

    if (outEffect != nullptr) {
        *outEffect = effect;
    }
}

void flame_init(EffectInstance* effect) {
}

void flame_update(EffectInstance* effect) {
    FlameFXData* part = effect->data.flame;
    s32 i;

    for (i = 0; i < effect->numParts; i++, part++) {
        part->unk_1C += part->unk_20;
        if (part->unk_1C > 64.0f) {
            part->unk_1C -= 64.0f;
        }
        if (part->unk_1C < 0.0f) {
            part->unk_1C += 64.0f;
        }

        part->unk_24 += part->unk_28;
        if (part->unk_24 > 128.0f) {
            part->unk_24 -= 128.0f;
        }
        if (part->unk_24 < 0.0f) {
            part->unk_24 += 128.0f;
        }
    }
}

void flame_render(EffectInstance* effect) {
    FlameFXData* data = effect->data.flame;
    RenderTask renderTask;
    RenderTask* renderTaskPtr = &renderTask;
    RenderTask* retTask;
    s32 outDist;
    f32 outX;
    f32 outY;
    f32 outZ;
    f32 outW;

    transform_point(gCameras[gCurrentCameraID].mtxPerspective, data->pos.x, data->pos.y, data->pos.z, 1.0f,
                         &outX, &outY, &outZ, &outW);

    outDist = outZ + 5000;
    if (outDist < 0) {
        outDist = 0;
    } else if (outDist > 10000) {
        outDist = 10000;
    }

    if (outW < 0.01 && -0.01 < outW) {
        outDist = 0;
    }

    renderTaskPtr->appendGfx = flame_appendGfx;
    renderTaskPtr->dist = -outDist;
    renderTaskPtr->appendGfxArg = effect;
    renderTaskPtr->renderMode = RENDER_MODE_SURFACE_XLU_LAYER1;

    queue_render_task(renderTaskPtr);
}

#ifdef PORT
// PORT: Generate flame texture into alternating buffer (32×64 I8).
// Procedural teardrop flame shape with animated noise.
static void port_generate_flame_texture(FlameFXData* data, u8* buf) {
    s32 scroll = (s32)(data->unk_24 * 3.0f);
    s32 x, y;

    for (y = 0; y < 64; y++) {
        s32 flameY = 63 - y; // 0=bottom, 63=top

        // Vertical intensity: bright base with gradual falloff
        s32 vInt;
        if (flameY < 10) {
            vInt = 255;
        } else if (flameY < 42) {
            vInt = 255 - ((flameY - 10) * 255 / 32);
        } else {
            vInt = 0;
        }

        // Flame narrows toward top (teardrop shape)
        s32 halfW = 13 - (flameY * 12 / 42);
        if (halfW < 1) halfW = 1;

        for (x = 0; x < 32; x++) {
            s32 dx = x - 16;
            if (dx < 0) dx = -dx;

            if (dx > halfW || vInt == 0) {
                buf[y * 32 + x] = 0;
                continue;
            }

            // Horizontal falloff
            s32 hInt = 255 - (dx * dx * 255 / (halfW * halfW));

            // Animated procedural noise (flickery edges)
            u32 hash = (u32)(x * 7 + flameY * 13 + scroll) * 2654435761U;
            s32 noiseVal = (hash >> 24) & 0xFF;
            // Noise only affects outer edges — core stays solid
            s32 edgeDist = halfW - dx;
            s32 noiseMod = (edgeDist < 4) ? (noiseVal / 3) : 0;

            s32 raw = (vInt * hInt) / 255 - noiseMod;
            if (raw < 0) raw = 0;

            // Hard threshold mimicking N64 chroma key: binary opaque/transparent
            u8 intensity;
            if (raw < 20) {
                intensity = 0;
            } else {
                intensity = 220 + (raw * 35 / 255);
                if (intensity > 255) intensity = 255;
            }

            buf[y * 32 + x] = intensity;
        }
    }
}
#endif

void flame_appendGfx(void* effect) {
#ifdef PORT
    // PORT: Flame effect disabled — needs proper chroma key implementation.
    return;
#endif
    FlameFXData* data = ((EffectInstance*)effect)->data.flame;
    Camera* camera = &gCameras[gCurrentCameraID];
    s32 type = data->type;
    s32 uls = data->unk_1C * 4.0f;
    s32 ult = data->unk_24 * 4.0f;
    FlamePreset* preset;
    Matrix4f sp18;
    Matrix4f sp58;
    Matrix4f sp98;

    gDPPipeSync(gMainGfxPos++);
    gSPSegment(gMainGfxPos++, 0x09, VIRTUAL_TO_PHYSICAL(((EffectInstance*)effect)->shared->graphics));

    if (LastFlameRenderFrame != gGameStatusPtr->frameCounter) {
        LastFlameRenderFrame = gGameStatusPtr->frameCounter;
#ifdef PORT
        port_generate_flame_texture(data, D_800B32E0);
#else
        gSPDisplayList(gMainGfxPos++, D_09000918_3544C8);
        gDPSetTileSize(gMainGfxPos++, 1, uls, ult, uls + 128, ult + 256);
        gSPDisplayList(gMainGfxPos++, D_090009E0_354590);
#endif
        gDPSetColorImage(gMainGfxPos++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH, VIRTUAL_TO_PHYSICAL(nuGfxCfb_ptr));
        gDPSetScissorFrac(gMainGfxPos++, G_SC_NON_INTERLACE,
            camera->viewportStartX * 4.0f,
            camera->viewportStartY * 4.0f,
            (camera->viewportStartX + camera->viewportW) * 4.0f,
            (camera->viewportStartY + camera->viewportH) * 4.0f);
    }

    preset = &FlamePresets[type];

    // D_09000800_3543B0 loads D_800B32E0 texture + sets chroma key combine mode.
    // On PORT, D_800B32E0 is filled by port_generate_flame_texture().
    gSPDisplayList(gMainGfxPos++, preset->dlist);
    gDPSetKeyR(gMainGfxPos++, preset->keyCenter, preset->colorScale.r, 0);
    gDPSetKeyGB(gMainGfxPos++, preset->keyCenter, preset->colorScale.g, 0, preset->keyCenter, preset->colorScale.b, 0);
    gDPSetPrimColor(gMainGfxPos++, 0, 0, preset->primIntensity, preset->primIntensity, preset->primIntensity, 0);
    gDPSetEnvColor(gMainGfxPos++, preset->envColor.r, preset->envColor.g, preset->envColor.b, 0);

    guTranslateF(sp18, data->pos.x, data->pos.y, data->pos.z);
    guRotateF(sp58, -gCameras[gCurrentCameraID].curYaw, 0.0f, 1.0f, 0.0f);
    guMtxCatF(sp58, sp18, sp98);
    guScaleF(sp58, data->baseScale * data->scaleW, data->baseScale * data->scaleH, data->baseScale);
    guMtxCatF(sp58, sp98, sp98);
    guMtxF2L(sp98, &gDisplayContext->matrixStack[gMatrixListPos]);

    gSPMatrix(gMainGfxPos++, &gDisplayContext->matrixStack[gMatrixListPos++],
              G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
#ifdef PORT
    // PORT: Draw quad inline — ROM DL stub D_090008F8 gets overwritten with N64-format
    // data that corrupts 24-byte GBI_FLOATS vertices. Use static vertices instead.
    {
        static Vtx flameVtx[4] = {
            {{{ -16,   0, 0 }, 0, {     0, 63 << 5 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
            {{{  16,   0, 0 }, 0, { 31 << 5, 63 << 5 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
            {{{  16,  64, 0 }, 0, { 31 << 5,     0 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
            {{{ -16,  64, 0 }, 0, {     0,     0 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
        };
        gSPVertex(gMainGfxPos++, flameVtx, 4, 0);
        gSP2Triangles(gMainGfxPos++, 0, 1, 2, 0, 0, 2, 3, 0);
    }
#else
    gSPDisplayList(gMainGfxPos++, D_090008F8_3544A8);
#endif
    gSPPopMatrix(gMainGfxPos++, G_MTX_MODELVIEW);
    gDPPipeSync(gMainGfxPos++);
}
