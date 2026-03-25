/**
 * dead_stubs.c - Symbol aliases for dead_* symbols
 *
 * In the Paper Mario 64 decomp, files in src/world/dead/ include dead.h which
 * #defines common function/variable names to dead_* versions (e.g.,
 * #define evt_get_variable dead_evt_get_variable). On N64, the linker resolves
 * these as separate overlay symbols. On PC, everything is statically linked,
 * so we need dead_* symbols to point to the same definitions as the originals.
 *
 * macOS does not support __attribute__((alias(...))), so we use assembler
 * .set directives to create true symbol aliases at the object file level.
 * This works for both functions and variables without needing their types.
 *
 * The leading underscore is required on macOS (Mach-O name mangling).
 * On Linux/ELF, symbols don't have a leading underscore, so we use a
 * platform macro to handle both.
 */

#ifdef __APPLE__
#define SYM(name) "_" #name
#else
#define SYM(name) #name
#endif

#define DEAD_ALIAS(original) \
    __asm__(".globl " SYM(dead_##original) "\n" \
            ".set " SYM(dead_##original) ", " SYM(original) "\n");

// =============================================================================
// Global variables
// =============================================================================

// Camera
DEAD_ALIAS(gCameras)
DEAD_ALIAS(gCurrentCameraID)
DEAD_ALIAS(gCurrentCamID)

// Player
DEAD_ALIAS(gPlayerStatusPtr)
DEAD_ALIAS(gPlayerStatus)
DEAD_ALIAS(gPlayerData)

// Game state
DEAD_ALIAS(gGameStatusPtr)
DEAD_ALIAS(gItemTable)
DEAD_ALIAS(gOverrideFlags)
DEAD_ALIAS(gCollisionStatus)
DEAD_ALIAS(gCurrentEncounter)
DEAD_ALIAS(gPartnerStatus)
DEAD_ALIAS(gPartnerPopupProperties)

// Graphics
DEAD_ALIAS(gMainGfxPos)
DEAD_ALIAS(gMatrixListPos)
DEAD_ALIAS(gDisplayContext)
DEAD_ALIAS(nuGfxZBuffer)
DEAD_ALIAS(nuGfxCfb_ptr)

// Partner HUD
DEAD_ALIAS(wPartnerHudScripts)
DEAD_ALIAS(wDisabledPartnerHudScripts)

// =============================================================================
// Core engine functions
// =============================================================================

// Event system
DEAD_ALIAS(evt_get_variable)
DEAD_ALIAS(evt_set_variable)
DEAD_ALIAS(evt_get_float_variable)
DEAD_ALIAS(evt_set_float_variable)

// Math
DEAD_ALIAS(sin_rad)
DEAD_ALIAS(cos_rad)
DEAD_ALIAS(sin_deg)
DEAD_ALIAS(cos_deg)
DEAD_ALIAS(sin_cos_deg)
DEAD_ALIAS(clamp_angle)
DEAD_ALIAS(dist2D)
DEAD_ALIAS(dist3D)
DEAD_ALIAS(update_lerp)
DEAD_ALIAS(add_vec2D_polar)
DEAD_ALIAS(get_clamped_angle_diff)

// atan2 is renamed to pm_atan2 via macro in common.h
DEAD_ALIAS(pm_atan2)

// Random
DEAD_ALIAS(rand_int)

// NPC functions
DEAD_ALIAS(get_npc_unsafe)
DEAD_ALIAS(get_npc_safe)
DEAD_ALIAS(get_npc_by_index)
DEAD_ALIAS(get_enemy)
DEAD_ALIAS(npc_move_heading)
DEAD_ALIAS(npc_raycast_down_sides)
DEAD_ALIAS(npc_test_move_simple_with_slipping)
DEAD_ALIAS(npc_test_move_simple_without_slipping)
DEAD_ALIAS(set_npc_yaw)
DEAD_ALIAS(disable_npc_shadow)
DEAD_ALIAS(is_point_outside_territory)

// AI functions
DEAD_ALIAS(basic_ai_check_player_dist)
DEAD_ALIAS(basic_ai_suspend)
DEAD_ALIAS(basic_ai_wander_init)
DEAD_ALIAS(basic_ai_wander)
DEAD_ALIAS(basic_ai_loiter_init)
DEAD_ALIAS(basic_ai_loiter)
DEAD_ALIAS(basic_ai_found_player_jump_init)
DEAD_ALIAS(basic_ai_found_player_jump)
DEAD_ALIAS(basic_ai_chase_init)
DEAD_ALIAS(basic_ai_chase)
DEAD_ALIAS(basic_ai_lose_player)
DEAD_ALIAS(ai_enemy_play_sound)

// Effects
DEAD_ALIAS(fx_sparkles)
DEAD_ALIAS(fx_emote)
DEAD_ALIAS(fx_star_spirits_energy)
DEAD_ALIAS(fx_spirit_card)
DEAD_ALIAS(fx_energy_orb_wave)
DEAD_ALIAS(fx_radial_shimmer)
DEAD_ALIAS(fx_sun)
DEAD_ALIAS(fx_sun_undeclared)
DEAD_ALIAS(fx_motion_blur_flame)
DEAD_ALIAS(fx_misc_particles)

// Texture panning
DEAD_ALIAS(set_main_pan_u)
DEAD_ALIAS(set_main_pan_v)
DEAD_ALIAS(set_aux_pan_u)
DEAD_ALIAS(set_aux_pan_v)

// Screen overlay
DEAD_ALIAS(set_screen_overlay_params_back)
DEAD_ALIAS(set_screen_overlay_params_front)
DEAD_ALIAS(set_screen_overlay_color)

// Sound
DEAD_ALIAS(sfx_adjust_env_sound_pos)
DEAD_ALIAS(sfx_play_sound_with_params)
DEAD_ALIAS(sfx_get_spatialized_sound_params)
DEAD_ALIAS(snd_ambient_fade_out)

// Memory
DEAD_ALIAS(heap_malloc)
DEAD_ALIAS(heap_free)
DEAD_ALIAS(general_heap_malloc)
DEAD_ALIAS(general_heap_free)

// Rendering/model
DEAD_ALIAS(get_screen_coords)
DEAD_ALIAS(get_model_from_list_index)
DEAD_ALIAS(get_model_list_index_from_tree_index)
DEAD_ALIAS(mdl_get_copied_vertices)
DEAD_ALIAS(mdl_get_copied_gfx)
DEAD_ALIAS(queue_render_task)
DEAD_ALIAS(transform_point)
DEAD_ALIAS(create_shadow_type)
DEAD_ALIAS(create_worker_scene)

// Misc engine functions
DEAD_ALIAS(subtract_hp)
DEAD_ALIAS(get_entity_by_index)
DEAD_ALIAS(get_item_entity)
DEAD_ALIAS(set_message_images)
DEAD_ALIAS(set_message_text_var)
DEAD_ALIAS(get_current_map_settings)
DEAD_ALIAS(switch_to_partner)
DEAD_ALIAS(partner_clear_player_tracking)
DEAD_ALIAS(spawn_surface_effects)
DEAD_ALIAS(increment_status_bar_disabled)
DEAD_ALIAS(decrement_status_bar_disabled)
DEAD_ALIAS(sync_status_bar)
DEAD_ALIAS(set_max_star_power)
DEAD_ALIAS(decode_yay0)
DEAD_ALIAS(load_asset_by_name)
DEAD_ALIAS(dma_copy)
DEAD_ALIAS(entity_upgrade_block_hide_content)
DEAD_ALIAS(create_standard_popup_menu)
DEAD_ALIAS(hide_popup_menu)
DEAD_ALIAS(destroy_popup_menu)

// OS / libultra functions
DEAD_ALIAS(osVirtualToPhysical)

// GU matrix functions
DEAD_ALIAS(guTranslateF)
DEAD_ALIAS(guMtxCatF)
DEAD_ALIAS(guMtxIdentF)
DEAD_ALIAS(guMtxF2L)

// Unnamed/address-based functions from decomp
DEAD_ALIAS(func_800EB2A4)
DEAD_ALIAS(func_80044238)
DEAD_ALIAS(func_802CF56C)
DEAD_ALIAS(func_802D2C14)

// PlayEffect
DEAD_ALIAS(PlayEffect_impl)

// =============================================================================
// Script API callable functions (API_CALLABLE / EVT commands)
// =============================================================================

DEAD_ALIAS(GetEntryID)
DEAD_ALIAS(SetMusic)
DEAD_ALIAS(FadeInMusic)
DEAD_ALIAS(FadeOutMusic)
DEAD_ALIAS(RandInt)
DEAD_ALIAS(DisablePlayerInput)
DEAD_ALIAS(ModifyColliderFlags)
DEAD_ALIAS(PlayerMoveTo)
DEAD_ALIAS(InterpPlayerYaw)
DEAD_ALIAS(PlaySoundAtCollider)
DEAD_ALIAS(MakeLerp)
DEAD_ALIAS(UpdateLerp)
DEAD_ALIAS(RotateGroup)
DEAD_ALIAS(GotoMap)
DEAD_ALIAS(GotoMapSpecial)
DEAD_ALIAS(UseExitHeading)
DEAD_ALIAS(GetLoadType)
DEAD_ALIAS(SetSpriteShading)
DEAD_ALIAS(SetCamLeadPlayer)
DEAD_ALIAS(SetCamPerspective)
DEAD_ALIAS(SetCamBGColor)
DEAD_ALIAS(SetCamEnabled)
DEAD_ALIAS(MakeNpcs)
DEAD_ALIAS(ParentColliderToModel)
DEAD_ALIAS(EnableTexPanning)
DEAD_ALIAS(SetTexPanOffset)
DEAD_ALIAS(DisablePlayerPhysics)
DEAD_ALIAS(UseSettingsFrom)
DEAD_ALIAS(SetPanTarget)
DEAD_ALIAS(SetCamDistance)
DEAD_ALIAS(SetCamPitch)
DEAD_ALIAS(SetCamPosA)
DEAD_ALIAS(SetCamPosB)
DEAD_ALIAS(SetCamPosC)
DEAD_ALIAS(SetCamSpeed)
DEAD_ALIAS(SetCamTarget)
DEAD_ALIAS(SetCamType)
DEAD_ALIAS(SetCamProperties)
DEAD_ALIAS(PanToTarget)
DEAD_ALIAS(WaitForCam)
DEAD_ALIAS(AdjustCam)
DEAD_ALIAS(ResetCam)
DEAD_ALIAS(ShowMessageAtWorldPos)
DEAD_ALIAS(ShowMessageAtScreenPos)
DEAD_ALIAS(GetPlayerPos)
DEAD_ALIAS(SetPlayerJumpscale)
DEAD_ALIAS(PlayerJump1)
DEAD_ALIAS(PlayerJump)
DEAD_ALIAS(DisablePartnerAI)
DEAD_ALIAS(EnablePartnerAI)
DEAD_ALIAS(InterpNpcYaw)
DEAD_ALIAS(SpeakToPlayer)
DEAD_ALIAS(SpeakToNpc)
DEAD_ALIAS(SetPlayerAnimation)
DEAD_ALIAS(ContinueSpeech)
DEAD_ALIAS(GetPlayerActionState)
DEAD_ALIAS(SetPlayerActionState)
DEAD_ALIAS(ShowChoice)
DEAD_ALIAS(SwitchMessage)
DEAD_ALIAS(CloseMessage)
DEAD_ALIAS(EndSpeech)
DEAD_ALIAS(SetNpcPos)
DEAD_ALIAS(SetNpcYaw)
DEAD_ALIAS(SetNpcSpeed)
DEAD_ALIAS(SetNpcJumpscale)
DEAD_ALIAS(SetNpcAnimation)
DEAD_ALIAS(SetNpcRotation)
DEAD_ALIAS(SetNpcRotationPivot)
DEAD_ALIAS(SetNpcCollisionSize)
DEAD_ALIAS(SetNpcFlagBits)
DEAD_ALIAS(SetNpcVar)
DEAD_ALIAS(SetNpcAux)
DEAD_ALIAS(SetNpcImgFXParams)
DEAD_ALIAS(SetNpcPaletteSwapMode)
DEAD_ALIAS(SetNpcPaletteSwapping)
DEAD_ALIAS(SetNpcScale)
DEAD_ALIAS(GetNpcPos)
DEAD_ALIAS(GetNpcYaw)
DEAD_ALIAS(GetNpcPointer)
DEAD_ALIAS(NpcMoveTo)
DEAD_ALIAS(NpcJump0)
DEAD_ALIAS(NpcJump1)
DEAD_ALIAS(NpcFlyTo)
DEAD_ALIAS(NpcFacePlayer)
DEAD_ALIAS(NpcFaceNpc)
DEAD_ALIAS(PlayerFaceNpc)
DEAD_ALIAS(FacePlayerTowardPoint)
DEAD_ALIAS(EnableGroup)
DEAD_ALIAS(EnableModel)
DEAD_ALIAS(EnableNpcShadow)
DEAD_ALIAS(EnableNpcAI)
DEAD_ALIAS(BindNpcInteract)
DEAD_ALIAS(BindNpcIdle)
DEAD_ALIAS(BindNpcAI)
DEAD_ALIAS(BindNpcAux)
DEAD_ALIAS(BindNpcDefeat)
DEAD_ALIAS(BindNpcHit)
DEAD_ALIAS(WaitForPlayerInputEnabled)
DEAD_ALIAS(PlaySoundAtNpc)
DEAD_ALIAS(PlaySoundAtPlayer)
DEAD_ALIAS(PlaySoundAtModel)
DEAD_ALIAS(PlaySoundAt)
DEAD_ALIAS(PlaySound)
DEAD_ALIAS(StopSound)
DEAD_ALIAS(StopTrackingSoundPos)
DEAD_ALIAS(PlayAmbientSounds)
DEAD_ALIAS(GetCurrentPartnerID)
DEAD_ALIAS(SetPlayerPos)
DEAD_ALIAS(SetPlayerSpeed)
DEAD_ALIAS(SetPlayerFlagBits)
DEAD_ALIAS(SetPlayerImgFXFlags)
DEAD_ALIAS(UpdatePlayerImgFX)
DEAD_ALIAS(GetPlayerAnimation)
DEAD_ALIAS(GetPlayerTargetYaw)
DEAD_ALIAS(HidePlayerShadow)
DEAD_ALIAS(RotateModel)
DEAD_ALIAS(TranslateModel)
DEAD_ALIAS(ScaleModel)
DEAD_ALIAS(TranslateGroup)
DEAD_ALIAS(ScaleGroup)
DEAD_ALIAS(UpdateColliderTransform)
DEAD_ALIAS(SetGroupVisibility)
DEAD_ALIAS(InterruptUsePartner)
DEAD_ALIAS(RemoveNpc)
DEAD_ALIAS(GetAngleBetweenNPCs)
DEAD_ALIAS(GetAngleToPlayer)
DEAD_ALIAS(AwaitPlayerApproach)
DEAD_ALIAS(AwaitPlayerLeave)
DEAD_ALIAS(BringPartnerOut)
DEAD_ALIAS(PutPartnerAway)
DEAD_ALIAS(ClearPartnerMoveHistory)
DEAD_ALIAS(SetTimeFreezeMode)
DEAD_ALIAS(ShowKeyChoicePopup)
DEAD_ALIAS(ShowConsumableChoicePopup)
DEAD_ALIAS(CloseChoicePopup)
DEAD_ALIAS(RemoveKeyItemAt)
DEAD_ALIAS(RemoveItemAt)
DEAD_ALIAS(MakeItemEntity)
DEAD_ALIAS(RemoveItemEntity)
DEAD_ALIAS(DropItemEntity)
DEAD_ALIAS(IsPlayerWithin)
DEAD_ALIAS(IsPlayerOnValidFloor)
DEAD_ALIAS(GetPartnerInUse)
DEAD_ALIAS(MakeEntity)
DEAD_ALIAS(PopSong)
DEAD_ALIAS(PushSong)
DEAD_ALIAS(ShowGotItem)
DEAD_ALIAS(GetBattleOutcome)
DEAD_ALIAS(DoNpcDefeat)
DEAD_ALIAS(ShowEmote)
DEAD_ALIAS(SetEnemyFlagBits)
DEAD_ALIAS(SetSelfEnemyFlagBits)
DEAD_ALIAS(AddKeyItem)
DEAD_ALIAS(FindKeyItem)
DEAD_ALIAS(FindItem)
DEAD_ALIAS(AddStarPieces)
DEAD_ALIAS(GetValueByRef)
DEAD_ALIAS(SetValueByRef)
DEAD_ALIAS(AssignPanelFlag)
DEAD_ALIAS(AssignBlockFlag)
DEAD_ALIAS(AssignScript)
DEAD_ALIAS(ShakeCam)
DEAD_ALIAS(GetCamDistance)
DEAD_ALIAS(GetCamPitch)
DEAD_ALIAS(IsStartingConversation)
DEAD_ALIAS(ModifyGlobalOverrideFlags)
DEAD_ALIAS(SetEncounterStatusFlags)
DEAD_ALIAS(OnPlayerFled)
DEAD_ALIAS(StartBossBattle)
DEAD_ALIAS(StartBattle)
DEAD_ALIAS(SetSelfVar)
DEAD_ALIAS(GetSelfVar)
DEAD_ALIAS(GetSelfNpcID)
DEAD_ALIAS(GetOwnerEncounterTrigger)
DEAD_ALIAS(GetSelfAnimationFromTable)
DEAD_ALIAS(SelfEnemyOverrideSyncPos)
DEAD_ALIAS(GetModelCenter)
DEAD_ALIAS(SetItemPos)
DEAD_ALIAS(LoadPath)
DEAD_ALIAS(GetNextPathPos)
DEAD_ALIAS(CloneModel)
DEAD_ALIAS(MakeLocalVertexCopy)
DEAD_ALIAS(SetModelCustomGfx)
DEAD_ALIAS(SetCustomGfxBuilders)
DEAD_ALIAS(SetModelFlags)
DEAD_ALIAS(SetTexPanner)
DEAD_ALIAS(DismissEffect)
DEAD_ALIAS(CreatePushBlockGrid)
DEAD_ALIAS(SetPushBlock)
DEAD_ALIAS(PlayModelAnimation)
DEAD_ALIAS(SetAnimatedModelRootPosition)
DEAD_ALIAS(LoadAnimatedModel)
DEAD_ALIAS(SetAnimatorFlags)
DEAD_ALIAS(GetAnimatedPositionByTreeIndex)
DEAD_ALIAS(GetAnimatedRotationByTreeIndex)
DEAD_ALIAS(MakeTransformGroup)

// =============================================================================
// EvtScript globals (referenced from script data)
// =============================================================================

DEAD_ALIAS(EVS_NpcHitRecoil)
DEAD_ALIAS(EnterSavePoint)
DEAD_ALIAS(EnterWalk)
DEAD_ALIAS(ExitWalk)
DEAD_ALIAS(EnemyNpcHit)
DEAD_ALIAS(EnemyNpcDefeat)
DEAD_ALIAS(ResetFromLava)
DEAD_ALIAS(BasicAI_Main)

// =============================================================================
// EntityBlueprint globals
// =============================================================================

DEAD_ALIAS(Entity_SavePoint)
DEAD_ALIAS(Entity_BombableRock)
DEAD_ALIAS(Entity_MulticoinBlock)
DEAD_ALIAS(Entity_Chest)
DEAD_ALIAS(Entity_HiddenPanel)
DEAD_ALIAS(Entity_SuperBlock)
DEAD_ALIAS(Entity_ScriptSpring)
DEAD_ALIAS(Entity_HiddenYellowBlock)
DEAD_ALIAS(Entity_YellowBlock)
DEAD_ALIAS(Entity_Hammer3Block)
DEAD_ALIAS(Entity_HeartBlock)
DEAD_ALIAS(Entity_BrickBlock)
DEAD_ALIAS(Entity_SimpleSpring)
