#include "stdafx.h"
#include "Script_SurgeManager.h"

namespace Cordis
{
namespace Scripts
{
Script_SurgeManager::~Script_SurgeManager(void) {}

void Script_SurgeManager::init_surge_covers(void)
{
    const char* id_name;
    const char* temp_name;
    for (std::uint32_t i = 0; i < this->m_ini.line_count("list"); ++i)
    {
        if (this->m_ini.r_line("list", i, &id_name, &temp_name))
        {
            CScriptGameObject* const p_zone = DataBase::Storage::getInstance().getZoneByName().at(id_name);
            if (p_zone)
            {
                ++this->m_count;
                this->m_covers[this->m_count].first = p_zone;
                if (this->m_ini.line_exist(id_name, "condlist"))
                {
                    this->m_covers[this->m_count].second = XR_LOGIC::parse_condlist_by_script_object(
                        id_name, "condlist", this->m_ini.r_string(id_name, "condlist"));
                }
            }
        }
    }
}

std::uint16_t Script_SurgeManager::get_nearest_cover(void) { return std::uint16_t(); }

void Script_SurgeManager::update(void) {}

void Script_SurgeManager::load(NET_Packet& packet) {}

void Script_SurgeManager::save(NET_Packet& packet) {}

void Script_SurgeManager::launch_rockets(void) {}

void Script_SurgeManager::kill_all_unhided_after_actor_death(void) {}

void Script_SurgeManager::kill_all_unhided(void) {}

void Script_SurgeManager::give_surge_hide_task(void) {}

void Script_SurgeManager::respawn_artefacts_and_replace_anomaly_zone(void) {}

void Script_SurgeManager::end_surge(void) {}

void Script_SurgeManager::skip_surge(void) {}

void Script_SurgeManager::start(const bool is_manual) {}

} // namespace Scripts
} // namespace Cordis
