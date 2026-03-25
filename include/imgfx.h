#ifndef _IMGFX_H_
#define _IMGFX_H_

#include "PR/gbi.h"

// 'compressed' vertex data for animated image fx keyframes
typedef struct ImgFXVtx {
    /* 0x00 */ s16 ob[3];
    /* 0x06 */ u8 tc[2];
    /* 0x08 */ s8 cn[3];
    /* 0x0B */ char unk_0B;
} ImgFXVtx; // size = 0x0C

typedef struct ImgFXAnimHeader {
#ifdef PORT
    // On PC, these are ROM offsets (not valid PC pointers), and must be u32
    // to preserve the N64 struct layout (sizeof = 0x10) for DMA compatibility.
    /* 0x00 */ u32 keyframesOffset;
    /* 0x04 */ u32 gfxOffset;
#else
    /* 0x00 */ ImgFXVtx* keyframesOffset;
    /* 0x04 */ Gfx* gfxOffset; // Gfx for creating mesh from vertices
#endif
    /* 0x08 */ u16 vtxCount; // conserved across keyframes
    /* 0x0A */ u16 gfxCount;
    /* 0x0C */ u16 keyframesCount;
    /* 0x0E */ u16 flags;
} ImgFXAnimHeader; // size = 0x10

#endif /* _IMGFX_H_ */
