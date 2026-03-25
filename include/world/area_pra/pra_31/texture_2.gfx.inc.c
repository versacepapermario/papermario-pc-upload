// Extracted from ROM by extract_gfx.py
Gfx N(texture_2_gfx)[] = {
    {(uintptr_t)0xE3001001, (uintptr_t)0x00008000},  // gsDPSetOtherMode_L
    {(uintptr_t)0xFD100000, (uintptr_t)N(texture_2_pal)},  // gsDPSetTextureImage -> N(texture_2_pal)
    {(uintptr_t)0xE8000000, (uintptr_t)0x00000000},  // gsDPTileSync
    {(uintptr_t)0xF5000100, (uintptr_t)0x07000000},  // gsDPSetTile
    {(uintptr_t)0xE6000000, (uintptr_t)0x00000000},  // gsDPLoadSync
    {(uintptr_t)0xF0000000, (uintptr_t)0x0703C000},  // gsDPLoadTLUT
    {(uintptr_t)0xE7000000, (uintptr_t)0x00000000},  // gsDPPipeSync
    {(uintptr_t)0xD7000002, (uintptr_t)0xFFFFFFFF},  // gsSPTexture
    {(uintptr_t)0xFC121824, (uintptr_t)0xFF33FFFF},  // gsDPSetCombineMode
    {(uintptr_t)0xE3000D01, (uintptr_t)0x00000000},  // gsDPSetOtherMode_L
    {(uintptr_t)0xE3000F00, (uintptr_t)0x00000000},  // gsDPSetOtherMode_L
    {(uintptr_t)0xFD500000, (uintptr_t)N(texture_2_img)},  // gsDPSetTextureImage -> N(texture_2_img)
    {(uintptr_t)0xF5500000, (uintptr_t)0x07018060},  // gsDPSetTile
    {(uintptr_t)0xE6000000, (uintptr_t)0x00000000},  // gsDPLoadSync
    {(uintptr_t)0xF3000000, (uintptr_t)0x073FF200},  // gsDPLoadBlock
    {(uintptr_t)0xE7000000, (uintptr_t)0x00000000},  // gsDPPipeSync
    {(uintptr_t)0xF5400800, (uintptr_t)0x00018060},  // gsDPSetTile
    {(uintptr_t)0xF2000000, (uintptr_t)0x000FC0FC},  // gsDPSetTileSize
    {(uintptr_t)0xE3000C00, (uintptr_t)0x00080000},  // gsDPSetOtherMode_L
    {(uintptr_t)0xE3001201, (uintptr_t)0x00002000},  // gsDPSetOtherMode_L
    {(uintptr_t)0xDF000000, (uintptr_t)0x00000000},  // gsSPEndDisplayList
};
