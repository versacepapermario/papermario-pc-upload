/**
 * effect_shim_stubs.c - Implementations of shim_* wrapper functions for effects.
 *
 * On N64, effect code is loaded into TLB-mapped pages at runtime and cannot
 * directly reference main-binary symbols. Instead, a shim table is set up so
 * effects call shim_* wrappers which then jump to the real functions.
 *
 * On PC, all code is statically linked so the shims just forward directly.
 * We MUST NOT include effect_shims.h here, because it #defines the real
 * function names to shim_* names, which would cause infinite recursion.
 *
 * The real function declarations come from common.h (which includes
 * functions.h, ultra64.h -> gu.h, etc.) and effects.h.
 */

#include "common.h"
#include "effects.h"

// Audio functions not in common.h
extern void sfx_play_sound_at_position(s32 soundID, s32 flags, f32 posX, f32 posY, f32 posZ);

// ---------------------------------------------------------------------------
// libultra gu* shims
// ---------------------------------------------------------------------------

void shim_guRotateF(float mf[4][4], float a, float x, float y, float z) {
    guRotateF(mf, a, x, y, z);
}

void shim_guTranslateF(float mf[4][4], float x, float y, float z) {
    guTranslateF(mf, x, y, z);
}

void shim_guTranslate(Mtx *m, float x, float y, float z) {
    guTranslate(m, x, y, z);
}

void shim_guScaleF(float mf[4][4], float x, float y, float z) {
    guScaleF(mf, x, y, z);
}

void shim_guMtxCatF(float m[4][4], float n[4][4], float r[4][4]) {
    guMtxCatF(m, n, r);
}

void shim_guMtxF2L(float mf[4][4], Mtx *m) {
    guMtxF2L(mf, m);
}

void shim_guPerspectiveF(f32 mf[4][4], u16* perspNorm, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale) {
    guPerspectiveF(mf, perspNorm, fovy, aspect, near, far, scale);
}

void shim_guPositionF(float mf[4][4], float r, float p, float h, float s, float x, float y, float z) {
    guPositionF(mf, r, p, h, s, x, y, z);
}

void shim_guOrthoF(float mf[4][4], float l, float r, float b, float t, float n, float f, float scale) {
    guOrthoF(mf, l, r, b, t, n, f, scale);
}

void shim_guFrustumF(float mf[4][4], float l, float r, float b, float t, float n, float f, float scale) {
    guFrustumF(mf, l, r, b, t, n, f, scale);
}

// ---------------------------------------------------------------------------
// Effect system shims
// ---------------------------------------------------------------------------

RenderTask* shim_queue_render_task(RenderTask* task) {
    return queue_render_task(task);
}

EffectInstance* shim_create_effect_instance(EffectBlueprint* effectBp) {
    return create_effect_instance(effectBp);
}

void shim_remove_effect(EffectInstance* effect) {
    remove_effect(effect);
}

s32 shim_load_effect(s32 effectIndex) {
    return load_effect(effectIndex);
}

// ---------------------------------------------------------------------------
// General utility shims
// ---------------------------------------------------------------------------

void* shim_general_heap_malloc(s32 size) {
    return general_heap_malloc(size);
}

void shim_mem_clear(void* data, s32 numBytes) {
    mem_clear(data, numBytes);
}

s32 shim_rand_int(s32 max) {
    return rand_int(max);
}

f32 shim_clamp_angle(f32 theta) {
    return clamp_angle(theta);
}

// ---------------------------------------------------------------------------
// Math shims
// ---------------------------------------------------------------------------

f32 shim_sin_deg(f32 x) {
    return sin_deg(x);
}

f32 shim_cos_deg(f32 x) {
    return cos_deg(x);
}

// Note: common.h does #define atan2 pm_atan2, so we call pm_atan2 directly.
f32 shim_atan2(f32 startX, f32 startZ, f32 endX, f32 endZ) {
    return pm_atan2(startX, startZ, endX, endZ);
}

float shim_sqrtf(float value) {
    return sqrtf(value);
}

void shim_transform_point(Matrix4f mtx, f32 inX, f32 inY, f32 inZ, f32 inS, f32* outX, f32* outY, f32* outZ, f32* outW) {
    transform_point(mtx, inX, inY, inZ, inS, outX, outY, outZ, outW);
}

// ---------------------------------------------------------------------------
// Collision shims
// ---------------------------------------------------------------------------

bool shim_npc_raycast_down_sides(s32 ignoreFlags, f32* posX, f32* posY, f32* posZ, f32* hitDepth) {
    return npc_raycast_down_sides(ignoreFlags, posX, posY, posZ, hitDepth);
}

// ---------------------------------------------------------------------------
// Rendering / UI shims
// ---------------------------------------------------------------------------

void shim_mdl_draw_hidden_panel_surface(Gfx** arg0, u16 treeIndex) {
    mdl_draw_hidden_panel_surface(arg0, treeIndex);
}

bool shim_is_point_visible(f32 x, f32 y, f32 z, s32 depthQueryID, f32* screenX, f32* screenY) {
    return is_point_visible(x, y, z, depthQueryID, screenX, screenY);
}

void shim_draw_prev_frame_buffer_at_screen_pos(s32 a, s32 b, s32 c, s32 d, f32 e) {
    draw_prev_frame_buffer_at_screen_pos(a, b, c, d, e);
}

// Note: The shim signature uses void return and void* callback,
// while the real draw_box returns s32 and has a different callback signature.
// We cast through to maintain binary compatibility.
void shim_draw_box(
    s32 flags, WindowStyle windowStyle, s32 posX, s32 posY, s32 posZ, s32 width, s32 height, u8 opacity,
    u8 darkening, f32 scaleX, f32 scaleY, f32 rotX, f32 rotY, f32 rotZ, void (*fpDrawContents)(void*),
    void* drawContentsArg0, Matrix4f rotScaleMtx, s32 translateX, s32 translateY, f32 (*outMtx)[4]
) {
    draw_box(flags, windowStyle, posX, posY, posZ, width, height, opacity,
             darkening, scaleX, scaleY, rotX, rotY, rotZ,
             (void (*)(s32, s32, s32, s32, s32, s32, s32))fpDrawContents,
             drawContentsArg0, rotScaleMtx, translateX, translateY, outMtx);
}

void shim_draw_msg(s32 msgID, s32 posX, s32 posY, s32 opacity, s32 palette, s32 style) {
    draw_msg(msgID, posX, posY, opacity, palette, (u8)style);
}

s32 shim_get_msg_width(s32 msgID, u16 charset) {
    return get_msg_width(msgID, charset);
}

void shim_mdl_get_shroud_tint_params(u8* r, u8* g, u8* b, u8* a) {
    mdl_get_shroud_tint_params(r, g, b, a);
}

// ---------------------------------------------------------------------------
// Audio shims
// ---------------------------------------------------------------------------

void shim_sfx_play_sound_at_position(s32 soundID, s32 value2, f32 posX, f32 posY, f32 posZ) {
    sfx_play_sound_at_position(soundID, value2, posX, posY, posZ);
}
