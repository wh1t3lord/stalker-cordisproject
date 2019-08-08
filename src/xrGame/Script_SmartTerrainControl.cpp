#include "stdafx.h"
#include "Script_SmartTerrainControl.h"

namespace Cordis
{
namespace Scripts
{
Script_SmartTerrainControl::Script_SmartTerrainControl(
    Script_SE_SmartTerrain* smart, const CInifile& ini, const xr_string& section)
    : m_smart(smart), m_state(Script_SmartTerrainControl_States::kNormal),
      m_noweapon_zone(Globals::Utils::cfg_get_string(&ini, section, "noweap_zone", true)),
      m_ignore_zone(Globals::Utils::cfg_get_string(&ini, section, "ignore_zone")),
      m_alarm_start_sound(XR_LOGIC::parse_condlist_by_server_object(
          section, "alarm_start_sound", Globals::Utils::cfg_get_string(&ini, section, "alarm_start_sound"))),
      m_alarm_stop_sound(XR_LOGIC::parse_condlist_by_server_object(
          section, "alarm_stop_sound", Globals::Utils::cfg_get_string(&ini, section, "alarm_stop_sound")))
{
    if (!this->m_smart)
    {
        R_ASSERT2(false, "object was null!");
    }
}

Script_SmartTerrainControl::~Script_SmartTerrainControl(void) {}

void Script_SmartTerrainControl::update(void)
{
    if (this->m_state == Script_SmartTerrainControl_States::kAlarm)
    {
        if (get_time_struct().diffSec(this->m_alarm_time) < SMARTTERRAINCONTROL_ALARM_TIME)
        {
            Msg("[Scripts/Script_SmartTerrainControl/update] Time was less than SMARTTERRAINCONTROL_ALARM_TIME. The value is: %s", std::to_string(get_time_struct().diffSec(this->m_alarm_time)).c_str());
            return;
        }


    }
}

} // namespace Scripts
} // namespace Cordis
