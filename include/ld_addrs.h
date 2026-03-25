#ifndef _LD_ADDRS_H_
#define _LD_ADDRS_H_

/**
 * ld_addrs.h - PC port version
 *
 * On N64, this file is auto-generated from the linker script and declares
 * extern symbols for every ROM segment's start/end/VRAM addresses.
 * These are used for DMA operations (copying data from ROM to RAM).
 *
 * On PC, all data is either statically linked or loaded from O2R archives
 * at runtime. These symbols are declared here as dummy externs to satisfy
 * compilation. The actual implementations are in port/ld_addrs_stubs.c
 * which provides zero-length or NULL definitions.
 *
 * DMA_COPY_SEGMENT() in macros.h is already a no-op on PC, so most of
 * these symbols are never actually dereferenced — they just need to exist
 * for the code to compile.
 */

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Macro for declaring overlay segment symbols (ROM_START/END/VRAM)
#define DECLARE_OVERLAY_SEGMENT(name) \
    extern Addr name##_ROM_START; \
    extern Addr name##_ROM_END; \
    extern Addr name##_VRAM;

// ============================================================
// Core engine segments (used by DMA_COPY_SEGMENT in load_engine_data)
// ============================================================
extern Addr engine1_ROM_START;
extern Addr engine1_ROM_END;
extern Addr engine1_VRAM;

extern Addr engine2_ROM_START;
extern Addr engine2_ROM_END;
extern Addr engine2_VRAM;

extern Addr engine4_ROM_START;
extern Addr engine4_ROM_END;
extern Addr engine4_VRAM;

extern Addr evt_ROM_START;
extern Addr evt_ROM_END;
extern Addr evt_VRAM;

extern Addr entity_ROM_START;
extern Addr entity_ROM_END;
extern Addr entity_VRAM;

extern Addr font_width_ROM_START;
extern Addr font_width_ROM_END;
extern Addr font_width_VRAM;

// ============================================================
// Audio segment
// ============================================================
extern Addr audio_ROM_START;
extern Addr audio_ROM_END;
extern Addr audio_VRAM;

// ============================================================
// Overlay segments (filemenu, pause, etc.)
// ============================================================
extern Addr filemenu_ROM_START;
extern Addr filemenu_ROM_END;
extern Addr filemenu_VRAM;
extern Addr filemenu_TEXT_START;
extern Addr filemenu_TEXT_END;
extern Addr filemenu_DATA_START;
extern Addr filemenu_RODATA_END;
extern Addr filemenu_BSS_START;
extern Addr filemenu_BSS_END;

extern Addr pause_ROM_START;
extern Addr pause_ROM_END;
extern Addr pause_VRAM;
extern Addr pause_TEXT_START;
extern Addr pause_TEXT_END;
extern Addr pause_DATA_START;
extern Addr pause_RODATA_END;
extern Addr pause_BSS_START;
extern Addr pause_BSS_END;

extern Addr pause_gfx_en_ROM_START;
extern Addr pause_gfx_en_ROM_END;
extern Addr pause_gfx_en_VRAM;
extern Addr pause_gfx_en_TEXT_START;
extern Addr pause_gfx_en_TEXT_END;
extern Addr pause_gfx_en_DATA_START;
extern Addr pause_gfx_en_RODATA_END;
extern Addr pause_gfx_en_BSS_START;
extern Addr pause_gfx_en_BSS_END;

extern Addr pause_gfx_de_ROM_START;
extern Addr pause_gfx_de_ROM_END;
extern Addr pause_gfx_de_VRAM;
extern Addr pause_gfx_de_TEXT_START;
extern Addr pause_gfx_de_TEXT_END;
extern Addr pause_gfx_de_DATA_START;
extern Addr pause_gfx_de_RODATA_END;
extern Addr pause_gfx_de_BSS_START;
extern Addr pause_gfx_de_BSS_END;

extern Addr pause_gfx_fr_ROM_START;
extern Addr pause_gfx_fr_ROM_END;
extern Addr pause_gfx_fr_VRAM;
extern Addr pause_gfx_fr_TEXT_START;
extern Addr pause_gfx_fr_TEXT_END;
extern Addr pause_gfx_fr_DATA_START;
extern Addr pause_gfx_fr_RODATA_END;
extern Addr pause_gfx_fr_BSS_START;
extern Addr pause_gfx_fr_BSS_END;

extern Addr pause_gfx_es_ROM_START;
extern Addr pause_gfx_es_ROM_END;
extern Addr pause_gfx_es_VRAM;
extern Addr pause_gfx_es_TEXT_START;
extern Addr pause_gfx_es_TEXT_END;
extern Addr pause_gfx_es_DATA_START;
extern Addr pause_gfx_es_RODATA_END;
extern Addr pause_gfx_es_BSS_START;
extern Addr pause_gfx_es_BSS_END;

// ============================================================
// Asset/data segments
// ============================================================
extern Addr charset_ROM_START;
extern Addr charset_ROM_END;
extern Addr charset_VRAM;

extern Addr icon_ROM_START;
extern Addr icon_ROM_END;
extern Addr icon_VRAM;

extern Addr msg_ROM_START;
extern Addr msg_ROM_END;
extern Addr msg_VRAM;

extern Addr msg_pal_en_ROM_START;
extern Addr msg_pal_de_ROM_START;
extern Addr msg_pal_fr_ROM_START;
extern Addr msg_pal_es_ROM_START;

extern Addr sprites_ROM_START;
extern Addr sprites_ROM_END;
extern Addr sprites_VRAM;

extern Addr sprite_shading_profiles_ROM_START;
extern Addr sprite_shading_profiles_ROM_END;
extern Addr sprite_shading_profiles_VRAM;

extern Addr sprite_shading_profiles_data_ROM_START;

extern Addr imgfx_data_ROM_START;
extern Addr imgfx_data_ROM_END;
extern Addr imgfx_data_VRAM;

extern Addr mapfs_ROM_START;
extern Addr mapfs_ROM_END;
extern Addr mapfs_VRAM;

extern Addr logos_ROM_START;
extern Addr logos_ROM_END;
extern Addr logos_VRAM;

// ============================================================
// Title screen assets
// ============================================================
extern Addr titlemenu_ROM_START;
extern Addr titlemenu_ROM_END;
extern Addr titlemenu_VRAM;

extern Addr titlemenu_flags_ROM_START;
extern Addr titlemenu_flags_ROM_END;
extern Addr titlemenu_flags_VRAM;

extern Addr title_bg_1_ROM_START;
extern Addr title_bg_1_ROM_END;
extern Addr title_bg_1_VRAM;

extern Addr title_tape_ROM_START;
extern Addr title_tape_ROM_END;
extern Addr title_tape_VRAM;

// ============================================================
// UI assets
// ============================================================
extern Addr ui_no_controller_ROM_START;
extern Addr ui_no_controller_ROM_END;
extern Addr ui_no_controller_VRAM;

// ============================================================
// HUD / menu
// ============================================================
extern Addr level_up_letters_en_de_ROM_START;
extern Addr level_up_letters_en_de_ROM_END;

extern Addr level_up_letters_es_ROM_START;
extern Addr level_up_letters_es_ROM_END;

extern Addr level_up_letters_fr_ROM_START;
extern Addr level_up_letters_fr_ROM_END;

extern Addr starpoint_en_ROM_START;
extern Addr starpoint_en_ROM_END;

extern Addr starpoint_de_ROM_START;
extern Addr starpoint_de_ROM_END;

extern Addr starpoint_es_ROM_START;
extern Addr starpoint_es_ROM_END;

extern Addr starpoint_fr_ROM_START;
extern Addr starpoint_fr_ROM_END;

// ============================================================
// Obfuscation / decompilation segments
// ============================================================
extern Addr obfuscated_obfuscation_shims_ROM_START;
extern Addr obfuscated_obfuscation_shims_VRAM;

extern Addr obfuscation_shims_ROM_START;
extern Addr obfuscation_shims_ROM_END;
extern Addr obfuscation_shims_VRAM;

extern Addr battle_heap_create_obfuscated_ROM_START;
extern Addr battle_heap_create_obfuscated_ROM_END;

extern Addr create_audio_system_obfuscated_ROM_START;
extern Addr create_audio_system_obfuscated_ROM_END;

extern Addr general_heap_create_obfuscated_ROM_START;
extern Addr general_heap_create_obfuscated_ROM_END;

extern Addr load_engine_data_obfuscated_ROM_START;
extern Addr load_engine_data_obfuscated_ROM_END;

// ============================================================
// Effect segments
// ============================================================
extern Addr effect_globals_ROM_START;
extern Addr effect_globals_ROM_END;
extern Addr effect_globals_VRAM;

extern Addr effect_gfx_attack_result_text_ROM_START;
extern Addr effect_gfx_attack_result_text_ROM_END;

extern Addr effect_gfx_attack_result_text_de_ROM_START;
extern Addr effect_gfx_attack_result_text_de_ROM_END;

extern Addr effect_gfx_attack_result_text_es_ROM_START;
extern Addr effect_gfx_attack_result_text_es_ROM_END;

extern Addr effect_gfx_attack_result_text_fr_ROM_START;
extern Addr effect_gfx_attack_result_text_fr_ROM_END;

extern Addr effect_gfx_chapter_change_ROM_START;
extern Addr effect_gfx_chapter_change_ROM_END;

extern Addr effect_chapter_change_gfx_de_ROM_START;
extern Addr effect_chapter_change_gfx_de_ROM_END;

extern Addr effect_chapter_change_gfx_es_ROM_START;
extern Addr effect_chapter_change_gfx_es_ROM_END;

extern Addr effect_chapter_change_gfx_fr_ROM_START;
extern Addr effect_chapter_change_gfx_fr_ROM_END;

// ============================================================
// Entity segments
// ============================================================
extern Addr entity_default_ROM_START;
extern Addr entity_default_ROM_END;
extern Addr entity_default_VRAM;

extern Addr entity_jan_iwa_ROM_START;
extern Addr entity_jan_iwa_ROM_END;
extern Addr entity_jan_iwa_VRAM;

extern Addr entity_sbk_omo_ROM_START;
extern Addr entity_sbk_omo_ROM_END;
extern Addr entity_sbk_omo_VRAM;

extern Addr entity_model_Signpost_ROM_START;
extern Addr entity_model_Signpost_ROM_END;
extern Addr entity_model_Signpost_VRAM;
extern Addr entity_model_ArrowSign_ROM_START;
extern Addr entity_model_ArrowSign_ROM_END;
extern Addr entity_model_ArrowSign_VRAM;
extern Addr entity_model_BellbellPlant_anim_ROM_START;
extern Addr entity_model_BellbellPlant_anim_ROM_END;
extern Addr entity_model_BellbellPlant_anim_VRAM;
extern Addr entity_model_BellbellPlant_gfx_ROM_START;
extern Addr entity_model_BellbellPlant_gfx_ROM_END;
extern Addr entity_model_BellbellPlant_gfx_VRAM;
extern Addr entity_model_BlueSwitch_ROM_START;
extern Addr entity_model_BlueSwitch_ROM_END;
extern Addr entity_model_BlueSwitch_VRAM;
extern Addr entity_model_BlueWarpPipe_ROM_START;
extern Addr entity_model_BlueWarpPipe_ROM_END;
extern Addr entity_model_BlueWarpPipe_VRAM;
extern Addr entity_model_BoardedFloor_ROM_START;
extern Addr entity_model_BoardedFloor_ROM_END;
extern Addr entity_model_BoardedFloor_VRAM;
extern Addr entity_model_BombableRock_ROM_START;
extern Addr entity_model_BombableRock_ROM_END;
extern Addr entity_model_BombableRock_VRAM;
extern Addr entity_model_BrickBlock_ROM_START;
extern Addr entity_model_BrickBlock_ROM_END;
extern Addr entity_model_BrickBlock_VRAM;
extern Addr entity_model_Chest_ROM_START;
extern Addr entity_model_Chest_ROM_END;
extern Addr entity_model_Chest_VRAM;
extern Addr entity_model_CymbalPlant_anim_ROM_START;
extern Addr entity_model_CymbalPlant_anim_ROM_END;
extern Addr entity_model_CymbalPlant_anim_VRAM;
extern Addr entity_model_CymbalPlant_gfx_ROM_START;
extern Addr entity_model_CymbalPlant_gfx_ROM_END;
extern Addr entity_model_CymbalPlant_gfx_VRAM;
extern Addr entity_model_GreenStompSwitch_ROM_START;
extern Addr entity_model_GreenStompSwitch_ROM_END;
extern Addr entity_model_GreenStompSwitch_VRAM;
extern Addr entity_model_Hammer1Block_ROM_START;
extern Addr entity_model_Hammer1Block_ROM_END;
extern Addr entity_model_Hammer1Block_VRAM;
extern Addr entity_model_Hammer2Block_ROM_START;
extern Addr entity_model_Hammer2Block_ROM_END;
extern Addr entity_model_Hammer2Block_VRAM;
extern Addr entity_model_Hammer3Block_ROM_START;
extern Addr entity_model_Hammer3Block_ROM_END;
extern Addr entity_model_Hammer3Block_VRAM;
extern Addr entity_model_HeartBlock_ROM_START;
extern Addr entity_model_HeartBlock_ROM_END;
extern Addr entity_model_HeartBlock_VRAM;
extern Addr entity_model_HeartBlockContent_ROM_START;
extern Addr entity_model_HeartBlockContent_ROM_END;
extern Addr entity_model_HeartBlockContent_VRAM;
extern Addr entity_model_HiddenPanel_ROM_START;
extern Addr entity_model_HiddenPanel_ROM_END;
extern Addr entity_model_HiddenPanel_VRAM;
extern Addr entity_model_HitFloatingYellowBlock_anim_ROM_START;
extern Addr entity_model_HitFloatingYellowBlock_anim_ROM_END;
extern Addr entity_model_HitFloatingYellowBlock_anim_VRAM;
extern Addr entity_model_HitFloatingYellowBlock_gfx_ROM_START;
extern Addr entity_model_HitFloatingYellowBlock_gfx_ROM_END;
extern Addr entity_model_HitFloatingYellowBlock_gfx_VRAM;
extern Addr entity_model_HitRedBlock_anim_ROM_START;
extern Addr entity_model_HitRedBlock_anim_ROM_END;
extern Addr entity_model_HitRedBlock_anim_VRAM;
extern Addr entity_model_HitRedBlock_gfx_ROM_START;
extern Addr entity_model_HitRedBlock_gfx_ROM_END;
extern Addr entity_model_HitRedBlock_gfx_VRAM;
extern Addr entity_model_HitYellowBlock_anim_ROM_START;
extern Addr entity_model_HitYellowBlock_anim_ROM_END;
extern Addr entity_model_HitYellowBlock_anim_VRAM;
extern Addr entity_model_HitYellowBlock_gfx_ROM_START;
extern Addr entity_model_HitYellowBlock_gfx_ROM_END;
extern Addr entity_model_HitYellowBlock_gfx_VRAM;
extern Addr entity_model_InertRedBlock_ROM_START;
extern Addr entity_model_InertRedBlock_ROM_END;
extern Addr entity_model_InertRedBlock_VRAM;
extern Addr entity_model_InertYellowBlock_ROM_START;
extern Addr entity_model_InertYellowBlock_ROM_END;
extern Addr entity_model_InertYellowBlock_VRAM;
extern Addr entity_model_MunchlesiaBeginChew_anim_ROM_START;
extern Addr entity_model_MunchlesiaBeginChew_anim_ROM_END;
extern Addr entity_model_MunchlesiaBeginChew_anim_VRAM;
extern Addr entity_model_MunchlesiaBeginChew_gfx_ROM_START;
extern Addr entity_model_MunchlesiaBeginChew_gfx_ROM_END;
extern Addr entity_model_MunchlesiaBeginChew_gfx_VRAM;
extern Addr entity_model_MunchlesiaChewing_anim_ROM_START;
extern Addr entity_model_MunchlesiaChewing_anim_ROM_END;
extern Addr entity_model_MunchlesiaChewing_anim_VRAM;
extern Addr entity_model_MunchlesiaChewing_gfx_ROM_START;
extern Addr entity_model_MunchlesiaChewing_gfx_ROM_END;
extern Addr entity_model_MunchlesiaChewing_gfx_VRAM;
extern Addr entity_model_MunchlesiaEnvelop_anim_ROM_START;
extern Addr entity_model_MunchlesiaEnvelop_anim_ROM_END;
extern Addr entity_model_MunchlesiaEnvelop_anim_VRAM;
extern Addr entity_model_MunchlesiaEnvelop_gfx_ROM_START;
extern Addr entity_model_MunchlesiaEnvelop_gfx_ROM_END;
extern Addr entity_model_MunchlesiaEnvelop_gfx_VRAM;
extern Addr entity_model_MunchlesiaGrab_anim_ROM_START;
extern Addr entity_model_MunchlesiaGrab_anim_ROM_END;
extern Addr entity_model_MunchlesiaGrab_anim_VRAM;
extern Addr entity_model_MunchlesiaGrab_gfx_ROM_START;
extern Addr entity_model_MunchlesiaGrab_gfx_ROM_END;
extern Addr entity_model_MunchlesiaGrab_gfx_VRAM;
extern Addr entity_model_MunchlesiaReset_anim_ROM_START;
extern Addr entity_model_MunchlesiaReset_anim_ROM_END;
extern Addr entity_model_MunchlesiaReset_anim_VRAM;
extern Addr entity_model_MunchlesiaReset_gfx_ROM_START;
extern Addr entity_model_MunchlesiaReset_gfx_ROM_END;
extern Addr entity_model_MunchlesiaReset_gfx_VRAM;
extern Addr entity_model_MunchlesiaReset1_anim_ROM_START;
extern Addr entity_model_MunchlesiaReset1_anim_ROM_END;
extern Addr entity_model_MunchlesiaReset1_anim_VRAM;
extern Addr entity_model_MunchlesiaReset1_gfx_ROM_START;
extern Addr entity_model_MunchlesiaReset1_gfx_ROM_END;
extern Addr entity_model_MunchlesiaReset1_gfx_VRAM;
extern Addr entity_model_MunchlesiaSpitOut_anim_ROM_START;
extern Addr entity_model_MunchlesiaSpitOut_anim_ROM_END;
extern Addr entity_model_MunchlesiaSpitOut_anim_VRAM;
extern Addr entity_model_MunchlesiaSpitOut_gfx_ROM_START;
extern Addr entity_model_MunchlesiaSpitOut_gfx_ROM_END;
extern Addr entity_model_MunchlesiaSpitOut_gfx_VRAM;
extern Addr entity_model_Padlock_ROM_START;
extern Addr entity_model_Padlock_ROM_END;
extern Addr entity_model_Padlock_VRAM;
extern Addr entity_model_PadlockBlueFace_ROM_START;
extern Addr entity_model_PadlockBlueFace_ROM_END;
extern Addr entity_model_PadlockBlueFace_VRAM;
extern Addr entity_model_PadlockRedFace_ROM_START;
extern Addr entity_model_PadlockRedFace_ROM_END;
extern Addr entity_model_PadlockRedFace_VRAM;
extern Addr entity_model_PadlockRedFrame_ROM_START;
extern Addr entity_model_PadlockRedFrame_ROM_END;
extern Addr entity_model_PadlockRedFrame_VRAM;
extern Addr entity_model_PinkFlower_anim_ROM_START;
extern Addr entity_model_PinkFlower_anim_ROM_END;
extern Addr entity_model_PinkFlower_anim_VRAM;
extern Addr entity_model_PinkFlower_gfx_ROM_START;
extern Addr entity_model_PinkFlower_gfx_ROM_END;
extern Addr entity_model_PinkFlower_gfx_VRAM;
extern Addr entity_model_PowBlock_ROM_START;
extern Addr entity_model_PowBlock_ROM_END;
extern Addr entity_model_PowBlock_VRAM;
extern Addr entity_model_PushBlock_ROM_START;
extern Addr entity_model_PushBlock_ROM_END;
extern Addr entity_model_PushBlock_VRAM;
extern Addr entity_model_RedBlock_ROM_START;
extern Addr entity_model_RedBlock_ROM_END;
extern Addr entity_model_RedBlock_VRAM;
extern Addr entity_model_RedSwitch_ROM_START;
extern Addr entity_model_RedSwitch_ROM_END;
extern Addr entity_model_RedSwitch_VRAM;
extern Addr entity_model_SaveBlock_ROM_START;
extern Addr entity_model_SaveBlock_ROM_END;
extern Addr entity_model_SaveBlock_VRAM;
extern Addr entity_model_ScriptSpring_anim_ROM_START;
extern Addr entity_model_ScriptSpring_anim_ROM_END;
extern Addr entity_model_ScriptSpring_anim_VRAM;
extern Addr entity_model_ScriptSpring_gfx_ROM_START;
extern Addr entity_model_ScriptSpring_gfx_ROM_END;
extern Addr entity_model_ScriptSpring_gfx_VRAM;
extern Addr entity_model_ShatteringBrickBlock_ROM_START;
extern Addr entity_model_ShatteringBrickBlock_ROM_END;
extern Addr entity_model_ShatteringBrickBlock_VRAM;
extern Addr entity_model_ShatteringHammer1Block_ROM_START;
extern Addr entity_model_ShatteringHammer1Block_ROM_END;
extern Addr entity_model_ShatteringHammer1Block_VRAM;
extern Addr entity_model_ShatteringHammer2Block_ROM_START;
extern Addr entity_model_ShatteringHammer2Block_ROM_END;
extern Addr entity_model_ShatteringHammer2Block_VRAM;
extern Addr entity_model_ShatteringHammer3Block_ROM_START;
extern Addr entity_model_ShatteringHammer3Block_ROM_END;
extern Addr entity_model_ShatteringHammer3Block_VRAM;
extern Addr entity_model_SimpleSpring_anim_ROM_START;
extern Addr entity_model_SimpleSpring_anim_ROM_END;
extern Addr entity_model_SimpleSpring_anim_VRAM;
extern Addr entity_model_SimpleSpring_gfx_ROM_START;
extern Addr entity_model_SimpleSpring_gfx_ROM_END;
extern Addr entity_model_SimpleSpring_gfx_VRAM;
extern Addr entity_model_SpinningFlower_ROM_START;
extern Addr entity_model_SpinningFlower_ROM_END;
extern Addr entity_model_SpinningFlower_VRAM;
extern Addr entity_model_StarBoxLauncher_ROM_START;
extern Addr entity_model_StarBoxLauncher_ROM_END;
extern Addr entity_model_StarBoxLauncher_VRAM;
extern Addr entity_model_SuperBlock_ROM_START;
extern Addr entity_model_SuperBlock_ROM_END;
extern Addr entity_model_SuperBlock_VRAM;
extern Addr entity_model_SuperBlockContent_ROM_START;
extern Addr entity_model_SuperBlockContent_ROM_END;
extern Addr entity_model_SuperBlockContent_VRAM;
extern Addr entity_model_TrumpetPlant_anim_ROM_START;
extern Addr entity_model_TrumpetPlant_anim_ROM_END;
extern Addr entity_model_TrumpetPlant_anim_VRAM;
extern Addr entity_model_TrumpetPlant_gfx_ROM_START;
extern Addr entity_model_TrumpetPlant_gfx_ROM_END;
extern Addr entity_model_TrumpetPlant_gfx_VRAM;
extern Addr entity_model_Tweester_ROM_START;
extern Addr entity_model_Tweester_ROM_END;
extern Addr entity_model_Tweester_VRAM;
extern Addr entity_model_UltraBlock_ROM_START;
extern Addr entity_model_UltraBlock_ROM_END;
extern Addr entity_model_UltraBlock_VRAM;
extern Addr entity_model_UltraBlockContent_ROM_START;
extern Addr entity_model_UltraBlockContent_ROM_END;
extern Addr entity_model_UltraBlockContent_VRAM;
extern Addr entity_model_WoodenCrate_ROM_START;
extern Addr entity_model_WoodenCrate_ROM_END;
extern Addr entity_model_WoodenCrate_VRAM;
extern Addr entity_model_YellowBlock_ROM_START;
extern Addr entity_model_YellowBlock_ROM_END;
extern Addr entity_model_YellowBlock_VRAM;

// ============================================================
// World partner overlays
// ============================================================
extern Addr world_partner_goombario_ROM_START;
extern Addr world_partner_goombario_ROM_END;
extern Addr world_partner_goombario_VRAM;

extern Addr world_partner_kooper_ROM_START;
extern Addr world_partner_kooper_ROM_END;
extern Addr world_partner_kooper_VRAM;

extern Addr world_partner_bombette_ROM_START;
extern Addr world_partner_bombette_ROM_END;
extern Addr world_partner_bombette_VRAM;

extern Addr world_partner_parakarry_ROM_START;
extern Addr world_partner_parakarry_ROM_END;
extern Addr world_partner_parakarry_VRAM;

extern Addr world_partner_bow_ROM_START;
extern Addr world_partner_bow_ROM_END;
extern Addr world_partner_bow_VRAM;

extern Addr world_partner_watt_ROM_START;
extern Addr world_partner_watt_ROM_END;
extern Addr world_partner_watt_VRAM;

extern Addr world_partner_sushie_ROM_START;
extern Addr world_partner_sushie_ROM_END;
extern Addr world_partner_sushie_VRAM;

extern Addr world_partner_lakilester_ROM_START;
extern Addr world_partner_lakilester_ROM_END;
extern Addr world_partner_lakilester_VRAM;

extern Addr world_partner_goombaria_ROM_START;
extern Addr world_partner_goombaria_ROM_END;
extern Addr world_partner_goombaria_VRAM;

extern Addr world_partner_goompa_ROM_START;
extern Addr world_partner_goompa_ROM_END;
extern Addr world_partner_goompa_VRAM;

extern Addr world_partner_twink_ROM_START;
extern Addr world_partner_twink_ROM_END;
extern Addr world_partner_twink_VRAM;

// ============================================================
// World action/item overlays
// ============================================================
extern Addr world_use_item_ROM_START;
extern Addr world_use_item_ROM_END;
extern Addr world_use_item_VRAM;

// ============================================================
// Misc symbols referenced in code
// ============================================================
extern Addr D_80200000;
extern Addr TextureHeap;
extern Addr BattleEntityHeapBottom;
extern Addr main_BSS_START;

// Asset table
extern Addr ASSET_TABLE_ROM_START;
extern Addr ASSET_TABLE_VRAM;

// SBN audio offset
extern Addr SBN_ROM_OFFSET;

// ImgFX animation headers (extern Addr in imgfx.c)
extern Addr shock_header;
extern Addr shiver_header;
extern Addr vertical_pipe_curl_header;
extern Addr horizontal_pipe_curl_header;
extern Addr startle_header;
extern Addr flutter_down_header;
extern Addr unfurl_header;
extern Addr get_in_bed_header;
extern Addr spirit_capture_header;
extern Addr unused_1_header;
extern Addr unused_2_header;
extern Addr unused_3_header;
extern Addr tutankoopa_gather_header;
extern Addr tutankoopa_swirl_2_header;
extern Addr tutankoopa_swirl_1_header;
extern Addr shuffle_cards_header;
extern Addr flip_card_1_header;
extern Addr flip_card_2_header;
extern Addr flip_card_3_header;
extern Addr cymbal_crush_header;

// ============================================================
// Player action overlays (used by actions.c)
// ============================================================
extern Addr world_action_idle_ROM_START;
extern Addr world_action_idle_ROM_END;
extern Addr world_action_walk_ROM_START;
extern Addr world_action_walk_ROM_END;
extern Addr world_action_jump_ROM_START;
extern Addr world_action_jump_ROM_END;
extern Addr world_action_land_ROM_START;
extern Addr world_action_land_ROM_END;
extern Addr world_action_misc_ROM_START;
extern Addr world_action_misc_ROM_END;
extern Addr world_action_spin_jump_ROM_START;
extern Addr world_action_spin_jump_ROM_END;
extern Addr world_action_tornado_jump_ROM_START;
extern Addr world_action_tornado_jump_ROM_END;
extern Addr world_action_slide_ROM_START;
extern Addr world_action_slide_ROM_END;
extern Addr world_action_hammer_ROM_START;
extern Addr world_action_hammer_ROM_END;
extern Addr world_action_hit_fire_ROM_START;
extern Addr world_action_hit_fire_ROM_END;
extern Addr world_action_knockback_ROM_START;
extern Addr world_action_knockback_ROM_END;
extern Addr world_action_hit_lava_ROM_START;
extern Addr world_action_hit_lava_ROM_END;
extern Addr world_action_step_up_ROM_START;
extern Addr world_action_step_up_ROM_END;
extern Addr world_action_sneaky_parasol_ROM_START;
extern Addr world_action_sneaky_parasol_ROM_END;
extern Addr world_action_spin_ROM_START;
extern Addr world_action_spin_ROM_END;
extern Addr world_action_use_spinning_flower_ROM_START;
extern Addr world_action_use_spinning_flower_ROM_END;
extern Addr world_action_use_munchlesia_ROM_START;
extern Addr world_action_use_munchlesia_ROM_END;
extern Addr world_action_use_tweester_ROM_START;
extern Addr world_action_use_tweester_ROM_END;

// ============================================================
// Inspect/UI overlays
// ============================================================
extern Addr inspect_icon_ROM_START;
extern Addr inspect_icon_ROM_END;
extern Addr inspect_icon_VRAM;

extern Addr speech_bubble_ROM_START;
extern Addr speech_bubble_ROM_END;
extern Addr speech_bubble_VRAM;

extern Addr pulse_stone_ROM_START;
extern Addr pulse_stone_ROM_END;
extern Addr pulse_stone_VRAM;

extern Addr i_spy_ROM_START;
extern Addr i_spy_ROM_END;
extern Addr i_spy_VRAM;

// ============================================================
// Action command overlays
// ============================================================
extern Addr action_cmd_jump_ROM_START;
extern Addr action_cmd_jump_ROM_END;
extern Addr action_cmd_jump_VRAM;

extern Addr action_cmd_hammer_ROM_START;
extern Addr action_cmd_hammer_ROM_END;
extern Addr action_cmd_hammer_VRAM;

extern Addr action_cmd_flee_ROM_START;
extern Addr action_cmd_flee_ROM_END;
extern Addr action_cmd_flee_VRAM;

extern Addr action_cmd_unused_flee_ROM_START;

extern Addr action_cmd_break_free_ROM_START;
extern Addr action_cmd_break_free_ROM_END;
extern Addr action_cmd_break_free_VRAM;

extern Addr action_cmd_whirlwind_ROM_START;
extern Addr action_cmd_whirlwind_ROM_END;
extern Addr action_cmd_whirlwind_VRAM;

extern Addr action_cmd_stop_leech_ROM_START;
extern Addr action_cmd_stop_leech_ROM_END;
extern Addr action_cmd_stop_leech_VRAM;

extern Addr action_cmd_fire_shell_ROM_START;
extern Addr action_cmd_fire_shell_ROM_END;
extern Addr action_cmd_fire_shell_VRAM;

extern Addr action_cmd_bomb_ROM_START;
extern Addr action_cmd_bomb_ROM_END;
extern Addr action_cmd_bomb_VRAM;

extern Addr action_cmd_body_slam_ROM_START;
extern Addr action_cmd_body_slam_ROM_END;
extern Addr action_cmd_body_slam_VRAM;

extern Addr action_cmd_air_lift_ROM_START;
extern Addr action_cmd_air_lift_ROM_END;
extern Addr action_cmd_air_lift_VRAM;

extern Addr action_cmd_air_raid_ROM_START;
extern Addr action_cmd_air_raid_ROM_END;
extern Addr action_cmd_air_raid_VRAM;

extern Addr action_cmd_squirt_ROM_START;
extern Addr action_cmd_squirt_ROM_END;
extern Addr action_cmd_squirt_VRAM;

extern Addr action_cmd_power_shock_ROM_START;
extern Addr action_cmd_power_shock_ROM_END;
extern Addr action_cmd_power_shock_VRAM;

extern Addr action_cmd_mega_shock_ROM_START;
extern Addr action_cmd_mega_shock_ROM_END;
extern Addr action_cmd_mega_shock_VRAM;

extern Addr action_cmd_smack_ROM_START;
extern Addr action_cmd_smack_ROM_END;
extern Addr action_cmd_smack_VRAM;

extern Addr action_cmd_spiny_surge_ROM_START;
extern Addr action_cmd_spiny_surge_ROM_END;
extern Addr action_cmd_spiny_surge_VRAM;

extern Addr action_cmd_hurricane_ROM_START;
extern Addr action_cmd_hurricane_ROM_END;
extern Addr action_cmd_hurricane_VRAM;

extern Addr action_cmd_spook_ROM_START;
extern Addr action_cmd_spook_ROM_END;
extern Addr action_cmd_spook_VRAM;

extern Addr action_cmd_three_chances_ROM_START;
extern Addr action_cmd_three_chances_ROM_END;
extern Addr action_cmd_three_chances_VRAM;

extern Addr action_cmd_tidal_wave_ROM_START;
extern Addr action_cmd_tidal_wave_ROM_END;
extern Addr action_cmd_tidal_wave_VRAM;

extern Addr action_cmd_dizzy_shell_ROM_START;
extern Addr action_cmd_dizzy_shell_ROM_END;
extern Addr action_cmd_dizzy_shell_VRAM;

extern Addr action_cmd_unused_flee_ROM_END;
extern Addr action_cmd_unused_flee_VRAM;

extern Addr action_cmd_unused_mash_a_ROM_START;
extern Addr action_cmd_unused_mash_a_ROM_END;
extern Addr action_cmd_unused_mash_a_VRAM;

// ============================================================
// World model animation overlays (KZN volcano area)
// ============================================================
extern Addr world_model_anim_kzn_00_ROM_START;
extern Addr world_model_anim_kzn_00_ROM_END;
extern Addr world_model_anim_kzn_00_VRAM;

extern Addr world_model_anim_kzn_01_ROM_START;
extern Addr world_model_anim_kzn_01_ROM_END;
extern Addr world_model_anim_kzn_01_VRAM;

extern Addr world_model_anim_kzn_02_ROM_START;
extern Addr world_model_anim_kzn_02_ROM_END;
extern Addr world_model_anim_kzn_02_VRAM;

extern Addr world_model_anim_kzn_03_ROM_START;
extern Addr world_model_anim_kzn_03_ROM_END;
extern Addr world_model_anim_kzn_03_VRAM;

extern Addr world_model_anim_kzn_04_ROM_START;
extern Addr world_model_anim_kzn_04_ROM_END;
extern Addr world_model_anim_kzn_04_VRAM;

extern Addr world_model_anim_kzn_05_ROM_START;
extern Addr world_model_anim_kzn_05_ROM_END;
extern Addr world_model_anim_kzn_05_VRAM;

extern Addr world_model_anim_kzn_06_ROM_START;
extern Addr world_model_anim_kzn_06_ROM_END;
extern Addr world_model_anim_kzn_06_VRAM;

extern Addr world_model_anim_kzn_07_ROM_START;
extern Addr world_model_anim_kzn_07_ROM_END;
extern Addr world_model_anim_kzn_07_VRAM;

extern Addr world_model_anim_kzn_08_ROM_START;
extern Addr world_model_anim_kzn_08_ROM_END;
extern Addr world_model_anim_kzn_08_VRAM;

extern Addr world_model_anim_kzn_09_ROM_START;
extern Addr world_model_anim_kzn_09_ROM_END;
extern Addr world_model_anim_kzn_09_VRAM;

extern Addr world_model_anim_kzn_0A_ROM_START;
extern Addr world_model_anim_kzn_0A_ROM_END;
extern Addr world_model_anim_kzn_0A_VRAM;

extern Addr world_model_anim_kzn_0B_ROM_START;
extern Addr world_model_anim_kzn_0B_ROM_END;
extern Addr world_model_anim_kzn_0B_VRAM;

extern Addr world_model_anim_kzn_0C_ROM_START;
extern Addr world_model_anim_kzn_0C_ROM_END;
extern Addr world_model_anim_kzn_0C_VRAM;

extern Addr world_model_anim_kzn_0D_ROM_START;
extern Addr world_model_anim_kzn_0D_ROM_END;
extern Addr world_model_anim_kzn_0D_VRAM;

extern Addr world_model_anim_kzn_0E_ROM_START;
extern Addr world_model_anim_kzn_0E_ROM_END;
extern Addr world_model_anim_kzn_0E_VRAM;

extern Addr world_model_anim_kzn_0F_ROM_START;
extern Addr world_model_anim_kzn_0F_ROM_END;
extern Addr world_model_anim_kzn_0F_VRAM;

extern Addr world_model_anim_kzn_10_ROM_START;
extern Addr world_model_anim_kzn_10_ROM_END;
extern Addr world_model_anim_kzn_10_VRAM;

extern Addr world_model_anim_kzn_11_ROM_START;
extern Addr world_model_anim_kzn_11_ROM_END;
extern Addr world_model_anim_kzn_11_VRAM;

extern Addr world_model_anim_kzn_12_ROM_START;
extern Addr world_model_anim_kzn_12_ROM_END;
extern Addr world_model_anim_kzn_12_VRAM;

extern Addr world_model_anim_kzn_13_ROM_START;
extern Addr world_model_anim_kzn_13_ROM_END;
extern Addr world_model_anim_kzn_13_VRAM;

extern Addr world_model_anim_kzn_14_ROM_START;
extern Addr world_model_anim_kzn_14_ROM_END;
extern Addr world_model_anim_kzn_14_VRAM;

extern Addr world_model_anim_kzn_15_ROM_START;
extern Addr world_model_anim_kzn_15_ROM_END;
extern Addr world_model_anim_kzn_15_VRAM;

extern Addr world_model_anim_kzn_16_ROM_START;
extern Addr world_model_anim_kzn_16_ROM_END;
extern Addr world_model_anim_kzn_16_VRAM;

extern Addr world_model_anim_kzn_17_ROM_START;
extern Addr world_model_anim_kzn_17_ROM_END;
extern Addr world_model_anim_kzn_17_VRAM;

extern Addr world_model_anim_kzn_18_ROM_START;
extern Addr world_model_anim_kzn_18_ROM_END;
extern Addr world_model_anim_kzn_18_VRAM;

extern Addr world_model_anim_kzn_19_ROM_START;
extern Addr world_model_anim_kzn_19_ROM_END;
extern Addr world_model_anim_kzn_19_VRAM;

extern Addr world_model_anim_kzn_1A_ROM_START;
extern Addr world_model_anim_kzn_1A_ROM_END;
extern Addr world_model_anim_kzn_1A_VRAM;

extern Addr world_model_anim_kzn_1B_ROM_START;
extern Addr world_model_anim_kzn_1B_ROM_END;
extern Addr world_model_anim_kzn_1B_VRAM;

extern Addr world_model_anim_kzn_1C_ROM_START;
extern Addr world_model_anim_kzn_1C_ROM_END;
extern Addr world_model_anim_kzn_1C_VRAM;

extern Addr world_model_anim_kzn_1D_ROM_START;
extern Addr world_model_anim_kzn_1D_ROM_END;
extern Addr world_model_anim_kzn_1D_VRAM;

extern Addr world_model_anim_kzn_1E_ROM_START;
extern Addr world_model_anim_kzn_1E_ROM_END;
extern Addr world_model_anim_kzn_1E_VRAM;

extern Addr world_model_anim_kzn_1F_ROM_START;
extern Addr world_model_anim_kzn_1F_ROM_END;
extern Addr world_model_anim_kzn_1F_VRAM;

extern Addr world_model_anim_kzn_20_ROM_START;
extern Addr world_model_anim_kzn_20_ROM_END;
extern Addr world_model_anim_kzn_20_VRAM;

extern Addr world_model_anim_kzn_21_ROM_START;
extern Addr world_model_anim_kzn_21_ROM_END;
extern Addr world_model_anim_kzn_21_VRAM;

extern Addr world_model_anim_kzn_22_ROM_START;
extern Addr world_model_anim_kzn_22_ROM_END;
extern Addr world_model_anim_kzn_22_VRAM;

extern Addr world_model_anim_kzn_23_ROM_START;
extern Addr world_model_anim_kzn_23_ROM_END;
extern Addr world_model_anim_kzn_23_VRAM;

extern Addr world_model_anim_kzn_24_ROM_START;
extern Addr world_model_anim_kzn_24_ROM_END;
extern Addr world_model_anim_kzn_24_VRAM;


/* Battle area overlay symbols */
extern Addr battle_area_kmr_part_1_ROM_START;
extern Addr battle_area_kmr_part_1_ROM_END;
extern Addr battle_area_kmr_part_1_VRAM;

extern Addr battle_area_kmr_part_2_ROM_START;
extern Addr battle_area_kmr_part_2_ROM_END;
extern Addr battle_area_kmr_part_2_VRAM;

extern Addr battle_area_kmr_part_3_ROM_START;
extern Addr battle_area_kmr_part_3_ROM_END;
extern Addr battle_area_kmr_part_3_VRAM;

extern Addr battle_area_mac_ROM_START;
extern Addr battle_area_mac_ROM_END;
extern Addr battle_area_mac_VRAM;

extern Addr battle_area_hos_ROM_START;
extern Addr battle_area_hos_ROM_END;
extern Addr battle_area_hos_VRAM;

extern Addr battle_area_nok_ROM_START;
extern Addr battle_area_nok_ROM_END;
extern Addr battle_area_nok_VRAM;

extern Addr battle_area_trd_part_1_ROM_START;
extern Addr battle_area_trd_part_1_ROM_END;
extern Addr battle_area_trd_part_1_VRAM;

extern Addr battle_area_trd_part_2_ROM_START;
extern Addr battle_area_trd_part_2_ROM_END;
extern Addr battle_area_trd_part_2_VRAM;

extern Addr battle_area_trd_part_3_ROM_START;
extern Addr battle_area_trd_part_3_ROM_END;
extern Addr battle_area_trd_part_3_VRAM;

extern Addr battle_area_iwa_ROM_START;
extern Addr battle_area_iwa_ROM_END;
extern Addr battle_area_iwa_VRAM;

extern Addr battle_area_sbk_ROM_START;
extern Addr battle_area_sbk_ROM_END;
extern Addr battle_area_sbk_VRAM;

extern Addr battle_area_isk_part_1_ROM_START;
extern Addr battle_area_isk_part_1_ROM_END;
extern Addr battle_area_isk_part_1_VRAM;

extern Addr battle_area_isk_part_2_ROM_START;
extern Addr battle_area_isk_part_2_ROM_END;
extern Addr battle_area_isk_part_2_VRAM;

extern Addr battle_area_mim_ROM_START;
extern Addr battle_area_mim_ROM_END;
extern Addr battle_area_mim_VRAM;

extern Addr battle_area_arn_ROM_START;
extern Addr battle_area_arn_ROM_END;
extern Addr battle_area_arn_VRAM;

extern Addr battle_area_dgb_ROM_START;
extern Addr battle_area_dgb_ROM_END;
extern Addr battle_area_dgb_VRAM;

extern Addr battle_area_omo_ROM_START;
extern Addr battle_area_omo_ROM_END;
extern Addr battle_area_omo_VRAM;

extern Addr battle_area_omo2_ROM_START;
extern Addr battle_area_omo2_ROM_END;
extern Addr battle_area_omo2_VRAM;

extern Addr battle_area_omo3_ROM_START;
extern Addr battle_area_omo3_ROM_END;
extern Addr battle_area_omo3_VRAM;

extern Addr battle_area_kgr_ROM_START;
extern Addr battle_area_kgr_ROM_END;
extern Addr battle_area_kgr_VRAM;

extern Addr battle_area_jan_ROM_START;
extern Addr battle_area_jan_ROM_END;
extern Addr battle_area_jan_VRAM;

extern Addr battle_area_jan2_ROM_START;
extern Addr battle_area_jan2_ROM_END;
extern Addr battle_area_jan2_VRAM;

extern Addr battle_area_kzn_ROM_START;
extern Addr battle_area_kzn_ROM_END;
extern Addr battle_area_kzn_VRAM;

extern Addr battle_area_kzn2_ROM_START;
extern Addr battle_area_kzn2_ROM_END;
extern Addr battle_area_kzn2_VRAM;

extern Addr battle_area_flo_ROM_START;
extern Addr battle_area_flo_ROM_END;
extern Addr battle_area_flo_VRAM;

extern Addr battle_area_flo2_ROM_START;
extern Addr battle_area_flo2_ROM_END;
extern Addr battle_area_flo2_VRAM;

extern Addr battle_area_tik_ROM_START;
extern Addr battle_area_tik_ROM_END;
extern Addr battle_area_tik_VRAM;

extern Addr battle_area_tik2_ROM_START;
extern Addr battle_area_tik2_ROM_END;
extern Addr battle_area_tik2_VRAM;

extern Addr battle_area_tik3_ROM_START;
extern Addr battle_area_tik3_ROM_END;
extern Addr battle_area_tik3_VRAM;

extern Addr battle_area_sam_ROM_START;
extern Addr battle_area_sam_ROM_END;
extern Addr battle_area_sam_VRAM;

extern Addr battle_area_sam2_ROM_START;
extern Addr battle_area_sam2_ROM_END;
extern Addr battle_area_sam2_VRAM;

extern Addr battle_area_pra_ROM_START;
extern Addr battle_area_pra_ROM_END;
extern Addr battle_area_pra_VRAM;

extern Addr battle_area_pra2_ROM_START;
extern Addr battle_area_pra2_ROM_END;
extern Addr battle_area_pra2_VRAM;

extern Addr battle_area_pra3_ROM_START;
extern Addr battle_area_pra3_ROM_END;
extern Addr battle_area_pra3_VRAM;

extern Addr battle_area_kpa_ROM_START;
extern Addr battle_area_kpa_ROM_END;
extern Addr battle_area_kpa_VRAM;

extern Addr battle_area_kpa2_ROM_START;
extern Addr battle_area_kpa2_ROM_END;
extern Addr battle_area_kpa2_VRAM;

extern Addr battle_area_kpa3_ROM_START;
extern Addr battle_area_kpa3_ROM_END;
extern Addr battle_area_kpa3_VRAM;

extern Addr battle_area_kpa4_ROM_START;
extern Addr battle_area_kpa4_ROM_END;
extern Addr battle_area_kpa4_VRAM;

extern Addr battle_area_kkj_ROM_START;
extern Addr battle_area_kkj_ROM_END;
extern Addr battle_area_kkj_VRAM;

extern Addr battle_area_dig_ROM_START;
extern Addr battle_area_dig_ROM_END;
extern Addr battle_area_dig_VRAM;

extern Addr battle_area_omo2_1_ROM_START;
extern Addr battle_area_omo2_1_ROM_END;
extern Addr battle_area_omo2_1_VRAM;

extern Addr battle_area_omo2_2_ROM_START;
extern Addr battle_area_omo2_2_ROM_END;
extern Addr battle_area_omo2_2_VRAM;

extern Addr battle_area_omo2_3_ROM_START;
extern Addr battle_area_omo2_3_ROM_END;
extern Addr battle_area_omo2_3_VRAM;

extern Addr battle_area_omo2_4_ROM_START;
extern Addr battle_area_omo2_4_ROM_END;
extern Addr battle_area_omo2_4_VRAM;

extern Addr battle_area_omo2_5_ROM_START;
extern Addr battle_area_omo2_5_ROM_END;
extern Addr battle_area_omo2_5_VRAM;

extern Addr battle_area_omo2_6_ROM_START;
extern Addr battle_area_omo2_6_ROM_END;
extern Addr battle_area_omo2_6_VRAM;

// Battle Item Overlays
DECLARE_OVERLAY_SEGMENT(food)
DECLARE_OVERLAY_SEGMENT(mushroom)
DECLARE_OVERLAY_SEGMENT(fire_flower)
DECLARE_OVERLAY_SEGMENT(dusty_hammer)
DECLARE_OVERLAY_SEGMENT(pow_block)
DECLARE_OVERLAY_SEGMENT(pebble)
DECLARE_OVERLAY_SEGMENT(volt_shroom)
DECLARE_OVERLAY_SEGMENT(thunder_rage)
DECLARE_OVERLAY_SEGMENT(snowman_doll)
DECLARE_OVERLAY_SEGMENT(shooting_star)
DECLARE_OVERLAY_SEGMENT(sleepy_sheep)
DECLARE_OVERLAY_SEGMENT(stone_cap)
DECLARE_OVERLAY_SEGMENT(tasty_tonic)
DECLARE_OVERLAY_SEGMENT(thunder_bolt)
DECLARE_OVERLAY_SEGMENT(super_soda)
DECLARE_OVERLAY_SEGMENT(hustle_drink)
DECLARE_OVERLAY_SEGMENT(stop_watch)
DECLARE_OVERLAY_SEGMENT(dizzy_dial)
DECLARE_OVERLAY_SEGMENT(please_come_back)
DECLARE_OVERLAY_SEGMENT(egg_missile)
DECLARE_OVERLAY_SEGMENT(insecticide_herb)
DECLARE_OVERLAY_SEGMENT(fright_jar)
DECLARE_OVERLAY_SEGMENT(mystery)
DECLARE_OVERLAY_SEGMENT(repel_gel)
DECLARE_OVERLAY_SEGMENT(life_shroom)
DECLARE_OVERLAY_SEGMENT(coconut)
DECLARE_OVERLAY_SEGMENT(electro_pop)
DECLARE_OVERLAY_SEGMENT(strange_cake)

// Battle Move Overlays
DECLARE_OVERLAY_SEGMENT(battle_move_hammer_attack)
DECLARE_OVERLAY_SEGMENT(battle_move_hammer_charge_0)
DECLARE_OVERLAY_SEGMENT(battle_move_hammer_charge_1)
DECLARE_OVERLAY_SEGMENT(battle_move_hammer_charge_2)
DECLARE_OVERLAY_SEGMENT(battle_move_auto_smash)
DECLARE_OVERLAY_SEGMENT(battle_move_power_smash)
DECLARE_OVERLAY_SEGMENT(battle_move_super_smash)
DECLARE_OVERLAY_SEGMENT(battle_move_mega_smash)
DECLARE_OVERLAY_SEGMENT(battle_move_quake_hammer)
DECLARE_OVERLAY_SEGMENT(battle_move_power_quake)
DECLARE_OVERLAY_SEGMENT(battle_move_mega_quake)
DECLARE_OVERLAY_SEGMENT(battle_move_hammer_throw)
DECLARE_OVERLAY_SEGMENT(battle_move_d_down_pound)
DECLARE_OVERLAY_SEGMENT(battle_move_shrink_smash)
DECLARE_OVERLAY_SEGMENT(battle_move_spin_smash)
DECLARE_OVERLAY_SEGMENT(battle_move_shell_crack)
DECLARE_OVERLAY_SEGMENT(battle_move_jump_attack)
DECLARE_OVERLAY_SEGMENT(battle_move_jump_charge_0)
DECLARE_OVERLAY_SEGMENT(battle_move_jump_charge_1)
DECLARE_OVERLAY_SEGMENT(battle_move_jump_charge_2)
DECLARE_OVERLAY_SEGMENT(battle_move_auto_jump)
DECLARE_OVERLAY_SEGMENT(battle_move_power_jump)
DECLARE_OVERLAY_SEGMENT(battle_move_super_jump)
DECLARE_OVERLAY_SEGMENT(battle_move_mega_jump)
DECLARE_OVERLAY_SEGMENT(battle_move_multibounce)
DECLARE_OVERLAY_SEGMENT(battle_move_power_bounce)
DECLARE_OVERLAY_SEGMENT(battle_move_auto_multibounce)
DECLARE_OVERLAY_SEGMENT(battle_move_d_down_jump)
DECLARE_OVERLAY_SEGMENT(battle_move_sleep_stomp)
DECLARE_OVERLAY_SEGMENT(battle_move_dizzy_stomp)
DECLARE_OVERLAY_SEGMENT(battle_move_shrink_stomp)
DECLARE_OVERLAY_SEGMENT(battle_move_earthquake_jump)

// Star Power Overlays
DECLARE_OVERLAY_SEGMENT(battle_move_focus)
DECLARE_OVERLAY_SEGMENT(battle_move_refresh)
DECLARE_OVERLAY_SEGMENT(battle_move_lullaby)
DECLARE_OVERLAY_SEGMENT(battle_move_star_storm)
DECLARE_OVERLAY_SEGMENT(battle_move_chill_out)
DECLARE_OVERLAY_SEGMENT(battle_move_smooch)
DECLARE_OVERLAY_SEGMENT(battle_move_time_out)
DECLARE_OVERLAY_SEGMENT(battle_move_up_and_away)
DECLARE_OVERLAY_SEGMENT(battle_move_star_beam)
DECLARE_OVERLAY_SEGMENT(battle_move_peach_beam)
DECLARE_OVERLAY_SEGMENT(battle_move_peach_focus)
DECLARE_OVERLAY_SEGMENT(battle_move_peach_focus_alt)

// Battle Partner Overlays
DECLARE_OVERLAY_SEGMENT(battle_partner_goombario)
DECLARE_OVERLAY_SEGMENT(battle_partner_kooper)
DECLARE_OVERLAY_SEGMENT(battle_partner_bombette)
DECLARE_OVERLAY_SEGMENT(battle_partner_parakarry)
DECLARE_OVERLAY_SEGMENT(battle_partner_goompa)
DECLARE_OVERLAY_SEGMENT(battle_partner_watt)
DECLARE_OVERLAY_SEGMENT(battle_partner_sushie)
DECLARE_OVERLAY_SEGMENT(battle_partner_lakilester)
DECLARE_OVERLAY_SEGMENT(battle_partner_bow)
DECLARE_OVERLAY_SEGMENT(battle_partner_twink)

// Effect overlay segments (code)
DECLARE_OVERLAY_SEGMENT(effect_big_smoke_puff)
DECLARE_OVERLAY_SEGMENT(effect_landing_dust)
DECLARE_OVERLAY_SEGMENT(effect_walking_dust)
DECLARE_OVERLAY_SEGMENT(effect_flower_splash)
DECLARE_OVERLAY_SEGMENT(effect_flower_trail)
DECLARE_OVERLAY_SEGMENT(effect_cloud_puff)
DECLARE_OVERLAY_SEGMENT(effect_cloud_trail)
DECLARE_OVERLAY_SEGMENT(effect_footprint)
DECLARE_OVERLAY_SEGMENT(effect_floating_flower)
DECLARE_OVERLAY_SEGMENT(effect_snowflake)
DECLARE_OVERLAY_SEGMENT(effect_star)
DECLARE_OVERLAY_SEGMENT(effect_emote)
DECLARE_OVERLAY_SEGMENT(effect_sparkles)
DECLARE_OVERLAY_SEGMENT(effect_shape_spell)
DECLARE_OVERLAY_SEGMENT(effect_gather_energy_pink)
DECLARE_OVERLAY_SEGMENT(effect_drop_leaves)
DECLARE_OVERLAY_SEGMENT(effect_dust)
DECLARE_OVERLAY_SEGMENT(effect_shattering_stones)
DECLARE_OVERLAY_SEGMENT(effect_smoke_ring)
DECLARE_OVERLAY_SEGMENT(effect_damage_stars)
DECLARE_OVERLAY_SEGMENT(effect_explosion)
DECLARE_OVERLAY_SEGMENT(effect_lens_flare)
DECLARE_OVERLAY_SEGMENT(effect_got_item_outline)
DECLARE_OVERLAY_SEGMENT(effect_spiky_white_aura)
DECLARE_OVERLAY_SEGMENT(effect_smoke_impact)
DECLARE_OVERLAY_SEGMENT(effect_damage_indicator)
DECLARE_OVERLAY_SEGMENT(effect_purple_ring)
DECLARE_OVERLAY_SEGMENT(effect_flame)
DECLARE_OVERLAY_SEGMENT(effect_stars_burst)
DECLARE_OVERLAY_SEGMENT(effect_stars_shimmer)
DECLARE_OVERLAY_SEGMENT(effect_rising_bubble)
DECLARE_OVERLAY_SEGMENT(effect_ring_blast)
DECLARE_OVERLAY_SEGMENT(effect_shockwave)
DECLARE_OVERLAY_SEGMENT(effect_music_note)
DECLARE_OVERLAY_SEGMENT(effect_smoke_burst)
DECLARE_OVERLAY_SEGMENT(effect_sweat)
DECLARE_OVERLAY_SEGMENT(effect_sleep_bubble)
DECLARE_OVERLAY_SEGMENT(effect_windy_leaves)
DECLARE_OVERLAY_SEGMENT(effect_falling_leaves)
DECLARE_OVERLAY_SEGMENT(effect_stars_spread)
DECLARE_OVERLAY_SEGMENT(effect_steam_burst)
DECLARE_OVERLAY_SEGMENT(effect_stars_orbiting)
DECLARE_OVERLAY_SEGMENT(effect_big_snowflakes)
DECLARE_OVERLAY_SEGMENT(effect_debuff)
DECLARE_OVERLAY_SEGMENT(effect_green_impact)
DECLARE_OVERLAY_SEGMENT(effect_radial_shimmer)
DECLARE_OVERLAY_SEGMENT(effect_ending_decals)
DECLARE_OVERLAY_SEGMENT(effect_light_rays)
DECLARE_OVERLAY_SEGMENT(effect_lightning)
DECLARE_OVERLAY_SEGMENT(effect_fire_breath)
DECLARE_OVERLAY_SEGMENT(effect_shimmer_burst)
DECLARE_OVERLAY_SEGMENT(effect_energy_shockwave)
DECLARE_OVERLAY_SEGMENT(effect_shimmer_wave)
DECLARE_OVERLAY_SEGMENT(effect_aura)
DECLARE_OVERLAY_SEGMENT(effect_bulb_glow)
DECLARE_OVERLAY_SEGMENT(effect_effect_3D)
DECLARE_OVERLAY_SEGMENT(effect_blast)
DECLARE_OVERLAY_SEGMENT(effect_fire_flower)
DECLARE_OVERLAY_SEGMENT(effect_recover)
DECLARE_OVERLAY_SEGMENT(effect_disable_x)
DECLARE_OVERLAY_SEGMENT(effect_bombette_breaking)
DECLARE_OVERLAY_SEGMENT(effect_firework)
DECLARE_OVERLAY_SEGMENT(effect_confetti)
DECLARE_OVERLAY_SEGMENT(effect_snowfall)
DECLARE_OVERLAY_SEGMENT(effect_effect_46)
DECLARE_OVERLAY_SEGMENT(effect_gather_magic)
DECLARE_OVERLAY_SEGMENT(effect_attack_result_text)
DECLARE_OVERLAY_SEGMENT(effect_small_gold_sparkle)
DECLARE_OVERLAY_SEGMENT(effect_flashing_box_shockwave)
DECLARE_OVERLAY_SEGMENT(effect_balloon)
DECLARE_OVERLAY_SEGMENT(effect_floating_rock)
DECLARE_OVERLAY_SEGMENT(effect_chomp_drop)
DECLARE_OVERLAY_SEGMENT(effect_quizmo_stage)
DECLARE_OVERLAY_SEGMENT(effect_radiating_energy_orb)
DECLARE_OVERLAY_SEGMENT(effect_quizmo_answer)
DECLARE_OVERLAY_SEGMENT(effect_motion_blur_flame)
DECLARE_OVERLAY_SEGMENT(effect_energy_orb_wave)
DECLARE_OVERLAY_SEGMENT(effect_merlin_house_stars)
DECLARE_OVERLAY_SEGMENT(effect_quizmo_audience)
DECLARE_OVERLAY_SEGMENT(effect_butterflies)
DECLARE_OVERLAY_SEGMENT(effect_stat_change)
DECLARE_OVERLAY_SEGMENT(effect_snaking_static)
DECLARE_OVERLAY_SEGMENT(effect_thunderbolt_ring)
DECLARE_OVERLAY_SEGMENT(effect_squirt)
DECLARE_OVERLAY_SEGMENT(effect_water_block)
DECLARE_OVERLAY_SEGMENT(effect_waterfall)
DECLARE_OVERLAY_SEGMENT(effect_water_fountain)
DECLARE_OVERLAY_SEGMENT(effect_underwater)
DECLARE_OVERLAY_SEGMENT(effect_lightning_bolt)
DECLARE_OVERLAY_SEGMENT(effect_water_splash)
DECLARE_OVERLAY_SEGMENT(effect_snowman_doll)
DECLARE_OVERLAY_SEGMENT(effect_fright_jar)
DECLARE_OVERLAY_SEGMENT(effect_stop_watch)
DECLARE_OVERLAY_SEGMENT(effect_effect_63)
DECLARE_OVERLAY_SEGMENT(effect_throw_spiny)
DECLARE_OVERLAY_SEGMENT(effect_effect_65)
DECLARE_OVERLAY_SEGMENT(effect_tubba_heart_attack)
DECLARE_OVERLAY_SEGMENT(effect_whirlwind)
DECLARE_OVERLAY_SEGMENT(effect_red_impact)
DECLARE_OVERLAY_SEGMENT(effect_floating_cloud_puff)
DECLARE_OVERLAY_SEGMENT(effect_energy_in_out)
DECLARE_OVERLAY_SEGMENT(effect_tattle_window)
DECLARE_OVERLAY_SEGMENT(effect_shiny_flare)
DECLARE_OVERLAY_SEGMENT(effect_huff_puff_breath)
DECLARE_OVERLAY_SEGMENT(effect_cold_breath)
DECLARE_OVERLAY_SEGMENT(effect_embers)
DECLARE_OVERLAY_SEGMENT(effect_hieroglyphs)
DECLARE_OVERLAY_SEGMENT(effect_misc_particles)
DECLARE_OVERLAY_SEGMENT(effect_static_status)
DECLARE_OVERLAY_SEGMENT(effect_moving_cloud)
DECLARE_OVERLAY_SEGMENT(effect_effect_75)
DECLARE_OVERLAY_SEGMENT(effect_firework_rocket)
DECLARE_OVERLAY_SEGMENT(effect_peach_star_beam)
DECLARE_OVERLAY_SEGMENT(effect_chapter_change)
DECLARE_OVERLAY_SEGMENT(effect_ice_shard)
DECLARE_OVERLAY_SEGMENT(effect_spirit_card)
DECLARE_OVERLAY_SEGMENT(effect_lil_oink)
DECLARE_OVERLAY_SEGMENT(effect_something_rotating)
DECLARE_OVERLAY_SEGMENT(effect_breaking_junk)
DECLARE_OVERLAY_SEGMENT(effect_partner_buff)
DECLARE_OVERLAY_SEGMENT(effect_quizmo_assistant)
DECLARE_OVERLAY_SEGMENT(effect_ice_pillar)
DECLARE_OVERLAY_SEGMENT(effect_sun)
DECLARE_OVERLAY_SEGMENT(effect_star_spirits_energy)
DECLARE_OVERLAY_SEGMENT(effect_pink_sparkles)
DECLARE_OVERLAY_SEGMENT(effect_star_outline)
DECLARE_OVERLAY_SEGMENT(effect_effect_86)

// Effect graphics overlay segments
extern Addr effect_gfx_big_smoke_puff_ROM_START;
extern Addr effect_gfx_big_smoke_puff_ROM_END;
extern Addr effect_gfx_landing_dust_ROM_START;
extern Addr effect_gfx_landing_dust_ROM_END;
extern Addr effect_gfx_flower_splash_trail_ROM_START;
extern Addr effect_gfx_flower_splash_trail_ROM_END;
extern Addr effect_gfx_cloud_puff_trail_ROM_START;
extern Addr effect_gfx_cloud_puff_trail_ROM_END;
extern Addr effect_gfx_footprint_ROM_START;
extern Addr effect_gfx_footprint_ROM_END;
extern Addr effect_gfx_floating_flower_ROM_START;
extern Addr effect_gfx_floating_flower_ROM_END;
extern Addr effect_gfx_snowflake_ROM_START;
extern Addr effect_gfx_snowflake_ROM_END;
extern Addr effect_gfx_star_ROM_START;
extern Addr effect_gfx_star_ROM_END;
extern Addr effect_gfx_emote_ROM_START;
extern Addr effect_gfx_emote_ROM_END;
extern Addr effect_gfx_sparkles_ROM_START;
extern Addr effect_gfx_sparkles_ROM_END;
extern Addr effect_gfx_shape_spell_ROM_START;
extern Addr effect_gfx_shape_spell_ROM_END;
extern Addr effect_gfx_gather_energy_pink_ROM_START;
extern Addr effect_gfx_gather_energy_pink_ROM_END;
extern Addr effect_gfx_drop_leaves_ROM_START;
extern Addr effect_gfx_drop_leaves_ROM_END;
extern Addr effect_gfx_dust_ROM_START;
extern Addr effect_gfx_dust_ROM_END;
extern Addr effect_gfx_shattering_stones_ROM_START;
extern Addr effect_gfx_shattering_stones_ROM_END;
extern Addr effect_gfx_damage_stars_ROM_START;
extern Addr effect_gfx_damage_stars_ROM_END;
extern Addr effect_gfx_explosion_ROM_START;
extern Addr effect_gfx_explosion_ROM_END;
extern Addr effect_gfx_lens_flare_ROM_START;
extern Addr effect_gfx_lens_flare_ROM_END;
extern Addr effect_gfx_got_item_outline_ROM_START;
extern Addr effect_gfx_got_item_outline_ROM_END;
extern Addr effect_gfx_spiky_white_aura_ROM_START;
extern Addr effect_gfx_spiky_white_aura_ROM_END;
extern Addr effect_gfx_damage_indicator_ROM_START;
extern Addr effect_gfx_damage_indicator_ROM_END;
extern Addr effect_gfx_purple_ring_ROM_START;
extern Addr effect_gfx_purple_ring_ROM_END;
extern Addr effect_gfx_flame_ROM_START;
extern Addr effect_gfx_flame_ROM_END;
extern Addr effect_gfx_rising_bubble_ROM_START;
extern Addr effect_gfx_rising_bubble_ROM_END;
extern Addr effect_gfx_ring_blast_ROM_START;
extern Addr effect_gfx_ring_blast_ROM_END;
extern Addr effect_gfx_shockwave_ROM_START;
extern Addr effect_gfx_shockwave_ROM_END;
extern Addr effect_gfx_music_note_ROM_START;
extern Addr effect_gfx_music_note_ROM_END;
extern Addr effect_gfx_sweat_ROM_START;
extern Addr effect_gfx_sweat_ROM_END;
extern Addr effect_gfx_sleep_bubble_ROM_START;
extern Addr effect_gfx_sleep_bubble_ROM_END;
extern Addr effect_gfx_falling_leaves_ROM_START;
extern Addr effect_gfx_falling_leaves_ROM_END;
extern Addr effect_gfx_stars_spread_ROM_START;
extern Addr effect_gfx_stars_spread_ROM_END;
extern Addr effect_gfx_steam_burst_ROM_START;
extern Addr effect_gfx_steam_burst_ROM_END;
extern Addr effect_gfx_stars_orbiting_ROM_START;
extern Addr effect_gfx_stars_orbiting_ROM_END;
extern Addr effect_gfx_big_snowflakes_ROM_START;
extern Addr effect_gfx_big_snowflakes_ROM_END;
extern Addr effect_gfx_debuff_ROM_START;
extern Addr effect_gfx_debuff_ROM_END;
extern Addr effect_gfx_green_impact_ROM_START;
extern Addr effect_gfx_green_impact_ROM_END;
extern Addr effect_gfx_radial_shimmer_ROM_START;
extern Addr effect_gfx_radial_shimmer_ROM_END;
extern Addr effect_gfx_ending_decals_ROM_START;
extern Addr effect_gfx_ending_decals_ROM_END;
extern Addr effect_gfx_light_rays_ROM_START;
extern Addr effect_gfx_light_rays_ROM_END;
extern Addr effect_gfx_lightning_ROM_START;
extern Addr effect_gfx_lightning_ROM_END;
extern Addr effect_gfx_fire_breath_ROM_START;
extern Addr effect_gfx_fire_breath_ROM_END;
extern Addr effect_gfx_energy_shockwave_ROM_START;
extern Addr effect_gfx_energy_shockwave_ROM_END;
extern Addr effect_gfx_aura_ROM_START;
extern Addr effect_gfx_aura_ROM_END;
extern Addr effect_gfx_bulb_glow_ROM_START;
extern Addr effect_gfx_bulb_glow_ROM_END;
extern Addr effect_gfx_effect_3D_ROM_START;
extern Addr effect_gfx_effect_3D_ROM_END;
extern Addr effect_gfx_blast_ROM_START;
extern Addr effect_gfx_blast_ROM_END;
extern Addr effect_gfx_fire_flower_ROM_START;
extern Addr effect_gfx_fire_flower_ROM_END;
extern Addr effect_gfx_recover_ROM_START;
extern Addr effect_gfx_recover_ROM_END;
extern Addr effect_gfx_disable_x_ROM_START;
extern Addr effect_gfx_disable_x_ROM_END;
extern Addr effect_gfx_firework_ROM_START;
extern Addr effect_gfx_firework_ROM_END;
extern Addr effect_gfx_confetti_ROM_START;
extern Addr effect_gfx_confetti_ROM_END;
extern Addr effect_gfx_snowfall_ROM_START;
extern Addr effect_gfx_snowfall_ROM_END;
extern Addr effect_gfx_effect_46_ROM_START;
extern Addr effect_gfx_effect_46_ROM_END;
extern Addr effect_gfx_gather_magic_ROM_START;
extern Addr effect_gfx_gather_magic_ROM_END;
extern Addr effect_gfx_small_gold_sparkle_ROM_START;
extern Addr effect_gfx_small_gold_sparkle_ROM_END;
extern Addr effect_gfx_flashing_box_shockwave_ROM_START;
extern Addr effect_gfx_flashing_box_shockwave_ROM_END;
extern Addr effect_gfx_balloon_ROM_START;
extern Addr effect_gfx_balloon_ROM_END;
extern Addr effect_gfx_floating_rock_ROM_START;
extern Addr effect_gfx_floating_rock_ROM_END;
extern Addr effect_gfx_chomp_drop_ROM_START;
extern Addr effect_gfx_chomp_drop_ROM_END;
extern Addr effect_gfx_quizmo_stage_ROM_START;
extern Addr effect_gfx_quizmo_stage_ROM_END;
extern Addr effect_gfx_radiating_energy_orb_ROM_START;
extern Addr effect_gfx_radiating_energy_orb_ROM_END;
extern Addr effect_gfx_quizmo_answer_ROM_START;
extern Addr effect_gfx_quizmo_answer_ROM_END;
extern Addr effect_gfx_motion_blur_flame_ROM_START;
extern Addr effect_gfx_motion_blur_flame_ROM_END;
extern Addr effect_gfx_energy_orb_wave_ROM_START;
extern Addr effect_gfx_energy_orb_wave_ROM_END;
extern Addr effect_gfx_merlin_house_stars_ROM_START;
extern Addr effect_gfx_merlin_house_stars_ROM_END;
extern Addr effect_gfx_quizmo_audience_ROM_START;
extern Addr effect_gfx_quizmo_audience_ROM_END;
extern Addr effect_gfx_butterflies_ROM_START;
extern Addr effect_gfx_butterflies_ROM_END;
extern Addr effect_gfx_stat_change_ROM_START;
extern Addr effect_gfx_stat_change_ROM_END;
extern Addr effect_gfx_snaking_static_ROM_START;
extern Addr effect_gfx_snaking_static_ROM_END;
extern Addr effect_gfx_thunderbolt_ring_ROM_START;
extern Addr effect_gfx_thunderbolt_ring_ROM_END;
extern Addr effect_gfx_squirt_ROM_START;
extern Addr effect_gfx_squirt_ROM_END;
extern Addr effect_gfx_water_block_ROM_START;
extern Addr effect_gfx_water_block_ROM_END;
extern Addr effect_gfx_waterfall_ROM_START;
extern Addr effect_gfx_waterfall_ROM_END;
extern Addr effect_gfx_water_fountain_ROM_START;
extern Addr effect_gfx_water_fountain_ROM_END;
extern Addr effect_gfx_underwater_ROM_START;
extern Addr effect_gfx_underwater_ROM_END;
extern Addr effect_gfx_lightning_bolt_ROM_START;
extern Addr effect_gfx_lightning_bolt_ROM_END;
extern Addr effect_gfx_water_splash_ROM_START;
extern Addr effect_gfx_water_splash_ROM_END;
extern Addr effect_gfx_snowman_doll_ROM_START;
extern Addr effect_gfx_snowman_doll_ROM_END;
extern Addr effect_gfx_fright_jar_ROM_START;
extern Addr effect_gfx_fright_jar_ROM_END;
extern Addr effect_gfx_stop_watch_ROM_START;
extern Addr effect_gfx_stop_watch_ROM_END;
extern Addr effect_gfx_effect_63_ROM_START;
extern Addr effect_gfx_effect_63_ROM_END;
extern Addr effect_gfx_throw_spiny_ROM_START;
extern Addr effect_gfx_throw_spiny_ROM_END;
extern Addr effect_gfx_effect_65_ROM_START;
extern Addr effect_gfx_effect_65_ROM_END;
extern Addr effect_gfx_tubba_heart_attack_ROM_START;
extern Addr effect_gfx_tubba_heart_attack_ROM_END;
extern Addr effect_gfx_whirlwind_ROM_START;
extern Addr effect_gfx_whirlwind_ROM_END;
extern Addr effect_gfx_floating_cloud_puff_ROM_START;
extern Addr effect_gfx_floating_cloud_puff_ROM_END;
extern Addr effect_gfx_energy_in_out_ROM_START;
extern Addr effect_gfx_energy_in_out_ROM_END;
extern Addr effect_gfx_tattle_window_ROM_START;
extern Addr effect_gfx_tattle_window_ROM_END;
extern Addr effect_gfx_shiny_flare_ROM_START;
extern Addr effect_gfx_shiny_flare_ROM_END;
extern Addr effect_gfx_huff_puff_breath_ROM_START;
extern Addr effect_gfx_huff_puff_breath_ROM_END;
extern Addr effect_gfx_cold_breath_ROM_START;
extern Addr effect_gfx_cold_breath_ROM_END;
extern Addr effect_gfx_embers_ROM_START;
extern Addr effect_gfx_embers_ROM_END;
extern Addr effect_gfx_hieroglyphs_ROM_START;
extern Addr effect_gfx_hieroglyphs_ROM_END;
extern Addr effect_gfx_misc_particles_ROM_START;
extern Addr effect_gfx_misc_particles_ROM_END;
extern Addr effect_gfx_static_status_ROM_START;
extern Addr effect_gfx_static_status_ROM_END;
extern Addr effect_gfx_moving_cloud_ROM_START;
extern Addr effect_gfx_moving_cloud_ROM_END;
extern Addr effect_gfx_effect_75_ROM_START;
extern Addr effect_gfx_effect_75_ROM_END;
extern Addr effect_gfx_firework_rocket_ROM_START;
extern Addr effect_gfx_firework_rocket_ROM_END;
extern Addr effect_gfx_peach_star_beam_ROM_START;
extern Addr effect_gfx_peach_star_beam_ROM_END;
extern Addr effect_gfx_ice_shard_ROM_START;
extern Addr effect_gfx_ice_shard_ROM_END;
extern Addr effect_gfx_spirit_card_ROM_START;
extern Addr effect_gfx_spirit_card_ROM_END;
extern Addr effect_gfx_lil_oink_ROM_START;
extern Addr effect_gfx_lil_oink_ROM_END;
extern Addr effect_gfx_breaking_junk_ROM_START;
extern Addr effect_gfx_breaking_junk_ROM_END;
extern Addr effect_gfx_partner_buff_ROM_START;
extern Addr effect_gfx_partner_buff_ROM_END;
extern Addr effect_gfx_quizmo_assistant_ROM_START;
extern Addr effect_gfx_quizmo_assistant_ROM_END;
extern Addr effect_gfx_ice_pillar_ROM_START;
extern Addr effect_gfx_ice_pillar_ROM_END;
extern Addr effect_gfx_sun_ROM_START;
extern Addr effect_gfx_sun_ROM_END;
extern Addr effect_gfx_star_spirits_energy_ROM_START;
extern Addr effect_gfx_star_spirits_energy_ROM_END;
extern Addr effect_gfx_star_outline_ROM_START;
extern Addr effect_gfx_star_outline_ROM_END;
extern Addr effect_gfx_effect_86_ROM_START;
extern Addr effect_gfx_effect_86_ROM_END;


// ============================================================
// World map overlays (used by world.c)
// ============================================================
extern Addr arn_02_ROM_START;
extern Addr arn_02_ROM_END;
extern Addr arn_02_VRAM;
extern Addr arn_03_ROM_START;
extern Addr arn_03_ROM_END;
extern Addr arn_03_VRAM;
extern Addr arn_04_ROM_START;
extern Addr arn_04_ROM_END;
extern Addr arn_04_VRAM;
extern Addr arn_05_ROM_START;
extern Addr arn_05_ROM_END;
extern Addr arn_05_VRAM;
extern Addr arn_07_ROM_START;
extern Addr arn_07_ROM_END;
extern Addr arn_07_VRAM;
extern Addr arn_08_ROM_START;
extern Addr arn_08_ROM_END;
extern Addr arn_08_VRAM;
extern Addr arn_09_ROM_START;
extern Addr arn_09_ROM_END;
extern Addr arn_09_VRAM;
extern Addr arn_10_ROM_START;
extern Addr arn_10_ROM_END;
extern Addr arn_10_VRAM;
extern Addr arn_11_ROM_START;
extern Addr arn_11_ROM_END;
extern Addr arn_11_VRAM;
extern Addr arn_12_ROM_START;
extern Addr arn_12_ROM_END;
extern Addr arn_12_VRAM;
extern Addr arn_13_ROM_START;
extern Addr arn_13_ROM_END;
extern Addr arn_13_VRAM;
extern Addr dgb_00_ROM_START;
extern Addr dgb_00_ROM_END;
extern Addr dgb_00_VRAM;
extern Addr dgb_01_ROM_START;
extern Addr dgb_01_ROM_END;
extern Addr dgb_01_VRAM;
extern Addr dgb_02_ROM_START;
extern Addr dgb_02_ROM_END;
extern Addr dgb_02_VRAM;
extern Addr dgb_03_ROM_START;
extern Addr dgb_03_ROM_END;
extern Addr dgb_03_VRAM;
extern Addr dgb_04_ROM_START;
extern Addr dgb_04_ROM_END;
extern Addr dgb_04_VRAM;
extern Addr dgb_05_ROM_START;
extern Addr dgb_05_ROM_END;
extern Addr dgb_05_VRAM;
extern Addr dgb_06_ROM_START;
extern Addr dgb_06_ROM_END;
extern Addr dgb_06_VRAM;
extern Addr dgb_07_ROM_START;
extern Addr dgb_07_ROM_END;
extern Addr dgb_07_VRAM;
extern Addr dgb_08_ROM_START;
extern Addr dgb_08_ROM_END;
extern Addr dgb_08_VRAM;
extern Addr dgb_09_ROM_START;
extern Addr dgb_09_ROM_END;
extern Addr dgb_09_VRAM;
extern Addr dgb_10_ROM_START;
extern Addr dgb_10_ROM_END;
extern Addr dgb_10_VRAM;
extern Addr dgb_11_ROM_START;
extern Addr dgb_11_ROM_END;
extern Addr dgb_11_VRAM;
extern Addr dgb_12_ROM_START;
extern Addr dgb_12_ROM_END;
extern Addr dgb_12_VRAM;
extern Addr dgb_13_ROM_START;
extern Addr dgb_13_ROM_END;
extern Addr dgb_13_VRAM;
extern Addr dgb_14_ROM_START;
extern Addr dgb_14_ROM_END;
extern Addr dgb_14_VRAM;
extern Addr dgb_15_ROM_START;
extern Addr dgb_15_ROM_END;
extern Addr dgb_15_VRAM;
extern Addr dgb_16_ROM_START;
extern Addr dgb_16_ROM_END;
extern Addr dgb_16_VRAM;
extern Addr dgb_17_ROM_START;
extern Addr dgb_17_ROM_END;
extern Addr dgb_17_VRAM;
extern Addr dgb_18_ROM_START;
extern Addr dgb_18_ROM_END;
extern Addr dgb_18_VRAM;
extern Addr dro_01_ROM_START;
extern Addr dro_01_ROM_END;
extern Addr dro_01_VRAM;
extern Addr dro_02_ROM_START;
extern Addr dro_02_ROM_END;
extern Addr dro_02_VRAM;
extern Addr end_00_ROM_START;
extern Addr end_00_ROM_END;
extern Addr end_00_VRAM;
extern Addr end_01_ROM_START;
extern Addr end_01_ROM_END;
extern Addr end_01_VRAM;
extern Addr flo_00_ROM_START;
extern Addr flo_00_ROM_END;
extern Addr flo_00_VRAM;
extern Addr flo_03_ROM_START;
extern Addr flo_03_ROM_END;
extern Addr flo_03_VRAM;
extern Addr flo_07_ROM_START;
extern Addr flo_07_ROM_END;
extern Addr flo_07_VRAM;
extern Addr flo_08_ROM_START;
extern Addr flo_08_ROM_END;
extern Addr flo_08_VRAM;
extern Addr flo_09_ROM_START;
extern Addr flo_09_ROM_END;
extern Addr flo_09_VRAM;
extern Addr flo_10_ROM_START;
extern Addr flo_10_ROM_END;
extern Addr flo_10_VRAM;
extern Addr flo_11_ROM_START;
extern Addr flo_11_ROM_END;
extern Addr flo_11_VRAM;
extern Addr flo_12_ROM_START;
extern Addr flo_12_ROM_END;
extern Addr flo_12_VRAM;
extern Addr flo_13_ROM_START;
extern Addr flo_13_ROM_END;
extern Addr flo_13_VRAM;
extern Addr flo_14_ROM_START;
extern Addr flo_14_ROM_END;
extern Addr flo_14_VRAM;
extern Addr flo_15_ROM_START;
extern Addr flo_15_ROM_END;
extern Addr flo_15_VRAM;
extern Addr flo_16_ROM_START;
extern Addr flo_16_ROM_END;
extern Addr flo_16_VRAM;
extern Addr flo_17_ROM_START;
extern Addr flo_17_ROM_END;
extern Addr flo_17_VRAM;
extern Addr flo_18_ROM_START;
extern Addr flo_18_ROM_END;
extern Addr flo_18_VRAM;
extern Addr flo_19_ROM_START;
extern Addr flo_19_ROM_END;
extern Addr flo_19_VRAM;
extern Addr flo_21_ROM_START;
extern Addr flo_21_ROM_END;
extern Addr flo_21_VRAM;
extern Addr flo_22_ROM_START;
extern Addr flo_22_ROM_END;
extern Addr flo_22_VRAM;
extern Addr flo_23_ROM_START;
extern Addr flo_23_ROM_END;
extern Addr flo_23_VRAM;
extern Addr flo_24_ROM_START;
extern Addr flo_24_ROM_END;
extern Addr flo_24_VRAM;
extern Addr flo_25_ROM_START;
extern Addr flo_25_ROM_END;
extern Addr flo_25_VRAM;
extern Addr gv_01_ROM_START;
extern Addr gv_01_ROM_END;
extern Addr gv_01_VRAM;
extern Addr hos_00_ROM_START;
extern Addr hos_00_ROM_END;
extern Addr hos_00_VRAM;
extern Addr hos_01_ROM_START;
extern Addr hos_01_ROM_END;
extern Addr hos_01_VRAM;
extern Addr hos_02_ROM_START;
extern Addr hos_02_ROM_END;
extern Addr hos_02_VRAM;
extern Addr hos_03_ROM_START;
extern Addr hos_03_ROM_END;
extern Addr hos_03_VRAM;
extern Addr hos_04_ROM_START;
extern Addr hos_04_ROM_END;
extern Addr hos_04_VRAM;
extern Addr hos_05_ROM_START;
extern Addr hos_05_ROM_END;
extern Addr hos_05_VRAM;
extern Addr hos_06_ROM_START;
extern Addr hos_06_ROM_END;
extern Addr hos_06_VRAM;
extern Addr hos_10_ROM_START;
extern Addr hos_10_ROM_END;
extern Addr hos_10_VRAM;
extern Addr hos_20_ROM_START;
extern Addr hos_20_ROM_END;
extern Addr hos_20_VRAM;
extern Addr isk_01_ROM_START;
extern Addr isk_01_ROM_END;
extern Addr isk_01_VRAM;
extern Addr isk_02_ROM_START;
extern Addr isk_02_ROM_END;
extern Addr isk_02_VRAM;
extern Addr isk_03_ROM_START;
extern Addr isk_03_ROM_END;
extern Addr isk_03_VRAM;
extern Addr isk_04_ROM_START;
extern Addr isk_04_ROM_END;
extern Addr isk_04_VRAM;
extern Addr isk_05_ROM_START;
extern Addr isk_05_ROM_END;
extern Addr isk_05_VRAM;
extern Addr isk_06_ROM_START;
extern Addr isk_06_ROM_END;
extern Addr isk_06_VRAM;
extern Addr isk_07_ROM_START;
extern Addr isk_07_ROM_END;
extern Addr isk_07_VRAM;
extern Addr isk_08_ROM_START;
extern Addr isk_08_ROM_END;
extern Addr isk_08_VRAM;
extern Addr isk_09_ROM_START;
extern Addr isk_09_ROM_END;
extern Addr isk_09_VRAM;
extern Addr isk_10_ROM_START;
extern Addr isk_10_ROM_END;
extern Addr isk_10_VRAM;
extern Addr isk_11_ROM_START;
extern Addr isk_11_ROM_END;
extern Addr isk_11_VRAM;
extern Addr isk_12_ROM_START;
extern Addr isk_12_ROM_END;
extern Addr isk_12_VRAM;
extern Addr isk_13_ROM_START;
extern Addr isk_13_ROM_END;
extern Addr isk_13_VRAM;
extern Addr isk_14_ROM_START;
extern Addr isk_14_ROM_END;
extern Addr isk_14_VRAM;
extern Addr isk_16_ROM_START;
extern Addr isk_16_ROM_END;
extern Addr isk_16_VRAM;
extern Addr isk_18_ROM_START;
extern Addr isk_18_ROM_END;
extern Addr isk_18_VRAM;
extern Addr isk_19_ROM_START;
extern Addr isk_19_ROM_END;
extern Addr isk_19_VRAM;
extern Addr iwa_00_ROM_START;
extern Addr iwa_00_ROM_END;
extern Addr iwa_00_VRAM;
extern Addr iwa_01_ROM_START;
extern Addr iwa_01_ROM_END;
extern Addr iwa_01_VRAM;
extern Addr iwa_02_ROM_START;
extern Addr iwa_02_ROM_END;
extern Addr iwa_02_VRAM;
extern Addr iwa_03_ROM_START;
extern Addr iwa_03_ROM_END;
extern Addr iwa_03_VRAM;
extern Addr iwa_04_ROM_START;
extern Addr iwa_04_ROM_END;
extern Addr iwa_04_VRAM;
extern Addr iwa_10_ROM_START;
extern Addr iwa_10_ROM_END;
extern Addr iwa_10_VRAM;
extern Addr iwa_11_ROM_START;
extern Addr iwa_11_ROM_END;
extern Addr iwa_11_VRAM;
extern Addr jan_00_ROM_START;
extern Addr jan_00_ROM_END;
extern Addr jan_00_VRAM;
extern Addr jan_01_ROM_START;
extern Addr jan_01_ROM_END;
extern Addr jan_01_VRAM;
extern Addr jan_02_ROM_START;
extern Addr jan_02_ROM_END;
extern Addr jan_02_VRAM;
extern Addr jan_03_ROM_START;
extern Addr jan_03_ROM_END;
extern Addr jan_03_VRAM;
extern Addr jan_04_ROM_START;
extern Addr jan_04_ROM_END;
extern Addr jan_04_VRAM;
extern Addr jan_05_ROM_START;
extern Addr jan_05_ROM_END;
extern Addr jan_05_VRAM;
extern Addr jan_06_ROM_START;
extern Addr jan_06_ROM_END;
extern Addr jan_06_VRAM;
extern Addr jan_07_ROM_START;
extern Addr jan_07_ROM_END;
extern Addr jan_07_VRAM;
extern Addr jan_08_ROM_START;
extern Addr jan_08_ROM_END;
extern Addr jan_08_VRAM;
extern Addr jan_09_ROM_START;
extern Addr jan_09_ROM_END;
extern Addr jan_09_VRAM;
extern Addr jan_10_ROM_START;
extern Addr jan_10_ROM_END;
extern Addr jan_10_VRAM;
extern Addr jan_11_ROM_START;
extern Addr jan_11_ROM_END;
extern Addr jan_11_VRAM;
extern Addr jan_12_ROM_START;
extern Addr jan_12_ROM_END;
extern Addr jan_12_VRAM;
extern Addr jan_13_ROM_START;
extern Addr jan_13_ROM_END;
extern Addr jan_13_VRAM;
extern Addr jan_14_ROM_START;
extern Addr jan_14_ROM_END;
extern Addr jan_14_VRAM;
extern Addr jan_15_ROM_START;
extern Addr jan_15_ROM_END;
extern Addr jan_15_VRAM;
extern Addr jan_16_ROM_START;
extern Addr jan_16_ROM_END;
extern Addr jan_16_VRAM;
extern Addr jan_17_ROM_START;
extern Addr jan_17_ROM_END;
extern Addr jan_17_VRAM;
extern Addr jan_18_ROM_START;
extern Addr jan_18_ROM_END;
extern Addr jan_18_VRAM;
extern Addr jan_19_ROM_START;
extern Addr jan_19_ROM_END;
extern Addr jan_19_VRAM;
extern Addr jan_22_ROM_START;
extern Addr jan_22_ROM_END;
extern Addr jan_22_VRAM;
extern Addr jan_23_ROM_START;
extern Addr jan_23_ROM_END;
extern Addr jan_23_VRAM;
extern Addr kgr_01_ROM_START;
extern Addr kgr_01_ROM_END;
extern Addr kgr_01_VRAM;
extern Addr kgr_02_ROM_START;
extern Addr kgr_02_ROM_END;
extern Addr kgr_02_VRAM;
extern Addr kkj_00_ROM_START;
extern Addr kkj_00_ROM_END;
extern Addr kkj_00_VRAM;
extern Addr kkj_01_ROM_START;
extern Addr kkj_01_ROM_END;
extern Addr kkj_01_VRAM;
extern Addr kkj_02_ROM_START;
extern Addr kkj_02_ROM_END;
extern Addr kkj_02_VRAM;
extern Addr kkj_03_ROM_START;
extern Addr kkj_03_ROM_END;
extern Addr kkj_03_VRAM;
extern Addr kkj_10_ROM_START;
extern Addr kkj_10_ROM_END;
extern Addr kkj_10_VRAM;
extern Addr kkj_11_ROM_START;
extern Addr kkj_11_ROM_END;
extern Addr kkj_11_VRAM;
extern Addr kkj_12_ROM_START;
extern Addr kkj_12_ROM_END;
extern Addr kkj_12_VRAM;
extern Addr kkj_13_ROM_START;
extern Addr kkj_13_ROM_END;
extern Addr kkj_13_VRAM;
extern Addr kkj_14_ROM_START;
extern Addr kkj_14_ROM_END;
extern Addr kkj_14_VRAM;
extern Addr kkj_15_ROM_START;
extern Addr kkj_15_ROM_END;
extern Addr kkj_15_VRAM;
extern Addr kkj_16_ROM_START;
extern Addr kkj_16_ROM_END;
extern Addr kkj_16_VRAM;
extern Addr kkj_17_ROM_START;
extern Addr kkj_17_ROM_END;
extern Addr kkj_17_VRAM;
extern Addr kkj_18_ROM_START;
extern Addr kkj_18_ROM_END;
extern Addr kkj_18_VRAM;
extern Addr kkj_19_ROM_START;
extern Addr kkj_19_ROM_END;
extern Addr kkj_19_VRAM;
extern Addr kkj_20_ROM_START;
extern Addr kkj_20_ROM_END;
extern Addr kkj_20_VRAM;
extern Addr kkj_21_ROM_START;
extern Addr kkj_21_ROM_END;
extern Addr kkj_21_VRAM;
extern Addr kkj_22_ROM_START;
extern Addr kkj_22_ROM_END;
extern Addr kkj_22_VRAM;
extern Addr kkj_23_ROM_START;
extern Addr kkj_23_ROM_END;
extern Addr kkj_23_VRAM;
extern Addr kkj_24_ROM_START;
extern Addr kkj_24_ROM_END;
extern Addr kkj_24_VRAM;
extern Addr kkj_25_ROM_START;
extern Addr kkj_25_ROM_END;
extern Addr kkj_25_VRAM;
extern Addr kkj_26_ROM_START;
extern Addr kkj_26_ROM_END;
extern Addr kkj_26_VRAM;
extern Addr kkj_27_ROM_START;
extern Addr kkj_27_ROM_END;
extern Addr kkj_27_VRAM;
extern Addr kkj_28_ROM_START;
extern Addr kkj_28_ROM_END;
extern Addr kkj_28_VRAM;
extern Addr kkj_29_ROM_START;
extern Addr kkj_29_ROM_END;
extern Addr kkj_29_VRAM;
extern Addr kmr_00_ROM_START;
extern Addr kmr_00_ROM_END;
extern Addr kmr_00_VRAM;
extern Addr kmr_02_ROM_START;
extern Addr kmr_02_ROM_END;
extern Addr kmr_02_VRAM;
extern Addr kmr_03_ROM_START;
extern Addr kmr_03_ROM_END;
extern Addr kmr_03_VRAM;
extern Addr kmr_04_ROM_START;
extern Addr kmr_04_ROM_END;
extern Addr kmr_04_VRAM;
extern Addr kmr_05_ROM_START;
extern Addr kmr_05_ROM_END;
extern Addr kmr_05_VRAM;
extern Addr kmr_06_ROM_START;
extern Addr kmr_06_ROM_END;
extern Addr kmr_06_VRAM;
extern Addr kmr_07_ROM_START;
extern Addr kmr_07_ROM_END;
extern Addr kmr_07_VRAM;
extern Addr kmr_09_ROM_START;
extern Addr kmr_09_ROM_END;
extern Addr kmr_09_VRAM;
extern Addr kmr_10_ROM_START;
extern Addr kmr_10_ROM_END;
extern Addr kmr_10_VRAM;
extern Addr kmr_11_ROM_START;
extern Addr kmr_11_ROM_END;
extern Addr kmr_11_VRAM;
extern Addr kmr_12_ROM_START;
extern Addr kmr_12_ROM_END;
extern Addr kmr_12_VRAM;
extern Addr kmr_20_ROM_START;
extern Addr kmr_20_ROM_END;
extern Addr kmr_20_VRAM;
extern Addr kmr_21_ROM_START;
extern Addr kmr_21_ROM_END;
extern Addr kmr_21_VRAM;
extern Addr kmr_22_ROM_START;
extern Addr kmr_22_ROM_END;
extern Addr kmr_22_VRAM;
extern Addr kmr_23_ROM_START;
extern Addr kmr_23_ROM_END;
extern Addr kmr_23_VRAM;
extern Addr kmr_24_ROM_START;
extern Addr kmr_24_ROM_END;
extern Addr kmr_24_VRAM;
extern Addr kmr_30_ROM_START;
extern Addr kmr_30_ROM_END;
extern Addr kmr_30_VRAM;
extern Addr kpa_01_ROM_START;
extern Addr kpa_01_ROM_END;
extern Addr kpa_01_VRAM;
extern Addr kpa_03_ROM_START;
extern Addr kpa_03_ROM_END;
extern Addr kpa_03_VRAM;
extern Addr kpa_04_ROM_START;
extern Addr kpa_04_ROM_END;
extern Addr kpa_04_VRAM;
extern Addr kpa_08_ROM_START;
extern Addr kpa_08_ROM_END;
extern Addr kpa_08_VRAM;
extern Addr kpa_09_ROM_START;
extern Addr kpa_09_ROM_END;
extern Addr kpa_09_VRAM;
extern Addr kpa_10_ROM_START;
extern Addr kpa_10_ROM_END;
extern Addr kpa_10_VRAM;
extern Addr kpa_100_ROM_START;
extern Addr kpa_100_ROM_END;
extern Addr kpa_100_VRAM;
extern Addr kpa_101_ROM_START;
extern Addr kpa_101_ROM_END;
extern Addr kpa_101_VRAM;
extern Addr kpa_102_ROM_START;
extern Addr kpa_102_ROM_END;
extern Addr kpa_102_VRAM;
extern Addr kpa_11_ROM_START;
extern Addr kpa_11_ROM_END;
extern Addr kpa_11_VRAM;
extern Addr kpa_111_ROM_START;
extern Addr kpa_111_ROM_END;
extern Addr kpa_111_VRAM;
extern Addr kpa_112_ROM_START;
extern Addr kpa_112_ROM_END;
extern Addr kpa_112_VRAM;
extern Addr kpa_113_ROM_START;
extern Addr kpa_113_ROM_END;
extern Addr kpa_113_VRAM;
extern Addr kpa_114_ROM_START;
extern Addr kpa_114_ROM_END;
extern Addr kpa_114_VRAM;
extern Addr kpa_115_ROM_START;
extern Addr kpa_115_ROM_END;
extern Addr kpa_115_VRAM;
extern Addr kpa_116_ROM_START;
extern Addr kpa_116_ROM_END;
extern Addr kpa_116_VRAM;
extern Addr kpa_117_ROM_START;
extern Addr kpa_117_ROM_END;
extern Addr kpa_117_VRAM;
extern Addr kpa_118_ROM_START;
extern Addr kpa_118_ROM_END;
extern Addr kpa_118_VRAM;
extern Addr kpa_119_ROM_START;
extern Addr kpa_119_ROM_END;
extern Addr kpa_119_VRAM;
extern Addr kpa_12_ROM_START;
extern Addr kpa_12_ROM_END;
extern Addr kpa_12_VRAM;
extern Addr kpa_121_ROM_START;
extern Addr kpa_121_ROM_END;
extern Addr kpa_121_VRAM;
extern Addr kpa_13_ROM_START;
extern Addr kpa_13_ROM_END;
extern Addr kpa_13_VRAM;
extern Addr kpa_130_ROM_START;
extern Addr kpa_130_ROM_END;
extern Addr kpa_130_VRAM;
extern Addr kpa_133_ROM_START;
extern Addr kpa_133_ROM_END;
extern Addr kpa_133_VRAM;
extern Addr kpa_134_ROM_START;
extern Addr kpa_134_ROM_END;
extern Addr kpa_134_VRAM;
extern Addr kpa_14_ROM_START;
extern Addr kpa_14_ROM_END;
extern Addr kpa_14_VRAM;
extern Addr kpa_15_ROM_START;
extern Addr kpa_15_ROM_END;
extern Addr kpa_15_VRAM;
extern Addr kpa_16_ROM_START;
extern Addr kpa_16_ROM_END;
extern Addr kpa_16_VRAM;
extern Addr kpa_17_ROM_START;
extern Addr kpa_17_ROM_END;
extern Addr kpa_17_VRAM;
extern Addr kpa_32_ROM_START;
extern Addr kpa_32_ROM_END;
extern Addr kpa_32_VRAM;
extern Addr kpa_33_ROM_START;
extern Addr kpa_33_ROM_END;
extern Addr kpa_33_VRAM;
extern Addr kpa_40_ROM_START;
extern Addr kpa_40_ROM_END;
extern Addr kpa_40_VRAM;
extern Addr kpa_41_ROM_START;
extern Addr kpa_41_ROM_END;
extern Addr kpa_41_VRAM;
extern Addr kpa_50_ROM_START;
extern Addr kpa_50_ROM_END;
extern Addr kpa_50_VRAM;
extern Addr kpa_51_ROM_START;
extern Addr kpa_51_ROM_END;
extern Addr kpa_51_VRAM;
extern Addr kpa_52_ROM_START;
extern Addr kpa_52_ROM_END;
extern Addr kpa_52_VRAM;
extern Addr kpa_53_ROM_START;
extern Addr kpa_53_ROM_END;
extern Addr kpa_53_VRAM;
extern Addr kpa_60_ROM_START;
extern Addr kpa_60_ROM_END;
extern Addr kpa_60_VRAM;
extern Addr kpa_61_ROM_START;
extern Addr kpa_61_ROM_END;
extern Addr kpa_61_VRAM;
extern Addr kpa_62_ROM_START;
extern Addr kpa_62_ROM_END;
extern Addr kpa_62_VRAM;
extern Addr kpa_63_ROM_START;
extern Addr kpa_63_ROM_END;
extern Addr kpa_63_VRAM;
extern Addr kpa_70_ROM_START;
extern Addr kpa_70_ROM_END;
extern Addr kpa_70_VRAM;
extern Addr kpa_81_ROM_START;
extern Addr kpa_81_ROM_END;
extern Addr kpa_81_VRAM;
extern Addr kpa_82_ROM_START;
extern Addr kpa_82_ROM_END;
extern Addr kpa_82_VRAM;
extern Addr kpa_83_ROM_START;
extern Addr kpa_83_ROM_END;
extern Addr kpa_83_VRAM;
extern Addr kpa_90_ROM_START;
extern Addr kpa_90_ROM_END;
extern Addr kpa_90_VRAM;
extern Addr kpa_91_ROM_START;
extern Addr kpa_91_ROM_END;
extern Addr kpa_91_VRAM;
extern Addr kpa_94_ROM_START;
extern Addr kpa_94_ROM_END;
extern Addr kpa_94_VRAM;
extern Addr kpa_95_ROM_START;
extern Addr kpa_95_ROM_END;
extern Addr kpa_95_VRAM;
extern Addr kpa_96_ROM_START;
extern Addr kpa_96_ROM_END;
extern Addr kpa_96_VRAM;
extern Addr kzn_01_ROM_START;
extern Addr kzn_01_ROM_END;
extern Addr kzn_01_VRAM;
extern Addr kzn_02_ROM_START;
extern Addr kzn_02_ROM_END;
extern Addr kzn_02_VRAM;
extern Addr kzn_03_ROM_START;
extern Addr kzn_03_ROM_END;
extern Addr kzn_03_VRAM;
extern Addr kzn_04_ROM_START;
extern Addr kzn_04_ROM_END;
extern Addr kzn_04_VRAM;
extern Addr kzn_05_ROM_START;
extern Addr kzn_05_ROM_END;
extern Addr kzn_05_VRAM;
extern Addr kzn_06_ROM_START;
extern Addr kzn_06_ROM_END;
extern Addr kzn_06_VRAM;
extern Addr kzn_07_ROM_START;
extern Addr kzn_07_ROM_END;
extern Addr kzn_07_VRAM;
extern Addr kzn_08_ROM_START;
extern Addr kzn_08_ROM_END;
extern Addr kzn_08_VRAM;
extern Addr kzn_09_ROM_START;
extern Addr kzn_09_ROM_END;
extern Addr kzn_09_VRAM;
extern Addr kzn_10_ROM_START;
extern Addr kzn_10_ROM_END;
extern Addr kzn_10_VRAM;
extern Addr kzn_11_ROM_START;
extern Addr kzn_11_ROM_END;
extern Addr kzn_11_VRAM;
extern Addr kzn_17_ROM_START;
extern Addr kzn_17_ROM_END;
extern Addr kzn_17_VRAM;
extern Addr kzn_18_ROM_START;
extern Addr kzn_18_ROM_END;
extern Addr kzn_18_VRAM;
extern Addr kzn_19_ROM_START;
extern Addr kzn_19_ROM_END;
extern Addr kzn_19_VRAM;
extern Addr kzn_20_ROM_START;
extern Addr kzn_20_ROM_END;
extern Addr kzn_20_VRAM;
extern Addr kzn_22_ROM_START;
extern Addr kzn_22_ROM_END;
extern Addr kzn_22_VRAM;
extern Addr kzn_23_ROM_START;
extern Addr kzn_23_ROM_END;
extern Addr kzn_23_VRAM;
extern Addr mac_00_ROM_START;
extern Addr mac_00_ROM_END;
extern Addr mac_00_VRAM;
extern Addr mac_01_ROM_START;
extern Addr mac_01_ROM_END;
extern Addr mac_01_VRAM;
extern Addr mac_02_ROM_START;
extern Addr mac_02_ROM_END;
extern Addr mac_02_VRAM;
extern Addr mac_03_ROM_START;
extern Addr mac_03_ROM_END;
extern Addr mac_03_VRAM;
extern Addr mac_04_ROM_START;
extern Addr mac_04_ROM_END;
extern Addr mac_04_VRAM;
extern Addr mac_05_ROM_START;
extern Addr mac_05_ROM_END;
extern Addr mac_05_VRAM;
extern Addr mac_06_ROM_START;
extern Addr mac_06_ROM_END;
extern Addr mac_06_VRAM;
extern Addr machi_ROM_START;
extern Addr machi_ROM_END;
extern Addr machi_VRAM;
extern Addr mgm_00_ROM_START;
extern Addr mgm_00_ROM_END;
extern Addr mgm_00_VRAM;
extern Addr mgm_01_ROM_START;
extern Addr mgm_01_ROM_END;
extern Addr mgm_01_VRAM;
extern Addr mgm_02_ROM_START;
extern Addr mgm_02_ROM_END;
extern Addr mgm_02_VRAM;
extern Addr mgm_03_ROM_START;
extern Addr mgm_03_ROM_END;
extern Addr mgm_03_VRAM;
extern Addr mim_01_ROM_START;
extern Addr mim_01_ROM_END;
extern Addr mim_01_VRAM;
extern Addr mim_02_ROM_START;
extern Addr mim_02_ROM_END;
extern Addr mim_02_VRAM;
extern Addr mim_03_ROM_START;
extern Addr mim_03_ROM_END;
extern Addr mim_03_VRAM;
extern Addr mim_04_ROM_START;
extern Addr mim_04_ROM_END;
extern Addr mim_04_VRAM;
extern Addr mim_05_ROM_START;
extern Addr mim_05_ROM_END;
extern Addr mim_05_VRAM;
extern Addr mim_06_ROM_START;
extern Addr mim_06_ROM_END;
extern Addr mim_06_VRAM;
extern Addr mim_07_ROM_START;
extern Addr mim_07_ROM_END;
extern Addr mim_07_VRAM;
extern Addr mim_08_ROM_START;
extern Addr mim_08_ROM_END;
extern Addr mim_08_VRAM;
extern Addr mim_09_ROM_START;
extern Addr mim_09_ROM_END;
extern Addr mim_09_VRAM;
extern Addr mim_10_ROM_START;
extern Addr mim_10_ROM_END;
extern Addr mim_10_VRAM;
extern Addr mim_11_ROM_START;
extern Addr mim_11_ROM_END;
extern Addr mim_11_VRAM;
extern Addr mim_12_ROM_START;
extern Addr mim_12_ROM_END;
extern Addr mim_12_VRAM;
extern Addr nok_01_ROM_START;
extern Addr nok_01_ROM_END;
extern Addr nok_01_VRAM;
extern Addr nok_02_ROM_START;
extern Addr nok_02_ROM_END;
extern Addr nok_02_VRAM;
extern Addr nok_03_ROM_START;
extern Addr nok_03_ROM_END;
extern Addr nok_03_VRAM;
extern Addr nok_04_ROM_START;
extern Addr nok_04_ROM_END;
extern Addr nok_04_VRAM;
extern Addr nok_11_ROM_START;
extern Addr nok_11_ROM_END;
extern Addr nok_11_VRAM;
extern Addr nok_12_ROM_START;
extern Addr nok_12_ROM_END;
extern Addr nok_12_VRAM;
extern Addr nok_13_ROM_START;
extern Addr nok_13_ROM_END;
extern Addr nok_13_VRAM;
extern Addr nok_14_ROM_START;
extern Addr nok_14_ROM_END;
extern Addr nok_14_VRAM;
extern Addr nok_15_ROM_START;
extern Addr nok_15_ROM_END;
extern Addr nok_15_VRAM;
extern Addr obk_01_ROM_START;
extern Addr obk_01_ROM_END;
extern Addr obk_01_VRAM;
extern Addr obk_02_ROM_START;
extern Addr obk_02_ROM_END;
extern Addr obk_02_VRAM;
extern Addr obk_03_ROM_START;
extern Addr obk_03_ROM_END;
extern Addr obk_03_VRAM;
extern Addr obk_04_ROM_START;
extern Addr obk_04_ROM_END;
extern Addr obk_04_VRAM;
extern Addr obk_05_ROM_START;
extern Addr obk_05_ROM_END;
extern Addr obk_05_VRAM;
extern Addr obk_06_ROM_START;
extern Addr obk_06_ROM_END;
extern Addr obk_06_VRAM;
extern Addr obk_07_ROM_START;
extern Addr obk_07_ROM_END;
extern Addr obk_07_VRAM;
extern Addr obk_08_ROM_START;
extern Addr obk_08_ROM_END;
extern Addr obk_08_VRAM;
extern Addr obk_09_ROM_START;
extern Addr obk_09_ROM_END;
extern Addr obk_09_VRAM;
extern Addr omo_01_ROM_START;
extern Addr omo_01_ROM_END;
extern Addr omo_01_VRAM;
extern Addr omo_02_ROM_START;
extern Addr omo_02_ROM_END;
extern Addr omo_02_VRAM;
extern Addr omo_03_ROM_START;
extern Addr omo_03_ROM_END;
extern Addr omo_03_VRAM;
extern Addr omo_04_ROM_START;
extern Addr omo_04_ROM_END;
extern Addr omo_04_VRAM;
extern Addr omo_05_ROM_START;
extern Addr omo_05_ROM_END;
extern Addr omo_05_VRAM;
extern Addr omo_06_ROM_START;
extern Addr omo_06_ROM_END;
extern Addr omo_06_VRAM;
extern Addr omo_07_ROM_START;
extern Addr omo_07_ROM_END;
extern Addr omo_07_VRAM;
extern Addr omo_08_ROM_START;
extern Addr omo_08_ROM_END;
extern Addr omo_08_VRAM;
extern Addr omo_09_ROM_START;
extern Addr omo_09_ROM_END;
extern Addr omo_09_VRAM;
extern Addr omo_10_ROM_START;
extern Addr omo_10_ROM_END;
extern Addr omo_10_VRAM;
extern Addr omo_11_ROM_START;
extern Addr omo_11_ROM_END;
extern Addr omo_11_VRAM;
extern Addr omo_12_ROM_START;
extern Addr omo_12_ROM_END;
extern Addr omo_12_VRAM;
extern Addr omo_13_ROM_START;
extern Addr omo_13_ROM_END;
extern Addr omo_13_VRAM;
extern Addr omo_14_ROM_START;
extern Addr omo_14_ROM_END;
extern Addr omo_14_VRAM;
extern Addr omo_15_ROM_START;
extern Addr omo_15_ROM_END;
extern Addr omo_15_VRAM;
extern Addr omo_16_ROM_START;
extern Addr omo_16_ROM_END;
extern Addr omo_16_VRAM;
extern Addr omo_17_ROM_START;
extern Addr omo_17_ROM_END;
extern Addr omo_17_VRAM;
extern Addr osr_00_ROM_START;
extern Addr osr_00_ROM_END;
extern Addr osr_00_VRAM;
extern Addr osr_01_ROM_START;
extern Addr osr_01_ROM_END;
extern Addr osr_01_VRAM;
extern Addr osr_02_ROM_START;
extern Addr osr_02_ROM_END;
extern Addr osr_02_VRAM;
extern Addr osr_03_ROM_START;
extern Addr osr_03_ROM_END;
extern Addr osr_03_VRAM;
extern Addr osr_04_ROM_START;
extern Addr osr_04_ROM_END;
extern Addr osr_04_VRAM;
extern Addr pra_01_ROM_START;
extern Addr pra_01_ROM_END;
extern Addr pra_01_VRAM;
extern Addr pra_02_ROM_START;
extern Addr pra_02_ROM_END;
extern Addr pra_02_VRAM;
extern Addr pra_03_ROM_START;
extern Addr pra_03_ROM_END;
extern Addr pra_03_VRAM;
extern Addr pra_04_ROM_START;
extern Addr pra_04_ROM_END;
extern Addr pra_04_VRAM;
extern Addr pra_05_ROM_START;
extern Addr pra_05_ROM_END;
extern Addr pra_05_VRAM;
extern Addr pra_06_ROM_START;
extern Addr pra_06_ROM_END;
extern Addr pra_06_VRAM;
extern Addr pra_09_ROM_START;
extern Addr pra_09_ROM_END;
extern Addr pra_09_VRAM;
extern Addr pra_10_ROM_START;
extern Addr pra_10_ROM_END;
extern Addr pra_10_VRAM;
extern Addr pra_11_ROM_START;
extern Addr pra_11_ROM_END;
extern Addr pra_11_VRAM;
extern Addr pra_12_ROM_START;
extern Addr pra_12_ROM_END;
extern Addr pra_12_VRAM;
extern Addr pra_13_ROM_START;
extern Addr pra_13_ROM_END;
extern Addr pra_13_VRAM;
extern Addr pra_14_ROM_START;
extern Addr pra_14_ROM_END;
extern Addr pra_14_VRAM;
extern Addr pra_15_ROM_START;
extern Addr pra_15_ROM_END;
extern Addr pra_15_VRAM;
extern Addr pra_16_ROM_START;
extern Addr pra_16_ROM_END;
extern Addr pra_16_VRAM;
extern Addr pra_18_ROM_START;
extern Addr pra_18_ROM_END;
extern Addr pra_18_VRAM;
extern Addr pra_19_ROM_START;
extern Addr pra_19_ROM_END;
extern Addr pra_19_VRAM;
extern Addr pra_20_ROM_START;
extern Addr pra_20_ROM_END;
extern Addr pra_20_VRAM;
extern Addr pra_21_ROM_START;
extern Addr pra_21_ROM_END;
extern Addr pra_21_VRAM;
extern Addr pra_22_ROM_START;
extern Addr pra_22_ROM_END;
extern Addr pra_22_VRAM;
extern Addr pra_27_ROM_START;
extern Addr pra_27_ROM_END;
extern Addr pra_27_VRAM;
extern Addr pra_28_ROM_START;
extern Addr pra_28_ROM_END;
extern Addr pra_28_VRAM;
extern Addr pra_29_ROM_START;
extern Addr pra_29_ROM_END;
extern Addr pra_29_VRAM;
extern Addr pra_31_ROM_START;
extern Addr pra_31_ROM_END;
extern Addr pra_31_VRAM;
extern Addr pra_32_ROM_START;
extern Addr pra_32_ROM_END;
extern Addr pra_32_VRAM;
extern Addr pra_33_ROM_START;
extern Addr pra_33_ROM_END;
extern Addr pra_33_VRAM;
extern Addr pra_34_ROM_START;
extern Addr pra_34_ROM_END;
extern Addr pra_34_VRAM;
extern Addr pra_35_ROM_START;
extern Addr pra_35_ROM_END;
extern Addr pra_35_VRAM;
extern Addr pra_36_ROM_START;
extern Addr pra_36_ROM_END;
extern Addr pra_36_VRAM;
extern Addr pra_37_ROM_START;
extern Addr pra_37_ROM_END;
extern Addr pra_37_VRAM;
extern Addr pra_38_ROM_START;
extern Addr pra_38_ROM_END;
extern Addr pra_38_VRAM;
extern Addr pra_39_ROM_START;
extern Addr pra_39_ROM_END;
extern Addr pra_39_VRAM;
extern Addr pra_40_ROM_START;
extern Addr pra_40_ROM_END;
extern Addr pra_40_VRAM;
extern Addr sam_01_ROM_START;
extern Addr sam_01_ROM_END;
extern Addr sam_01_VRAM;
extern Addr sam_02_ROM_START;
extern Addr sam_02_ROM_END;
extern Addr sam_02_VRAM;
extern Addr sam_03_ROM_START;
extern Addr sam_03_ROM_END;
extern Addr sam_03_VRAM;
extern Addr sam_04_ROM_START;
extern Addr sam_04_ROM_END;
extern Addr sam_04_VRAM;
extern Addr sam_05_ROM_START;
extern Addr sam_05_ROM_END;
extern Addr sam_05_VRAM;
extern Addr sam_06_ROM_START;
extern Addr sam_06_ROM_END;
extern Addr sam_06_VRAM;
extern Addr sam_07_ROM_START;
extern Addr sam_07_ROM_END;
extern Addr sam_07_VRAM;
extern Addr sam_08_ROM_START;
extern Addr sam_08_ROM_END;
extern Addr sam_08_VRAM;
extern Addr sam_09_ROM_START;
extern Addr sam_09_ROM_END;
extern Addr sam_09_VRAM;
extern Addr sam_10_ROM_START;
extern Addr sam_10_ROM_END;
extern Addr sam_10_VRAM;
extern Addr sam_11_ROM_START;
extern Addr sam_11_ROM_END;
extern Addr sam_11_VRAM;
extern Addr sam_12_ROM_START;
extern Addr sam_12_ROM_END;
extern Addr sam_12_VRAM;
extern Addr sbk_00_ROM_START;
extern Addr sbk_00_ROM_END;
extern Addr sbk_00_VRAM;
extern Addr sbk_01_ROM_START;
extern Addr sbk_01_ROM_END;
extern Addr sbk_01_VRAM;
extern Addr sbk_02_ROM_START;
extern Addr sbk_02_ROM_END;
extern Addr sbk_02_VRAM;
extern Addr sbk_03_ROM_START;
extern Addr sbk_03_ROM_END;
extern Addr sbk_03_VRAM;
extern Addr sbk_04_ROM_START;
extern Addr sbk_04_ROM_END;
extern Addr sbk_04_VRAM;
extern Addr sbk_05_ROM_START;
extern Addr sbk_05_ROM_END;
extern Addr sbk_05_VRAM;
extern Addr sbk_06_ROM_START;
extern Addr sbk_06_ROM_END;
extern Addr sbk_06_VRAM;
extern Addr sbk_10_ROM_START;
extern Addr sbk_10_ROM_END;
extern Addr sbk_10_VRAM;
extern Addr sbk_11_ROM_START;
extern Addr sbk_11_ROM_END;
extern Addr sbk_11_VRAM;
extern Addr sbk_12_ROM_START;
extern Addr sbk_12_ROM_END;
extern Addr sbk_12_VRAM;
extern Addr sbk_13_ROM_START;
extern Addr sbk_13_ROM_END;
extern Addr sbk_13_VRAM;
extern Addr sbk_14_ROM_START;
extern Addr sbk_14_ROM_END;
extern Addr sbk_14_VRAM;
extern Addr sbk_15_ROM_START;
extern Addr sbk_15_ROM_END;
extern Addr sbk_15_VRAM;
extern Addr sbk_16_ROM_START;
extern Addr sbk_16_ROM_END;
extern Addr sbk_16_VRAM;
extern Addr sbk_20_ROM_START;
extern Addr sbk_20_ROM_END;
extern Addr sbk_20_VRAM;
extern Addr sbk_21_ROM_START;
extern Addr sbk_21_ROM_END;
extern Addr sbk_21_VRAM;
extern Addr sbk_22_ROM_START;
extern Addr sbk_22_ROM_END;
extern Addr sbk_22_VRAM;
extern Addr sbk_23_ROM_START;
extern Addr sbk_23_ROM_END;
extern Addr sbk_23_VRAM;
extern Addr sbk_24_ROM_START;
extern Addr sbk_24_ROM_END;
extern Addr sbk_24_VRAM;
extern Addr sbk_25_ROM_START;
extern Addr sbk_25_ROM_END;
extern Addr sbk_25_VRAM;
extern Addr sbk_26_ROM_START;
extern Addr sbk_26_ROM_END;
extern Addr sbk_26_VRAM;
extern Addr sbk_30_ROM_START;
extern Addr sbk_30_ROM_END;
extern Addr sbk_30_VRAM;
extern Addr sbk_31_ROM_START;
extern Addr sbk_31_ROM_END;
extern Addr sbk_31_VRAM;
extern Addr sbk_32_ROM_START;
extern Addr sbk_32_ROM_END;
extern Addr sbk_32_VRAM;
extern Addr sbk_33_ROM_START;
extern Addr sbk_33_ROM_END;
extern Addr sbk_33_VRAM;
extern Addr sbk_34_ROM_START;
extern Addr sbk_34_ROM_END;
extern Addr sbk_34_VRAM;
extern Addr sbk_35_ROM_START;
extern Addr sbk_35_ROM_END;
extern Addr sbk_35_VRAM;
extern Addr sbk_36_ROM_START;
extern Addr sbk_36_ROM_END;
extern Addr sbk_36_VRAM;
extern Addr sbk_40_ROM_START;
extern Addr sbk_40_ROM_END;
extern Addr sbk_40_VRAM;
extern Addr sbk_41_ROM_START;
extern Addr sbk_41_ROM_END;
extern Addr sbk_41_VRAM;
extern Addr sbk_42_ROM_START;
extern Addr sbk_42_ROM_END;
extern Addr sbk_42_VRAM;
extern Addr sbk_43_ROM_START;
extern Addr sbk_43_ROM_END;
extern Addr sbk_43_VRAM;
extern Addr sbk_44_ROM_START;
extern Addr sbk_44_ROM_END;
extern Addr sbk_44_VRAM;
extern Addr sbk_45_ROM_START;
extern Addr sbk_45_ROM_END;
extern Addr sbk_45_VRAM;
extern Addr sbk_46_ROM_START;
extern Addr sbk_46_ROM_END;
extern Addr sbk_46_VRAM;
extern Addr sbk_50_ROM_START;
extern Addr sbk_50_ROM_END;
extern Addr sbk_50_VRAM;
extern Addr sbk_51_ROM_START;
extern Addr sbk_51_ROM_END;
extern Addr sbk_51_VRAM;
extern Addr sbk_52_ROM_START;
extern Addr sbk_52_ROM_END;
extern Addr sbk_52_VRAM;
extern Addr sbk_53_ROM_START;
extern Addr sbk_53_ROM_END;
extern Addr sbk_53_VRAM;
extern Addr sbk_54_ROM_START;
extern Addr sbk_54_ROM_END;
extern Addr sbk_54_VRAM;
extern Addr sbk_55_ROM_START;
extern Addr sbk_55_ROM_END;
extern Addr sbk_55_VRAM;
extern Addr sbk_56_ROM_START;
extern Addr sbk_56_ROM_END;
extern Addr sbk_56_VRAM;
extern Addr sbk_60_ROM_START;
extern Addr sbk_60_ROM_END;
extern Addr sbk_60_VRAM;
extern Addr sbk_61_ROM_START;
extern Addr sbk_61_ROM_END;
extern Addr sbk_61_VRAM;
extern Addr sbk_62_ROM_START;
extern Addr sbk_62_ROM_END;
extern Addr sbk_62_VRAM;
extern Addr sbk_63_ROM_START;
extern Addr sbk_63_ROM_END;
extern Addr sbk_63_VRAM;
extern Addr sbk_64_ROM_START;
extern Addr sbk_64_ROM_END;
extern Addr sbk_64_VRAM;
extern Addr sbk_65_ROM_START;
extern Addr sbk_65_ROM_END;
extern Addr sbk_65_VRAM;
extern Addr sbk_66_ROM_START;
extern Addr sbk_66_ROM_END;
extern Addr sbk_66_VRAM;
extern Addr sbk_99_ROM_START;
extern Addr sbk_99_ROM_END;
extern Addr sbk_99_VRAM;
extern Addr tik_01_ROM_START;
extern Addr tik_01_ROM_END;
extern Addr tik_01_VRAM;
extern Addr tik_02_ROM_START;
extern Addr tik_02_ROM_END;
extern Addr tik_02_VRAM;
extern Addr tik_03_ROM_START;
extern Addr tik_03_ROM_END;
extern Addr tik_03_VRAM;
extern Addr tik_04_ROM_START;
extern Addr tik_04_ROM_END;
extern Addr tik_04_VRAM;
extern Addr tik_05_ROM_START;
extern Addr tik_05_ROM_END;
extern Addr tik_05_VRAM;
extern Addr tik_06_ROM_START;
extern Addr tik_06_ROM_END;
extern Addr tik_06_VRAM;
extern Addr tik_07_ROM_START;
extern Addr tik_07_ROM_END;
extern Addr tik_07_VRAM;
extern Addr tik_08_ROM_START;
extern Addr tik_08_ROM_END;
extern Addr tik_08_VRAM;
extern Addr tik_09_ROM_START;
extern Addr tik_09_ROM_END;
extern Addr tik_09_VRAM;
extern Addr tik_10_ROM_START;
extern Addr tik_10_ROM_END;
extern Addr tik_10_VRAM;
extern Addr tik_12_ROM_START;
extern Addr tik_12_ROM_END;
extern Addr tik_12_VRAM;
extern Addr tik_14_ROM_START;
extern Addr tik_14_ROM_END;
extern Addr tik_14_VRAM;
extern Addr tik_15_ROM_START;
extern Addr tik_15_ROM_END;
extern Addr tik_15_VRAM;
extern Addr tik_17_ROM_START;
extern Addr tik_17_ROM_END;
extern Addr tik_17_VRAM;
extern Addr tik_18_ROM_START;
extern Addr tik_18_ROM_END;
extern Addr tik_18_VRAM;
extern Addr tik_19_ROM_START;
extern Addr tik_19_ROM_END;
extern Addr tik_19_VRAM;
extern Addr tik_20_ROM_START;
extern Addr tik_20_ROM_END;
extern Addr tik_20_VRAM;
extern Addr tik_21_ROM_START;
extern Addr tik_21_ROM_END;
extern Addr tik_21_VRAM;
extern Addr tik_22_ROM_START;
extern Addr tik_22_ROM_END;
extern Addr tik_22_VRAM;
extern Addr tik_23_ROM_START;
extern Addr tik_23_ROM_END;
extern Addr tik_23_VRAM;
extern Addr tik_24_ROM_START;
extern Addr tik_24_ROM_END;
extern Addr tik_24_VRAM;
extern Addr tik_25_ROM_START;
extern Addr tik_25_ROM_END;
extern Addr tik_25_VRAM;
extern Addr trd_00_ROM_START;
extern Addr trd_00_ROM_END;
extern Addr trd_00_VRAM;
extern Addr trd_01_ROM_START;
extern Addr trd_01_ROM_END;
extern Addr trd_01_VRAM;
extern Addr trd_02_ROM_START;
extern Addr trd_02_ROM_END;
extern Addr trd_02_VRAM;
extern Addr trd_03_ROM_START;
extern Addr trd_03_ROM_END;
extern Addr trd_03_VRAM;
extern Addr trd_04_ROM_START;
extern Addr trd_04_ROM_END;
extern Addr trd_04_VRAM;
extern Addr trd_05_ROM_START;
extern Addr trd_05_ROM_END;
extern Addr trd_05_VRAM;
extern Addr trd_06_ROM_START;
extern Addr trd_06_ROM_END;
extern Addr trd_06_VRAM;
extern Addr trd_07_ROM_START;
extern Addr trd_07_ROM_END;
extern Addr trd_07_VRAM;
extern Addr trd_08_ROM_START;
extern Addr trd_08_ROM_END;
extern Addr trd_08_VRAM;
extern Addr trd_09_ROM_START;
extern Addr trd_09_ROM_END;
extern Addr trd_09_VRAM;
extern Addr trd_10_ROM_START;
extern Addr trd_10_ROM_END;
extern Addr trd_10_VRAM;
extern Addr tst_01_ROM_START;
extern Addr tst_01_ROM_END;
extern Addr tst_01_VRAM;
extern Addr tst_02_ROM_START;
extern Addr tst_02_ROM_END;
extern Addr tst_02_VRAM;
extern Addr tst_03_ROM_START;
extern Addr tst_03_ROM_END;
extern Addr tst_03_VRAM;
extern Addr tst_04_ROM_START;
extern Addr tst_04_ROM_END;
extern Addr tst_04_VRAM;
extern Addr tst_10_ROM_START;
extern Addr tst_10_ROM_END;
extern Addr tst_10_VRAM;
extern Addr tst_11_ROM_START;
extern Addr tst_11_ROM_END;
extern Addr tst_11_VRAM;
extern Addr tst_12_ROM_START;
extern Addr tst_12_ROM_END;
extern Addr tst_12_VRAM;
extern Addr tst_13_ROM_START;
extern Addr tst_13_ROM_END;
extern Addr tst_13_VRAM;
extern Addr tst_20_ROM_START;
extern Addr tst_20_ROM_END;
extern Addr tst_20_VRAM;

#ifdef __cplusplus
}
#endif

#endif /* _LD_ADDRS_H_ */
