#ifndef PORT_ASPECT_H
#define PORT_ASPECT_H

#ifdef PORT
// Current window aspect ratio, updated each frame by the port layer.
// Used by background rendering to pre-scale coordinates so they fill
// the full window after the interpreter's AdjXForAspectRatio correction.
extern float gPortWindowAspectRatio;

// Returns the X scale factor to counteract AdjXForAspectRatio for full-window rendering.
// Multiply N64 X coordinates by this to make them fill the full window width.
static inline float port_get_bg_x_scale(void) {
    float nativeAspect = 4.0f / 3.0f;
    if (gPortWindowAspectRatio <= 0.0f) return 1.0f;
    return gPortWindowAspectRatio / nativeAspect;
}
#endif

#endif
