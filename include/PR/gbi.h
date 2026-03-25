/**
 * PR/gbi.h - Bridge stub for PaperShip PC port
 * Redirects to libultraship's GBI which provides all types (Vtx, Mtx, Vp,
 * Light, Gfx, etc.) and all GBI macros (gSPVertex, gDPSetTextureImage, etc.).
 */
#ifndef _GBI_H_
#define _GBI_H_

#include <libultra/gbi.h>

// libultraship defines these with __ prefix; alias to standard N64 SDK names
#ifndef gSPDisplayList
#define gSPDisplayList(pkt, dl) __gSPDisplayList(pkt, dl)
#endif
#ifndef gSPVertex
#define gSPVertex(pkt, v, n, v0) __gSPVertex(pkt, v, n, v0)
#endif
#ifndef gSPSegment
#define gSPSegment(pkt, segment, base) __gSPSegment(pkt, segment, base)
#endif
#ifndef gSPInvalidateTexCache
#define gSPInvalidateTexCache(pkt, texAddr) __gSPInvalidateTexCache(pkt, texAddr)
#endif

#endif /* _GBI_H_ */
