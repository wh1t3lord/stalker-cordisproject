#pragma once
// XXX: Identify what parts of xrGame requires only "core", ui, ai, and finally script.
// Split up source (PCH-wise) accordingly.
// This will likely for a long time be WIP.

#include "Common/Common.hpp"

#include "xrEngine/stdafx.h" // XXX: This seems bad. PCH's are for internal (building) use.
#if !defined(LINUX)
#include "DPlay/dplay8.h"
#endif
// xrEngine src file count is ~1100.
// Comments following individual includes refers to number of times they are included in xrEngine as a whole.
//#include <assert.h> // ~440 - but it has no include guard! Perhaps that's intentional?
#include <math.h>
#include <queue> // ~360
#include "luabind/luabind.hpp" // luabind/*, almost 5000
#include "xrServerEntities/smart_cast.h" // a lot
#include "xrScriptEngine/script_space_forward.hpp" // ~765 // XXX: See to it this goes to pch_script
#include "xrScriptEngine/DebugMacros.hpp" // ~700 // XXX: See to it this goes to pch_script
#include "Common/LevelStructure.hpp" // ~730
#include "xrCommon/misc_math_types.h" // ~770
#include "xrEngine/ISheduled.h" // ~740
#include "xrCDB/ISpatial.h" // ~700
#include "xrCore/xrPool.h" // ~700
#include "xrEngine/ICollidable.h" // ~700
#include "xrEngine/IObjectPhysicsCollision.h" // ~700
#include "xrEngine/IRenderable.h" // ~700
#include "xrEngine/xr_object.h" // ~700
#include "xrEngine/PS_instance.h" // ~650
#include "xrPhysics/IPhysicsShellHolder.h" // ~640
#include "Level.h" // ~550
#include "Common/GUID.hpp" // ~530
#include "Common/object_broker.h" // ~500
#include "Common/object_cloner.h" // ~500
#include "Common/object_comparer.h" // ~500
#include "Common/object_destroyer.h" // ~500
#include "Common/object_loader.h" // ~500
#include "Common/object_saver.h" // ~500
#include "Include/xrRender/animation_blend.h" // ~500
#include "Include/xrRender/animation_motion.h" // ~500
#include "Kinematics.h" // ~360
#include "KinematicsAnimated.h" // ~500
#include "RenderVisual.h" // ~370
#include "UIRender.h" // ~450
#include "UIShader.h" // ~490
#include "xrCore/_plane2.h" // ~450
#include "xrAICore/AISpaceBase.hpp" // ~650
#include "xrAICore/Navigation/game_graph.h" // ~600
#include "xrServerEntities/xrServer_Objects.h" // ~500
#include "xrServerEntities/xrServer_Objects_ALife.h" // ~500
#include "xrServerEntities/xrServer_Objects_ALife_Items.h" // ~500
#include "xrGame/Entity.h" // ~490
#include "xrGame/damage_manager.h" // ~490
#include "xrGame/EntityCondition.h" // ~490
#include "xrUICore/ui_defs.h" // ~450
#include "xrGame/entity_alive.h" // ~430
#include "xrCore/XML/XMLDocument.hpp" // ~400
#include "xrPhysics/xrPhysics.h" // ~400
#include "xrEngine/Feel_Sound.h" // ~400
#include "xrAICore/Navigation/graph_edge.h" // ~380
#include "xrAICore/Navigation/graph_abstract.h" // ~380
#include "loki/EmptyType.h" // ~380
#include "loki/NullType.h" // only ~50, so small it's OK.
#include "xrPhysics/PhysicsShell.h" // ~350
#include "xrServerEntities/ShapeData.h" // ~330
#include "xrScriptEngine/ScriptExporter.hpp" // ~330 // XXX: See to it this goes to pch_script
#include "xrServerEntities/specific_character.h" // ~330
#include "xrServerEntities/shared_data.h" // ~330
#include "xrServerEntities/xml_str_id_loader.h" // ~330
#include "xrServerEntities/character_info.h" // ~320
#include "xrServerEntities/ai_sounds.h" // ~320
#include "xrGame/attachment_owner.h" // ~320
#include "xrGame/step_manager.h" // ~370
#include "xrGame/physic_item.h" // ~330
#include "xrGame/pda_space.h" // ~330
#include "xrGame/PhraseDialogManager.h" // ~300
#include "xrGame/script_entity.h" // ~290
#include "xrEngine/Feel_Vision.h" // ~270
#include "xrGame/CustomMonster.h" // ~265
#include "xrUICore/ui_base.h" // ~260
#include "xrPhysics/MathUtils.h" // ~260
#include "xrGame/WeaponAmmo.h" // ~250
#include "xrEngine/GameFont.h" // ~250
#include "xrGame/detail_path_manager.h" // ~120 + 120 for its inlines
#include "xrPhysics/MovementBoxDynamicActivate.h" // ~240
#include "xrPhysics/PHItemList.h"
#include "xrUICore/EditBox/UIEditBox.h"
#include "xrUICore/Windows/UIWindow.h" // ~225
#include "xrUICore/UIMessages.h" // ~225  one single enum
#include "xrScriptEngine/Functor.hpp" // ~225 // XXX: See to it this goes to pch_script
#include "xrGame/fire_disp_controller.h" // ~220
#include "xrGame/Actor.h" // ~220
#include "xrEngine/CameraDefs.h" // ~210
#include "xrEngine/CameraManager.h" // ~190
#include "xrCore/PostProcess/PPInfo.hpp" // ~190
#include "xrScriptEngine/script_engine.hpp" // only ~200, VERY heavy! // XXX: See to it this goes to pch_script
#include "xrCore/_fbox2.h" // ~155
#include "xrCore/_vector3d_ext.h"
#include "xrCore/buffer_vector.h"
#include "xrCore/Crypto/xr_dsa_signer.h" // ~70, very heavy to compile
#include "xrCore/Crypto/xr_dsa_verifyer.h" // ~70, very heavy to compile
#include "xrServerEntities/restriction_space.h" // only ~110, so small it's worth it
#include "xrAICore/Components/condition_state.h" // only ~100, but it includes more
//#include "xrScriptEngine/script_engine.hpp" // ~210 // XXX: See to it this goes to pch_script
//#include "xrCore/dump_string.h" // ~260
//#include "xrCore/Math/Random32.hpp" // ~220
//#include "xrEngine/LightAnimLibrary.h" // ~200
#include "xrUICore/Cursor/UICursor.h"
#include "xrGame/UIDialogHolder.h" // ~135, somewhat heavy to compile
#include "xrUICore/Static/UIStaticItem.h" // ~190, and quite heavy to compile
#include "xrGame/ui/Restrictions.h"
#include "xrUICore/InteractiveBackground/UI_IB_Static.h" // ~60, very heavy to compile
#include "xrUICore/Buttons/UI3tButton.h" // ~60, very heavy to compile
#include "xrGame/ui/UIActorMenu.h"
#include "xrUICore/Buttons/UIButton.h" // ~80, very heavy to compile
#include "xrGame/ui/UIBuyWndBase.h"
#include "xrGame/ui/UIBuyWndShared.h"
#include "xrGame/ui/UICellItem.h"
#include "xrUICore/EditBox/UICustomEdit.h"
#include "xrGame/ui/UIDialogWnd.h" // ~100
#include "xrGame/ui/UIDragDropListEx.h"
#include "xrUICore/Windows/UIFrameWindow.h"
#include "xrUICore/Hint/UIHint.h"
#include "xrGame/ui/UIInventoryUtilities.h"
#include "xrUICore/Static/UILanimController.h" // ~190
#include "xrUICore/Lines/UILine.h" // ~190
#include "xrUICore/Lines/UILines.h" // ~190
#include "xrUICore/ListBox/UIListBox.h"
#include "xrUICore/Options/UIOptionsItem.h" // ~60, somewhat heavy to compile
//#include "xrUICore/Options/UIOptionsManager.h" // ~60, somewhat heavy to compile
#include "xrUICore/ProgressBar/UIProgressBar.h" // ~23, very heavy to compile
#include "xrUICore/ScrollView/UIScrollView.h" // ~70, quite heavy to compile
#include "xrUICore/Static/UIStatic.h" // ~190
#include "xrUICore/Lines/UISubLine.h" // ~190
#include "xrUICore/Callbacks/UIWndCallback.h" // 135, somewhat heavy to compile
#include "xrGame/ui/UIXmlInit.h" // ~105, somewhat heavy to compile
#include "xrGame/smart_cover.h" // ~30, VERY heavy to compile
#include "xrGame/team_hierarchy_holder.h" // ~35, but quite heavy to compile
#include "xrGame/Tracer.h" // ~60, somewhat heavy to compile
//#include "xrCore/_fbox2.h" // ~155
#include "xr_time.h" // only ~125, includes "alife_space.h" !
#include "xrServerEntities/alife_movement_manager_holder.h" // only ~120, small and self-contained
#include "xrServerEntities/xrServer_Objects_ALife_Monsters.h" // only ~120, very heavy to compile
#include "xrServerEntities/clsid_game.h" // only ~20, only macros
//#include "xrServerEntities/xrServer_Objects_ALife_Monsters.h" // ~120
#include "xrPhysics/PHUpdateObject.h" // ~95 - includes PHItemList.h
//#include "xrPhysics/PHItemList.h" // ~110 - template class, so better include in pch
#include "xrGame/Weapon.h" // ~125, VERY heavy to compile
#include "xrGame/string_table.h" // ~100, somewhat heavy to compile
#include "xrGame/WeaponMagazined.h" // ~70, VERY heavy to compile
#include "visual_memory_manager.h" // only ~40, quite heavy to compile
#include "xrNetServer/NET_Messages.h" // only ~60, depends on dplay macros
#include "xrPhysics/DamageSource.h" // only ~65, very small
#include "xrPhysics/debug_output.h" // only ~60, quite heavy to compile
#include "xrServerEntities/alife_monster_brain.h" // only ~30, very heavy to compile
#include "xrServerEntities/object_factory.h" // only ~15, very heavy to compile
#include "xrServerEntities/xrServer_Objects_ALife_All.h" // only ~25, very heavy to compile
#include "xrServerEntities/xrServer_script_macroses.h" // only ~13, very heavy to compile
#include "xrEngine/CameraBase.h" // only ~40, pretty heavy to compile
#include "xrEngine/Effector.h" // ~80, very heavy to compile
#include "xrGame/CameraEffector.h" // ~75, very heavy to compile
#include "xrGame/alife_simulator.h" // ~80
#include "xrGame/alife_update_manager.h"
#include "xrGame/ammunition_groups.h"
#include "xrGame/animation_utils.h"
#include "xrGame/Artefact.h" // ~50
#include "xrGame/character_hit_animations.h"
#include "xrGame/character_shell_control.h" // ~65
#include "xrGame/CharacterPhysicsSupport.h" // ~65
#include "xrGame/danger_object.h"
#include "xrGame/Inventory.h" // ~120
#include "xrGame/kills_store.h"
#include "xrGame/MainMenu.h"
#include "xrGame/map_location.h"
#include "xrGame/map_location_defs.h"
#include "xrGame/member_order.h"
#include "xrGame/memory_manager.h"
#include "xrGame/Missile.h"
#include "xrGame/movement_manager_space.h"
#include "xrGame/object_manager.h"
#include "xrGame/PHDebug.h"
#include "xrGame/PHDestroyable.h"
#include "xrGame/PHDestroyableNotificate.h"
#include "xrGame/PHSkeleton.h"
#include "xrGame/property_evaluator.h"
#include "xrGame/property_evaluator_const.h"
#include "xrGame/quadtree.h"
#include "xrGame/Random.hpp" // ~150
#include "xrScriptEngine/script_callback_ex.h" // ~120, VERY heavy to compile
#include "xrGame/seniority_hierarchy_holder.h"
#include "xrGame/setup_manager.h"
#include "xrGame/sight_action.h"
#include "xrGame/sight_manager.h"
#include "xrGame/smart_cover.h"
#include "xrGame/smart_cover_animation_planner.h"
#include "xrGame/sound_player.h"
#include "xrGame/space_restrictor.h"
#include "xrGame/squad_hierarchy_holder.h"
#include "xrGame/stalker_animation_manager.h"
#include "xrGame/stalker_animation_pair.h"
#include "xrGame/stalker_animation_script.h"
#include "xrGame/stalker_planner.h"
#include "xrGame/static_obstacles_avoider.h" // ~60, VERY heavy to compile
#include "xrGame/stalker_movement_manager_obstacles.h" // ~60, INCREDIBLY heavy to compile
#include "xrGame/stalker_movement_manager_smart_cover.h" // ~60, INCREDIBLY heavy to compile
#include "xrGame/stalker_base_action.h" // ~30, INCREDIBLY heavy to compile
#include "xrGame/stalker_decision_space.h" // ~60, only enum's
#include "xrGame/Spectator.h" // ~60, INCREDIBLY heavy to compile
#include "xrGame/game_cl_mp.h" // ~60, INCREDIBLY heavy to compile
#include "xrGame/game_events_handler.h" // ~30, INCREDIBLY heavy to compile
#include "xrGame/steering_behaviour.h" // ~30, measurable compile time
#include "xr_level_controller.h"
#include "xrGame/UIGameCustom.h" // ~85, quite heavy to compile
#include "xrGame/UIGameMP.h"
#include "xrGame/UIGameSP.h"
#include "xrGame/wallmark_manager.h" // ~60, measurable compile time
#include "xrGame/WeaponCustomPistol.h" // ~30, INCREDIBLY heavy to compile
#include "xrGame/wrapper_abstract.h"
#include "xrGame/ai/monsters/ai_monster_utils.h" // ~130, quite heavy to compile
#include "xrGame/ai/monsters/control_animation.h"
#include "xrGame/ai/monsters/control_animation_base.h"
#include "xrGame/ai/monsters/control_com_defs.h"
#include "xrGame/ai/monsters/control_combase.h" // ~130
#include "xrGame/ai/monsters/control_direction.h" // ~130
#include "xrGame/ai/monsters/control_jump.h" // ~130
#include "xrGame/ai/monsters/control_manager.h" // ~130
#include "xrGame/ai/monsters/control_manager_custom.h" // ~130
#include "xrGame/ai/monsters/control_melee_jump.h" // ~130
#include "xrGame/ai/monsters/control_movement.h" // ~130
#include "xrGame/ai/monsters/control_path_builder.h" // ~130
#include "xrGame/ai/monsters/monster_corpse_manager.h" // ~130
#include "xrGame/ai/stalker/ai_stalker.h"

#include <d3dx9.h>

#include <D3D11.h>
#include <D3Dx11core.h>
#include <D3DCompiler.h>

#include "HW.h"
#include "Shader.h"
#include "R_Backend.h"
#include "R_Backend_Runtime.h"
#include "ResourceManager.h"
#include "Blender.h"
#include "Blender_CLSID.h"
#include "xrRender_console.h"
#include "r4.h"

#include "xrCore/Imgui/imgui.h"
#include "xrCore/Imgui/imgui_impl_sdl.h"
#include "xrCore/Imgui/imgui_impl_dx11.h"

#include "EditorPref.h"
#include "SDK_Names.h"
#include "SDKUI.h"

#include "WallMarkArray.h" // ~80
//#include "xrAICore/Navigation/ai_object_location.h" // ~95, very heavy to compile
#include "xrAICore/Navigation/graph_engine.h" // ~80, VERY heavy to compile
#include "xrAICore/Navigation/PatrolPath/patrol_path.h" // ~80, VERY heavy to compile

#ifdef DEBUG
#include "DebugRender.h"
#endif

#define R_GL 0
#define R_R1 1
#define R_R2 2
#define R_R3 3
#define R_R4 4
#define RENDER R_R4

IC void jitter(CBlender_Compile& C)
{
    //	C.r_Sampler	("jitter0",	JITTER(0), true, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT);
    //	C.r_Sampler	("jitter1",	JITTER(1), true, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT);
    //	C.r_Sampler	("jitter2",	JITTER(2), true, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT);
    //	C.r_Sampler	("jitter3",	JITTER(3), true, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT);
    C.r_dx10Texture("jitter0", JITTER(0));
    C.r_dx10Texture("jitter1", JITTER(1));
    C.r_dx10Texture("jitter2", JITTER(2));
    C.r_dx10Texture("jitter3", JITTER(3));
    C.r_dx10Texture("jitter4", JITTER(4));
    C.r_dx10Texture("jitterMipped", r2_jitter_mipped);
    C.r_dx10Sampler("smp_jitter");
}

using AStringVec = xr_vector<xr_string>;
using AStringIt = AStringVec::iterator;

using LPAStringVec = xr_vector<xr_string*>;
using LPAStringIt = LPAStringVec::iterator;

inline float CalcSSA(Fvector& C, float R)
{
    float distSQ = Device.vCameraPosition.distance_to_sqr(C);
    return R * R / distSQ;
}
// Lord: Понять как это используется и улучшить эти возможности
enum
{
    rsFilterLinear = (1ul << 20ul),
    rsEdgedFaces = (1ul << 21ul),
    rsRenderTextures = (1ul << 22ul),
    rsLighting = (1ul << 23ul),
    rsFog = (1ul << 24ul),
    rsRenderRealTime = (1ul << 25ul),
    rsDrawGrid = (1ul << 26ul),
    rsDrawSafeRect = (1ul << 27ul),
    rsMuteSounds = (1ul << 28ul),
    rsEnvironment = (1ul << 29ul),
};
#include "physics_shell_scripted.h"
#include "physics_joint_scripted.h"
#include "script_object_action.h"
#include "script_entity_action.h"
#include "script_animation_action.h"
#include "script_movement_action.h"
#include "script_action_condition.h"
#include "ScriptXMLInit.h"
#include "script_sound_action.h"
#include "script_ini_file.h"
#include "script_hit.h"
#include "xrEngine/IGame_Level.h"
#include "ai_space.h"
#include "date_time.h"
#include "alife_simulator_base.h"
#include "alife_object_registry.h"
#include "alife_graph_registry.h"
#include "alife_human_brain.h"
#include "alife_time_manager.h"
#include "relation_registry.h"
#include "InfoPortionDefs.h"
#include "xrServer_Objects_ALife_All.h"
#include "xrServer_Objects_Alife_Smartcovers.h"
#include "script_movement_action.h"
#include "script_monster_hit_info.h"
#include "script_effector.h"
#include "xr_time.h"
#include <random>
#include "Script_GlobalDefinitions.h"
#include "Script_Globals.h"
#include "xrAICore/AISpaceBase.hpp"
#include "xrAICore/Navigation/PatrolPath/patrol_path_storage.h"
#include "xrAICore/Navigation/PatrolPath/patrol_path_params.h"
#include "Level.h"
#include "ui/UIGameTutorial.h"
#include "map_manager.h"
#include "xrAICore/Navigation/game_graph.h"
#include "script_ini_file.h"
#include "Script_Animpoint.h"
#include "Script_Database.h"
#include "Script_SE_SimulationSquad.h"
#include "PhraseDialog.h"
#include "Helicopter.h"
#include "PostprocessAnimator.h"
#include "ActorEffector.h"
#include "Script_DialogManager.h"
#include "Script_PhantomManager.h"
#include "Script_SimulationObjects.h"
#include "Script_EntitySounds.h"
#include "Script_SE_Actor.h"
#include "Script_SE_Monster.h"
#include "Script_SE_SmartCover.h"
#include "Script_SE_Outfit.h"
#include "Script_SE_Artefact.h"
#include "Script_SE_Helmet.h"
#include "Script_SE_Helicopter.h"
#include "Script_SE_Weapon.h"
#include "Script_SE_WeaponShotgun.h"
#include "Script_SE_WeaponAutomaticShotgun.h"
#include "Script_SE_WeaponMagazined.h"
#include "Script_SE_WeaponMagazinedWGL.h"
#include "Script_SE_Item.h"
#include "Script_SE_ItemTorch.h"
#include "Script_SE_Physic.h"
#include "Script_SE_Lamp.h"
#include "Script_SE_Ammo.h"
#include "Script_SE_Grenade.h"
#include "Script_SE_Eatable.h"
#include "Script_SE_InventoryBox.h"
#include "Script_SE_Explosive.h"
#include "Script_SE_PDA.h"
#include "Script_SE_Detector.h"
#include "Script_SE_LevelChanger.h"
#include "Script_SE_NewAttachableItem.h"
#include "Script_SE_Stalker.h"
#include "Script_SE_WeaponAutomaticShotgun.h"
#include "Script_SE_Restrictor.h"
#include "Script_SE_ZoneVisual.h"
#include "Script_SE_ZoneTorrid.h"
#include "Script_SE_ZoneAnomaly.h"
#include "Script_SmartTerrainControl.h"
/*#include "Script_GulagGenerator.h"*/
#include "Script_SE_SmartTerrain.h"
#include "Script_SimulationBoard.h"
#include "Script_LogicManager.h"
#include "Script_TreasureManager.h"
#include "Script_SoundManager.h"
#include "Script_NewsManager.h"
#include "Script_CRD_Wounded.h"
#include "Script_XR_Condition.h"
#include "Script_XR_Gulag.h"
/*#include "Script_XR_Logic.h"*/
#include "Script_XR_Sound.h"
#include "Script_XR_Effects.h"
#include "Script_XR_Meet.h"
#include "Script_XR_Patrol.h"
#include "Script_SurgeManager.h"
#include "Script_EntitySounds.h"
#include "script_binder_object.h"
#include "Script_Binder_Actor.h"
#include "Script_Binder_AnomalField.h"
#include "Script_Binder_AnomalZone.h"
#include "Script_Binder_Artefact.h"
#include "Script_Binder_Camp.h"
#include "Script_Binder_Campfire.h"
#include "Script_Binder_Crow.h"
#include "Script_Binder_DoorLabx8.h"
#include "Script_Binder_Faction.h"
#include "Script_Binder_Helicopter.h"
#include "Script_Binder_LevelChanger.h"
#include "Script_Binder_Monster.h"
#include "Script_Binder_PhysicObject.h"
#include "Script_Binder_Restrictor.h"
#include "Script_Binder_SignalLight.h"
#include "Script_Binder_SmartCover.h"
#include "Script_Binder_SmartTerrain.h"
#include "Script_Binder_Stalker.h"
#include "Script_ISchemeEntity.h"
#include "Script_ISchemeMonster.h"
#include "Script_ISchemeStalker.h"
#include "Script_MobStateManager.h"
#include "Script_SchemeMobWalker.h"
#include "Script_SchemeMobRemark.h"
#include "Script_SchemeMobJump.h"
#include "Script_SchemeMobHome.h"
#include "Script_SchemeMobDeath.h"
#include "Script_SchemeMobCombat.h"
#include "Script_SchemeMobCamp.h"
#include "Script_HelicopterLook.h"
#include "Script_HelicopterFly.h"
#include "Script_HelicopterFire.h"
#include "Script_SchemeHelicopterMove.h"
#include "Script_SchemePHForce.h"
#include "Script_SchemePHButton.h"
#include "Script_SchemePHDeath.h"
#include "Script_SchemePHCode.h"
#include "Script_SchemePHDoor.h"
#include "Script_SchemePHHit.h"
#include "Script_SchemePHIdle.h"
#include "Script_SchemePHMinigun.h"
#include "Script_SchemePHOnHit.h"
#include "Script_SchemePHOscillate.h"
#include "Script_SchemePHSound.h"
#include "Script_SchemeSRTimer.h"
#include "Script_SchemeSRTeleport.h"
#include "Script_SchemeSRSilenceZone.h"
#include "Script_SchemeSRPsyAntenna.h"
#include "Script_SchemeSRPostProcess.h"
#include "Script_SchemeSRParticle.h"
#include "Script_SchemeSRNoWeapon.h"
#include "Script_SchemeSRLight.h"
#include "Script_SchemeSRIdle.h"
#include "Script_SchemeSRDeimos.h"
#include "Script_SchemeXRDeath.h"
#include "Script_Globalsinline.h"
