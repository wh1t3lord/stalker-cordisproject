#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_SmartTerrain;

constexpr float SMARTTERRAINCONTROL_ALARM_TIME = 2.0f * 60.0f * 60.0f;

enum Script_SmartTerrainControl_States
{
    kNormal,
    kDanger,
    kAlarm
};

inline static std::uint16_t& getCurrentSmartTerrainID(void) noexcept
{
    static std::uint16_t id;
    return id;
}

class Script_SmartTerrainControl
{
public:
    Script_SmartTerrainControl(Script_SE_SmartTerrain* smart, const CInifile& ini, const xr_string& section);
    ~Script_SmartTerrainControl(void);

    void update(void);
    bool IsActorTreat(void);

    inline Script_SmartTerrainControl_States& getState(void) noexcept { return this->m_state; }
    inline xr_string& getIgnoreZoneName(void) noexcept { return this->m_ignore_zone; }
    inline xr_string& getNoweaponZoneName(void) noexcept { return this->m_noweapon_zone; }
    inline Script_SE_SmartTerrain* getSmart(void) { return this->m_smart; }
    inline xrTime& getAlarmTime(void) noexcept { return this->m_alarm_time; }
    inline xr_map<std::uint32_t, CondlistData>& getAlarmStartSound(void) noexcept { return this->m_alarm_start_sound; }
    inline xr_map<std::uint32_t, CondlistData>& getAlarmStopSound(void) noexcept { return this->m_alarm_stop_sound; }

private:
    Script_SmartTerrainControl_States m_state;
    Script_SE_SmartTerrain* m_smart;
    xrTime m_alarm_time;
    xr_map<std::uint32_t, CondlistData> m_alarm_start_sound;
    xr_map<std::uint32_t, CondlistData> m_alarm_stop_sound;
    xr_string m_noweapon_zone;
    xr_string m_ignore_zone;
};
} // namespace Scripts
} // namespace Cordis
