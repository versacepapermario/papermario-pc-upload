#!/usr/bin/env python3
"""
Generate NPC sprite header files from npc_sprite_names.yaml.
Replicates the decomp's tools/build/sprite/header.py output format.
"""

import sys
import os
import yaml

YAML_PATH = os.path.join(os.path.dirname(__file__),
    "../../papermario-main/tools/splat_ext/npc_sprite_names.yaml")
OUTPUT_DIR = os.path.join(os.path.dirname(__file__), "../include/sprite/npc")

def main():
    yaml_path = os.path.abspath(YAML_PATH)
    output_dir = os.path.abspath(OUTPUT_DIR)

    with open(yaml_path) as f:
        cfg = yaml.safe_load(f.read())

    os.makedirs(output_dir, exist_ok=True)

    total_anims = 0
    for sprite_id, sprite_name in enumerate(cfg, 1):
        sprite = cfg[sprite_name]
        palettes = sprite.get("palettes", [])
        animations = sprite.get("animations", [])
        images = sprite.get("frames", [])

        header_path = os.path.join(output_dir, f"{sprite_name}.h")

        with open(header_path, "w") as f:
            guard = f"_NPC_SPRITE_{sprite_name.upper()}_H_"
            f.write(f"#ifndef {guard}\n")
            f.write(f"#define {guard}\n")
            f.write("\n")
            f.write('#include "types.h"\n')
            f.write("\n")

            # sprite definition
            f.write(f"#define SPR_{sprite_name} 0x{sprite_id:02X}\n")
            f.write("\n")

            # definitions for images
            for i, image_name in enumerate(images):
                f.write(f"#define SPR_IMG_{sprite_name}_{image_name} 0x{i:X}\n")
            f.write("\n")

            # definitions for palettes
            for p, palette_name in enumerate(palettes):
                if palette_name == "Default":
                    f.write(f"#define SPR_PAL_{sprite_name} 0x{p:X}\n")
                else:
                    f.write(f"#define SPR_PAL_{sprite_name}_{palette_name} 0x{p:X}\n")
            f.write("\n")

            # definitions for animations
            for p, palette_name in enumerate(palettes):
                for a, name in enumerate(animations):
                    anim_id = (sprite_id << 16) | (p << 8) | a
                    if palette_name == "Default":
                        f.write(f"#define ANIM_{sprite_name}_{name} 0x{sprite_id:02X}{p:02X}{a:02X}\n")
                    else:
                        f.write(f"#define ANIM_{sprite_name}_{palette_name}_{name} 0x{sprite_id:02X}{p:02X}{a:02X}\n")
                    total_anims += 1
                f.write("\n")

            f.write(f"#endif\n")

    print(f"Generated {len(cfg)} NPC sprite headers with {total_anims} animation defines in {output_dir}")

if __name__ == "__main__":
    main()
