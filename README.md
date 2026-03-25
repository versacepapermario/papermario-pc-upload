# PaperShip

A decomp-based PC port of Paper Mario 64 using [libultraship]([https://github.com/HarbourMasters/libultraship](https://github.com/Kenix3/libultraship)).

Built from the [Paper Mario decompilation](https://github.com/pmret/papermario) with a custom PORT layer that bridges N64 hardware calls to modern PC graphics (Metal/OpenGL/DirectX), audio, and input via libultraship.

## Legal Notice

This project is a **clean-room PC port** built entirely from the publicly available [Paper Mario decompilation](https://github.com/pmret/papermario). It follows the same legal approach as [Ship of Harkinian](https://github.com/HarbourMasters/Shipwright) (Zelda OoT PC port) and [Ghostship](https://github.com/HarbourMasters/Ghostship) (SM64 PC port).

- **This repository contains NO copyrighted Nintendo assets** — no ROM data, no textures, no audio, no models
- All game assets are read at runtime from a user-provided ROM file
- We do not condone piracy. You must provide your own legally obtained copy of the game.

## Features

- Native PC rendering via Metal (macOS), OpenGL, or DirectX 11 (Windows)
- Full audio (BGM, sound effects, MSEQ sequences)
- Controller and keyboard support with remappable controls
- Settings menu (Escape key) with resolution scaling, MSAA, VSync, volume controls
- Save system (file-backed flash emulation)
- 4:3 aspect ratio with borderless fullscreen

## Quick Start

### 1. Get the source code

```bash
git clone --recursive <repo-url>
cd papermario-recomp
```

If you already cloned without `--recursive`:
```bash
git submodule update --init --recursive
```

### 2. Provide your ROM

You need a **US** Paper Mario ROM in `.z64` format. Place it in the project root or build directory.

Accepted filenames: `Paper Mario (USA).z64`, `baserom.us.z64`, `pm64.z64`, `papermario.z64`

| Version | SHA-1 Hash |
|---------|-----------|
| US | `3837f44cda784b466c9a2d99df70d77c322b97a0` |

You can verify your ROM hash at https://www.romhacking.net/hash/

### 3. Build and run

#### macOS (primary tested platform)

```bash
brew install cmake sdl2 glew
mkdir build && cd build
cmake ..
cmake --build . --target PaperShip -j$(sysctl -n hw.ncpu)
cmake --build . --target GeneratePortO2R
./PaperShip
```

#### Linux

```bash
sudo apt install cmake build-essential libsdl2-dev libpng-dev libglew-dev
mkdir build && cd build
cmake ..
cmake --build . --target PaperShip -j$(nproc)
cmake --build . --target GeneratePortO2R
./PaperShip
```

#### Windows (Visual Studio)

```bash
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --target PaperShip --config Release
cmake --build . --target GeneratePortO2R --config Release
```

#### Windows (MSYS2/MinGW)

```bash
pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-gcc mingw-w64-x86_64-SDL2 mingw-w64-x86_64-glew
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
cmake --build . --target PaperShip -j$(nproc)
cmake --build . --target GeneratePortO2R
```

The game automatically searches for the ROM in the executable's directory and parent directories. No extraction step needed.

## Platform Support

| Platform | Status | Notes |
|----------|--------|-------|
| macOS (Apple Silicon) | Fully tested | Primary development platform |
| macOS (Intel) | Should work | Not actively tested |
| Windows | Builds supported | Community contributions welcome for platform-specific fixes |
| Linux | Builds supported | Community contributions welcome for platform-specific fixes |

## Controls

### Default Keyboard

| N64 | A | B | Z | Start | Analog Stick | C Buttons | D-Pad |
|-----|---|---|---|-------|-------------|-----------|-------|
| Keyboard | X | C | Z | Space | WASD | Arrow Keys | TFGH |

### Other Shortcuts

| Key | Action |
|-----|--------|
| Escape | Toggle settings menu |
| F11 | Toggle fullscreen |

Controllers are automatically detected. Button mapping can be configured in the settings menu.

## Graphics Backends

| Platform | Default Backend | Alternatives |
|----------|----------------|-------------|
| macOS | Metal | OpenGL |
| Windows | DirectX 11 | OpenGL |
| Linux | OpenGL | — |

## Current Status

This port has been **playtested through Chapter 2** (through the Tutankoopa boss fight). The game is largely playable from start through that point, but there are still visual artifacts and bugs. Chapters beyond Chapter 2 have not been tested and may have additional issues.

## Known Issues

**Gameplay-blocking**: None known through Chapter 2.

**Visual artifacts**:
- Flames in Tutankoopa's tomb are not rendered (N64 uses chroma key render-to-texture which is not yet supported)
- Visual artifacting around text boxes after chapter completion and when acquiring the upgraded hammer in Tutankoopa's tomb
- Minor visual artifacting when Tutankoopa uses the giant chain chomp ability
- Image displayed when a new partner is acquired is not quite right
- Toad Town dock area has water geometry clipping issues
- Some dual-texture blend modes fall back to single-texture (TEXEL1 sampling limitation)

**Unimplemented effects**:
- Water splash refraction effect
- Underwater distortion warp effect
- Pause screen shows a dark fill instead of freeze-frame (no GPU framebuffer readback)
- Crystal ball reflection in Merlon's house

See `PORT_SKIPS.md` for a complete list of stubbed features.

## Architecture

```
papermario-recomp/
├── src/              # Decomp game source (C) with #ifdef PORT adaptations
├── include/          # Game headers
├── port/             # PORT layer: OS stubs, ROM loading, texture conversion,
│                     #   UI menu, shape swizzling, audio bridge
├── libultraship/     # Rendering engine (submodule)
├── assets/           # Asset YAML definitions (non-copyrighted metadata)
└── build/            # Build output
```

The game reads all assets at runtime from the provided ROM file. No copyrighted data is stored in this repository.

## Contributing

See [ARCHITECTURE.md](ARCHITECTURE.md) for a detailed guide to the codebase, frame lifecycle, subsystem explanations, and common development patterns.

Key areas that need work:

- **Flame effect**: Needs proper chroma key emulation or shader-based approach
- **TEXEL1 sampling**: Fix the Fast3D interpreter to properly sample tile 1 in 2-cycle mode
- **Framebuffer effects**: water_splash, underwater, crystal_ball, pause freeze-frame
- **Frame interpolation**: 60fps support via matrix interpolation
- **Windows/Linux testing**: Platform-specific bug fixes and testing

## License

This project does not include any copyrighted Nintendo assets. You must provide your own legally obtained ROM to use this software.

This software is provided as-is for educational and interoperability purposes.

## Credits

- **versacepapermario** — Project lead
- [Paper Mario Decompilation Team](https://github.com/pmret/papermario) — Complete US/JP/PAL/iQue decomp
- [libultraship / Ship of Harkinian Team](https://github.com/HarbourMasters) — PC rendering engine
- [Ghostship (SM64 PC Port)](https://github.com/HarbourMasters/Ghostship) — Reference implementation
