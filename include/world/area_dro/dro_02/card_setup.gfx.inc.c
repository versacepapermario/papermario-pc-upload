// Extracted from ROM by extract_gfx.py
Gfx N(card_setup_gfx)[] = {
    {(uintptr_t)0xE7000000, (uintptr_t)0x00000000},  // gsDPPipeSync
    {(uintptr_t)0xE3000A01, (uintptr_t)0x00000000},  // gsDPSetOtherMode_L
    {(uintptr_t)0xD9C0F9FB, (uintptr_t)0x00000000},  // gsSPGeometryMode
    {(uintptr_t)0xD9FFFFFF, (uintptr_t)0x00200405},  // gsSPGeometryMode
    {(uintptr_t)0xD7000002, (uintptr_t)0xFFFFFFFF},  // gsSPTexture
    {(uintptr_t)0xE3000F00, (uintptr_t)0x00000000},  // gsDPSetOtherMode_L
    {(uintptr_t)0xE3001001, (uintptr_t)0x00008000},  // gsDPSetOtherMode_L
    {(uintptr_t)0xE3000C00, (uintptr_t)0x00080000},  // gsDPSetOtherMode_L
    {(uintptr_t)0xE3001201, (uintptr_t)0x00002000},  // gsDPSetOtherMode_L
    {(uintptr_t)0xE3001801, (uintptr_t)0x000000C0},  // gsDPSetOtherMode_L
    {(uintptr_t)0xE3000D01, (uintptr_t)0x00000000},  // gsDPSetOtherMode_L
    {(uintptr_t)0xE3001402, (uintptr_t)0x00000C00},  // gsDPSetOtherMode_L
    {(uintptr_t)0xE3001700, (uintptr_t)0x00000000},  // gsDPSetOtherMode_L
    {(uintptr_t)0xE2001E01, (uintptr_t)0x00000000},  // gsDPSetOtherMode_H
    {(uintptr_t)0xFCFFFFFF, (uintptr_t)0xFFFCF279},  // gsDPSetCombineMode
    {(uintptr_t)0xFD100000, (uintptr_t)dro_02_card_pal},  // gsDPSetTextureImage -> dro_02_card_pal
    {(uintptr_t)0xE8000000, (uintptr_t)0x00000000},  // gsDPTileSync
    {(uintptr_t)0xF5000100, (uintptr_t)0x07000000},  // gsDPSetTile
    {(uintptr_t)0xE6000000, (uintptr_t)0x00000000},  // gsDPLoadSync
    {(uintptr_t)0xF0000000, (uintptr_t)0x0703C000},  // gsDPLoadTLUT
    {(uintptr_t)0xE7000000, (uintptr_t)0x00000000},  // gsDPPipeSync
    {(uintptr_t)0xFD48000F, (uintptr_t)dro_02_card},  // gsDPSetTextureImage -> dro_02_card
    {(uintptr_t)0xF5480400, (uintptr_t)0x07054150},  // gsDPSetTile
    {(uintptr_t)0xE6000000, (uintptr_t)0x00000000},  // gsDPLoadSync
    {(uintptr_t)0xF4000000, (uintptr_t)0x0703E07C},  // gsDPLoadTile
    {(uintptr_t)0xE7000000, (uintptr_t)0x00000000},  // gsDPPipeSync
    {(uintptr_t)0xF5400400, (uintptr_t)0x00054150},  // gsDPSetTile
    {(uintptr_t)0xF2000000, (uintptr_t)0x0007C07C},  // gsDPSetTileSize
    {(uintptr_t)0xE200001C, (uintptr_t)0x00553078},  // gsDPSetOtherMode_H
    {(uintptr_t)0xDF000000, (uintptr_t)0x00000000},  // gsSPEndDisplayList
};
