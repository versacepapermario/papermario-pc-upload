// Extracted from ROM by extract_gfx.py
Gfx N(card_2_gfx)[] = {
    {(uintptr_t)0xE7000000, (uintptr_t)0x00000000},  // gsDPPipeSync
    {(uintptr_t)0xFCFFFFFF, (uintptr_t)0xFFFCF279},  // gsDPSetCombineMode
    {(uintptr_t)0xD9FFF9FF, (uintptr_t)0x00000000},  // gsSPGeometryMode
    {(uintptr_t)0xD9FFFFFF, (uintptr_t)0x00000200},  // gsSPGeometryMode
    {(uintptr_t)0x01004008, (uintptr_t)vtx_card_2},  // gsSPVertex -> vtx_card_2
    {(uintptr_t)0x06000402, (uintptr_t)0x00020406},  // gsSP2Triangles
    {(uintptr_t)0xD9FFFFFF, (uintptr_t)0x00000400},  // gsSPGeometryMode
    {(uintptr_t)0xE7000000, (uintptr_t)0x00000000},  // gsDPPipeSync
    {(uintptr_t)0xDF000000, (uintptr_t)0x00000000},  // gsSPEndDisplayList
};
