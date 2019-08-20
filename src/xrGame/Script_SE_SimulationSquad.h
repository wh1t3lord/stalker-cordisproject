#pragma once

#include "xrServer_Objects_ALife_Monsters.h"

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

#pragma region Cordis Getters
    inline std::uint32_t getSmartTerrainID(void) noexcept { return this->m_smart_terrain_id; }
    inline std::uint32_t getCurrentSpotID(void) noexcept { return this->m_current_spot_id; }
    inline std::uint32_t getCurrentTargetID(void) noexcept { return this->m_current_target_id; }
    inline std::uint32_t getAssignedTargetID(void) noexcept { return this->m_assigned_target_id; }
    inline xr_string& getSettingsID(void) noexcept { return this->m_settings_id; }
#pragma endregion

#pragma region Cordis Setters
    inline void setSmartTerrainID(const std::uint32_t& value) noexcept { this->m_smart_terrain_id = value; }
    inline void setCurrentSpotID(const std::uint32_t& value) noexcept { this->m_current_spot_id = value; }
    inline void setCurrentTargetID(const std::uint32_t& value) noexcept { this->m_current_target_id = value; }
    inline void setAssignedTargetID(const std::uint32_t& value) noexcept { this->m_assigned_target_id = value; }
#pragma endregion

private:
    void set_location_types_section(const xr_string& section);
    void set_squad_sympathy(const float& sympathy);

private:
    bool m_is_always_walk;
    bool m_is_always_arrived;
    std::uint32_t m_smart_terrain_id;
    std::uint32_t m_current_spot_id;
    std::uint32_t m_current_target_id;
    std::uint32_t m_assigned_target_id;
    std::uint32_t m_sympathy;
    xr_map<std::uint32_t, CondlistData> m_condlist_action;
    xr_map<std::uint32_t, CondlistData> m_condlist_death;
    xr_map<std::uint32_t, CondlistData> m_condlist_invulnerability;
    xr_map<std::uint32_t, CondlistData> m_condlist_show_spot;
    xr_string m_relationship;
    xr_string m_player_id;
    xr_string m_settings_id;
    Script_SoundManager m_sound_manager;
};

} // namespace Scripts
} // namespace Cordis
