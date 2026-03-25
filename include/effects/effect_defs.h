// Generated from effects.yaml
#include "effects/effect_macros.h"

enum EffectID {
    EFFECT_00 = 0x00,
    EFFECT_BIG_SMOKE_PUFF = 0x01,
    EFFECT_02 = 0x02,
    EFFECT_03 = 0x03,
    EFFECT_04 = 0x04,
    EFFECT_05 = 0x05,
    EFFECT_LANDING_DUST = 0x06,
    EFFECT_WALKING_DUST = 0x07,
    EFFECT_FLOWER_SPLASH = 0x08,
    EFFECT_FLOWER_TRAIL = 0x09,
    EFFECT_CLOUD_PUFF = 0x0A,
    EFFECT_CLOUD_TRAIL = 0x0B,
    EFFECT_FOOTPRINT = 0x0C,
    EFFECT_FLOATING_FLOWER = 0x0D,
    EFFECT_SNOWFLAKE = 0x0E,
    EFFECT_STAR = 0x0F,
    EFFECT_EMOTE = 0x10,
    EFFECT_SPARKLES = 0x11,
    EFFECT_SHAPE_SPELL = 0x12,
    EFFECT_GATHER_ENERGY_PINK = 0x13,
    EFFECT_DROP_LEAVES = 0x14,
    EFFECT_DUST = 0x15,
    EFFECT_SHATTERING_STONES = 0x16,
    EFFECT_SMOKE_RING = 0x17,
    EFFECT_DAMAGE_STARS = 0x18,
    EFFECT_EXPLOSION = 0x19,
    EFFECT_LENS_FLARE = 0x1A,
    EFFECT_GOT_ITEM_OUTLINE = 0x1B,
    EFFECT_SPIKY_WHITE_AURA = 0x1C,
    EFFECT_SMOKE_IMPACT = 0x1D,
    EFFECT_DAMAGE_INDICATOR = 0x1E,
    EFFECT_PURPLE_RING = 0x1F,
    EFFECT_FLAME = 0x20,
    EFFECT_STARS_BURST = 0x21,
    EFFECT_STARS_SHIMMER = 0x22,
    EFFECT_RISING_BUBBLE = 0x23,
    EFFECT_RING_BLAST = 0x24,
    EFFECT_SHOCKWAVE = 0x25,
    EFFECT_MUSIC_NOTE = 0x26,
    EFFECT_SMOKE_BURST = 0x27,
    EFFECT_SWEAT = 0x28,
    EFFECT_SLEEP_BUBBLE = 0x29,
    EFFECT_2A = 0x2A,
    EFFECT_WINDY_LEAVES = 0x2B,
    EFFECT_FALLING_LEAVES = 0x2C,
    EFFECT_STARS_SPREAD = 0x2D,
    EFFECT_STEAM_BURST = 0x2E,
    EFFECT_STARS_ORBITING = 0x2F,
    EFFECT_BIG_SNOWFLAKES = 0x30,
    EFFECT_DEBUFF = 0x31,
    EFFECT_GREEN_IMPACT = 0x32,
    EFFECT_RADIAL_SHIMMER = 0x33,
    EFFECT_ENDING_DECALS = 0x34,
    EFFECT_LIGHT_RAYS = 0x35,
    EFFECT_LIGHTNING = 0x36,
    EFFECT_FIRE_BREATH = 0x37,
    EFFECT_SHIMMER_BURST = 0x38,
    EFFECT_ENERGY_SHOCKWAVE = 0x39,
    EFFECT_SHIMMER_WAVE = 0x3A,
    EFFECT_AURA = 0x3B,
    EFFECT_BULB_GLOW = 0x3C,
    EFFECT_3D = 0x3D,
    EFFECT_BLAST = 0x3E,
    EFFECT_FIRE_FLOWER = 0x3F,
    EFFECT_RECOVER = 0x40,
    EFFECT_DISABLE_X = 0x41,
    EFFECT_BOMBETTE_BREAKING = 0x42,
    EFFECT_FIREWORK = 0x43,
    EFFECT_CONFETTI = 0x44,
    EFFECT_SNOWFALL = 0x45,
    EFFECT_46 = 0x46,
    EFFECT_GATHER_MAGIC = 0x47,
    EFFECT_ATTACK_RESULT_TEXT = 0x48,
    EFFECT_SMALL_GOLD_SPARKLE = 0x49,
    EFFECT_FLASHING_BOX_SHOCKWAVE = 0x4A,
    EFFECT_BALLOON = 0x4B,
    EFFECT_FLOATING_ROCK = 0x4C,
    EFFECT_CHOMP_DROP = 0x4D,
    EFFECT_QUIZMO_STAGE = 0x4E,
    EFFECT_RADIATING_ENERGY_ORB = 0x4F,
    EFFECT_QUIZMO_ANSWER = 0x50,
    EFFECT_MOTION_BLUR_FLAME = 0x51,
    EFFECT_ENERGY_ORB_WAVE = 0x52,
    EFFECT_MERLIN_HOUSE_STARS = 0x53,
    EFFECT_QUIZMO_AUDIENCE = 0x54,
    EFFECT_BUTTERFLIES = 0x55,
    EFFECT_STAT_CHANGE = 0x56,
    EFFECT_SNAKING_STATIC = 0x57,
    EFFECT_THUNDERBOLT_RING = 0x58,
    EFFECT_SQUIRT = 0x59,
    EFFECT_WATER_BLOCK = 0x5A,
    EFFECT_WATERFALL = 0x5B,
    EFFECT_WATER_FOUNTAIN = 0x5C,
    EFFECT_UNDERWATER = 0x5D,
    EFFECT_LIGHTNING_BOLT = 0x5E,
    EFFECT_WATER_SPLASH = 0x5F,
    EFFECT_SNOWMAN_DOLL = 0x60,
    EFFECT_FRIGHT_JAR = 0x61,
    EFFECT_STOP_WATCH = 0x62,
    EFFECT_63 = 0x63,
    EFFECT_THROW_SPINY = 0x64,
    EFFECT_65 = 0x65,
    EFFECT_TUBBA_HEART_ATTACK = 0x66,
    EFFECT_WHIRLWIND = 0x67,
    EFFECT_RED_IMPACT = 0x68,
    EFFECT_FLOATING_CLOUD_PUFF = 0x69,
    EFFECT_6A = 0x6A,
    EFFECT_ENERGY_IN_OUT = 0x6B,
    EFFECT_TATTLE_WINDOW = 0x6C,
    EFFECT_SHINY_FLARE = 0x6D,
    EFFECT_HUFF_PUFF_BREATH = 0x6E,
    EFFECT_COLD_BREATH = 0x6F,
    EFFECT_EMBERS = 0x70,
    EFFECT_HIEROGLYPHS = 0x71,
    EFFECT_MISC_PARTICLES = 0x72,
    EFFECT_STATIC_STATUS = 0x73,
    EFFECT_MOVING_CLOUD = 0x74,
    EFFECT_75 = 0x75,
    EFFECT_76 = 0x76,
    EFFECT_FIREWORK_ROCKET = 0x77,
    EFFECT_PEACH_STAR_BEAM = 0x78,
    EFFECT_CHAPTER_CHANGE = 0x79,
    EFFECT_ICE_SHARD = 0x7A,
    EFFECT_SPIRIT_CARD = 0x7B,
    EFFECT_LIL_OINK = 0x7C,
    EFFECT_SOMETHING_ROTATING = 0x7D,
    EFFECT_BREAKING_JUNK = 0x7E,
    EFFECT_PARTNER_BUFF = 0x7F,
    EFFECT_QUIZMO_ASSISTANT = 0x80,
    EFFECT_ICE_PILLAR = 0x81,
    EFFECT_SUN = 0x82,
    EFFECT_STAR_SPIRITS_ENERGY = 0x83,
    EFFECT_PINK_SPARKLES = 0x84,
    EFFECT_STAR_OUTLINE = 0x85,
    EFFECT_86 = 0x86,
};

void fx_big_smoke_puff(f32 arg0, f32 arg1, f32 arg2);
void fx_landing_dust(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4);
void fx_walking_dust(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5);
void fx_flower_splash(f32 arg0, f32 arg1, f32 arg2, f32 arg3);
void fx_flower_trail(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5);
void fx_cloud_puff(f32 arg0, f32 arg1, f32 arg2, f32 arg3);
void fx_cloud_trail(s32 arg0, f32 arg1, f32 arg2, f32 arg3);
void fx_footprint(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4);
void fx_floating_flower(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4);
void fx_snowflake(f32 arg0, f32 arg1, f32 arg2, s32 arg3);
EffectInstance* fx_star(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7);
void fx_emote(s32 arg0, Npc* arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7, EffectInstance** arg8);
void fx_sparkles(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4);
EffectInstance* fx_shape_spell(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7);
void fx_gather_energy_pink(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
void fx_drop_leaves(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4);
EffectInstance* fx_dust(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4);
void fx_shattering_stones(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4);
void fx_smoke_ring(s32 arg0, f32 arg1, f32 arg2, f32 arg3);
void fx_damage_stars(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7);
void fx_explosion(s32 arg0, f32 arg1, f32 arg2, f32 arg3);
void fx_lens_flare(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4);
void fx_got_item_outline(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, EffectInstance** arg5);
void fx_spiky_white_aura(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4);
void fx_smoke_impact(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, f32 arg6, s32 arg7);
void fx_damage_indicator(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6, EffectInstance** arg7);
EffectInstance* fx_purple_ring(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7);
void fx_flame(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, EffectInstance** arg5);
void fx_stars_burst(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6);
void fx_stars_shimmer(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6, s32 arg7);
void fx_rising_bubble(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4);
void fx_ring_blast(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
void fx_shockwave(s32 arg0, f32 arg1, f32 arg2, f32 arg3);
void fx_music_note(s32 arg0, f32 arg1, f32 arg2, f32 arg3);
void fx_smoke_burst(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
void fx_sweat(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6);
void fx_sleep_bubble(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, EffectInstance** arg6);
void fx_windy_leaves(s32 arg0, f32 arg1, f32 arg2, f32 arg3);
void fx_falling_leaves(s32 arg0, f32 arg1, f32 arg2, f32 arg3);
void fx_stars_spread(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4, s32 arg5);
void fx_steam_burst(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
void fx_stars_orbiting(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, EffectInstance** arg6);
void fx_big_snowflakes(s32 arg0, f32 arg1, f32 arg2, f32 arg3);
EffectInstance* fx_debuff(s32 arg0, f32 arg1, f32 arg2, f32 arg3);
EffectInstance* fx_green_impact(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4);
EffectInstance* fx_radial_shimmer(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
void fx_ending_decals(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, EffectInstance** arg5);
void fx_light_rays(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, EffectInstance** arg5);
EffectInstance* fx_lightning(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5);
EffectInstance* fx_fire_breath(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7, s32 arg8, s32 arg9);
EffectInstance* fx_shimmer_burst(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
void fx_energy_shockwave(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_shimmer_wave(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6, s32 arg7);
void fx_aura(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, EffectInstance** arg5);
void fx_bulb_glow(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, EffectInstance** arg5);
void fx_effect_3D(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7, EffectInstance** arg8);
void fx_blast(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_fire_flower(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4);
EffectInstance* fx_recover(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4);
EffectInstance* fx_disable_x(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4);
EffectInstance* fx_bombette_breaking(s32 arg0, s32 arg1, s32 arg2, f32 arg3, s32 arg4, s32 arg5);
EffectInstance* fx_firework(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_confetti(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_snowfall(s32 arg0, s32 arg1);
EffectInstance* fx_effect_46(s32 arg0, PlayerStatus* arg1, f32 arg2, s32 arg3);
EffectInstance* fx_gather_magic(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_attack_result_text(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_small_gold_sparkle(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_flashing_box_shockwave(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5);
EffectInstance* fx_balloon(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_floating_rock(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, s32 arg8);
EffectInstance* fx_chomp_drop(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, f32 arg6, s32 arg7, f32 arg8, s32 arg9);
EffectInstance* fx_quizmo_stage(s32 arg0, f32 arg1, f32 arg2, f32 arg3);
EffectInstance* fx_radiating_energy_orb(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_quizmo_answer(s32 arg0, f32 arg1, f32 arg2, f32 arg3);
EffectInstance* fx_motion_blur_flame(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_energy_orb_wave(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_merlin_house_stars(s32 arg0, f32 arg1, f32 arg2, f32 arg3);
EffectInstance* fx_quizmo_audience(s32 arg0, f32 arg1, f32 arg2, f32 arg3);
EffectInstance* fx_butterflies(s32 arg0, f32 arg1, f32 arg2, f32 arg3);
EffectInstance* fx_stat_change(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_snaking_static(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_thunderbolt_ring(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_squirt(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, s32 arg8);
EffectInstance* fx_water_block(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_waterfall(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_water_fountain(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_underwater(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_lightning_bolt(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, s32 arg8);
EffectInstance* fx_water_splash(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_snowman_doll(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_fright_jar(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_stop_watch(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_effect_63(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, s32 arg8, s32 arg9);
EffectInstance* fx_throw_spiny(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, s32 arg8);
EffectInstance* fx_effect_65(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_tubba_heart_attack(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_whirlwind(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_red_impact(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_floating_cloud_puff(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_energy_in_out(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_tattle_window(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_shiny_flare(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_huff_puff_breath(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7);
EffectInstance* fx_cold_breath(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_embers(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7, s32 arg8, f32 arg9, f32 arg10);
EffectInstance* fx_hieroglyphs(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_misc_particles(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7, s32 arg8);
EffectInstance* fx_static_status(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6);
EffectInstance* fx_moving_cloud(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8);
EffectInstance* fx_effect_75(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_firework_rocket(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, s32 arg8);
EffectInstance* fx_peach_star_beam(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_chapter_change(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_ice_shard(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_spirit_card(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_lil_oink(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_something_rotating(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_breaking_junk(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_partner_buff(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_quizmo_assistant(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_ice_pillar(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_sun(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_star_spirits_energy(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_pink_sparkles(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5);
EffectInstance* fx_star_outline(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
EffectInstance* fx_effect_86(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
