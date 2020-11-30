#pragma once

#include "Script_StateAnimation.h"

namespace Cordis
{
namespace Scripts
{
class Script_StateManager
{
public:
    Script_StateManager(void) = delete;
    Script_StateManager(CScriptGameObject* const p_client_object);
    ~Script_StateManager(void);

    void set_state(const xr_string& state_name, StateManagerCallbackData& callback, const std::uint32_t timeout,
        std::pair<Fvector, CScriptGameObject* const> target, const StateManagerExtraData& extra);
    void update(void);

    inline const xr_string& getTargetStateName(void) const noexcept { return this->m_target_state_name; }
    inline const xr_map<xr_string, std::uint32_t>& getProperties(void) const noexcept { return this->m_properties; }
    inline const xr_map<xr_string, std::uint32_t>& getOperators(void) const noexcept { return this->m_operators; }
    inline CScriptActionPlanner* const getActionPlanner(void) const { return this->m_p_action_planner; }

    inline bool isCombat(void) const noexcept { return this->m_is_combat; }
    inline void setCombat(const bool value) noexcept { this->m_is_combat = value; }

    inline bool isAlife(void) const noexcept { return this->m_is_alife; }
    inline void setAlife(const bool value) noexcept { this->m_is_alife = value; }

    inline bool isPointObjectDirection(void) const noexcept { return this->m_is_point_object_direction; }
    inline void setPointObjectDirection(const bool value) noexcept { this->m_is_point_object_direction = value; }

    inline bool isPositionDirectionApplied(void) const noexcept { return this->m_is_position_direction_applied;}
    inline void setPositionDirectionApplied(const bool value) noexcept
    {
        this->m_is_position_direction_applied = value;
    } 

    inline CScriptGameObject* const getLookObject(void) const { return this->m_p_look_object; }
    inline void setLookObject(CScriptGameObject* const p_client_object) { this->m_p_look_object = p_client_object; }

    inline const Fvector& getLookPosition(void) const noexcept { return this->m_look_position; }
    inline void setLookPosition(const Fvector& look_position) noexcept { this->m_look_position = look_position; }

    const StateManagerCallbackData& getCallbackData(void) const noexcept;

    inline Script_StateAnimation* getAnimation(void) const { return this->m_p_animation; }
    inline void setAnimation(Script_StateAnimation* p_animation) { this->m_p_animation = p_animation; }

    inline Script_StateAnimation* getAnimState(void) const { return this->m_p_animstate; }
    inline void setAnimState(Script_StateAnimation* p_animstate) { this->m_p_animstate = p_animstate; }

    inline const Fvector& getAnimationDirection(void) const noexcept { return this->m_animation_direction; }
    inline const Fvector& getAnimationPosition(void) const noexcept { return this->m_animation_position; }

    inline bool isFastSet(void) const noexcept { return this->m_is_fast_set; }

private:
    bool m_is_position_direction_applied;
    bool m_is_combat;
    bool m_is_alife;
    bool m_is_need_reweapon;
    bool m_is_point_object_direction; // @ uses in actions of direction
    bool m_is_fast_set;
    std::uint16_t m_look_object_id;
    CScriptGameObject* m_p_npc;
    CScriptGameObject* m_p_look_object;
    CScriptActionPlanner* m_p_action_planner;
    Script_StateAnimation* m_p_animation;
    Script_StateAnimation* m_p_animstate;
    Fvector m_look_position;
    Fvector m_animation_position;
    Fvector m_animation_direction;
    xr_map<xr_string, std::uint32_t> m_properties;
    xr_map<xr_string, std::uint32_t> m_operators;
    xr_string m_target_state_name;
    StateManagerCallbackData m_callback_data;
};
} // namespace Scripts
} // namespace Cordis
