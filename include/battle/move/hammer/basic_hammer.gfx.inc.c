// Auto-generated display list stub for PC port
Gfx N(basic_hammer_gfx)[] = {
    gsDPPipeSync(),
    gsSPTexture(-1, -1, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsDPLoadTLUT_pal16(0, N(basic_hammer_pal)),
    gsSPVertex(N(vtx), 4, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSP1Triangle(0, 2, 3, 0),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};
