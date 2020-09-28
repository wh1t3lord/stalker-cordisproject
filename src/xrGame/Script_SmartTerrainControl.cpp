#include "stdafx.h"
#include "Script_SmartTerrainControl.h"
#include "Script_SE_SmartTerrain.h"
#include "Script_XR_Logic.h"

namespace Cordis
{
namespace Scripts
{
Script_SmartTerrainControl::Script_SmartTerrainControl(
    Script_SE_SmartTerrain* smart, CScriptIniFile& ini, const xr_string& section)
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

Script_SmartTerrainControl::Script_SmartTerrainControl(
    Script_SE_SmartTerrain* smart, CInifile& ini, const xr_string& section_name)
    : m_smart(smart), m_state(Script_SmartTerrainControl_States::kNormal),
      m_noweapon_zone(Globals::Utils::cfg_get_string(&ini, section_name, "noweap_zone", true)),
      m_ignore_zone(Globals::Utils::cfg_get_string(&ini, section_name, "ignore_zone")),
      m_alarm_start_sound(XR_LOGIC::parse_condlist_by_server_object(
          section_name, "alarm_start_sound", Globals::Utils::cfg_get_string(&ini, section_name, "alarm_start_sound"))),
      m_alarm_stop_sound(XR_LOGIC::parse_condlist_by_server_object(
          section_name, "alarm_stop_sound", Globals::Utils::cfg_get_string(&ini, section_name, "alarm_stop_sound")))
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
            Msg("[Scripts/Script_SmartTerrainControl/update] Time was less than SMARTTERRAINCONTROL_ALARM_TIME. The "
                "value is: %s",
                std::to_string(get_time_struct().diffSec(this->m_alarm_time)).c_str());
            return;
        }
    }
}

bool Script_SmartTerrainControl::IsActorTreat(void)
{
    CScriptGameObject* client_zone = DataBase::Storage::getInstance().getZoneByName().at(this->m_noweapon_zone);

    if (!client_zone)
        return false;

    if (!client_zone->inside(DataBase::Storage::getInstance().getActor()->Position()))
    {
        if (getCurrentSmartTerrainID() == this->m_smart->ID)
        {
            getCurrentSmartTerrainID() = 0;
        }

        return false;
    }
    else
    {
        getCurrentSmartTerrainID() = this->m_smart->ID;
    }

    if (Globals::IsWeapon(DataBase::Storage::getInstance().getActor()->GetActiveItem()))
    {
        return true;
    }

    return false;
}

void Script_SmartTerrainControl::actor_attack(void)
{
    if (this->m_state != Script_SmartTerrainControl_States::kAlarm)
    {
        xr_string sound_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_smart, this->m_alarm_start_sound);

        if (sound_name.empty() == false)
        {
            XR_SOUND::set_sound_play(DataBase::Storage::getInstance().getActor()->ID(), sound_name, xr_string(), 0);
        }

        for (const std::pair<std::uint32_t, Script_SE_SimulationSquad*> it : Script_SimulationBoard::getInstance().getSmarts().at(this->m_smart->ID).getSquads())
        {
            Globals::GameRelations::set_squad_goodwill(std::to_string(it.first).c_str(), "enemy");
        }
    }

    this->m_state = Script_SmartTerrainControl_States::kAlarm;
    this->m_alarm_time = Globals::Game::get_game_time();
}

void Script_SmartTerrainControl::load(NET_Packet& packet) 
{
    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Load, false, "Script_SmartTerrainControl");

    this->m_state = static_cast<Script_SmartTerrainControl_States>(packet.r_u8());
    this->m_alarm_time = Globals::Utils::r_CTime(packet);

    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Load, true, "Script_SmartTerrainControl");
}

void Script_SmartTerrainControl::save(NET_Packet& packet)
{
    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, false, "Script_SmartTerrainControl");

    packet.w_u8(this->m_state);
    Globals::Utils::w_CTime(packet, this->m_alarm_time);

    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, true, "Script_SmartTerrainControl");
}

} // namespace Scripts
} // namespace Cordis
