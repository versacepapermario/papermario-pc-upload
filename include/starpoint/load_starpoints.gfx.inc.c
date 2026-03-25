// Starpoints text texture load display list (128x32 IA8)
extern unsigned char D_802A21D8_7A9B78[];
Gfx D_802A31D8_7AAB78[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 1, D_802A21D8_7A9B78),
    gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_8b, 0, 0, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 5, 0, G_TX_NOMIRROR | G_TX_CLAMP, 7, 0),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 128 * 32 - 1, CALC_DXT(128, G_IM_SIZ_8b_BYTES)),
    gsDPPipeSync(),
    gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_8b, 16, 0, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 5, 0, G_TX_NOMIRROR | G_TX_CLAMP, 7, 0),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, (128 - 1) << 2, (32 - 1) << 2),
    gsSPEndDisplayList(),
};
