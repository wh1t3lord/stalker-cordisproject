#pragma once

#include "xrServer_Objects_ALife_Monsters.h"

#include "Script_SoundManager.h"

namespace Cordis
{
namespace Scripts
{
class Script_SE_SimulationSquad : public CSE_ALifeOnlineOfflineGroup
{
    using inherited = CSE_ALifeOnlineOfflineGroup;

public:
    Script_SE_SimulationSquad(LPCSTR section);
    virtual ~Script_SE_SimulationSquad(void);

    virtual Script_SE_SimulationSquad* cast_script_se_simulationsquad(void) { return this; }

#pragma region Cordis Getters
    inline std::uint32_t getSmartTerrainID(void) noexcept { return this->m_smart_terrain_id; }
    inline std::uint32_t getCurrentSpotID(void) noexcept { return this->m_current_spot_id; }
    inline std::uint32_t getCurrentTargetID(void) noexcept { return this->m_current_target_id; }
    inline std::uint32_t getAssignedTargetID(void) noexcept { return this->m_assigned_target_id; }
    inline xr_string& getSettingsID(void) noexcept { return this->m_settings_id_name; }
    inline std::uint16_t getScriptTarget(void);
    inline Globals::StayReachOnTarget& getCurrentAction(void) noexcept { return this->m_current_action; }
#pragma endregion

#pragma region Cordis Setters
    inline void setSmartTerrainID(const std::uint32_t& value) noexcept { this->m_smart_terrain_id = value; }
    inline void setCurrentSpotID(const std::uint32_t& value) noexcept { this->m_current_spot_id = value; }
    inline void setCurrentTargetID(const std::uint32_t& value) noexcept { this->m_current_target_id = value; }
    inline void setAssignedTargetID(const std::uint32_t& value) noexcept { this->m_assigned_target_id = value; }
#pragma endregion

    void set_location_types(const xr_string& new_smart_name);
    std::uint16_t add_squad_member(const xr_string& spawn_section_name, const Fvector& spawn_position, const std::uint32_t& level_vertex_id, const std::uint16_t& game_vertex_id);
 

private:
    void set_location_types_section(const xr_string& section);
    void set_squad_sympathy(const float& sympathy = 0.0f);
    void set_squad_behaviour(void);
    void init_squad_on_load(void);
    bool check_squad_come_to_point(void);
    inline xr_string& pick_next_target(void) { return this->m_parsed_targets[this->m_next_target_index]; }

private:
    bool m_is_always_walk;
    bool m_is_always_arrived;
    bool m_is_need_to_reset_location_masks;
    bool m_is_need_free_update;
    std::uint32_t m_smart_terrain_id;
    std::uint32_t m_current_spot_id;
    std::uint32_t m_current_target_id;
    std::uint32_t m_assigned_target_id;
    std::uint32_t m_next_target_index;
    float m_sympathy;
    xr_map<std::uint32_t, CondlistData> m_condlist_action;
    xr_map<std::uint32_t, CondlistData> m_condlist_death;
    xr_map<std::uint32_t, CondlistData> m_condlist_invulnerability;
    xr_map<std::uint32_t, CondlistData> m_condlist_show_spot;
    xr_map<xr_string, xr_string> m_behaviour;
    xr_vector<xr_string> m_parsed_targets;
    xr_string m_relationship_name;
    xr_string m_player_id_name;
    xr_string m_settings_id_name;
    xr_string m_last_target_name;
    Globals::StayReachOnTarget m_current_action;
    Script_SoundManager m_sound_manager;
};

} // namespace Scripts
} // namespace Cordis
