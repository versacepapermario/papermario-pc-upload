// Extracted from ROM by extract_gfx.py
Gfx N(unk_32_gfx)[] = {
    {(uintptr_t)0xDB020000, (uintptr_t)0x00000018},  // gsMoveWd
    {(uintptr_t)0xDC08060A, (uintptr_t)((u8*)&N(stairs_lights) + 0x8)},  // gsSPMoveMem -> N(stairs_lights) + 0x8
    {(uintptr_t)0xDC08090A, (uintptr_t)&N(stairs_lights)},  // gsSPMoveMem -> N(stairs_lights)
    {(uintptr_t)0xDE000000, (uintptr_t)N(unk_31_gfx)},  // gsSPDisplayList -> N(unk_31_gfx)
    {(uintptr_t)0xDF000000, (uintptr_t)0x00000000},  // gsSPEndDisplayList
};
