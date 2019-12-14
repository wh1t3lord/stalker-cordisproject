#pragma once

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

    inline CScriptGameObject* const getLookObject(void) const { return this->m_p_look_object; }
    inline void setLookObject(CScriptGameObject* const p_client_object) { this->m_p_look_object = p_client_object; }

    inline const Fvector& getLookPosition(void) const noexcept { return this->m_look_position; }
    inline void setLookPosition(const Fvector& look_position) noexcept { this->m_look_position = look_position; }

private:
    bool m_is_position_direction_applied;
    bool m_is_combat;
    bool m_is_alife;
    bool m_is_need_reweapon;
    bool m_is_point_object_direction; // @ uses in actions of direction
    CScriptGameObject* m_p_npc;
    CScriptGameObject* m_p_look_object;
    CScriptActionPlanner* m_p_action_planner;
    Fvector m_look_position;
    Fvector m_animation_position;
    Fvector m_animation_direction;
    xr_map<xr_string, std::uint32_t> m_properties;
    xr_map<xr_string, std::uint32_t> m_operators;
    xr_string m_target_state_name;
};
} // namespace Scripts
} // namespace Cordis
