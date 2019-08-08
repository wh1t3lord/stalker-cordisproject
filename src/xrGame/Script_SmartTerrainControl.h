#pragma once

namespace Cordis
{
namespace Scripts
{
    constexpr float SMARTTERRAINCONTROL_ALARM_TIME = 2.0f * 60.0f * 60.0f;
    
enum Script_SmartTerrainControl_States
{
    kNormal,
    kDanger,
    kAlarm
};

class Script_SmartTerrainControl
{
public:
    Script_SmartTerrainControl(Script_SE_SmartTerrain* smart, const CInifile& ini, const xr_string& section);
    ~Script_SmartTerrainControl(void);

    void update(void);
 
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
