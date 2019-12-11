#include "stdafx.h"
#include "Script_StateManager.h"

namespace Cordis
{
namespace Scripts
{
Script_StateManager::Script_StateManager(CScriptGameObject* const p_client_object)
    : m_p_npc(p_client_object), m_is_position_direction_applied(false), m_target_state_name("idle"),
      m_p_action_planner(nullptr), m_is_alife(true), m_is_combat(false), m_is_point_object_direction(false)
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
    #pragma endregion

    #pragma region Cordis State Manager Init this->m_operators

    #pragma endregion
}

Script_StateManager::~Script_StateManager(void) {}
} // namespace Scripts
} // namespace Cordis
