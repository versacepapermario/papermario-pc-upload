/**
 * GBI Middleware for PaperShip
 *
 * Intercepts N64 GBI (Graphics Binary Interface) calls to load resources
 * from OTR/O2R archives instead of ROM addresses. This is the bridge
 * between Paper Mario's display list generation code and libultraship's
 * Fast3D renderer.
 *
 * Paper Mario uses F3DEX2 microcode (same as OoT), so all GBI commands
 * are well-supported by libultraship.
 *
 * Based on Ghostship/src/port/GBIMiddleware.cpp
 */
#include <libultraship.h>

#include "Engine.h"
#include <fast/resource/ResourceType.h>
#include <fast/resource/type/DisplayList.h>

// GfxPatch — for runtime display list patching (cosmetics, bug fixes)
typedef struct {
    int index;
    Gfx instruction;
} GfxPatch;

std::unordered_map<std::string, std::unordered_map<std::string, std::vector<GfxPatch>>> originalGfx;

/**
 * Patch a display list resource by name.
 * Used for cosmetic changes and bug fixes that modify specific GBI commands.
 */
extern "C" void ResourceMgr_PatchGfxByName(const char* path, const char* patchName, GfxPatch* patches, int patchCount) {
    auto res = std::static_pointer_cast<Fast::DisplayList>(
        Ship::Context::GetInstance()->GetResourceManager()->LoadResource(path));

    // Do not patch custom assets
    if (res->GetInitData()->IsCustom) {
        return;
    }

    for (int i = 0; i < patchCount; i++) {
        GfxPatch patch = patches[i];

        if (originalGfx.find(path) == originalGfx.end() ||
            originalGfx[path].find(patchName) == originalGfx[path].end()) {
            originalGfx[path][patchName] = {};
            Gfx originalGfxCmd = res->Instructions[patch.index];
            GfxPatch originalPatch = { patch.index, originalGfxCmd };
            originalGfx[path][patchName].push_back(originalPatch);
        }
        res->Instructions[patch.index] = patch.instruction;
    }
}

/**
 * gSPDisplayList - Call a sub-display list
 * If the display list pointer has an OTR signature, load it from the archive.
 */
extern "C" void gSPDisplayList(Gfx* pkt, Gfx* dl) {
    char* imgData = (char*)dl;

    if (GameEngine_OTRSigCheck(imgData) == 1) {
        auto resource = Ship::Context::GetInstance()->GetResourceManager()->LoadResource(imgData);
        auto res = std::static_pointer_cast<Fast::DisplayList>(resource);
        dl = &res->Instructions[0];
    }

    // Validate display list pointer before submitting
    if (dl != nullptr) {
        uint8_t opcode = (dl->words.w0 >> 24) & 0xFF;
        // Valid F3DEX2 opcodes: 0x00-0x07 (geometry), 0xD7-0xFF (RDP)
        if (opcode > 0x07 && opcode < 0xD7) {
            static int sGarbageMwCount = 0;
            if (sGarbageMwCount < 10) {
                fprintf(stderr, "[GBIMw] SKIP garbage DL %p opcode=0x%02X w0=0x%lX w1=0x%lX (pkt=%p)\n",
                        (void*)dl, opcode, (unsigned long)dl->words.w0,
                        (unsigned long)dl->words.w1, (void*)pkt);
                sGarbageMwCount++;
            }
            // Skip this garbage display list
            gSPNoOp(pkt);
            return;
        }
    }

    __gSPDisplayList(pkt, dl);
}

/**
 * gSPVertex - Load vertices
 * If the vertex data pointer has an OTR signature, load it from the archive.
 */
extern "C" void gSPVertex(Gfx* pkt, uintptr_t v, int n, int v0) {
    if (GameEngine_OTRSigCheck((char*)v) == 1) {
        v = (uintptr_t)ResourceGetDataByName((char*)v);
    }

    __gSPVertex(pkt, v, n, v0);
}

/**
 * gSPInvalidateTexCache - Invalidate texture cache entry
 * Resolves OTR-based texture addresses before invalidating.
 */
extern "C" void gSPInvalidateTexCache(Gfx* pkt, uintptr_t texAddr) {
    char* imgData = (char*)texAddr;

    if (texAddr != 0 && GameEngine_OTRSigCheck(imgData)) {
        auto res = Ship::Context::GetInstance()->GetResourceManager()->LoadResource(imgData);

        if (res->GetInitData()->Type == (uint32_t)Fast::ResourceType::DisplayList)
            texAddr = (uintptr_t) & ((std::static_pointer_cast<Fast::DisplayList>(res))->Instructions[0]);
        else {
            texAddr = (uintptr_t)res->GetRawPointer();
        }
    }
    __gSPInvalidateTexCache(pkt, texAddr);
}

/**
 * ResourceMgr_FileExists - Check if a resource exists in any loaded archive
 */
extern "C" uint8_t ResourceMgr_FileExists(const char* filePath) {
    std::string path = filePath;
    if (path.substr(0, 7) == "__OTR__") {
        path = path.substr(7);
    }
    return 1; // TODO: Actually check archive
}
