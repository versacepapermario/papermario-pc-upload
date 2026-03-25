#ifndef _COMMON_H_
#define _COMMON_H_

// Standard library headers needed by decomp code
// (N64 SDK provided these implicitly; on PC we need explicit includes)
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <strings.h> // bcopy, bzero

// M_PI may not be defined by math.h on all platforms
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Paper Mario's round() and atan2() have different signatures than C standard library.
// Redirect decomp names to renamed versions to avoid conflicts with math.h.
#define round pm_round
#define atan2 pm_atan2

#include "ultra64.h"

// Fix: The F3DEX_GBI version of __gsSP1Triangle_w1f uses ternary operators which
// Apple Clang rejects as non-constant in C static initializers. Replace with
// arithmetic that produces the same result using compile-time constant expressions.
#if (defined(F3DEX_GBI) || defined(F3DLP_GBI))
#undef __gsSP1Triangle_w1f
#define __gsSP1Triangle_w1f(v0, v1, v2, flag) \
    __gsSP1Triangle_w1( \
        (((flag) == 0) * (v0) + ((flag) == 1) * (v1) + ((flag) == 2) * (v2)), \
        (((flag) == 0) * (v1) + ((flag) == 1) * (v2) + ((flag) == 2) * (v0)), \
        (((flag) == 0) * (v2) + ((flag) == 1) * (v0) + ((flag) == 2) * (v1)))
#endif

#include "sprite/player.h"   // Correct player sprite IDs (overrides anim_stubs.h #ifndef guards)
#include "sprite/npc_all.h"  // Correct NPC sprite IDs (overrides anim_stubs.h #ifndef guards)
#include "anim_stubs.h"
#include "gbi_custom.h"
#include "macros.h"
#include "types.h"
#include "common_structs.h"
#include "functions.h"
#ifndef NO_EXTERN_VARIABLES
#include "variables.h"
#endif
#include "enums.h"
#include "sprite_shading_profiles.h"
#include "evt.h"
#include "messages.h"
#include "battle/battle_names.h"
#include "battle/stage_names.h"
#include "battle/actor_types.h"

#ifdef PERMUTER
extern int TEXEL0, TEXEL1, PRIMITIVE, PRIMITIVE_ALPHA;
#endif

#endif
