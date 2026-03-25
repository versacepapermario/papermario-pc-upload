# PORT Skips & Stubs Tracker

All `#ifdef PORT` blocks that skip, stub, or bypass original game functionality.
These should be removed/replaced when proper implementations are done.

## Startup/Menu Skips

| File | Lines | What's Skipped | Notes |
|------|-------|----------------|-------|
| `src/state_file_select.c` | ~302-306 | ROM overlay load skipped (file menu code statically linked) | CORRECT for PC — no need to restore |
| ~~`src/state_startup.c`~~ | ~~startup~~ | ~~LOGOS state skipped~~ | **DONE** — Logos now display on startup (N64, Nintendo, Intelligent Systems) |
| ~~`src/state_title_screen.c`~~ | ~~removed~~ | ~~Test auto-advance (PORT_AUTO_WALK/PORT_AUTO_ADVANCE)~~ | **REMOVED** — test automation cleaned up |
| ~~`src/state_file_select.c`~~ | ~~removed~~ | ~~Test auto-start (PORT_AUTO_WALK/PORT_AUTO_ADVANCE)~~ | **REMOVED** — test automation cleaned up |

## Major Stubs (Unimplemented Subsystems)

| File | Lines | What's Stubbed | Notes |
|------|-------|----------------|-------|
| ~~`port/Engine.cpp`~~ | ~~~380~~ | ~~Audio: `HandleAudioThread()` fills buffer with 0 (silence)~~ | **DONE** (Session 31) — Full software audio mixer |
| ~~`port/NuSystemShims.cpp`~~ | ~~482-493~~ | ~~Saves: `nuEepromRead()` returns 0, `nuEepromWrite()` discards~~ | **N/A** — PM64 uses Flash not EEPROM. Flash save implemented in `port/os_stubs.c` (file-backed, `papership_save.bin`) |
| `port/Engine.cpp` | ~52-56 | Frame interpolation: returns empty map | Need matrix interpolation impl |
| ~~`src/msg.c`~~ | ~~843-870~~ | ~~Postcard/letter image loading: memset to 0~~ | **DONE** (Session 36) — All charset ROM offsets mapped, nuPiReadRom loads from ROM |

## Functional Skips (Due to 32/64-bit Issues)

| File | Lines | What's Skipped | Notes |
|------|-------|----------------|-------|
| `src/entity.c` | ~660 | `reload_world_entity_data()`: skips animated entity reload | PORT uses persistent malloc'd memory, so reload is unnecessary. May be CORRECT. |

## Safety Guards (Not in Original)

| File | Lines | What's Added | Notes |
|------|-------|--------------|-------|
| `src/entity.c` | ~169-184 | Infinite loop guard on `step_entity_commandlist()` (10000 iterations) | Debug safety; could be removed once scripts are stable |
| `src/101b90_len_8f0.c` | ~95-150 | YAY0 magic validation + decompressed size bounds check | Safety; prevents OOM. Good to keep. |
| `src/43F0.c` | ~290-310 | DMA copy: skips if ROM offset resolution fails | Safety; prevents invalid reads. Good to keep. |
| `src/audio/sfx_control.c` | ~120 | Environment sound pointer validation | Safety; prevents corrupt pointer access |
| `src/audio/core/engine.c` | ~960 | Audio file offset bounds check | Safety; prevents OOB ROM reads |

## Framebuffer Effects (GPU VRAM Not CPU-Accessible)

These effects read `nuGfxCfb_ptr` as a texture, which is impossible on PC where the framebuffer is in GPU VRAM:

| File | Effect | Status | Notes |
|------|--------|--------|-------|
| ~~`src/effects/bulb_glow.c`~~ | Glow bloom around light bulbs | **APPROXIMATED** — I8 radial gradient texture with additive blending + render state cleanup |
| ~~`src/effects/motion_blur_flame.c`~~ | Motion trail on flame effects | **APPROXIMATED** — I8 radial gradient texture with additive blending (circular glow orbs) |
| `src/world/area_mac/mac_01/crystal_ball_gfx.c` | Reflection inside Merlon's crystal ball | Stubbed — specific to one NPC prop |
| `src/screen_render_util.c` | Prev-frame blur for darkness stencil | Partially working — has fallback using blurry circle texture |
| `src/draw_box.c` | ROTSCALE box path with fpDrawContents | Stubbed — almost never triggered |
| `src/background_gfx.c` | Pause screen freeze-frame | Stubbed — shows dark fill instead of frozen scene |

## Endianness Conversions (Required, Not Skips)

These are NOT skips — they're necessary for big-endian ROM data on little-endian PC:
- `src/collision.c` — Collision header/vertex/collider byte-swaps
- `src/background.c` — Background header big-endian reads
- `src/state_title_screen.c` — Title image offset conversion
- `src/audio/core/engine.c` — Audio struct u16 pair swaps
- `src/audio/load_banks.c` — Bank entry byte reconstruction
- `src/101b90_len_8f0.c` — Sprite asset entry byte-swaps
- `src/model.c` — TextureHeader byte-swaps
- `src/msg.c` — Message ROM offset byte-swaps

## 64-bit Pointer Widening (Required, Not Skips)

These are NOT skips — they fix 32→64 bit pointer truncation:
- `src/101b90_len_8f0.c` — Sprite struct pointer widening
- `src/entity.c` — Entity animated model 64-bit conversion
- `port/shape_swizzle.c` — Shape data N64→PC pointer rebasing
- Various files — `(s32)ptr` → `(intptr_t)ptr` casts

## ROM Texture Loading (port/ui_texture_loader.c)

All boot-time textures loaded from ROM via 8 loader functions:
1. UI box textures — 14 entries (window borders, file menu DLs)
2. Menu HUD CI — 0x131340 (15,872 bytes)
3. msg_img — 0x10CEB0 (8,960 bytes, 53 entries)
4. global_hud_scripts — 0x919B0 (50,352 bytes, 376 entries)
5. battle_ui_gfx — 0x1B7CE0 (35,712 bytes, 147 entries)
6. stencils — 0xE2E00 (7,680 bytes, 4 entries)
7. pause_gfx — 0x1439F0 (123,552 bytes, 29 entries)
8. theater/curtains — 0x4F9E0 (11,104 bytes, 5 entries)

All 353 overlay-specific stubs resized to correct sizes (Session 36). These are loaded at runtime by the overlay system via `dma_copy()` → ROM offset resolution:
- `D_0A*` (133) — Map overlay textures (ci4/ci8/i4/i8/ia8/rgba16/rgba32 + palettes)
- `level_up_*` (88) — Level-up screen textures and palettes
- `battle_action_cmd_*` (32) — Whirlwind + three_chances action command textures
- `battle_item_*` (22) — Item projectile textures (coconut, egg missiles, sleepy sheep, etc.)
- `toad_house_blanket_vtx` (12) — Shared vertex data across 12 maps (5600 bytes each)
- `mgm_*` (10) — Minigame panel textures
- `title_*` (8) — Title screen text (IA8, auto-skipped in test mode)
- `kmr_*` (7) — Goomba Village map-specific textures
- `battle_item_strange_cake_*` (6) — Strange cake item textures
- `ispy_*` (4), `sam_*` (4), `battle_move_hammer_throw_*` (4) — Various overlay textures
- `D_PAL_09*` (3) — Chapter change text images (4160 bytes each)
- `speech_bubble_*` (2), `inspect_icon_*` (2), `pulse_stone_icon_*` (3) — World overlay textures
- `monstar_bubbles` (1) — 66 Vtx entries (1056 bytes)
- `ResetTilesImg` (1) — i4 128x8 tile reset image
- `n_aspMain*` (2) — Audio RSP microcode text/data

34 `charset_*_OFFSET` stubs remain at `[4]` — these are u32 ROM offsets, 4 bytes is correct.

---

Last updated: 2026-03-05
