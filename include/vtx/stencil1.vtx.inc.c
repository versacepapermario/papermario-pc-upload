// Star shape vertices for battle transition stencil effect
// 5-pointed star: 5 inner vertices + 5 outer vertices, all at z=0
// Color is white with alpha=0 (alpha comes from prim_color via PM_CC_SCREEN_OVERLAY)
Vtx vtx_stencil1[] = {
    {{{ -128,  169, 0 }, 0, { 0, 0 }, { 255, 255, 255, 0 }}},  // V0: inner
    {{{ -204,  -69, 0 }, 0, { 0, 0 }, { 255, 255, 255, 0 }}},  // V1: inner
    {{{    0, -220, 0 }, 0, { 0, 0 }, { 255, 255, 255, 0 }}},  // V2: inner
    {{{  128,  169, 0 }, 0, { 0, 0 }, { 255, 255, 255, 0 }}},  // V3: inner
    {{{ -376,  123, 0 }, 0, { 0, 0 }, { 255, 255, 255, 0 }}},  // V4: outer (left)
    {{{    0,  395, 0 }, 0, { 0, 0 }, { 255, 255, 255, 0 }}},  // V5: outer (top)
    {{{ -233, -320, 0 }, 0, { 0, 0 }, { 255, 255, 255, 0 }}},  // V6: outer (bottom-left)
    {{{  204,  -69, 0 }, 0, { 0, 0 }, { 255, 255, 255, 0 }}},  // V7: inner
    {{{  232, -320, 0 }, 0, { 0, 0 }, { 255, 255, 255, 0 }}},  // V8: outer (bottom-right)
    {{{  376,  122, 0 }, 0, { 0, 0 }, { 255, 255, 255, 0 }}},  // V9: outer (right)
};
