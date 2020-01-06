#include "stdafx.h"
#include "Script_StateManager.h"
#include "Script_StateEvaluators.h"
#include "Script_StateActions.h"
#include "property_evaluator.h"
#include "xrAICore/Components/condition_state.h"
#include "xrAICore/Components/script_world_property.h"

namespace Cordis
{
namespace Scripts
{
Script_StateManager::Script_StateManager(CScriptGameObject* const p_client_object)
    : m_p_npc(p_client_object), m_is_position_direction_applied(false), m_target_state_name("idle"),
      m_p_action_planner(new CScriptActionPlanner()), m_is_alife(true), m_is_combat(false),
      m_is_point_object_direction(false), m_is_need_reweapon(false), m_look_object_id(0), m_is_fast_set(false),
      m_p_animation(nullptr), m_p_animstate(nullptr)
{
    this->m_p_action_planner->setup(this->m_p_npc);

#pragma region Cordis State Manager Init this->m_properties
    this->m_properties["end"] = 1;
    this->m_properties["locked"] = 2;
    this->m_properties["locked_external"] = 3;
    this->m_properties["weapon"] = 11;
    this->m_properties["weapon_locked"] = 12;
    this->m_properties["weapon_strapped"] = 13;
    this->m_properties["weapon_strapped_now"] = 14;
    this->m_properties["weapon_unstrapped"] = 15;
    this->m_properties["weapon_unstrapped_now"] = 16;
    this->m_properties["weapon_none"] = 17;
    this->m_properties["weapon_none_now"] = 18;
    this->m_properties["weapon_drop"] = 19;
    this->m_properties["weapon_fire"] = 20;
    this->m_properties["movement"] = 21;
    this->m_properties["movement_walk"] = 22;
    this->m_properties["movement_run"] = 23;
    this->m_properties["movement_stand"] = 24;
    this->m_properties["movement_stand_now"] = 25;
    this->m_properties["mental"] = 31;
    this->m_properties["mental_free"] = 32;
    this->m_properties["mental_free_now"] = 33;
    this->m_properties["mental_danger"] = 34;
    this->m_properties["mental_danger_now"] = 35;
    this->m_properties["mental_panic"] = 36;
    this->m_properties["mental_panic_now"] = 37;
    this->m_properties["bodystate"] = 41;
    this->m_properties["bodystate_crouch"] = 42;
    this->m_properties["bodystate_standing"] = 43;
    this->m_properties["bodystate_crouch_now"] = 44;
    this->m_properties["bodystate_standing_now"] = 45;
    this->m_properties["direction"] = 51;
    this->m_properties["direction_search"] = 52;
    this->m_properties["animstate"] = 61;
    this->m_properties["animstate_locked"] = 62;
    this->m_properties["animstate_idle_now"] = 64;
    this->m_properties["animstate_play_now"] = 66;
    this->m_properties["animation"] = 81;
    this->m_properties["animation_locked"] = 82;
    this->m_properties["animation_play_now"] = 84;
    this->m_properties["animation_none_now"] = 86;
    this->m_properties["smartcover_need"] = 90;
    this->m_properties["smartcover"] = 91;
    this->m_properties["in_smartcover"] = 92;

    // Lord: проверить удаляются те Evaluators Actions которые выделил
    // @  берём через .at, потому что если не находит то по сути возвращает ноль но это не обрабатывается, поэтому
    // строгая привязка и сами проверяем что зарегистрировали выше (самопроверка самих же себя)
    this->m_p_action_planner->add_evaluator(
        this->m_properties.at("end"), new Script_EvaluatorStateManagerEnd("state_mgr_end", this));
    this->m_p_action_planner->add_evaluator(
        this->m_properties.at("locked"), new Script_EvaluatorStateManagerLocked("state_mgr_locked", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("locked_external"),
        new Script_EvaluatorStateManagerLockedExternal("state_mgr_locked_external", this));
    this->m_p_action_planner->add_evaluator(
        this->m_properties.at("weapon"), new Script_EvaluatorStateManagerWeapon("state_mgr_weapon", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("weapon_locked"),
        new Script_EvaluatorStateManagerWeaponLocked("state_mgr_weapon_locked", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("weapon_strapped"),
        new Script_EvaluatorStateManagerWeaponStrapped("state_mgr_weapon_strapped", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("weapon_strapped_now"),
        new Script_EvaluatorStateManagerWeaponStrappedNow("state_mgr_weapon_strapped_now", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("weapon_unstrapped"),
        new Script_EvaluatorStateManagerWeaponUnstrapped("state_mgr_weapon_unstrapped", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("weapon_unstrapped_now"),
        new Script_EvaluatorStateManagerWeaponUnstrappedNow("state_mgr_weapon_unstrapped_now", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("weapon_none"),
        new Script_EvaluatorStateManagerWeaponNone("state_mgr_weapon_none", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("weapon_none_now"),
        new Script_EvaluatorStateManagerWeaponNoneNow("state_mgr_weapon_none_now", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("weapon_drop"),
        new Script_EvaluatorStateManagerWeaponDrop("state_mgr_weapon_drop", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("weapon_fire"),
        new Script_EvaluatorStateManagerWeaponFire("state_mgr_weapon_fire", this));
    this->m_p_action_planner->add_evaluator(
        this->m_properties.at("movement"), new Script_EvaluatorStateManagerMovement("state_mgr_movement", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("movement_walk"),
        new Script_EvaluatorStateManagerMovementWalk("state_mgr_movement_walk", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("movement_run"),
        new Script_EvaluatorStateManagerMovementRun("state_mgr_movement_run", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("movement_stand"),
        new Script_EvaluatorStateManagerMovementStand("state_mgr_movement_stand", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("movement_stand_now"),
        new Script_EvaluatorStateManagerMovementStandNow("state_mgr_movement_stand_now", this));
    this->m_p_action_planner->add_evaluator(
        this->m_properties.at("mental"), new Script_EvaluatorStateManagerMental("state_mgr_mental", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("mental_free"),
        new Script_EvaluatorStateManagerMentalFree("state_mgr_mental_free", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("mental_free_now"),
        new Script_EvaluatorStateManagerMentalFreeNow("state_mgr_mental_free_now", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("mental_danger"),
        new Script_EvaluatorStateManagerMentalDanger("state_mgr_mental_danger", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("mental_danger_now"),
        new Script_EvaluatorStateManagerMentalDangerNow("state_mgr_mental_danger_now", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("mental_panic"),
        new Script_EvaluatorStateManagerMentalPanic("state_mgr_mental_panic", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("mental_panic_now"),
        new Script_EvaluatorStateManagerMentalPanicNow("state_mgr_mental_panic_now", this));
    this->m_p_action_planner->add_evaluator(
        this->m_properties.at("bodystate"), new Script_EvaluatorStateManagerBodyState("state_mgr_bodystate", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("bodystate_crouch"),
        new Script_EvaluatorStateManagerBodyStateCrouch("state_mgr_bodystate_crouch", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("bodystate_standing"),
        new Script_EvaluatorStateManagerBodyStateStanding("state_mgr_bodystate_standing", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("bodystate_crouch_now"),
        new Script_EvaluatorStateManagerBodyStateCrouchNow("state_mgr_bodystate_crouch_now", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("bodystate_standing_now"),
        new Script_EvaluatorStateManagerBodyStateStandingNow("state_mgr_bodystate_standing_now", this));
    this->m_p_action_planner->add_evaluator(
        this->m_properties.at("direction"), new Script_EvaluatorStateManagerDirection("state_mgr_direction", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("direction_search"),
        new Script_EvaluatorStateManagerDirectionSearch("state_mgr_direction_search", this));
    // st.animstate = state_mgr_animation.animation(npc, st, "state_mgr_animstate_list");
    this->m_p_action_planner->add_evaluator(this->m_properties.at("animstate"),
        new Script_EvaluatorStateManagerAnimationState("state_mgr_animstate", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("animstate_idle_now"),
        new Script_EvaluatorStateManagerAnimationStateIdleNow("state_mgr_animstate_idle_now", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("animstate_play_now"),
        new Script_EvaluatorStateManagerAnimationStatePlayNow("state_mgr_animstate_play_now", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("animstate_locked"),
        new Script_EvaluatorStateManagerAnimationStateLocked("state_mgr_animstate_locked", this));
    // st.animation = state_mgr_animation.animation(npc, st, "state_mgr_animation_list");
    this->m_p_action_planner->add_evaluator(
        this->m_properties.at("animation"), new Script_EvaluatorStateManagerAnimation("state_mgr_animation", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("animation_play_now"),
        new Script_EvaluatorStateManagerAnimationPlayNow("state_mgr_animation_play_now", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("animation_none_now"),
        new Script_EvaluatorStateManagerAnimationNoneNow("state_mgr_animation_none_now", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("animation_locked"),
        new Script_EvaluatorStateManagerAnimationLocked("state_mgr_animation_locked", this));
    this->m_p_action_planner->add_evaluator(
        this->m_properties.at("smartcover"), new Script_EvaluatorStateManagerSmartCover("state_mgr_smartcover", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("smartcover_need"),
        new Script_EvaluatorStateManagerSmartCoverNeed("state_mgr_smartcover_need", this));
    this->m_p_action_planner->add_evaluator(this->m_properties.at("in_smartcover"),
        new Script_EvaluatorStateManagerInSmartCover("state_mgr_in_smartcover", this));

#pragma endregion

#pragma region Cordis State Manager Init this->m_operators
    this->m_operators["end"] = 1;
    this->m_operators["locked"] = 2;
    this->m_operators["locked_external"] = 3;
    this->m_operators["locked_animation"] = 4;
    this->m_operators["locked_animstate"] = 5;
    this->m_operators["locked_smartcover"] = 6;
    this->m_operators["weapon_strapp"] = 11;
    this->m_operators["weapon_unstrapp"] = 12;
    this->m_operators["weapon_none"] = 13;
    this->m_operators["weapon_fire"] = 14;
    this->m_operators["weapon_drop"] = 15;
    this->m_operators["movement"] = 21;
    this->m_operators["movement_walk"] = 22;
    this->m_operators["movement_run"] = 23;
    this->m_operators["movement_stand"] = 24;
    this->m_operators["movement_walk_turn"] = 25;
    this->m_operators["movement_walk_search"] = 26;
    this->m_operators["movement_stand_turn"] = 27;
    this->m_operators["movement_stand_search"] = 28;
    this->m_operators["movement_run_turn"] = 29;
    this->m_operators["movement_run_search"] = 30;
    this->m_operators["mental_free"] = 31;
    this->m_operators["mental_danger"] = 32;
    this->m_operators["mental_panic"] = 33;
    this->m_operators["bodystate_crouch"] = 41;
    this->m_operators["bodystate_standing"] = 42;
    this->m_operators["bodystate_crouch_danger"] = 43;
    this->m_operators["bodystate_standing_free"] = 44;
    this->m_operators["direction_turn"] = 51;
    this->m_operators["direction_search"] = 52;
    this->m_operators["animstate_start"] = 61;
    this->m_operators["animstate_stop"] = 62;
    this->m_operators["animation_start"] = 71;
    this->m_operators["animation_stop"] = 72;
    this->m_operators["walk_turn"] = 75;
    this->m_operators["walk_search"] = 76;
    this->m_operators["stand_turn"] = 77;
    this->m_operators["stand_search"] = 78;
    this->m_operators["smartcover_enter"] = 80;
    this->m_operators["smartcover_exit"] = 81;

    CScriptActionBase* p_action_weaponunstrapped =
        new Script_ActionStateManagerWeaponUnStrapp("state_mgr_weapon_unstrapp", this);

    p_action_weaponunstrapped->add_condition(CWorldProperty(this->m_properties.at("locked"), false));
    p_action_weaponunstrapped->add_condition(CWorldProperty(this->m_properties.at("animstate_locked"), false));
    p_action_weaponunstrapped->add_condition(CWorldProperty(this->m_properties.at("animation_locked"), false));
    p_action_weaponunstrapped->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_weaponunstrapped->add_condition(CWorldProperty(this->m_properties.at("movement"), true));
    p_action_weaponunstrapped->add_condition(CWorldProperty(this->m_properties.at("bodystate"), true));
    p_action_weaponunstrapped->add_condition(CWorldProperty(this->m_properties.at("mental"), true));
    p_action_weaponunstrapped->add_condition(CWorldProperty(this->m_properties.at("weapon_unstrapped"), true));
    p_action_weaponunstrapped->add_condition(CWorldProperty(this->m_properties.at("animstate_idle_now"), true));
    p_action_weaponunstrapped->add_condition(CWorldProperty(this->m_properties.at("animation_none_now"), true));
    p_action_weaponunstrapped->add_effect(CWorldProperty(this->m_properties.at("weapon"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("weapon_unstrapp"), p_action_weaponunstrapped);

    CScriptActionBase* p_action_weapon_strapp =
        new Script_ActionStateManagerWeaponStrapp("state_mgr_weapon_strapp", this);

    p_action_weapon_strapp->add_condition(CWorldProperty(this->m_properties.at("locked"), false));
    p_action_weapon_strapp->add_condition(CWorldProperty(this->m_properties.at("animstate_locked"), false));
    p_action_weapon_strapp->add_condition(CWorldProperty(this->m_properties.at("animation_locked"), false));
    p_action_weapon_strapp->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_weapon_strapp->add_condition(CWorldProperty(this->m_properties.at("movement"), true));
    p_action_weapon_strapp->add_condition(CWorldProperty(this->m_properties.at("bodystate"), true));
    p_action_weapon_strapp->add_condition(CWorldProperty(this->m_properties.at("mental"), true));
    p_action_weapon_strapp->add_condition(CWorldProperty(this->m_properties.at("weapon_strapped"), true));
    p_action_weapon_strapp->add_condition(CWorldProperty(this->m_properties.at("animstate_idle_now"), true));
    p_action_weapon_strapp->add_condition(CWorldProperty(this->m_properties.at("animation_none_now"), true));
    p_action_weapon_strapp->add_effect(CWorldProperty(this->m_properties.at("weapon"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("weapon_strapp"), p_action_weapon_strapp);

    CScriptActionBase* p_action_weapon_none = new Script_ActionStateManagerWeaponNone("state_mgr_weapon_none", this);

    p_action_weapon_none->add_condition(CWorldProperty(this->m_properties.at("locked"), false));
    p_action_weapon_none->add_condition(CWorldProperty(this->m_properties.at("animstate_locked"), false));
    p_action_weapon_none->add_condition(CWorldProperty(this->m_properties.at("animation_locked"), false));
    p_action_weapon_none->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_weapon_none->add_condition(CWorldProperty(this->m_properties.at("movement"), true));
    p_action_weapon_none->add_condition(CWorldProperty(this->m_properties.at("bodystate"), true));
    p_action_weapon_none->add_condition(CWorldProperty(this->m_properties.at("mental"), true));
    p_action_weapon_none->add_condition(CWorldProperty(this->m_properties.at("weapon_none"), true));
    p_action_weapon_none->add_condition(CWorldProperty(this->m_properties.at("animation_none_now"), true));
    p_action_weapon_none->add_effect(CWorldProperty(this->m_properties.at("weapon"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("weapon_none"), p_action_weapon_none);

    CScriptActionBase* p_action_weapon_drop = new Script_ActionStateManagerWeaponDrop("state_mgr_weapon_drop", this);

    p_action_weapon_drop->add_condition(CWorldProperty(this->m_properties.at("locked"), false));
    p_action_weapon_drop->add_condition(CWorldProperty(this->m_properties.at("animstate_locked"), false));
    p_action_weapon_drop->add_condition(CWorldProperty(this->m_properties.at("animation_locked"), false));
    p_action_weapon_drop->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_weapon_drop->add_condition(CWorldProperty(this->m_properties.at("movement"), true));
    p_action_weapon_drop->add_condition(CWorldProperty(this->m_properties.at("bodystate"), true));
    p_action_weapon_drop->add_condition(CWorldProperty(this->m_properties.at("mental"), true));
    p_action_weapon_drop->add_condition(CWorldProperty(this->m_properties.at("weapon_drop"), true));
    p_action_weapon_drop->add_condition(CWorldProperty(this->m_properties.at("animation_none_now"), true));
    p_action_weapon_drop->add_effect(CWorldProperty(this->m_properties.at("weapon"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("weapon_drop"), p_action_weapon_drop);

    CScriptActionBase* p_action_movement_walk =
        new Script_ActionStateManagerMovementWalk("state_mgr_movement_walk", this);

    p_action_movement_walk->add_condition(CWorldProperty(this->m_properties.at("locked"), false));
    p_action_movement_walk->add_condition(CWorldProperty(this->m_properties.at("animstate_locked"), false));
    p_action_movement_walk->add_condition(CWorldProperty(this->m_properties.at("animation_locked"), false));
    p_action_movement_walk->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_movement_walk->add_condition(CWorldProperty(this->m_properties.at("movement"), false));
    p_action_movement_walk->add_condition(CWorldProperty(this->m_properties.at("bodystate"), true));
    p_action_movement_walk->add_condition(CWorldProperty(this->m_properties.at("mental"), true));
    p_action_movement_walk->add_condition(CWorldProperty(this->m_properties.at("movement_walk"), true));
    p_action_movement_walk->add_condition(CWorldProperty(this->m_properties.at("animstate_idle_now"), true));
    p_action_movement_walk->add_condition(CWorldProperty(this->m_properties.at("animation_none_now"), true));
    p_action_movement_walk->add_effect(CWorldProperty(this->m_properties.at("movement"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("movement_walk"), p_action_movement_walk);

    CScriptActionBase* p_action_movement_walk_turn =
        new Script_ActionStateManagerMovementWalkTurn("state_mgr_movement_walk_turn", this);

    p_action_movement_walk_turn->add_condition(CWorldProperty(this->m_properties.at("locked"), false));
    p_action_movement_walk_turn->add_condition(CWorldProperty(this->m_properties.at("animstate_locked"), false));
    p_action_movement_walk_turn->add_condition(CWorldProperty(this->m_properties.at("animation_locked"), false));
    p_action_movement_walk_turn->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_movement_walk_turn->add_condition(CWorldProperty(this->m_properties.at("movement"), false));
    p_action_movement_walk_turn->add_condition(CWorldProperty(this->m_properties.at("direction"), false));
    p_action_movement_walk_turn->add_condition(CWorldProperty(this->m_properties.at("direction_search"), false));
    p_action_movement_walk_turn->add_condition(CWorldProperty(this->m_properties.at("bodystate"), true));
    p_action_movement_walk_turn->add_condition(CWorldProperty(this->m_properties.at("mental"), true));
    p_action_movement_walk_turn->add_condition(CWorldProperty(this->m_properties.at("movement_walk"), true));
    p_action_movement_walk_turn->add_condition(CWorldProperty(this->m_properties.at("animstate_idle_now"), true));
    p_action_movement_walk_turn->add_condition(CWorldProperty(this->m_properties.at("animation_none_now"), true));
    p_action_movement_walk_turn->add_effect(CWorldProperty(this->m_properties.at("movement"), true));
    p_action_movement_walk_turn->add_effect(CWorldProperty(this->m_properties.at("direction"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("movement_walk_turn"), p_action_movement_walk_turn);

    CScriptActionBase* p_action_movement_walk_search =
        new Script_ActionStateManagerMovementWalkSearch("state_mgr_movement_walk_search", this);

    p_action_movement_walk_turn->add_condition(CWorldProperty(this->m_properties.at("locked"), false));
    p_action_movement_walk_turn->add_condition(CWorldProperty(this->m_properties.at("animstate_locked"), false));
    p_action_movement_walk_turn->add_condition(CWorldProperty(this->m_properties.at("animation_locked"), false));
    p_action_movement_walk_turn->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_movement_walk_turn->add_condition(CWorldProperty(this->m_properties.at("movement"), false));
    p_action_movement_walk_turn->add_condition(CWorldProperty(this->m_properties.at("direction"), false));
    p_action_movement_walk_turn->add_condition(CWorldProperty(this->m_properties.at("direction_search"), true));
    p_action_movement_walk_turn->add_condition(CWorldProperty(this->m_properties.at("bodystate"), true));
    p_action_movement_walk_turn->add_condition(CWorldProperty(this->m_properties.at("mental"), true));
    p_action_movement_walk_turn->add_condition(CWorldProperty(this->m_properties.at("movement_walk"), true));
    p_action_movement_walk_turn->add_condition(CWorldProperty(this->m_properties.at("animstate_idle_now"), true));
    p_action_movement_walk_turn->add_condition(CWorldProperty(this->m_properties.at("animation_none_now"), true));
    p_action_movement_walk_turn->add_effect(CWorldProperty(this->m_properties.at("movement"), true));
    p_action_movement_walk_turn->add_effect(CWorldProperty(this->m_properties.at("direction"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("movement_walk_search"), p_action_movement_walk_search);

    CScriptActionBase* p_action_movement_run = new Script_ActionStateManagerMovementRun("state_mgr_movement_run", this);

    p_action_movement_run->add_condition(CWorldProperty(this->m_properties.at("locked"), false));
    p_action_movement_run->add_condition(CWorldProperty(this->m_properties.at("animstate_locked"), false));
    p_action_movement_run->add_condition(CWorldProperty(this->m_properties.at("animation_locked"), false));
    p_action_movement_run->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_movement_run->add_condition(CWorldProperty(this->m_properties.at("movement"), false));
    p_action_movement_run->add_condition(CWorldProperty(this->m_properties.at("bodystate"), true));
    p_action_movement_run->add_condition(CWorldProperty(this->m_properties.at("mental"), true));
    p_action_movement_run->add_condition(CWorldProperty(this->m_properties.at("movement_run"), true));
    p_action_movement_run->add_condition(CWorldProperty(this->m_properties.at("animstate_idle_now"), true));
    p_action_movement_run->add_condition(CWorldProperty(this->m_properties.at("animation_none_now"), true));
    p_action_movement_run->add_effect(CWorldProperty(this->m_properties.at("movement"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("movement_run"), p_action_movement_run);

    CScriptActionBase* p_action_movement_run_turn =
        new Script_ActionStateManagerMovementRun("state_mgr_movement_run_turn", this);

    p_action_movement_run_turn->add_condition(CWorldProperty(this->m_properties.at("locked"), false));
    p_action_movement_run_turn->add_condition(CWorldProperty(this->m_properties.at("animstate_locked"), false));
    p_action_movement_run_turn->add_condition(CWorldProperty(this->m_properties.at("animation_locked"), false));
    p_action_movement_run_turn->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_movement_run_turn->add_condition(CWorldProperty(this->m_properties.at("movement"), false));
    p_action_movement_run_turn->add_condition(CWorldProperty(this->m_properties.at("direction"), false));
    p_action_movement_run_turn->add_condition(CWorldProperty(this->m_properties.at("direction_search"), false));
    p_action_movement_run_turn->add_condition(CWorldProperty(this->m_properties.at("bodystate"), true));
    p_action_movement_run_turn->add_condition(CWorldProperty(this->m_properties.at("mental"), true));
    p_action_movement_run_turn->add_condition(CWorldProperty(this->m_properties.at("movement_run"), true));
    p_action_movement_run_turn->add_condition(CWorldProperty(this->m_properties.at("animstate_idle_now"), true));
    p_action_movement_run_turn->add_condition(CWorldProperty(this->m_properties.at("animation_none_now"), true));
    p_action_movement_run_turn->add_effect(CWorldProperty(this->m_properties.at("movement"), true));
    p_action_movement_run_turn->add_effect(CWorldProperty(this->m_properties.at("direction"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("movement_run_turn"), p_action_movement_run_turn);

    CScriptActionBase* p_action_movement_run_search =
        new Script_ActionStateManagerMovementRun("state_mgr_movement_run_search", this);

    p_action_movement_run_search->add_condition(CWorldProperty(this->m_properties.at("locked"), false));
    p_action_movement_run_search->add_condition(CWorldProperty(this->m_properties.at("animstate_locked"), false));
    p_action_movement_run_search->add_condition(CWorldProperty(this->m_properties.at("animation_locked"), false));
    p_action_movement_run_search->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_movement_run_search->add_condition(CWorldProperty(this->m_properties.at("movement"), false));
    p_action_movement_run_search->add_condition(CWorldProperty(this->m_properties.at("direction"), false));
    p_action_movement_run_search->add_condition(CWorldProperty(this->m_properties.at("direction_search"), true));
    p_action_movement_run_search->add_condition(CWorldProperty(this->m_properties.at("bodystate"), true));
    p_action_movement_run_search->add_condition(CWorldProperty(this->m_properties.at("mental"), true));
    p_action_movement_run_search->add_condition(CWorldProperty(this->m_properties.at("movement_run"), true));
    p_action_movement_run_search->add_condition(CWorldProperty(this->m_properties.at("animstate_idle_now"), true));
    p_action_movement_run_search->add_condition(CWorldProperty(this->m_properties.at("animation_none_now"), true));
    p_action_movement_run_search->add_effect(CWorldProperty(this->m_properties.at("movement"), true));
    p_action_movement_run_search->add_effect(CWorldProperty(this->m_properties.at("direction"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("movement_run_search"), p_action_movement_run_search);

    CScriptActionBase* p_action_movement_stand =
        new Script_ActionStateManagerMovementStand("state_mgr_movement_stand", this);

    p_action_movement_stand->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_movement_stand->add_condition(CWorldProperty(this->m_properties.at("animstate_locked"), false));
    p_action_movement_stand->add_condition(CWorldProperty(this->m_properties.at("animation_locked"), false));
    p_action_movement_stand->add_condition(CWorldProperty(this->m_properties.at("movement"), false));
    p_action_movement_stand->add_condition(CWorldProperty(this->m_properties.at("movement_stand"), true));
    p_action_movement_stand->add_condition(CWorldProperty(this->m_properties.at("mental"), true));
    p_action_movement_stand->add_effect(CWorldProperty(this->m_properties.at("movement"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("movement_stand"), p_action_movement_stand);

    CScriptActionBase* p_action_movement_stand_turn =
        new Script_ActionStateManagerMovementStandTurn("state_mgr_movement_stand_turn", this);

    p_action_movement_stand_turn->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_movement_stand_turn->add_condition(CWorldProperty(this->m_properties.at("animstate_locked"), false));
    p_action_movement_stand_turn->add_condition(CWorldProperty(this->m_properties.at("animation_locked"), false));
    p_action_movement_stand_turn->add_condition(CWorldProperty(this->m_properties.at("movement"), false));
    p_action_movement_stand_turn->add_condition(CWorldProperty(this->m_properties.at("direction"), false));
    p_action_movement_stand_turn->add_condition(CWorldProperty(this->m_properties.at("direction_search"), false));
    p_action_movement_stand_turn->add_condition(CWorldProperty(this->m_properties.at("movement_stand"), true));
    p_action_movement_stand_turn->add_condition(CWorldProperty(this->m_properties.at("mental"), true));
    p_action_movement_stand_turn->add_effect(CWorldProperty(this->m_properties.at("movement"), true));
    p_action_movement_stand_turn->add_effect(CWorldProperty(this->m_properties.at("direction"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("movement_stand_turn"), p_action_movement_stand_turn);

    CScriptActionBase* p_action_movement_stand_search =
        new Script_ActionStateManagerMovementStandSearch("state_mgr_movement_stand_search", this);

    p_action_movement_stand_search->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_movement_stand_search->add_condition(CWorldProperty(this->m_properties.at("animstate_locked"), false));
    p_action_movement_stand_search->add_condition(CWorldProperty(this->m_properties.at("animation_locked"), false));
    p_action_movement_stand_search->add_condition(CWorldProperty(this->m_properties.at("movement"), false));
    p_action_movement_stand_search->add_condition(CWorldProperty(this->m_properties.at("direction"), false));
    p_action_movement_stand_search->add_condition(CWorldProperty(this->m_properties.at("direction_search"), true));
    p_action_movement_stand_search->add_condition(CWorldProperty(this->m_properties.at("movement_stand"), true));
    p_action_movement_stand_search->add_condition(CWorldProperty(this->m_properties.at("mental"), true));
    p_action_movement_stand_search->add_effect(CWorldProperty(this->m_properties.at("movement"), true));
    p_action_movement_stand_search->add_effect(CWorldProperty(this->m_properties.at("direction"), true));

    this->m_p_action_planner->add_operator(
        this->m_operators.at("movement_stand_search"), p_action_movement_stand_search);

    CScriptActionBase* p_action_direction_turn =
        new Script_ActionStateManagerDirectionTurn("state_mgr_direction_turn", this);

    p_action_direction_turn->add_condition(CWorldProperty(this->m_properties.at("animstate_locked"), false));
    p_action_direction_turn->add_condition(CWorldProperty(this->m_properties.at("animation_locked"), false));
    p_action_direction_turn->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_direction_turn->add_condition(CWorldProperty(this->m_properties.at("direction"), false));
    p_action_direction_turn->add_condition(CWorldProperty(this->m_properties.at("direction_search"), false));
    p_action_direction_turn->add_condition(CWorldProperty(this->m_properties.at("weapon"), true)); // From GSC: warning
    p_action_direction_turn->add_condition(CWorldProperty(this->m_properties.at("animation_none_now"), true));
    p_action_direction_turn->add_condition(CWorldProperty(this->m_properties.at("animstate_idle_now"), true));
    p_action_direction_turn->add_condition(CWorldProperty(this->m_properties.at("movement"), true));
    p_action_direction_turn->add_effect(CWorldProperty(this->m_properties.at("direction"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("direction_turn"), p_action_direction_turn);

    CScriptActionBase* p_action_direction_search =
        new Script_ActionStateManagerDirectionSearch("state_mgr_direction_search", this);

    p_action_direction_search->add_condition(CWorldProperty(this->m_properties.at("animstate_locked"), false));
    p_action_direction_search->add_condition(CWorldProperty(this->m_properties.at("animation_locked"), false));
    p_action_direction_search->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_direction_search->add_condition(CWorldProperty(this->m_properties.at("direction"), false));
    p_action_direction_search->add_condition(CWorldProperty(this->m_properties.at("direction_search"), true));
    p_action_direction_search->add_condition(CWorldProperty(this->m_properties.at("weapon"), true));
    p_action_direction_search->add_condition(CWorldProperty(this->m_properties.at("animation_none_now"), true));
    p_action_direction_search->add_condition(CWorldProperty(this->m_properties.at("animstate_idle_now"), true));
    p_action_direction_search->add_condition(CWorldProperty(this->m_properties.at("movement"), true));
    p_action_direction_search->add_effect(CWorldProperty(this->m_properties.at("direction"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("direction_search"), p_action_direction_search);

    CScriptActionBase* p_action_mental_free = new Script_ActionStateManagerMentalFree("state_mgr_mental_free", this);

    p_action_mental_free->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_mental_free->add_condition(CWorldProperty(this->m_properties.at("mental"), false));
    p_action_mental_free->add_condition(CWorldProperty(this->m_properties.at("animstate_idle_now"), true));
    p_action_mental_free->add_condition(CWorldProperty(this->m_properties.at("animation_none_now"), true));
    p_action_mental_free->add_condition(CWorldProperty(this->m_properties.at("mental_free"), true));
    p_action_mental_free->add_condition(CWorldProperty(this->m_properties.at("bodystate"), true));
    p_action_mental_free->add_condition(CWorldProperty(this->m_properties.at("bodystate_standing_now"), true));
    p_action_mental_free->add_effect(CWorldProperty(this->m_properties.at("mental"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("mental_free"), p_action_mental_free);

    CScriptActionBase* p_action_mental_danger =
        new Script_ActionStateManagerMentalDanger("state_mgr_mental_danger", this);

    p_action_mental_danger->add_condition(CWorldProperty(this->m_properties.at("mental"), false));
    p_action_mental_danger->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_mental_danger->add_condition(CWorldProperty(this->m_properties.at("animstate_idle_now"), true));
    p_action_mental_danger->add_condition(CWorldProperty(this->m_properties.at("animation_none_now"), true));
    p_action_mental_danger->add_condition(CWorldProperty(this->m_properties.at("mental_danger"), true));
    p_action_mental_danger->add_effect(CWorldProperty(this->m_properties.at("mental"), true));
    p_action_mental_danger->add_effect(CWorldProperty(this->m_properties.at("mental_danger_now"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("mental_danger"), p_action_mental_danger);

    CScriptActionBase* p_action_mental_panic = new Script_ActionStateManagerMentalPanic("state_mgr_mental_panic", this);

    p_action_mental_panic->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_mental_panic->add_condition(CWorldProperty(this->m_properties.at("mental"), false));
    p_action_mental_panic->add_condition(CWorldProperty(this->m_properties.at("animstate_idle_now"), true));
    p_action_mental_panic->add_condition(CWorldProperty(this->m_properties.at("animation_none_now"), true));
    p_action_mental_panic->add_condition(CWorldProperty(this->m_properties.at("mental_panic"), true));
    p_action_mental_panic->add_effect(CWorldProperty(this->m_properties.at("mental"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("mental_panic"), p_action_mental_panic);

    CScriptActionBase* p_action_bodystate_crouch =
        new Script_ActionStateManagerBodyStateCrouch("state_mgr_bodystate_crouch", this);

    p_action_bodystate_crouch->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_bodystate_crouch->add_condition(CWorldProperty(this->m_properties.at("bodystate"), false));
    p_action_bodystate_crouch->add_condition(CWorldProperty(this->m_properties.at("bodystate_crouch_now"), false));
    p_action_bodystate_crouch->add_condition(CWorldProperty(this->m_properties.at("bodystate_crouch"), true));
    p_action_bodystate_crouch->add_condition(CWorldProperty(this->m_properties.at("mental_danger_now"), true));
    p_action_bodystate_crouch->add_effect(CWorldProperty(this->m_properties.at("bodystate"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("bodystate_crouch"), p_action_bodystate_crouch);

    CScriptActionBase* p_action_bodystate_crouch_danger =
        new Script_ActionStateManagerBodyStateCrouchDanger("state_mgr_bodystate_crouch_danger", this);

    p_action_bodystate_crouch_danger->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_bodystate_crouch_danger->add_condition(CWorldProperty(this->m_properties.at("bodystate"), false));
    p_action_bodystate_crouch_danger->add_condition(CWorldProperty(this->m_properties.at("mental"), false));
    p_action_bodystate_crouch_danger->add_condition(
        CWorldProperty(this->m_properties.at("bodystate_crouch_now"), false));
    p_action_bodystate_crouch_danger->add_condition(CWorldProperty(this->m_properties.at("bodystate_crouch"), true));
    p_action_bodystate_crouch_danger->add_effect(CWorldProperty(this->m_properties.at("bodystate"), true));
    p_action_bodystate_crouch_danger->add_effect(CWorldProperty(this->m_properties.at("mental"), true));

    this->m_p_action_planner->add_operator(
        this->m_operators.at("bodystate_crouch_danger"), p_action_bodystate_crouch_danger);

    CScriptActionBase* p_action_bodystate_standing =
        new Script_ActionStateManagerBodyStateStanding("state_mgr_bodystate_standing", this);

    p_action_bodystate_standing->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_bodystate_standing->add_condition(CWorldProperty(this->m_properties.at("bodystate"), false));
    p_action_bodystate_standing->add_condition(CWorldProperty(this->m_properties.at("bodystate_standing_now"), false));
    p_action_bodystate_standing->add_condition(CWorldProperty(this->m_properties.at("bodystate_standing"), true));
    p_action_bodystate_standing->add_effect(CWorldProperty(this->m_properties.at("bodystate"), true));
    p_action_bodystate_standing->add_effect(CWorldProperty(this->m_properties.at("bodystate_standing_now"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("bodystate_standing"), p_action_bodystate_standing);

    CScriptActionBase* p_action_bodystate_standing_free =
        new Script_ActionStateManagerBodyStateStandingFree("state_mgr_bodystate_standing_free", this);

    p_action_bodystate_standing_free->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_bodystate_standing_free->add_condition(CWorldProperty(this->m_properties.at("bodystate"), false));
    p_action_bodystate_standing_free->add_condition(CWorldProperty(this->m_properties.at("mental"), false));
    p_action_bodystate_standing_free->add_condition(
        CWorldProperty(this->m_properties.at("bodystate_standing_now"), false));
    p_action_bodystate_standing_free->add_condition(CWorldProperty(this->m_properties.at("bodystate_standing"), true));
    p_action_bodystate_standing_free->add_condition(CWorldProperty(this->m_properties.at("mental_free"), false));
    p_action_bodystate_standing_free->add_effect(CWorldProperty(this->m_properties.at("bodystate"), true));
    p_action_bodystate_standing_free->add_effect(CWorldProperty(this->m_properties.at("bodystate_standing_now"), true));
    p_action_bodystate_standing_free->add_effect(CWorldProperty(this->m_properties.at("mental"), true));

    this->m_p_action_planner->add_operator(
        this->m_operators.at("bodystate_standing_free"), p_action_bodystate_standing_free);

    CScriptActionBase* p_action_animationstate_start =
        new Script_ActionStateManagerAnimationStateStart("state_mgr_animstate_start", this);

    p_action_animationstate_start->add_condition(CWorldProperty(this->m_properties.at("locked"), false));
    p_action_animationstate_start->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_animationstate_start->add_condition(CWorldProperty(this->m_properties.at("animstate"), false));
    p_action_animationstate_start->add_condition(CWorldProperty(this->m_properties.at("smartcover"), true));
    p_action_animationstate_start->add_condition(CWorldProperty(this->m_properties.at("animation_none_now"), true));
    p_action_animationstate_start->add_condition(CWorldProperty(this->m_properties.at("direction"), true));
    p_action_animationstate_start->add_condition(CWorldProperty(this->m_properties.at("mental"), true));
    p_action_animationstate_start->add_condition(CWorldProperty(this->m_properties.at("weapon"), true));
    p_action_animationstate_start->add_condition(CWorldProperty(this->m_properties.at("movement"), true));
    p_action_animationstate_start->add_condition(CWorldProperty(this->m_properties.at("animstate_play_now"), false));
    p_action_animationstate_start->add_effect(CWorldProperty(this->m_properties.at("animstate"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("animstate_start"), p_action_animationstate_start);

    CScriptActionBase* p_action_animationstate_stop =
        new Script_ActionStateManagerAnimationStateStop("state_mgr_animstate_stop", this);

    p_action_animationstate_stop->add_condition(CWorldProperty(this->m_properties.at("locked"), false));
    p_action_animationstate_stop->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_animationstate_stop->add_condition(CWorldProperty(this->m_properties.at("animation_locked"), false));
    p_action_animationstate_stop->add_condition(CWorldProperty(this->m_properties.at("animstate_locked"), false));
    p_action_animationstate_stop->add_condition(CWorldProperty(this->m_properties.at("animstate_idle_now"), false));
    p_action_animationstate_stop->add_condition(CWorldProperty(this->m_properties.at("animation_play_now"), false));
    p_action_animationstate_stop->add_effect(CWorldProperty(this->m_properties.at("animstate"), true));
    p_action_animationstate_stop->add_effect(CWorldProperty(this->m_properties.at("animstate_play_now"), false));
    p_action_animationstate_stop->add_effect(CWorldProperty(this->m_properties.at("animstate_idle_now"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("animstate_stop"), p_action_animationstate_stop);

    CScriptActionBase* p_action_animation_start =
        new Script_ActionStateManagerAnimationStart("state_mgr_animation_start", this);

    p_action_animation_start->add_condition(CWorldProperty(this->m_properties.at("locked"), false));
    p_action_animation_start->add_condition(CWorldProperty(this->m_properties.at("animstate_locked"), false));
    p_action_animation_start->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_animation_start->add_condition(CWorldProperty(this->m_properties.at("animstate"), true));
    p_action_animation_start->add_condition(CWorldProperty(this->m_properties.at("smartcover"), true));
    p_action_animation_start->add_condition(CWorldProperty(this->m_properties.at("in_smartcover"), false));
    p_action_animation_start->add_condition(CWorldProperty(this->m_properties.at("direction"), true));
    p_action_animation_start->add_condition(CWorldProperty(this->m_properties.at("weapon"), true));
    p_action_animation_start->add_condition(CWorldProperty(this->m_properties.at("movement"), true));
    p_action_animation_start->add_condition(CWorldProperty(this->m_properties.at("mental"), true));
    p_action_animation_start->add_condition(CWorldProperty(this->m_properties.at("bodystate"), true));
    p_action_animation_start->add_condition(CWorldProperty(this->m_properties.at("animation"), false));
    p_action_animation_start->add_condition(CWorldProperty(this->m_properties.at("animation_play_now"), false));
    p_action_animation_start->add_effect(CWorldProperty(this->m_properties.at("animation"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("animation_start"), p_action_animation_start);

    CScriptActionBase* p_action_animation_stop =
        new Script_ActionStateManagerAnimationStop("state_mgr_animation_stop", this);

    p_action_animation_stop->add_condition(CWorldProperty(this->m_properties.at("locked"), false));
    p_action_animation_stop->add_condition(CWorldProperty(this->m_properties.at("locked_external"), false));
    p_action_animation_stop->add_condition(CWorldProperty(this->m_properties.at("animation_play_now"), true));
    p_action_animation_stop->add_effect(CWorldProperty(this->m_properties.at("animation"), true));
    p_action_animation_stop->add_effect(CWorldProperty(this->m_properties.at("animation_play_now"), false));
    p_action_animation_stop->add_effect(CWorldProperty(this->m_properties.at("animation_none_now"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("animation_stop"), p_action_animation_stop);

    CScriptActionBase* p_action_smartcover_enter =
        new Script_ActionStateManagerSmartCoverEnter("act_state_mgr_smartcover_enter", this);

    p_action_smartcover_enter->add_condition(CWorldProperty(this->m_properties.at("locked"), false));
    p_action_smartcover_enter->add_condition(CWorldProperty(this->m_properties.at("weapon"), true));
    p_action_smartcover_enter->add_condition(CWorldProperty(this->m_properties.at("smartcover_need"), true));
    p_action_smartcover_enter->add_condition(CWorldProperty(this->m_properties.at("smartcover"), false));
    p_action_smartcover_enter->add_condition(CWorldProperty(this->m_properties.at("animstate_idle_now"), true));
    p_action_smartcover_enter->add_condition(CWorldProperty(this->m_properties.at("animation_play_now"), false));
    p_action_smartcover_enter->add_effect(CWorldProperty(this->m_properties.at("smartcover"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("smartcover_enter"), p_action_smartcover_enter);

    CScriptActionBase* p_action_smartcover_exit =
        new Script_ActionStateManagerSmartCoverExit("act_state_mgr_smartcover_exit", this);

    p_action_smartcover_exit->add_condition(CWorldProperty(this->m_properties.at("locked"), false));
    p_action_smartcover_exit->add_condition(CWorldProperty(this->m_properties.at("weapon"), true));
    p_action_smartcover_exit->add_condition(CWorldProperty(this->m_properties.at("smartcover_need"), false));
    p_action_smartcover_exit->add_condition(CWorldProperty(this->m_properties.at("smartcover"), false));
    p_action_smartcover_exit->add_effect(CWorldProperty(this->m_properties.at("smartcover"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("smartcover_exit"), p_action_smartcover_exit);

    CScriptActionBase* p_action_smartcover_locked =
        new Script_ActionStateManagerLocked("state_mgr_locked_smartcover", this);

    p_action_smartcover_locked->add_condition(CWorldProperty(this->m_properties.at("in_smartcover"), true));
    p_action_smartcover_locked->add_effect(CWorldProperty(this->m_properties.at("in_smartcover"), false));

    this->m_p_action_planner->add_operator(this->m_operators.at("locked_smartcover"), p_action_smartcover_locked);

    CScriptActionBase* p_action_locked = new Script_ActionStateManagerLocked("state_mgr_locked", this);

    p_action_locked->add_condition(CWorldProperty(this->m_properties.at("locked"), true));
    p_action_locked->add_effect(CWorldProperty(this->m_properties.at("locked"), false));

    this->m_p_action_planner->add_operator(this->m_operators.at("locked"), p_action_locked);

    CScriptActionBase* p_action_animation_locked =
        new Script_ActionStateManagerLocked("state_mgr_locked_animation", this);

    p_action_animation_locked->add_condition(CWorldProperty(this->m_properties.at("animation_locked"), true));
    p_action_animation_locked->add_effect(CWorldProperty(this->m_properties.at("animation_locked"), false));

    this->m_p_action_planner->add_operator(this->m_operators.at("locked_animation"), p_action_animation_locked);

    CScriptActionBase* p_action_animationstate_locked =
        new Script_ActionStateManagerLocked("state_mgr_locked_animstate", this);

    p_action_animationstate_locked->add_condition(CWorldProperty(this->m_properties.at("animstate_locked"), true));
    p_action_animationstate_locked->add_effect(CWorldProperty(this->m_properties.at("animstate_locked"), false));

    this->m_p_action_planner->add_operator(this->m_operators.at("locked_animstate"), p_action_animationstate_locked);

    CScriptActionBase* p_action_external_locked =
        new Script_ActionStateManagerLocked("state_mgr_locked_external", this);

    p_action_external_locked->add_condition(CWorldProperty(this->m_properties.at("locked_external"), true));
    p_action_external_locked->add_effect(CWorldProperty(this->m_properties.at("locked_external"), false));

    this->m_p_action_planner->add_operator(this->m_operators.at("locked_external"), p_action_external_locked);

    CScriptActionBase* p_action_end = new Script_ActionStateManagerEnd("state_mgr_end", this);

    p_action_end->add_condition(CWorldProperty(this->m_properties.at("end"), false));
    p_action_end->add_condition(CWorldProperty(this->m_properties.at("weapon"), true));
    p_action_end->add_condition(CWorldProperty(this->m_properties.at("movement"), true));
    p_action_end->add_condition(CWorldProperty(this->m_properties.at("mental"), true));
    p_action_end->add_condition(CWorldProperty(this->m_properties.at("bodystate"), true));
    p_action_end->add_condition(CWorldProperty(this->m_properties.at("direction"), true));
    p_action_end->add_condition(CWorldProperty(this->m_properties.at("animstate"), true));
    p_action_end->add_condition(CWorldProperty(this->m_properties.at("animation"), true));
    p_action_end->add_condition(CWorldProperty(this->m_properties.at("smartcover"), true));
    p_action_end->add_effect(CWorldProperty(this->m_properties.at("end"), true));

    this->m_p_action_planner->add_operator(this->m_operators.at("end"), p_action_end);
#pragma endregion

    GraphEngineSpace::CWorldState world_state;
    world_state.add_condition(CWorldProperty(this->m_properties.at("end"), true));

    this->m_p_action_planner->set_target_state(world_state);

#ifdef DEBUG
    this->m_p_npc->debug_planner(this->m_p_action_planner);
#endif

    this->m_p_animation = new Script_StateAnimation(this->m_p_npc, *this, true);
    this->m_p_animstate = new Script_StateAnimation(this->m_p_npc, *this, false);
}

Script_StateManager::~Script_StateManager(void)
{
    if (this->m_p_animation)
    {
        Msg("[Scripts/Script_StateManager/~dtor()] deleting m_p_animation");
        xr_delete(this->m_p_animation);
    }

    if (this->m_p_animstate)
    {
        Msg("[Scripts/Script_StateManager/~dtor()] deleting m_p_animstate");
        xr_delete(this->m_p_animstate);
    }
}

void Script_StateManager::set_state(const xr_string& state_name, StateManagerCallbackData& callback,
    const std::uint32_t timeout, std::pair<Fvector, CScriptGameObject* const> target,
    const StateManagerExtraData& extra)
{
    if (Script_GlobalHelper::getInstance().getStateLibrary().find(state_name) ==
        Script_GlobalHelper::getInstance().getStateLibrary().end())
    {
        R_ASSERT2(false, "can't find it!");
        return;
    }

    if (!Globals::is_vector_nil(target.first) || target.second)
    {
        this->m_look_position = target.first;
        if (target.second)
            this->m_look_object_id = target.second->ID();
        else
            this->m_look_object_id = 0;
    }
    else
    {
        this->m_look_position.set(0.0f, 0.0f, 0.0f);
        this->m_look_object_id = 0;
    }

    bool is_switched = false;
    xr_string last_state_name = this->m_target_state_name;

    // Lord: проверить сравнение строк!
    if (this->m_target_state_name != state_name)
    {
        if (((Script_GlobalHelper::getInstance().getStateLibrary().at(this->m_target_state_name).getWeaponTypeName() ==
                 "fire") ||
                (Script_GlobalHelper::getInstance()
                        .getStateLibrary()
                        .at(this->m_target_state_name)
                        .getWeaponTypeName() == "sniper_fire")) &&
            ((Script_GlobalHelper::getInstance().getStateLibrary().at(state_name).getWeaponTypeName() != "fire") &&
                (Script_GlobalHelper::getInstance().getStateLibrary().at(state_name).getWeaponTypeName() !=
                    "sniper_fire")))
        {
            if (this->m_p_npc->weapon_unstrapped())
                this->m_p_npc->set_item(eObjectActionIdle, Globals::get_weapon(this->m_p_npc, state_name));
        }
    }

    if (Script_GlobalHelper::getInstance().getStateLibrary().at(state_name).IsSpecialDangerMove())
    {
        if (!this->m_p_npc->special_danger_move())
            this->m_p_npc->special_danger_move(true);
    }
    else
    {
        if (this->m_p_npc->special_danger_move())
            this->m_p_npc->special_danger_move(false);
    }

    this->m_target_state_name = state_name;
    is_switched = true;

    if (extra.isInitialize())
    {
        this->m_is_fast_set = extra.isFastSet();

        if ((!this->m_is_position_direction_applied) ||
            ((!Globals::is_vector_nil(this->m_animation_position)) &&
                (!Globals::is_vector_nil(extra.getAnimationPosition())) &&
                (this->m_animation_position.x != extra.getAnimationPosition().x &&
                    this->m_animation_position.y != extra.getAnimationPosition().y &&
                    this->m_animation_position.z != extra.getAnimationPosition().z)) ||
            ((!Globals::is_vector_nil(this->m_animation_direction)) &&
                (!Globals::is_vector_nil(extra.getAnimationDirection())) &&
                (this->m_animation_direction.x != extra.getAnimationDirection().x &&
                    this->m_animation_direction.y != extra.getAnimationDirection().y &&
                    this->m_animation_direction.z != extra.getAnimationDirection().z)))
        {
            this->m_animation_direction = extra.getAnimationDirection();
            this->m_animation_position = extra.getAnimationPosition();
            this->m_is_position_direction_applied = false;
        }
        else
        {
            this->m_animation_position.set(0.0f, 0.0f, 0.0f);
            this->m_animation_direction.set(0.0f, 0.0f, 0.0f);
            this->m_is_position_direction_applied = false;
            this->m_is_fast_set = false;
        }
    }

    this->m_callback_data = callback;
    if (timeout >= 0)
    {
        this->m_callback_data.setTimeOut(timeout);
        this->m_callback_data.setBegin(0);
    }
    else
    {
        if (this->m_callback_data.isAllFieldEmpty())
        {
            this->m_callback_data.setCallbackTime(nullptr);
            this->m_callback_data.setCallbackTurnEnd(nullptr);
        }
    }
}

void Script_StateManager::update(void)
{
    if (this->m_p_animation->getStates().getCurrentStateName() ==
        Script_GlobalHelper::getInstance().getStateLibrary().at(this->m_target_state_name).getAnimationName())
    {
        if (!this->m_callback_data.isAllFieldEmpty() && this->m_callback_data.isCallbackTimeExist())
        {
            if (!this->m_callback_data.getBegin())
            {
                this->m_callback_data.setBegin(Globals::get_time_global());
            }
            else
            {
                if (Globals::get_time_global() - this->m_callback_data.getBegin() >= this->m_callback_data.getTimeOut())
                {
                    this->m_callback_data.setBegin(0);
                    this->m_callback_data.setCallbackTime(nullptr);
                    this->m_callback_data.CallCallbackTime();
                }
            }
        }
    }

    this->m_p_action_planner->update();
    if (!this->m_p_action_planner->initialized())
        return;

    std::uint32_t last_pl_id = 0;
    std::uint32_t pl_id = this->m_p_action_planner->current_action_id();
    while (pl_id != last_pl_id && pl_id != this->m_operators.at("end") && pl_id != this->m_operators.at("locked"))
    {
        last_pl_id = pl_id;
        this->m_p_action_planner->update();
        pl_id = this->m_p_action_planner->current_action_id();
    }
}

const StateManagerCallbackData& Script_StateManager::getCallbackData(void) const noexcept
{
    return this->m_callback_data;
}

} // namespace Scripts
} // namespace Cordis
