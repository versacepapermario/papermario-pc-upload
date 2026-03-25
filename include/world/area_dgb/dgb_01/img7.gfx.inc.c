// Extracted from ROM by extract_gfx.py
Gfx N(img7_gfx)[] = {
    {(uintptr_t)0xE3001001, (uintptr_t)0x00000000},  // gsDPSetOtherMode_L
    {(uintptr_t)0xD7000002, (uintptr_t)0xFFFFFFFF},  // gsSPTexture
    {(uintptr_t)0xFC121824, (uintptr_t)0xFF33FFFF},  // gsDPSetCombineMode
    {(uintptr_t)0xE3000D01, (uintptr_t)0x00000000},  // gsDPSetOtherMode_L
    {(uintptr_t)0xE3000F00, (uintptr_t)0x00000000},  // gsDPSetOtherMode_L
    {(uintptr_t)0xFD100000, (uintptr_t)dgb_01_img7_png},  // gsDPSetTextureImage -> dgb_01_img7_png
    {(uintptr_t)0xF5100000, (uintptr_t)0x07014050},  // gsDPSetTile
    {(uintptr_t)0xE6000000, (uintptr_t)0x00000000},  // gsDPLoadSync
    {(uintptr_t)0xF3000000, (uintptr_t)0x073FF100},  // gsDPLoadBlock
    {(uintptr_t)0xE7000000, (uintptr_t)0x00000000},  // gsDPPipeSync
    {(uintptr_t)0xF5101000, (uintptr_t)0x00014050},  // gsDPSetTile
    {(uintptr_t)0xF2000000, (uintptr_t)0x0007C07C},  // gsDPSetTileSize
    {(uintptr_t)0xE3000C00, (uintptr_t)0x00080000},  // gsDPSetOtherMode_L
    {(uintptr_t)0xE3001201, (uintptr_t)0x00002000},  // gsDPSetOtherMode_L
    {(uintptr_t)0xDF000000, (uintptr_t)0x00000000},  // gsSPEndDisplayList
    {(uintptr_t)0x00000000, (uintptr_t)0x00000000},  // op_0x00
};
