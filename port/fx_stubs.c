/**
 * fx_stubs.c - PC trampoline implementations for fx_* effect functions.
 *
 * On N64, each fx_* function is a small assembly trampoline that:
 *   1. Calls load_effect(EFFECT_ID) to DMA the effect code into a TLB-mapped page
 *   2. Jumps to the _main() entry point with the same arguments
 *
 * On PC, all effect code is statically linked, so we just need to call
 * load_effect() (to set up the EffectSharedData bookkeeping) and then
 * call the _main() function directly.
 */

#include "common.h"
#include "effects.h"

// Forward-declare all _main functions using the EFFECT_DEF macros.
// These are already defined in src/effects/*.c and compiled into the binary.
// The EFFECT_DEF macros expand to the full function prototype.
// (effect_macros.h is included transitively via effects.h -> effect_defs.h)

EFFECT_DEF_BIG_SMOKE_PUFF(big_smoke_puff_main);
EFFECT_DEF_LANDING_DUST(landing_dust_main);
EFFECT_DEF_WALKING_DUST(walking_dust_main);
EFFECT_DEF_FLOWER_SPLASH(flower_splash_main);
EFFECT_DEF_FLOWER_TRAIL(flower_trail_main);
EFFECT_DEF_CLOUD_PUFF(cloud_puff_main);
EFFECT_DEF_CLOUD_TRAIL(cloud_trail_main);
EFFECT_DEF_FOOTPRINT(footprint_main);
EFFECT_DEF_FLOATING_FLOWER(floating_flower_main);
EFFECT_DEF_SNOWFLAKE(snowflake_main);
EFFECT_DEF_STAR(star_main);
EFFECT_DEF_EMOTE(emote_main);
EFFECT_DEF_SPARKLES(sparkles_main);
EFFECT_DEF_SHAPE_SPELL(shape_spell_main);
EFFECT_DEF_GATHER_ENERGY_PINK(gather_energy_pink_main);
EFFECT_DEF_DROP_LEAVES(drop_leaves_main);
EFFECT_DEF_DUST(dust_main);
EFFECT_DEF_SHATTERING_STONES(shattering_stones_main);
EFFECT_DEF_SMOKE_RING(smoke_ring_main);
EFFECT_DEF_DAMAGE_STARS(damage_stars_main);
EFFECT_DEF_EXPLOSION(explosion_main);
EFFECT_DEF_LENS_FLARE(lens_flare_main);
EFFECT_DEF_GOT_ITEM_OUTLINE(got_item_outline_main);
EFFECT_DEF_SPIKY_WHITE_AURA(spiky_white_aura_main);
EFFECT_DEF_SMOKE_IMPACT(smoke_impact_main);
EFFECT_DEF_DAMAGE_INDICATOR(damage_indicator_main);
EFFECT_DEF_PURPLE_RING(purple_ring_main);
EFFECT_DEF_FLAME(flame_main);
EFFECT_DEF_STARS_BURST(stars_burst_main);
EFFECT_DEF_STARS_SHIMMER(stars_shimmer_main);
EFFECT_DEF_RISING_BUBBLE(rising_bubble_main);
EFFECT_DEF_RING_BLAST(ring_blast_main);
EFFECT_DEF_SHOCKWAVE(shockwave_main);
EFFECT_DEF_MUSIC_NOTE(music_note_main);
EFFECT_DEF_SMOKE_BURST(smoke_burst_main);
EFFECT_DEF_SWEAT(sweat_main);
EFFECT_DEF_SLEEP_BUBBLE(sleep_bubble_main);
EFFECT_DEF_WINDY_LEAVES(windy_leaves_main);
EFFECT_DEF_FALLING_LEAVES(falling_leaves_main);
EFFECT_DEF_STARS_SPREAD(stars_spread_main);
EFFECT_DEF_STEAM_BURST(steam_burst_main);
EFFECT_DEF_STARS_ORBITING(stars_orbiting_main);
EFFECT_DEF_BIG_SNOWFLAKES(big_snowflakes_main);
EFFECT_DEF_DEBUFF(debuff_main);
EFFECT_DEF_GREEN_IMPACT(green_impact_main);
EFFECT_DEF_RADIAL_SHIMMER(radial_shimmer_main);
EFFECT_DEF_ENDING_DECALS(ending_decals_main);
EFFECT_DEF_LIGHT_RAYS(light_rays_main);
EFFECT_DEF_LIGHTNING(lightning_main);
EFFECT_DEF_FIRE_BREATH(fire_breath_main);
EFFECT_DEF_SHIMMER_BURST(shimmer_burst_main);
EFFECT_DEF_ENERGY_SHOCKWAVE(energy_shockwave_main);
EFFECT_DEF_SHIMMER_WAVE(shimmer_wave_main);
EFFECT_DEF_AURA(aura_main);
EFFECT_DEF_BULB_GLOW(bulb_glow_main);
EFFECT_DEF_EFFECT_3D(effect_3D_main);
EFFECT_DEF_BLAST(blast_main);
EFFECT_DEF_FIRE_FLOWER(fire_flower_main);
EFFECT_DEF_RECOVER(recover_main);
EFFECT_DEF_DISABLE_X(disable_x_main);
EFFECT_DEF_BOMBETTE_BREAKING(bombette_breaking_main);
EFFECT_DEF_FIREWORK(firework_main);
EFFECT_DEF_CONFETTI(confetti_main);
EFFECT_DEF_SNOWFALL(snowfall_main);
EFFECT_DEF_EFFECT_46(effect_46_main);
EFFECT_DEF_GATHER_MAGIC(gather_magic_main);
EFFECT_DEF_ATTACK_RESULT_TEXT(attack_result_text_main);
EFFECT_DEF_SMALL_GOLD_SPARKLE(small_gold_sparkle_main);
EFFECT_DEF_FLASHING_BOX_SHOCKWAVE(flashing_box_shockwave_main);
EFFECT_DEF_BALLOON(balloon_main);
EFFECT_DEF_FLOATING_ROCK(floating_rock_main);
EFFECT_DEF_CHOMP_DROP(chomp_drop_main);
EFFECT_DEF_QUIZMO_STAGE(quizmo_stage_main);
EFFECT_DEF_RADIATING_ENERGY_ORB(radiating_energy_orb_main);
EFFECT_DEF_QUIZMO_ANSWER(quizmo_answer_main);
EFFECT_DEF_MOTION_BLUR_FLAME(motion_blur_flame_main);
EFFECT_DEF_ENERGY_ORB_WAVE(energy_orb_wave_main);
EFFECT_DEF_MERLIN_HOUSE_STARS(merlin_house_stars_main);
EFFECT_DEF_QUIZMO_AUDIENCE(quizmo_audience_main);
EFFECT_DEF_BUTTERFLIES(butterflies_main);
EFFECT_DEF_STAT_CHANGE(stat_change_main);
EFFECT_DEF_SNAKING_STATIC(snaking_static_main);
EFFECT_DEF_THUNDERBOLT_RING(thunderbolt_ring_main);
EFFECT_DEF_SQUIRT(squirt_main);
EFFECT_DEF_WATER_BLOCK(water_block_main);
EFFECT_DEF_WATERFALL(waterfall_main);
EFFECT_DEF_WATER_FOUNTAIN(water_fountain_main);
EFFECT_DEF_UNDERWATER(underwater_main);
EFFECT_DEF_LIGHTNING_BOLT(lightning_bolt_main);
EFFECT_DEF_WATER_SPLASH(water_splash_main);
EFFECT_DEF_SNOWMAN_DOLL(snowman_doll_main);
EFFECT_DEF_FRIGHT_JAR(fright_jar_main);
EFFECT_DEF_STOP_WATCH(stop_watch_main);
EFFECT_DEF_EFFECT_63(effect_63_main);
EFFECT_DEF_THROW_SPINY(throw_spiny_main);
EFFECT_DEF_EFFECT_65(effect_65_main);
EFFECT_DEF_TUBBA_HEART_ATTACK(tubba_heart_attack_main);
EFFECT_DEF_WHIRLWIND(whirlwind_main);
EFFECT_DEF_RED_IMPACT(red_impact_main);
EFFECT_DEF_FLOATING_CLOUD_PUFF(floating_cloud_puff_main);
EFFECT_DEF_ENERGY_IN_OUT(energy_in_out_main);
EFFECT_DEF_TATTLE_WINDOW(tattle_window_main);
EFFECT_DEF_SHINY_FLARE(shiny_flare_main);
EFFECT_DEF_HUFF_PUFF_BREATH(huff_puff_breath_main);
EFFECT_DEF_COLD_BREATH(cold_breath_main);
EFFECT_DEF_EMBERS(embers_main);
EFFECT_DEF_HIEROGLYPHS(hieroglyphs_main);
EFFECT_DEF_MISC_PARTICLES(misc_particles_main);
EFFECT_DEF_STATIC_STATUS(static_status_main);
EFFECT_DEF_MOVING_CLOUD(moving_cloud_main);
EFFECT_DEF_EFFECT_75(effect_75_main);
EFFECT_DEF_FIREWORK_ROCKET(firework_rocket_main);
EFFECT_DEF_PEACH_STAR_BEAM(peach_star_beam_main);
EFFECT_DEF_CHAPTER_CHANGE(chapter_change_main);
EFFECT_DEF_ICE_SHARD(ice_shard_main);
EFFECT_DEF_SPIRIT_CARD(spirit_card_main);
EFFECT_DEF_LIL_OINK(lil_oink_main);
EFFECT_DEF_SOMETHING_ROTATING(something_rotating_main);
EFFECT_DEF_BREAKING_JUNK(breaking_junk_main);
EFFECT_DEF_PARTNER_BUFF(partner_buff_main);
EFFECT_DEF_QUIZMO_ASSISTANT(quizmo_assistant_main);
EFFECT_DEF_ICE_PILLAR(ice_pillar_main);
EFFECT_DEF_SUN(sun_main);
EFFECT_DEF_STAR_SPIRITS_ENERGY(star_spirits_energy_main);
EFFECT_DEF_PINK_SPARKLES(pink_sparkles_main);
EFFECT_DEF_STAR_OUTLINE(star_outline_main);
EFFECT_DEF_EFFECT_86(effect_86_main);

// ---------------------------------------------------------------------------
// fx_* trampoline implementations
// Each calls load_effect() then forwards to the _main() entry point.
// ---------------------------------------------------------------------------

void fx_big_smoke_puff(f32 arg0, f32 arg1, f32 arg2) {
    load_effect(EFFECT_BIG_SMOKE_PUFF);
    big_smoke_puff_main(arg0, arg1, arg2);
}

void fx_landing_dust(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    load_effect(EFFECT_LANDING_DUST);
    landing_dust_main(arg0, arg1, arg2, arg3, arg4);
}

void fx_walking_dust(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5) {
    load_effect(EFFECT_WALKING_DUST);
    walking_dust_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

void fx_flower_splash(f32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    load_effect(EFFECT_FLOWER_SPLASH);
    flower_splash_main(arg0, arg1, arg2, arg3);
}

void fx_flower_trail(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5) {
    load_effect(EFFECT_FLOWER_TRAIL);
    flower_trail_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

void fx_cloud_puff(f32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    load_effect(EFFECT_CLOUD_PUFF);
    cloud_puff_main(arg0, arg1, arg2, arg3);
}

void fx_cloud_trail(s32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    load_effect(EFFECT_CLOUD_TRAIL);
    cloud_trail_main(arg0, arg1, arg2, arg3);
}

void fx_footprint(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    load_effect(EFFECT_FOOTPRINT);
    footprint_main(arg0, arg1, arg2, arg3, arg4);
}

void fx_floating_flower(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4) {
    load_effect(EFFECT_FLOATING_FLOWER);
    floating_flower_main(arg0, arg1, arg2, arg3, arg4);
}

void fx_snowflake(f32 arg0, f32 arg1, f32 arg2, s32 arg3) {
    load_effect(EFFECT_SNOWFLAKE);
    snowflake_main(arg0, arg1, arg2, arg3);
}

EffectInstance* fx_star(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7) {
    load_effect(EFFECT_STAR);
    return star_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

void fx_emote(s32 arg0, Npc* arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7, EffectInstance** arg8) {
    load_effect(EFFECT_EMOTE);
    emote_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
}

void fx_sparkles(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    load_effect(EFFECT_SPARKLES);
    sparkles_main(arg0, arg1, arg2, arg3, arg4);
}

EffectInstance* fx_shape_spell(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7) {
    load_effect(EFFECT_SHAPE_SPELL);
    return shape_spell_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

void fx_gather_energy_pink(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_GATHER_ENERGY_PINK);
    gather_energy_pink_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

void fx_drop_leaves(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4) {
    load_effect(EFFECT_DROP_LEAVES);
    drop_leaves_main(arg0, arg1, arg2, arg3, arg4);
}

EffectInstance* fx_dust(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4) {
    load_effect(EFFECT_DUST);
    return dust_main(arg0, arg1, arg2, arg3, arg4);
}

void fx_shattering_stones(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    load_effect(EFFECT_SHATTERING_STONES);
    shattering_stones_main(arg0, arg1, arg2, arg3, arg4);
}

void fx_smoke_ring(s32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    load_effect(EFFECT_SMOKE_RING);
    smoke_ring_main(arg0, arg1, arg2, arg3);
}

void fx_damage_stars(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7) {
    load_effect(EFFECT_DAMAGE_STARS);
    damage_stars_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

void fx_explosion(s32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    load_effect(EFFECT_EXPLOSION);
    explosion_main(arg0, arg1, arg2, arg3);
}

void fx_lens_flare(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4) {
    load_effect(EFFECT_LENS_FLARE);
    lens_flare_main(arg0, arg1, arg2, arg3, arg4);
}

void fx_got_item_outline(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, EffectInstance** arg5) {
    load_effect(EFFECT_GOT_ITEM_OUTLINE);
    got_item_outline_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

void fx_spiky_white_aura(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4) {
    load_effect(EFFECT_SPIKY_WHITE_AURA);
    spiky_white_aura_main(arg0, arg1, arg2, arg3, arg4);
}

void fx_smoke_impact(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, f32 arg6, s32 arg7) {
    load_effect(EFFECT_SMOKE_IMPACT);
    smoke_impact_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

void fx_damage_indicator(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6, EffectInstance** arg7) {
    load_effect(EFFECT_DAMAGE_INDICATOR);
    damage_indicator_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

EffectInstance* fx_purple_ring(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7) {
    load_effect(EFFECT_PURPLE_RING);
    return purple_ring_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

void fx_flame(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, EffectInstance** arg5) {
    load_effect(EFFECT_FLAME);
    flame_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

void fx_stars_burst(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6) {
    load_effect(EFFECT_STARS_BURST);
    stars_burst_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
}

void fx_stars_shimmer(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6, s32 arg7) {
    load_effect(EFFECT_STARS_SHIMMER);
    stars_shimmer_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

void fx_rising_bubble(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    load_effect(EFFECT_RISING_BUBBLE);
    rising_bubble_main(arg0, arg1, arg2, arg3, arg4);
}

void fx_ring_blast(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_RING_BLAST);
    ring_blast_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

void fx_shockwave(s32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    load_effect(EFFECT_SHOCKWAVE);
    shockwave_main(arg0, arg1, arg2, arg3);
}

void fx_music_note(s32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    load_effect(EFFECT_MUSIC_NOTE);
    music_note_main(arg0, arg1, arg2, arg3);
}

void fx_smoke_burst(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_SMOKE_BURST);
    smoke_burst_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

void fx_sweat(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6) {
    load_effect(EFFECT_SWEAT);
    sweat_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
}

void fx_sleep_bubble(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, EffectInstance** arg6) {
    load_effect(EFFECT_SLEEP_BUBBLE);
    sleep_bubble_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
}

void fx_windy_leaves(s32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    load_effect(EFFECT_WINDY_LEAVES);
    windy_leaves_main(arg0, arg1, arg2, arg3);
}

void fx_falling_leaves(s32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    load_effect(EFFECT_FALLING_LEAVES);
    falling_leaves_main(arg0, arg1, arg2, arg3);
}

void fx_stars_spread(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4, s32 arg5) {
    load_effect(EFFECT_STARS_SPREAD);
    stars_spread_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

void fx_steam_burst(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_STEAM_BURST);
    steam_burst_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

void fx_stars_orbiting(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, EffectInstance** arg6) {
    load_effect(EFFECT_STARS_ORBITING);
    stars_orbiting_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
}

void fx_big_snowflakes(s32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    load_effect(EFFECT_BIG_SNOWFLAKES);
    big_snowflakes_main(arg0, arg1, arg2, arg3);
}

EffectInstance* fx_debuff(s32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    load_effect(EFFECT_DEBUFF);
    return debuff_main(arg0, arg1, arg2, arg3);
}

EffectInstance* fx_green_impact(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    load_effect(EFFECT_GREEN_IMPACT);
    return green_impact_main(arg0, arg1, arg2, arg3, arg4);
}

EffectInstance* fx_radial_shimmer(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_RADIAL_SHIMMER);
    return radial_shimmer_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

void fx_ending_decals(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, EffectInstance** arg5) {
    load_effect(EFFECT_ENDING_DECALS);
    ending_decals_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

void fx_light_rays(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, EffectInstance** arg5) {
    load_effect(EFFECT_LIGHT_RAYS);
    light_rays_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_lightning(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5) {
    load_effect(EFFECT_LIGHTNING);
    return lightning_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_fire_breath(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7, s32 arg8, s32 arg9) {
    load_effect(EFFECT_FIRE_BREATH);
    return fire_breath_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
}

EffectInstance* fx_shimmer_burst(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_SHIMMER_BURST);
    return shimmer_burst_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

void fx_energy_shockwave(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_ENERGY_SHOCKWAVE);
    energy_shockwave_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_shimmer_wave(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6, s32 arg7) {
    load_effect(EFFECT_SHIMMER_WAVE);
    return shimmer_wave_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

void fx_aura(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, EffectInstance** arg5) {
    load_effect(EFFECT_AURA);
    aura_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

void fx_bulb_glow(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, EffectInstance** arg5) {
    load_effect(EFFECT_BULB_GLOW);
    bulb_glow_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

void fx_effect_3D(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7, EffectInstance** arg8) {
    load_effect(EFFECT_3D);
    effect_3D_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
}

void fx_blast(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_BLAST);
    blast_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_fire_flower(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4) {
    load_effect(EFFECT_FIRE_FLOWER);
    return fire_flower_main(arg0, arg1, arg2, arg3, arg4);
}

EffectInstance* fx_recover(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4) {
    load_effect(EFFECT_RECOVER);
    return recover_main(arg0, arg1, arg2, arg3, arg4);
}

EffectInstance* fx_disable_x(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4) {
    load_effect(EFFECT_DISABLE_X);
    return disable_x_main(arg0, arg1, arg2, arg3, arg4);
}

EffectInstance* fx_bombette_breaking(s32 arg0, s32 arg1, s32 arg2, f32 arg3, s32 arg4, s32 arg5) {
    load_effect(EFFECT_BOMBETTE_BREAKING);
    return bombette_breaking_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_firework(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_FIREWORK);
    return firework_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_confetti(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_CONFETTI);
    return confetti_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_snowfall(s32 arg0, s32 arg1) {
    load_effect(EFFECT_SNOWFALL);
    return snowfall_main(arg0, arg1);
}

EffectInstance* fx_effect_46(s32 arg0, PlayerStatus* arg1, f32 arg2, s32 arg3) {
    load_effect(EFFECT_46);
    return effect_46_main(arg0, arg1, arg2, arg3);
}

EffectInstance* fx_gather_magic(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_GATHER_MAGIC);
    return gather_magic_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_attack_result_text(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_ATTACK_RESULT_TEXT);
    return attack_result_text_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_small_gold_sparkle(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_SMALL_GOLD_SPARKLE);
    return small_gold_sparkle_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_flashing_box_shockwave(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5) {
    load_effect(EFFECT_FLASHING_BOX_SHOCKWAVE);
    return flashing_box_shockwave_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_balloon(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_BALLOON);
    return balloon_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_floating_rock(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, s32 arg8) {
    load_effect(EFFECT_FLOATING_ROCK);
    return floating_rock_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
}

EffectInstance* fx_chomp_drop(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, f32 arg6, s32 arg7, f32 arg8, s32 arg9) {
    load_effect(EFFECT_CHOMP_DROP);
    return chomp_drop_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
}

EffectInstance* fx_quizmo_stage(s32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    load_effect(EFFECT_QUIZMO_STAGE);
    return quizmo_stage_main(arg0, arg1, arg2, arg3);
}

EffectInstance* fx_radiating_energy_orb(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_RADIATING_ENERGY_ORB);
    return radiating_energy_orb_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_quizmo_answer(s32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    load_effect(EFFECT_QUIZMO_ANSWER);
    return quizmo_answer_main(arg0, arg1, arg2, arg3);
}

EffectInstance* fx_motion_blur_flame(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_MOTION_BLUR_FLAME);
    return motion_blur_flame_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_energy_orb_wave(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_ENERGY_ORB_WAVE);
    return energy_orb_wave_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_merlin_house_stars(s32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    load_effect(EFFECT_MERLIN_HOUSE_STARS);
    return merlin_house_stars_main(arg0, arg1, arg2, arg3);
}

EffectInstance* fx_quizmo_audience(s32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    load_effect(EFFECT_QUIZMO_AUDIENCE);
    return quizmo_audience_main(arg0, arg1, arg2, arg3);
}

EffectInstance* fx_butterflies(s32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    load_effect(EFFECT_BUTTERFLIES);
    return butterflies_main(arg0, arg1, arg2, arg3);
}

EffectInstance* fx_stat_change(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_STAT_CHANGE);
    return stat_change_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_snaking_static(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_SNAKING_STATIC);
    return snaking_static_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_thunderbolt_ring(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_THUNDERBOLT_RING);
    return thunderbolt_ring_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_squirt(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, s32 arg8) {
    load_effect(EFFECT_SQUIRT);
    return squirt_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
}

EffectInstance* fx_water_block(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_WATER_BLOCK);
    return water_block_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_waterfall(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_WATERFALL);
    return waterfall_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_water_fountain(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_WATER_FOUNTAIN);
    return water_fountain_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_underwater(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_UNDERWATER);
    return underwater_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_lightning_bolt(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, s32 arg8) {
    load_effect(EFFECT_LIGHTNING_BOLT);
    return lightning_bolt_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
}

EffectInstance* fx_water_splash(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_WATER_SPLASH);
    return water_splash_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_snowman_doll(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_SNOWMAN_DOLL);
    return snowman_doll_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_fright_jar(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_FRIGHT_JAR);
    return fright_jar_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_stop_watch(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_STOP_WATCH);
    return stop_watch_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_effect_63(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, s32 arg8, s32 arg9) {
    load_effect(EFFECT_63);
    return effect_63_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
}

EffectInstance* fx_throw_spiny(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, s32 arg8) {
    load_effect(EFFECT_THROW_SPINY);
    return throw_spiny_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
}

EffectInstance* fx_effect_65(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_65);
    return effect_65_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_tubba_heart_attack(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_TUBBA_HEART_ATTACK);
    return tubba_heart_attack_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_whirlwind(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_WHIRLWIND);
    return whirlwind_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_red_impact(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_RED_IMPACT);
    return red_impact_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_floating_cloud_puff(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_FLOATING_CLOUD_PUFF);
    return floating_cloud_puff_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_energy_in_out(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_ENERGY_IN_OUT);
    return energy_in_out_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_tattle_window(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_TATTLE_WINDOW);
    return tattle_window_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_shiny_flare(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_SHINY_FLARE);
    return shiny_flare_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_huff_puff_breath(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7) {
    load_effect(EFFECT_HUFF_PUFF_BREATH);
    return huff_puff_breath_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

EffectInstance* fx_cold_breath(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_COLD_BREATH);
    return cold_breath_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_embers(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7, s32 arg8, f32 arg9, f32 arg10) {
    load_effect(EFFECT_EMBERS);
    return embers_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
}

EffectInstance* fx_hieroglyphs(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_HIEROGLYPHS);
    return hieroglyphs_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_misc_particles(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7, s32 arg8) {
    load_effect(EFFECT_MISC_PARTICLES);
    return misc_particles_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
}

EffectInstance* fx_static_status(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6) {
    load_effect(EFFECT_STATIC_STATUS);
    return static_status_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
}

EffectInstance* fx_moving_cloud(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8) {
    load_effect(EFFECT_MOVING_CLOUD);
    return moving_cloud_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
}

EffectInstance* fx_effect_75(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_75);
    return effect_75_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_firework_rocket(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, s32 arg8) {
    load_effect(EFFECT_FIREWORK_ROCKET);
    return firework_rocket_main(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
}

EffectInstance* fx_peach_star_beam(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_PEACH_STAR_BEAM);
    return peach_star_beam_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_chapter_change(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_CHAPTER_CHANGE);
    return chapter_change_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_ice_shard(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_ICE_SHARD);
    return ice_shard_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_spirit_card(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_SPIRIT_CARD);
    return spirit_card_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_lil_oink(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_LIL_OINK);
    return lil_oink_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_something_rotating(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_SOMETHING_ROTATING);
    return something_rotating_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_breaking_junk(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_BREAKING_JUNK);
    return breaking_junk_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_partner_buff(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_PARTNER_BUFF);
    return partner_buff_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_quizmo_assistant(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_QUIZMO_ASSISTANT);
    return quizmo_assistant_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_ice_pillar(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_ICE_PILLAR);
    return ice_pillar_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_sun(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_SUN);
    return sun_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_star_spirits_energy(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_STAR_SPIRITS_ENERGY);
    return star_spirits_energy_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_pink_sparkles(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5) {
    load_effect(EFFECT_PINK_SPARKLES);
    return pink_sparkles_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_star_outline(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_STAR_OUTLINE);
    return star_outline_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

EffectInstance* fx_effect_86(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    load_effect(EFFECT_86);
    return effect_86_main(arg0, arg1, arg2, arg3, arg4, arg5);
}

/* fx_sun_undeclared — special sun effect not in the standard EFFECT_DEF table */
/* Signature from effects.h: void fx_sun_undeclared(s32, s32, s32, s32, s32, s32) */
void fx_sun_undeclared(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    /* The "undeclared" sun effect — likely fx_sun with different parameters.
     * Stub for now; will implement properly when the effect system is functional. */
    (void)arg0; (void)arg1; (void)arg2; (void)arg3; (void)arg4; (void)arg5;
}
