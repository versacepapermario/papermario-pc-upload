#include "common.h"
#include "effects_internal.h"
#include "nu/nusys.h"

extern Gfx D_09000200_3A35D0[];
extern Gfx D_090002A8_3A3678[];
extern Gfx D_09000358_3A3728[];

Gfx* D_E00A29D0[] = { D_09000200_3A35D0, D_090002A8_3A3678, D_09000358_3A3728 };

typedef struct UnkStruct {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ f32 unk_08;
    /* 0x0C */ f32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
} UnkStruct; // size = 0x18

UnkStruct D_E00A29DC[] = {
    { 16, 16, 1.0f, 1.0f, 8, 8 },
    { 16, 16, 1.0f, 1.0f, 8, 8 },
    { 16, 16, 1.0f, 1.0f, 8, 8 },
};

s32 D_E00A2A24[] = { 100, 60, 30, 10, 0, 0, 0 };

void motion_blur_flame_init(EffectInstance* effect);
void motion_blur_flame_update(EffectInstance* effect);
void motion_blur_flame_render(EffectInstance* effect);
void motion_blur_flame_appendGfx(void* effect);

EffectInstance* motion_blur_flame_main(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    EffectBlueprint bp;
    EffectInstance* effect;
    MotionBlurFlameFXData* data;
    s32 numParts = 1;
    s32 i;

    bp.init = motion_blur_flame_init;
    bp.update = motion_blur_flame_update;
    bp.renderScene = motion_blur_flame_render;
    bp.unk_00 = 0;
    bp.renderUI = nullptr;
    bp.effectID = EFFECT_MOTION_BLUR_FLAME;

    effect = create_effect_instance(&bp);
    effect->numParts = numParts;
    data = effect->data.motionBlurFlame = general_heap_malloc(numParts * sizeof(*data));
    ASSERT(effect->data.motionBlurFlame != nullptr);

    data->unk_00 = arg0;
    if (arg5 < 0) {
        data->unk_50 = 1000;
    } else {
        data->unk_50 = arg5;
    }
    data->unk_4C = 127;
    data->unk_54 = 0;
    data->unk_04 = arg1;
    data->unk_08 = arg2;
    data->unk_0C = arg3;
    data->pos.x = 0;
    data->pos.y = 0;
    data->pos.z = 0;
    data->unk_5C = 255;
    data->unk_60 = 255;
    data->unk_64 = 255;
    data->unk_3C[1] = 0;
    data->unk_3C[2] = 0;
    data->unk_3C[3] = 0;

    return effect;
}

void motion_blur_flame_init(EffectInstance* effect) {
}

void motion_blur_flame_update(EffectInstance* effect) {
    MotionBlurFlameFXData* data = effect->data.motionBlurFlame;
    s32 temp;

    if (effect->flags & FX_INSTANCE_FLAG_DISMISS) {
        effect->flags &= ~FX_INSTANCE_FLAG_DISMISS;
        data->unk_50 = 30;
    }
    data->unk_54++;
    if (data->unk_50 < 1000) {
        data->unk_50--;
    }

    if (data->unk_50 < 0) {
        remove_effect(effect);
        return;
    }
    temp = data->unk_50;
    if (data->unk_54 <= 16) {
        data->unk_4C = (data->unk_54 * 8) - 1;
    }
    if (temp < 16) {
        data->unk_4C = temp * 8;
    }

    temp = 0;
    data->unk_1C[temp] = data->unk_04 + data->pos.x;
    data->unk_2C[temp] = data->unk_08 + data->pos.y;
    data->unk_3C[temp] = data->unk_0C + data->pos.z;
}

void motion_blur_flame_render(EffectInstance* effect) {
    RenderTask renderTask;
    RenderTask* retTask;

    renderTask.appendGfx = motion_blur_flame_appendGfx;
    renderTask.appendGfxArg = effect;
    renderTask.dist = 100;
    renderTask.renderMode = RENDER_MODE_CLOUD_NO_ZCMP;

    retTask = queue_render_task(&renderTask);
#ifndef PORT
    // PORT: Reflection pass reads framebuffer which we can't do — skip reflected render
    retTask->renderMode |= RENDER_TASK_FLAG_REFLECT_FLOOR;
#endif
}

void func_E00A2234(void) {
}

void motion_blur_flame_appendGfx(void* effect) {
#ifdef PORT
    // PORT: Original reads framebuffer strips + I8 radial gradient textures to create
    // circular glow halos. Can't read framebuffer on PC. Use the effect's own I8 glow
    // texture with additive blending to approximate the circular orb shape.
    {
        extern u8 D_09000000_3A33D0[]; // 16x16 I8 radial gradient texture
        MotionBlurFlameFXData* data = ((EffectInstance*)effect)->data.motionBlurFlame;
        s32 sp50 = data->unk_4C;
        UnkStruct* cfg = &D_E00A29DC[data->unk_00];
        s32 halfExtent = cfg->unk_10; // 8 pixels
        s32 sp18[UNK_ARRAY_SIZE_1];
        f32 sp28[UNK_ARRAY_SIZE_1];
        f32 sp38[UNK_ARRAY_SIZE_1];
        s32 i;

        gDPPipeSync(gMainGfxPos++);
        gDPSetCycleType(gMainGfxPos++, G_CYC_1CYCLE);
        // Color = PRIMITIVE (flat white), Alpha = TEXEL0 * PRIMITIVE (gradient mask)
        gDPSetCombineLERP(gMainGfxPos++,
            0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0,
            0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0);
        // Additive blending: src*alpha + dst*1 (brightens background)
        gDPSetRenderMode(gMainGfxPos++,
            IM_RD | FORCE_BL | GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1),
            IM_RD | FORCE_BL | GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1));
        gDPSetTextureLUT(gMainGfxPos++, G_TT_NONE);
        gSPTexture(gMainGfxPos++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
        gDPLoadTextureBlock(gMainGfxPos++, D_09000000_3A33D0, G_IM_FMT_I, G_IM_SIZ_8b, 16, 16, 0,
            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 4, 4, G_TX_NOLOD, G_TX_NOLOD);
        gDPSetTexturePersp(gMainGfxPos++, G_TP_NONE);
        gDPSetTextureFilter(gMainGfxPos++, G_TF_BILERP);

        for (i = 0; i < UNK_ARRAY_SIZE_1; i++) {
            f32 fx = data->unk_1C[i];
            f32 fy = data->unk_2C[i];
            f32 fz = data->unk_3C[i];
            f32 centerX, centerY;
            s32 isVisible;

            if (i == 0) {
                isVisible = is_point_visible(fx, fy, fz, -1, &centerX, &centerY);
            } else {
                isVisible = (s32)fz;
                centerX = fx;
                centerY = fy;
            }

            sp18[i] = isVisible;
            sp28[i] = centerX;
            sp38[i] = centerY;

            if (isVisible && centerX >= 0.0f && centerY >= 0.0f &&
                centerX < SCREEN_WIDTH && centerY < SCREEN_HEIGHT) {
                s32 temp_a0 = (sp50 * D_E00A2A24[i]) / 100;

                if (temp_a0 > 0) {
                    s32 alpha = temp_a0;
                    if (alpha > 255) alpha = 255;
                    s32 xMin = (s32)(centerX - halfExtent);
                    s32 yMin = (s32)(centerY - halfExtent);
                    s32 xMax = (s32)(centerX + halfExtent);
                    s32 yMax = (s32)(centerY + halfExtent);

                    if (xMin < 0) xMin = 0;
                    if (yMin < 0) yMin = 0;
                    if (xMax >= SCREEN_WIDTH) xMax = SCREEN_WIDTH - 1;
                    if (yMax >= SCREEN_HEIGHT) yMax = SCREEN_HEIGHT - 1;

                    if (xMin < xMax && yMin < yMax) {
                        gDPSetPrimColor(gMainGfxPos++, 0, 0, 255, 255, 255, alpha);
                        gSPTextureRectangle(gMainGfxPos++,
                            xMin * 4, yMin * 4, xMax * 4, yMax * 4,
                            G_TX_RENDERTILE, 0, 0, 0x0400, 0x0400);
                        gDPPipeSync(gMainGfxPos++);
                    }
                }
            }
        }

        // Restore default render state so it doesn't leak into subsequent draws (e.g. water)
        gDPPipeSync(gMainGfxPos++);
        gDPSetRenderMode(gMainGfxPos++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetCombineMode(gMainGfxPos++, G_CC_SHADE, G_CC_SHADE);
        gSPTexture(gMainGfxPos++, 0, 0, 0, G_TX_RENDERTILE, G_OFF);

        for (i = 0; i < ARRAY_COUNT(data->unk_3C) - 1; i++) {
            data->unk_3C[i + 1] = sp18[i];
            data->unk_1C[i + 1] = sp28[i];
            data->unk_2C[i + 1] = sp38[i];
        }
    }
    return;
#endif
    MotionBlurFlameFXData* data = ((EffectInstance*)effect)->data.motionBlurFlame;
    s32 temp_a2 = data->unk_00;
    f32 sp48;
    f32 sp4C;
    s32 sp50 = data->unk_4C;
    UnkStruct* temp_s1 = &D_E00A29DC[temp_a2];
    s32 sp54; // declaration must come after sp50
    s32 envR, envG, envB;
    s32 sp18[4];
    f32 sp28[4];
    f32 sp38[4];
    s32 i;

    s32 t3;
    s32 v0;
    s32 temp_a0;
    s32 s3;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v1;
    s32 var_a0;
    s32 var_a1;
    s32 var_a2;
    s32 uly;
    s32 var_t2_2;
    s32 var_t4;
    s32 t6;
    s32 var_v1;

    gDPPipeSync(gMainGfxPos++);
    gSPSegment(gMainGfxPos++, 0x09, VIRTUAL_TO_PHYSICAL(((EffectInstance*)effect)->shared->graphics));
    gSPDisplayList(gMainGfxPos++, D_E00A29D0[temp_a2]);

    for (i = 0; i < UNK_ARRAY_SIZE_1; i++) {
        f32 temp_f12 = data->unk_1C[i];
        f32 temp_f14 = data->unk_2C[i];
        f32 temp_f0 = data->unk_3C[i];
        s32 temp_s5 = temp_s1->unk_10;
        s32 height = temp_s1->unk_14;

        if (i == 0) {
            var_a2 = is_point_visible(temp_f12, temp_f14, temp_f0, -1, &sp48, &sp4C);
        } else {
            var_a2 = temp_f0;
            sp48 = temp_f12;
            sp4C = temp_f14;
        }

        sp18[i] = var_a2;
        sp28[i] = sp48;
        sp38[i] = sp4C;

        if ((var_a2 != 0) && !(sp48 < 0.0f) && !(sp4C < 0.0f) && !(sp48 >= 320.0f) && !(sp4C >= 240.0f)) {
            temp_a0 = (sp50 * D_E00A2A24[i]) / 100;
            temp_v1 = (data->unk_5C * temp_a0) >> 9;
            temp_v0_2 = (data->unk_60 * temp_a0) >> 9;
            temp_v0_3 = (data->unk_64 * temp_a0) >> 9;

            gDPSetPrimColor(gMainGfxPos++, 0, 0,
                (temp_v1 * temp_a0) >> 8,
                (temp_v0_2 * temp_a0) >> 8,
                (temp_v0_3 * temp_a0) >> 8,
                255
            );

            envR = temp_v1 + 32;
            envG = temp_v0_2 + 32;
            envB = temp_v0_3 + 32;
            if (envR > 127) {
                envR = 127;
            }
            if (envG > 127) {
                envG = 127;
            }
            if (envB > 127) {
                envB = 127;
            }

            gDPSetEnvColor(gMainGfxPos++, envR, envG, envB, 0);

            s3 = sp48 - temp_s5;
            t6 = s3 + temp_s5 * 2;
            v0 = sp4C - temp_s5;
            var_a0 = v0 + 2 * temp_s5;
            t3 = uly = sp4C - temp_s5;

            if (s3 < 0) {
                var_a1 = -s3;
            } else {
                var_a1 = 0;
            }

            if (t3 < 0) {
                var_v1 = -uly;
            } else {
                var_v1 = 0;
            }

            if (t6 > SCREEN_WIDTH) {
                t6 = SCREEN_WIDTH - 1;
            }
            if (var_a0 > SCREEN_HEIGHT) {
                var_a0 = SCREEN_HEIGHT - 1;
            }

            var_t2_2 = var_v1 / height;
            sp54 = (var_a0 - t3) / height;

            while (var_t2_2 < sp54) {
                uly = t3 + var_t2_2 * height;
                if (uly + height >= SCREEN_HEIGHT) {
                    break;
                }

                gDPSetTileSize(gMainGfxPos++, G_TX_RENDERTILE,
                    (s32) (s3 * temp_s1->unk_08) * 4,
                    (s32) (temp_s1->unk_04 * 20 - var_t2_2 * temp_s1->unk_14 * temp_s1->unk_0C) * 4,
                    (s32) (s3 * temp_s1->unk_08 + temp_s1->unk_00) * 4,
                    (s32) (temp_s1->unk_04 * 21 - var_t2_2 * temp_s1->unk_14 * temp_s1->unk_0C) * 4);

                for (var_t4 = 0; var_t4 < 2; var_t4++) {
                    gDPLoadMultiTile(gMainGfxPos++, VIRTUAL_TO_PHYSICAL(nuGfxCfb_ptr + uly * SCREEN_WIDTH),
                        0x0100, 1, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH, SCREEN_HEIGHT,
                        s3 + var_a1,
                        0,
                        t6 - 1,
                        height - 1,
                        0,
                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 9, 8, G_TX_NOLOD, G_TX_NOLOD
                    );
                    gSPTextureRectangle(gMainGfxPos++,
                        (s3 + var_a1) * 4,
                        uly * 4,
                        t6 * 4,
                        (uly + height) * 4,
                        G_TX_RENDERTILE,
                        ((u32) (s3 + var_a1) % 0x200) * 32,
                        0,
                        0x0400,
                        0x0400
                    );
                    gDPPipeSync(gMainGfxPos++);
                }
                var_t2_2++;
            }
        }
    }

    for (i = 0; i < ARRAY_COUNT(data->unk_3C) - 1; i++) {
        data->unk_3C[i + 1] = sp18[i];
        data->unk_1C[i + 1] = sp28[i];
        data->unk_2C[i + 1] = sp38[i];
    }
}
