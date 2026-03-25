#!/usr/bin/env python3
"""
gen_effect_gfx_data.py - Generate effect graphics data for PC port

Reads the Paper Mario decomp's splat.yaml to extract ROM offsets and symbol
layouts for all effect_gfx segments, then generates:
  1. Corrected rom_offsets entries (effect_gfx_*_ROM_START/END)
  2. Sized .gfx.inc.c and .vtx.inc.c stubs (large enough to hold real data)
  3. Texture array definitions for effect IMG symbols
  4. A symbol table header for the runtime swizzle function

Usage:
  python3 tools/gen_effect_gfx_data.py
"""

import yaml
import os
import sys
import json

DECOMP_DIR = os.path.join(os.path.dirname(__file__), '..', '..', 'papermario-main')
SPLAT_YAML = os.path.join(DECOMP_DIR, 'ver', 'us', 'splat.yaml')
RECOMP_DIR = os.path.join(os.path.dirname(__file__), '..')
EFFECTS_GFX_INC_DIR = os.path.join(RECOMP_DIR, 'include', 'effects', 'gfx')
EFFECTS_GFX_SRC_DIR = os.path.join(RECOMP_DIR, 'src', 'effects', 'gfx')
PORT_DIR = os.path.join(RECOMP_DIR, 'port')

# N64 struct sizes
N64_GFX_CMD_SIZE = 8     # each N64 GBI command is 8 bytes
N64_VTX_SIZE = 16        # each N64 vertex is 16 bytes

def parse_splat_yaml():
    """Parse splat.yaml and extract all effect_gfx segment info."""
    with open(SPLAT_YAML, 'r') as f:
        data = yaml.safe_load(f)

    segments = data.get('segments', [])

    # Build sorted list of all segment start addresses for ROM end detection
    seg_starts = []
    for seg in segments:
        if isinstance(seg, dict) and 'start' in seg:
            seg_starts.append(seg['start'])
        elif isinstance(seg, list) and len(seg) >= 1 and isinstance(seg[0], int):
            seg_starts.append(seg[0])
    seg_starts.sort()

    effect_gfx_segments = {}

    for seg in segments:
        if not isinstance(seg, dict):
            continue
        name = seg.get('name', '')
        if not name.startswith('effect_gfx_'):
            continue

        start = seg['start']

        # Find ROM end from next segment
        idx = seg_starts.index(start)
        rom_end = seg_starts[idx + 1] if idx + 1 < len(seg_starts) else start

        # Parse sub-segments for symbol layout
        symbols = []
        max_end_marker = 0
        subs = seg.get('subsegments', [])
        for sub in subs:
            if isinstance(sub, dict) and sub.get('type') == '.data':
                inner = sub.get('subsegments', [])
                for item in inner:
                    if isinstance(item, list):
                        if len(item) == 1:
                            # End marker - track the largest one
                            if item[0] > max_end_marker:
                                max_end_marker = item[0]
                        elif len(item) >= 3:
                            rom_addr = item[0]
                            sym_type = item[1]
                            splat_name = item[2]
                            width = item[3] if len(item) > 3 else None
                            height = item[4] if len(item) > 4 else None
                            # Physical offset within the ROM blob (used for reading data)
                            rom_blob_offset = rom_addr - start
                            # Use splat name if it's a valid C identifier,
                            # otherwise compute one from rom_blob_offset and ROM addr
                            if isinstance(splat_name, str) and splat_name.startswith('D_09') and '/' not in splat_name and '.' not in splat_name:
                                c_name = splat_name
                            else:
                                c_name = f"D_09{rom_blob_offset:06X}_{rom_addr:06X}"
                            # seg9_offset = physical ROM offset (what DL commands reference)
                            seg9_offset = rom_blob_offset
                            symbols.append({
                                'rom': rom_addr,
                                'type': sym_type,
                                'name': c_name,
                                'splat_name': splat_name,
                                'width': width,
                                'height': height,
                                'seg9_offset': seg9_offset,
                            })

        # Use the largest end marker if it exceeds the next-segment boundary
        if max_end_marker > rom_end:
            rom_end = max_end_marker

        # Compute sizes for each symbol (ensure non-negative)
        for i, sym in enumerate(symbols):
            if i + 1 < len(symbols):
                sym['size'] = max(0, symbols[i + 1]['rom'] - sym['rom'])
            else:
                sym['size'] = max(0, rom_end - sym['rom'])

        effect_gfx_segments[name] = {
            'rom_start': start,
            'rom_end': rom_end,
            'blob_size': rom_end - start,
            'symbols': symbols,
        }

    return effect_gfx_segments


def gen_rom_offsets_patch(segments):
    """Generate corrected rom_offsets.c entries."""
    lines = []
    lines.append("// === Effect GFX ROM offsets (auto-generated by gen_effect_gfx_data.py) ===")
    for name in sorted(segments.keys()):
        info = segments[name]
        lines.append(f"    {{ {name}_ROM_START, 0x{info['rom_start']:X} }},")
        lines.append(f"    {{ {name}_ROM_END, 0x{info['rom_end']:X} }},")
    return '\n'.join(lines)


def gen_sized_gfx_stubs(segments):
    """Generate correctly-sized .gfx.inc.c stub files."""
    count = 0
    for name, info in segments.items():
        for sym in info['symbols']:
            if sym['type'] != 'gfx':
                continue
            sym_name = sym['name']
            size_bytes = sym['size']
            n_cmds = size_bytes // N64_GFX_CMD_SIZE
            if n_cmds < 1:
                n_cmds = 1

            filepath = os.path.join(EFFECTS_GFX_INC_DIR, f"{sym_name}.gfx.inc.c")
            content = (
                f"// Effect display list stub - sized for {n_cmds} commands ({size_bytes} ROM bytes)\n"
                f"// Auto-generated by gen_effect_gfx_data.py\n"
                f"Gfx {sym_name}[{n_cmds}] = {{\n"
                f"    gsSPEndDisplayList(),\n"
                f"}};\n"
            )
            with open(filepath, 'w') as f:
                f.write(content)
            count += 1
    return count


def gen_sized_vtx_stubs(segments):
    """Generate correctly-sized .vtx.inc.c stub files."""
    count = 0
    for name, info in segments.items():
        for sym in info['symbols']:
            if sym['type'] != 'vtx':
                continue
            sym_name = sym['name']
            size_bytes = sym['size']
            n_verts = size_bytes // N64_VTX_SIZE
            if n_verts < 1:
                n_verts = 1

            filepath = os.path.join(EFFECTS_GFX_INC_DIR, f"{sym_name}.vtx.inc.c")
            content = (
                f"// Effect vertex stub - sized for {n_verts} vertices ({size_bytes} ROM bytes)\n"
                f"// Auto-generated by gen_effect_gfx_data.py\n"
                f"Vtx {sym_name}[{n_verts}] = {{\n"
                f"    {{{{ 0, 0, 0 }}, 0, {{ 0, 0 }}, {{ 0, 0, 0, 0xFF }}}},\n"
                f"}};\n"
            )
            with open(filepath, 'w') as f:
                f.write(content)
            count += 1
    return count


def gen_effect_gfx_tables_header(segments):
    """Generate port/effect_gfx_tables.h with per-segment symbol tables."""
    lines = []
    lines.append("/**")
    lines.append(" * effect_gfx_tables.h - Symbol tables for effect graphics swizzle")
    lines.append(" * Auto-generated by gen_effect_gfx_data.py")
    lines.append(" */")
    lines.append("#ifndef EFFECT_GFX_TABLES_H")
    lines.append("#define EFFECT_GFX_TABLES_H")
    lines.append("")
    lines.append("#include \"common.h\"")
    lines.append("")
    lines.append("typedef enum {")
    lines.append("    EFX_SYM_IMG,")
    lines.append("    EFX_SYM_VTX,")
    lines.append("    EFX_SYM_GFX,")
    lines.append("    EFX_SYM_PAL,")
    lines.append("} EffectGfxSymType;")
    lines.append("")
    lines.append("typedef struct {")
    lines.append("    u32 seg9Offset;    // Offset within the graphics blob")
    lines.append("    void* pcArray;     // Pointer to the C array to fill")
    lines.append("    u32 sizeBytes;     // Size in ROM (N64 format)")
    lines.append("    u8 type;           // EffectGfxSymType")
    lines.append("} EffectGfxSymbol;")
    lines.append("")
    lines.append("typedef struct {")
    lines.append("    u32 romStart;")
    lines.append("    u32 romEnd;")
    lines.append("    const EffectGfxSymbol* symbols;")
    lines.append("    s32 numSymbols;")
    lines.append("} EffectGfxSegmentInfo;")
    lines.append("")

    # Map splat types to our enum
    type_map = {
        'gfx': 'EFX_SYM_GFX',
        'vtx': 'EFX_SYM_VTX',
        'palette': 'EFX_SYM_PAL',
    }
    # All image types map to IMG
    img_types = {'i4', 'i8', 'ia4', 'ia8', 'ia16', 'rgba16', 'rgba32', 'ci4', 'ci8'}

    # Forward declarations for all symbol arrays
    lines.append("// Forward declarations for effect symbol arrays")
    for name, info in sorted(segments.items()):
        for sym in info['symbols']:
            sym_type = sym['type']
            sym_name = sym['name']
            if sym_type == 'gfx':
                lines.append(f"extern Gfx {sym_name}[];")
            elif sym_type == 'vtx':
                lines.append(f"extern Vtx {sym_name}[];")
            elif sym_type in img_types or sym_type == 'palette':
                lines.append(f"extern u8 {sym_name}[];")
    lines.append("")

    # Per-segment symbol tables
    for name, info in sorted(segments.items()):
        c_name = name  # e.g., effect_gfx_explosion
        n_syms = len(info['symbols'])

        lines.append(f"static const EffectGfxSymbol {c_name}_symbols[] = {{")
        for sym in info['symbols']:
            sym_type = sym['type']
            sym_name = sym['name']
            offset = sym['seg9_offset']
            size = sym['size']

            if sym_type == 'gfx':
                type_enum = 'EFX_SYM_GFX'
            elif sym_type == 'vtx':
                type_enum = 'EFX_SYM_VTX'
            elif sym_type == 'palette':
                type_enum = 'EFX_SYM_PAL'
            elif sym_type in img_types:
                type_enum = 'EFX_SYM_IMG'
            else:
                type_enum = 'EFX_SYM_IMG'  # fallback

            lines.append(f"    {{ 0x{offset:04X}, (void*){sym_name}, 0x{size:04X}, {type_enum} }},")
        lines.append("};")
        lines.append("")

    # Master lookup table: maps ROM_START address to segment info
    lines.append("// Master table: indexed by searching for ROM_START match")
    lines.append(f"#define NUM_EFFECT_GFX_SEGMENTS {len(segments)}")
    lines.append("static const EffectGfxSegmentInfo sEffectGfxSegments[] = {")
    for name, info in sorted(segments.items()):
        n = len(info['symbols'])
        lines.append(f"    {{ 0x{info['rom_start']:X}, 0x{info['rom_end']:X}, "
                     f"{name}_symbols, {n} }}, // {name}")
    lines.append("};")
    lines.append("")

    lines.append("#endif // EFFECT_GFX_TABLES_H")
    return '\n'.join(lines)


def gen_texture_array_defs(segments):
    """Generate texture array definitions for effect IMG/PAL symbols.
    These are needed because INCLUDE_IMG only creates extern declarations."""
    img_types = {'i4', 'i8', 'ia4', 'ia8', 'ia16', 'rgba16', 'rgba32', 'ci4', 'ci8', 'palette'}

    # Collect all unique texture symbols with their sizes
    tex_symbols = {}
    for name, info in segments.items():
        for sym in info['symbols']:
            if sym['type'] in img_types:
                sym_name = sym['name']
                sym_size = sym['size']
                if sym_name not in tex_symbols:
                    tex_symbols[sym_name] = sym_size
                else:
                    # Take the larger size if duplicated
                    tex_symbols[sym_name] = max(tex_symbols[sym_name], sym_size)

    lines = []
    lines.append("/**")
    lines.append(" * effect_gfx_textures.c - Texture array definitions for effect graphics")
    lines.append(" * Auto-generated by gen_effect_gfx_data.py")
    lines.append(" *")
    lines.append(" * These provide storage for effect texture data loaded from ROM at runtime.")
    lines.append(" * On N64, textures are part of the DMA'd graphics blob. On PC, we need")
    lines.append(" * explicit arrays so DL commands can reference them by pointer.")
    lines.append(" */")
    lines.append("#ifdef PORT")
    lines.append("")
    lines.append("typedef unsigned char u8;")
    lines.append("")

    for sym_name in sorted(tex_symbols.keys()):
        size = tex_symbols[sym_name]
        lines.append(f"u8 {sym_name}[{size}]; // {size} bytes")

    lines.append("")
    lines.append("#endif // PORT")

    return '\n'.join(lines), len(tex_symbols)


def main():
    print("Parsing splat.yaml...")
    segments = parse_splat_yaml()
    print(f"Found {len(segments)} effect_gfx segments")

    # Count symbols
    total_gfx = sum(1 for s in segments.values() for sym in s['symbols'] if sym['type'] == 'gfx')
    total_vtx = sum(1 for s in segments.values() for sym in s['symbols'] if sym['type'] == 'vtx')
    total_img = sum(1 for s in segments.values()
                    for sym in s['symbols']
                    if sym['type'] not in ('gfx', 'vtx'))
    print(f"  GFX symbols: {total_gfx}")
    print(f"  VTX symbols: {total_vtx}")
    print(f"  IMG/PAL symbols: {total_img}")

    # 1. Generate rom_offsets patch
    print("\n--- ROM offsets patch ---")
    rom_patch = gen_rom_offsets_patch(segments)
    patch_path = os.path.join(PORT_DIR, 'effect_gfx_rom_offsets.txt')
    with open(patch_path, 'w') as f:
        f.write(rom_patch)
    print(f"Written to {patch_path}")

    # 2. Generate sized GFX stubs
    print("\nGenerating sized .gfx.inc.c stubs...")
    gfx_count = gen_sized_gfx_stubs(segments)
    print(f"  Generated {gfx_count} .gfx.inc.c files")

    # 3. Generate sized VTX stubs
    print("Generating sized .vtx.inc.c stubs...")
    vtx_count = gen_sized_vtx_stubs(segments)
    print(f"  Generated {vtx_count} .vtx.inc.c files")

    # 4. Generate symbol tables header
    print("Generating effect_gfx_tables.h...")
    tables_header = gen_effect_gfx_tables_header(segments)
    tables_path = os.path.join(PORT_DIR, 'effect_gfx_tables.h')
    with open(tables_path, 'w') as f:
        f.write(tables_header)
    print(f"  Written to {tables_path}")

    # 5. Generate texture array definitions
    print("Generating effect_gfx_textures.c...")
    tex_content, tex_count = gen_texture_array_defs(segments)
    tex_path = os.path.join(PORT_DIR, 'effect_gfx_textures.c')
    with open(tex_path, 'w') as f:
        f.write(tex_content)
    print(f"  Written to {tex_path} ({tex_count} texture arrays)")

    print("\nDone! Next steps:")
    print("  1. Apply ROM offset corrections to port/rom_offsets.c")
    print("  2. Add port/effect_gfx_textures.c to CMakeLists.txt")
    print("  3. Write port/effect_gfx_swizzle.c using the symbol tables")
    print("  4. Modify src/effects.c to load and swizzle effect graphics")


if __name__ == '__main__':
    main()
