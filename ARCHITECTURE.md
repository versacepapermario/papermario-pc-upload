# PaperShip Architecture Guide

This document explains how PaperShip works for developers who want to contribute or understand the codebase.

## High-Level Overview

```
┌─────────────────────────────────────────────────────────┐
│                    Paper Mario ROM (.z64)                │
│              (user-provided, read at runtime)            │
└──────────────────────┬──────────────────────────────────┘
                       │ nuPiReadRom()
                       ▼
┌─────────────────────────────────────────────────────────┐
│                  Game Code (src/)                        │
│  Original decomp C code with #ifdef PORT adaptations    │
│                                                         │
│  ┌──────────┐ ┌──────────┐ ┌──────────┐ ┌───────────┐  │
│  │ World    │ │ Battle   │ │ Effects  │ │ Entities  │  │
│  │ Maps     │ │ System   │ │ (fx_*)   │ │ Blocks,   │  │
│  │ NPCs     │ │ Actions  │ │ Particles│ │ Items,    │  │
│  │ Scripts  │ │ Partners │ │ Glow     │ │ Switches  │  │
│  └──────────┘ └──────────┘ └──────────┘ └───────────┘  │
│                                                         │
│  ┌──────────┐ ┌──────────┐ ┌──────────┐ ┌───────────┐  │
│  │ Model    │ │ Sprite   │ │ Audio    │ │ EVT       │  │
│  │ Renderer │ │ System   │ │ Engine   │ │ Script    │  │
│  │ (model.c)│ │(sprite.c)│ │(engine.c)│ │ System    │  │
│  └────┬─────┘ └────┬─────┘ └────┬─────┘ └───────────┘  │
│       │            │            │                       │
└───────┼────────────┼────────────┼───────────────────────┘
        │            │            │
        │  N64 GBI   │            │  N64 Audio
        │  Commands  │            │  Commands
        ▼            ▼            ▼
┌─────────────────────────────────────────────────────────┐
│                   PORT Layer (port/)                     │
│                                                         │
│  ┌──────────────┐  ┌──────────────┐  ┌───────────────┐  │
│  │ Game.cpp     │  │ os_stubs.c   │  │ audio_mixer.c │  │
│  │ main(),      │  │ Flash save,  │  │ ADPCM decode, │  │
│  │ push_frame(),│  │ PI/SI stubs, │  │ RSP mixer,    │  │
│  │ 30fps limit  │  │ ROM reading  │  │ resampling    │  │
│  └──────┬───────┘  └──────────────┘  └───────┬───────┘  │
│         │                                     │         │
│  ┌──────┴───────┐  ┌──────────────┐           │         │
│  │ shape_       │  │ ui_texture_  │           │         │
│  │ swizzle.c    │  │ loader.c     │           │         │
│  │ N64→PC model │  │ ROM texture  │           │         │
│  │ conversion   │  │ extraction   │           │         │
│  └──────────────┘  └──────────────┘           │         │
│                                               │         │
└───────────────┬───────────────────────────────┼─────────┘
                │                               │
                ▼                               ▼
┌─────────────────────────────┐  ┌────────────────────────┐
│   libultraship              │  │   SDL2 Audio           │
│                             │  │   44100 Hz stereo      │
│  ┌────────────────────────┐ │  └────────────────────────┘
│  │  Fast3D Interpreter    │ │
│  │  Processes N64 GBI     │ │
│  │  display list commands │ │
│  └───────────┬────────────┘ │
│              │              │
│  ┌───────────▼────────────┐ │
│  │  Metal / OpenGL / DX11 │ │
│  │  GPU Backend           │ │
│  └───────────┬────────────┘ │
│              │              │
│  ┌───────────▼────────────┐ │
│  │  ImGui + SDL2 Window   │ │
│  │  Settings menu overlay │ │
│  └────────────────────────┘ │
└─────────────────────────────┘
```

## Frame Lifecycle

Each frame at 30fps follows this sequence:

```
push_frame()
  ├── Frame rate limiter (30fps target)
  ├── StartAudioFrame()
  ├── StartFrame()
  ├── gfxRetrace_Callback(0)          ← Paper Mario's main loop
  │   ├── Game logic (state_step)
  │   ├── gfx_task_background()       ← Background display list
  │   │   ├── gfx_init_state()
  │   │   └── gfx_draw_background()   ← Sky/background fill
  │   ├── gfx_draw_frame()            ← Main display list
  │   │   ├── render_frame(false)     ← World cameras (models, entities, NPCs)
  │   │   ├── render_frame(true)      ← HUD camera
  │   │   └── state_drawUI()          ← Screen overlays, effects
  │   └── nuGfxTaskStart()            ← Submit DLs to interpreter
  │       └── Combined BG+Main DL → ProcessGfxCommands()
  │           └── Fast3D Interpreter → GPU
  └── EndAudioFrame()
```

## Key Subsystems

### Display List (GBI) Pipeline

Paper Mario generates N64 GBI display list commands (gSPVertex, gDPLoadTexture, gSP2Triangles, etc.). These are written to a command buffer during the frame, then processed by the Fast3D interpreter in libultraship.

```
Game Code → gMainGfxPos++ (writes GBI commands) → Fast3D Interpreter → GPU
```

The interpreter translates N64 RDP/RSP commands to modern GPU calls. It handles:
- Vertex transformation and lighting
- Texture loading, caching, and sampling
- Color combiners (1-cycle and 2-cycle modes)
- Blending modes, fog, alpha testing
- Scissor and viewport clipping

### Texture System

Textures come from multiple sources:
- **Map textures**: Loaded from ROM shape data at map load time
- **Entity textures**: Pre-loaded from ROM at boot (`port/ui_texture_loader.c`)
- **Effect textures**: Loaded from ROM effect GFX data (`port/effect_gfx_swizzle.c`)
- **Sprite rasters**: Loaded from ROM sprite data on demand

All texture data is kept in N64 big-endian format for the Fast3D interpreter.

### Model System (`src/model.c`)

The model system renders 3D map geometry. Key concepts:
- **Model tree**: Hierarchical scene graph from map shape data
- **Render modes**: 1-cycle, 2-cycle, fog, shroud tinting
- **Texture panning**: Animated UV scrolling (water, lava)
- **Transform groups**: Animated model subsets
- **Bounds culling**: Corner-based frustum visibility test for small models (same as N64)

Models are converted from N64 format at load time by `port/shape_swizzle.c`:
- Vertices: 16-byte N64 → 24-byte PC (GBI_FLOATS)
- Display lists: N64 pointer addresses → PC pointer addresses
- Lighting data: Preserved as-is (single-byte fields)

### Entity System (`src/entity.c`, `src/entity_model.c`)

Entities are interactive objects (blocks, switches, NPCs). Each entity has:
- A blueprint (type, init function, collision handler)
- A model script (display list assignment)
- An optional setupGfx callback (custom rendering per-frame)
- DMA data loaded from ROM at creation time

**PORT note**: Entity CI4 textures don't render correctly through the setupGfx path. Affected entities pre-convert CI4+palette to RGBA16 at init time (see SuperBlock.c for the pattern).

### Effect System (`src/effects/`)

Visual effects (particles, glows, flames, etc.) are managed by `src/effects.c`. Each effect has:
- `init`, `update`, `render`, `appendGfx` callbacks
- Shared graphics data loaded from ROM (seg9 textures/DLs)
- Per-instance data allocated from the general heap

**PORT notes on specific effects**:
- `motion_blur_flame`, `bulb_glow`: Use I8 radial gradient textures with additive blending (N64 reads framebuffer)
- `flame`: Disabled — needs chroma key emulation (N64 uses render-to-texture + gDPSetKeyR/GB)
- `water_splash`, `underwater`: Skipped (framebuffer refraction/distortion)
- `chomp_drop`, `squirt`, `waterfall`: Fixed inline vertex buffer sizes for GBI_FLOATS

### Audio System

Paper Mario's audio engine runs on a separate thread:

```
Audio Thread (HandleAudioThread)
  └── port/audio_mixer.c
      ├── ADPCM decompression
      ├── Resampling
      ├── EnvMixer (volume, pan)
      ├── Pole filter
      └── Interleave → SDL2 audio callback
```

Audio data (instrument banks, sequences) is read from ROM via `nuPiReadRom()`.

### EVT Script System

Paper Mario uses a custom bytecode scripting language (EVT) for game logic:
- Map initialization, NPC behavior, cutscenes
- Runs on a cooperative scheduler (`src/evt/`)
- Variables: local (LVar), map (MVar), global flags (GF/GByte)

**PORT note**: The EVT system uses `intptr_t` for variable storage on 64-bit. Array access via `script->array[N]` must use `port_array_get/set` helpers due to stride mismatch (intptr_t stride vs s32 data).

## PORT Adaptation Patterns

### `#ifdef PORT` Blocks

All PC-specific code is wrapped in `#ifdef PORT` / `#endif`. Common patterns:

```c
#ifdef PORT
    // PC-specific implementation
#else
    // Original N64 code
#endif
```

### GBI_FLOATS

With `GBI_FLOATS=1`, the `Vtx` struct uses `float` positions instead of `s16`:
- N64 Vtx: 16 bytes (s16 ob[3], u16 flag, s16 tc[2], u8 cn[4])
- PC Vtx: 24 bytes (float ob[3], u16 flag, s16 tc[2], u8 cn[4])

**Critical**: Any code that allocates space for vertices in the display list buffer must account for this size difference. See `chomp_drop.c` for the pattern.

**Critical**: Stack-allocated `Vtx` arrays must be `static` — the display list stores a pointer that's read later by the interpreter, after the stack frame is gone.

### Endianness

The Fast3D interpreter expects all texture and palette data in N64 big-endian format. Only byte-swap C struct fields that game code reads directly. Never byte-swap texture raster or palette data.

### N64 Mtx Format

With `GBI_FLOATS=1`, `Mtx` = `MtxF` (4x4 floats). Any static matrix data from ROM is in N64 fixed-point format and must be converted to floats.

## Build System

```
CMakeLists.txt
├── PaperShip target (the game executable)
├── libultraship (git submodule, built as library)
├── Torch (asset pipeline, git submodule)
├── ImGui, spdlog, SDL2 (dependencies)
└── Platform detection (macOS/Windows/Linux)
```

Key CMake variables:
- `GBI_FLOATS=1` — must be global (applies to game + libultraship)
- `F3DEX_GBI_2=1` — must NOT be global (breaks Torch/libgfxd)
- Build target: `PaperShip` (capital S)
- Shader archive: `GeneratePortO2R` target must be built after PaperShip (packs `assets/port/shaders/` into `papership.o2r`)

## ROM Data Flow

```
ROM File (.z64)
  │
  ├── Map shape data ──→ shape_swizzle.c ──→ Model tree + vertices + DLs
  ├── Map textures ────→ load_data_for_models() ──→ Texture cache
  ├── Entity data ─────→ port_load_entity_model_data() ──→ Entity stubs
  ├── Effect GFX ──────→ effect_gfx_swizzle.c ──→ Effect textures + DLs
  ├── Sprite data ─────→ spr_load_sprite() ──→ Sprite rasters + palettes
  ├── Audio banks ─────→ au_load_BK_to_bank() ──→ Instrument samples
  ├── Audio sequences ─→ bgm/mseq players ──→ Music + sound effects
  └── Messages ────────→ msg.c ──→ Dialog text
```

All ROM reads go through `nuPiReadRom()` in `port/NuSystemShims.cpp`, which reads from the .z64 file on disk.

## Common Development Tasks

### Adding a new effect PORT workaround

1. Check if the effect reads the framebuffer (`nuGfxCfb_ptr`) — if so, it needs a PORT path
2. Add `#ifdef PORT` / `#else` / `#endif` around the problematic code
3. If using inline vertices, make them `static` and account for GBI_FLOATS size
4. If the effect uses `RENDER_TASK_FLAG_REFLECT_FLOOR`, wrap it in `#ifndef PORT`
5. Add render state cleanup at the end if using unusual blend modes

### Fixing a texture rendering issue

1. Check if it's a CI4 texture — CI4 through entity setupGfx doesn't work; pre-convert to RGBA16
2. Check if it uses TEXEL1 (2-cycle blend) — fall back to TEXEL0-only on PORT
3. Check endianness — texture data must stay big-endian
4. Check the texture cache — dynamic textures need content hashing (I/IA formats)

### Debugging invisible geometry

1. Verify the Vtx array is `static` (not stack-allocated)
2. Check if the matrix uses G_MTX_MUL — the modelview stack may need the right base
3. Test with a screen-space `gDPFillRectangle` to verify the appendGfx is called
4. Check the render mode — Z-buffer rejection might hide the geometry
5. Check the scissor — it might clip to a smaller viewport area
