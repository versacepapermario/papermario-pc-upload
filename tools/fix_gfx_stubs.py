#!/usr/bin/env python3
"""
Fix auto-generated .gfx.inc.c stub files that have incorrect symbol names.

For each stub in include/ (excluding entity/ and level_up/):
1. Read the stub to get current symbol name(s)
2. Find source file(s) that #include it
3. Search the broader source directory for the expected symbol name
4. Fix any mismatches

The expected symbol follows one of these patterns:
- For named files like "foo.gfx.inc.c": symbol is "foo_gfx" or "N(foo_gfx)" or "N(prefix_foo_gfx)"
- For hex-named files like "D_09XXXX.gfx.inc.c": symbol matches the filename stem
- For hex-address files like "A2BC0.gfx.inc.c": symbol is D_VRAMADDR computed from splat config
"""

import os
import re
import sys
from pathlib import Path
from collections import defaultdict

PROJECT_ROOT = Path(__file__).resolve().parent.parent
INCLUDE_DIR = PROJECT_ROOT / "include"
SRC_DIR = PROJECT_ROOT / "src"

# Directories to skip (already fixed)
SKIP_DIRS = {"entity", "level_up"}

def find_gfx_stubs():
    """Find all .gfx.inc.c files in include/, excluding entity/ and level_up/."""
    stubs = []
    for root, dirs, files in os.walk(INCLUDE_DIR):
        rel = Path(root).relative_to(INCLUDE_DIR)
        parts = rel.parts
        if any(p in SKIP_DIRS for p in parts):
            continue
        for f in files:
            if f.endswith(".gfx.inc.c"):
                stubs.append(Path(root) / f)
    return sorted(stubs)

def get_stub_symbols(stub_path):
    """Extract symbol names from a stub file."""
    content = stub_path.read_text()
    symbols = []
    for m in re.finditer(r'^Gfx\s+((?:N\([^)]+\)|\S+?))\s*\[\s*\]\s*=\s*\{', content, re.MULTILINE):
        symbols.append(m.group(1))
    return symbols, content

def build_include_index():
    """Build an index of all #include "*.gfx.inc.c" references."""
    index = {}
    for root, dirs, files in os.walk(SRC_DIR):
        for f in files:
            if not f.endswith(".c"):
                continue
            src_path = Path(root) / f
            try:
                content = src_path.read_text()
            except:
                continue
            for i, line in enumerate(content.split('\n')):
                m = re.search(r'#include\s+"([^"]*\.gfx\.inc\.c)"', line)
                if m:
                    inc_path = m.group(1)
                    if inc_path not in index:
                        index[inc_path] = []
                    index[inc_path].append((src_path, line, i))
    return index

def build_symbol_reference_index():
    """
    Build a comprehensive index of all Gfx symbol references across all source files.
    Returns: dict mapping symbol_name -> list of (src_path, line_content, line_num)
    """
    index = defaultdict(list)
    patterns = [
        # gSPDisplayList(ptr, symbol) or gSPDisplayList(ptr, &symbol)
        re.compile(r'gSPDisplayList\s*\([^,]+,\s*(?:&\s*)?((?:N\([^)]+\)|\w+))'),
        # extern Gfx symbol[];
        re.compile(r'extern\s+Gfx\s+((?:N\([^)]+\)|\w+))\s*\[\s*\]'),
        # .displayList = symbol or .displayList = &symbol
        re.compile(r'\.displayList\s*=\s*(?:&\s*)?((?:N\([^)]+\)|\w+))'),
        # STANDARD_ENTITY_MODEL_SCRIPT(symbol, ...)
        re.compile(r'STANDARD_ENTITY_MODEL_SCRIPT\s*\(\s*((?:N\([^)]+\)|\w+))'),
    ]

    for root, dirs, files in os.walk(SRC_DIR):
        for f in files:
            if not f.endswith(".c"):
                continue
            src_path = Path(root) / f
            try:
                content = src_path.read_text()
            except:
                continue
            for i, line in enumerate(content.split('\n')):
                for pat in patterns:
                    for m in pat.finditer(line):
                        sym = m.group(1)
                        index[sym].append((src_path, line.strip(), i))

    return index

def get_gfx_include_path(stub_path):
    """Convert a stub file path to the include path used in source files."""
    rel = stub_path.relative_to(INCLUDE_DIR)
    return str(rel)

def get_basename(gfx_filename):
    """Get the base name from a gfx.inc.c filename."""
    return gfx_filename.replace(".gfx.inc.c", "")

def find_expected_symbol_comprehensive(stub_path, include_index, symbol_ref_index):
    """
    Determine the expected symbol name for a gfx stub by:
    1. Finding source files that include it
    2. Looking at the same directory tree for symbol references
    3. Matching by naming patterns
    """
    rel_include = get_gfx_include_path(stub_path)
    basename = get_basename(stub_path.name)

    # Find source files that include this stub
    sources = include_index.get(rel_include, [])
    if not sources:
        # Try just the filename
        for key, val in include_index.items():
            if key.endswith(stub_path.name) or key.endswith('/' + stub_path.name):
                sources = val
                break

    if not sources:
        return None, "no_source"

    # Get the directory of the including source to search nearby files
    src_dirs = set()
    for src_path, _, _ in sources:
        src_dirs.add(src_path.parent)
        # Also add parent directory for cases like dgb_01/img0.c -> dgb_01/gfx.c
        src_dirs.add(src_path.parent.parent)

    # Strategy 1: Search for N(basename_gfx) pattern - most common for world/battle files
    candidates = []

    # Generate possible symbol names based on the basename
    possible_names = [
        f"N({basename}_gfx)",          # N(foo_gfx)
        f"{basename}_gfx",              # foo_gfx
    ]

    # For files in subdirectories, also try with parent dir prefix
    # e.g., world/toad_house/blanket.gfx.inc.c -> N(toad_house_blanket_gfx)
    stub_rel = stub_path.relative_to(INCLUDE_DIR)
    parts = stub_rel.parts
    if len(parts) >= 2:
        # Try combining the last two path components
        parent = parts[-2]
        possible_names.extend([
            f"N({parent}_{basename}_gfx)",
            f"{parent}_{basename}_gfx",
        ])
    if len(parts) >= 3:
        # Try combining more path components (e.g., lava_piranha/vine -> lava_piranha_vine_gfx)
        parent2 = parts[-3]
        parent1 = parts[-2]
        possible_names.extend([
            f"N({parent1}_{basename}_gfx)",
            f"N({parent2}_{parent1}_{basename}_gfx)",
        ])

    # Check which of these possible names are actually referenced in source
    for possible_sym in possible_names:
        if possible_sym in symbol_ref_index:
            # Verify the reference is in a related source directory
            refs = symbol_ref_index[possible_sym]
            for ref_path, _, _ in refs:
                for src_dir in src_dirs:
                    try:
                        ref_path.relative_to(src_dir)
                        candidates.append(possible_sym)
                        break
                    except ValueError:
                        pass
                    # Also check if it's in a common/ directory that could be included
                    if 'common' in str(ref_path):
                        candidates.append(possible_sym)
                        break
                if candidates and candidates[-1] == possible_sym:
                    break

    # Deduplicate
    seen = set()
    unique_candidates = []
    for c in candidates:
        if c not in seen:
            seen.add(c)
            unique_candidates.append(c)

    if unique_candidates:
        return unique_candidates, "found"

    # Strategy 2: For D_XXXXXXXX named files, the symbol name IS the filename stem
    if basename.startswith("D_") and re.match(r'D_[0-9A-Fa-f]+(_[0-9A-Fa-f]+)?$', basename):
        if basename in symbol_ref_index:
            return [basename], "found_d_symbol"
        # No reference found but the convention is clear - keep as is
        return None, "d_symbol_no_ref"

    # Strategy 3: For hex-address files like A2BC0.gfx.inc.c, we need VRAM computation
    if re.match(r'^[0-9A-Fa-f]+$', basename):
        # This is a hex-address file - symbol name is D_VRAMADDR_ROMADDR
        # Skip these for now - they need special handling via splat yaml
        return None, "hex_address_needs_vram"

    # Strategy 4: Search all sources in the same area for any Gfx reference containing basename
    for sym, refs in symbol_ref_index.items():
        inner = sym
        if inner.startswith("N(") and inner.endswith(")"):
            inner = inner[2:-1]
        if basename in inner and inner.endswith("_gfx"):
            for ref_path, _, _ in refs:
                for src_dir in src_dirs:
                    try:
                        ref_path.relative_to(src_dir)
                        if sym not in unique_candidates:
                            unique_candidates.append(sym)
                        break
                    except ValueError:
                        pass
                    if 'common' in str(ref_path):
                        if sym not in unique_candidates:
                            unique_candidates.append(sym)
                        break

    if unique_candidates:
        return unique_candidates, "found_broad"

    return None, "no_match"

def generate_stub_content(symbols):
    """Generate the content for a gfx.inc.c stub file."""
    parts = ["// Auto-generated display list stub for PC port"]
    for sym in symbols:
        parts.append(f"Gfx {sym}[] = {{")
        parts.append("    gsSPEndDisplayList(),")
        parts.append("};")
        parts.append("")
    return '\n'.join(parts)

def main():
    print("=" * 70)
    print("Fix .gfx.inc.c stub symbol names")
    print("=" * 70)

    # Step 1: Build indices
    print("\nBuilding include index from source files...")
    include_index = build_include_index()
    print(f"Found {len(include_index)} unique .gfx.inc.c includes in source")

    print("Building comprehensive symbol reference index...")
    symbol_ref_index = build_symbol_reference_index()
    print(f"Found {len(symbol_ref_index)} unique Gfx symbol references")

    # Step 2: Find all stub files
    stubs = find_gfx_stubs()
    print(f"Found {len(stubs)} .gfx.inc.c stub files (excluding entity/ and level_up/)")

    stats = defaultdict(int)
    fixed = 0
    fixes_log = []

    for stub_path in stubs:
        rel_include = get_gfx_include_path(stub_path)
        stub_symbols, stub_content = get_stub_symbols(stub_path)

        expected, reason = find_expected_symbol_comprehensive(
            stub_path, include_index, symbol_ref_index
        )

        if expected is None:
            stats[reason] += 1
            if reason not in ("d_symbol_no_ref", "hex_address_needs_vram"):
                if stub_symbols:
                    # Only warn for cases that might actually be wrong
                    pass  # Suppress noisy warnings
            continue

        # Compare
        if set(stub_symbols) == set(expected):
            stats["already_correct"] += 1
            continue

        # Fix needed
        print(f"\n  FIX: {rel_include}")
        print(f"    Stub has:  {stub_symbols}")
        print(f"    Expected:  {expected}")

        new_content = generate_stub_content(expected)
        stub_path.write_text(new_content)
        fixed += 1
        fixes_log.append((rel_include, stub_symbols, expected))

    print("\n" + "=" * 70)
    print(f"Results:")
    print(f"  Fixed:                  {fixed}")
    print(f"  Already correct:        {stats.get('already_correct', 0)}")
    print(f"  No source found:        {stats.get('no_source', 0)}")
    print(f"  D_ symbol (no ref):     {stats.get('d_symbol_no_ref', 0)}")
    print(f"  Hex addr (needs VRAM):  {stats.get('hex_address_needs_vram', 0)}")
    print(f"  No match found:         {stats.get('no_match', 0)}")
    print(f"  Found via refs:         {stats.get('found', 0) + stats.get('found_d_symbol', 0) + stats.get('found_broad', 0)}")
    print(f"  Total stubs:            {len(stubs)}")
    print("=" * 70)

    if fixes_log:
        print("\nAll fixes applied:")
        for rel_inc, old_syms, new_syms in fixes_log:
            print(f"  {rel_inc}: {old_syms} -> {new_syms}")

if __name__ == "__main__":
    main()
