// Level_up big_E texture load display list (64x64 IA8)
extern unsigned char D_802A6670_7A45B0[];
Gfx D_802A7670_7A55B0[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 1, D_802A6670_7A45B0),
    gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_8b, 0, 0, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 6, 0, G_TX_NOMIRROR | G_TX_CLAMP, 6, 0),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 64 * 64 - 1, CALC_DXT(64, G_IM_SIZ_8b_BYTES)),
    gsDPPipeSync(),
    gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_8b, 8, 0, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 6, 0, G_TX_NOMIRROR | G_TX_CLAMP, 6, 0),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, (64 - 1) << 2, (64 - 1) << 2),
    gsSPEndDisplayList(),
};
