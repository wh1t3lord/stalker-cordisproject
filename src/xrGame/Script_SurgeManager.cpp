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

std::uint16_t Script_SurgeManager::get_nearest_cover(void)
{
    if (this->m_is_loaded)
        this->init_surge_covers();

    xr_map<int, std::pair<CScriptGameObject*, xr_map<std::uint32_t, CondlistData>>> hides = this->m_covers;

    if (this->m_count)
    {
        for (const std::pair<int, std::pair<CScriptGameObject*, xr_map<std::uint32_t, CondlistData>>>& it : hides)
        {
            if (!it.second.second.empty())
            {
                xr_string picked_section_name =
                    XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(),
                        static_cast<CScriptGameObject*>(nullptr), it.second.second);

                if (!picked_section_name.empty() && picked_section_name != "true")
                {
                    hides.erase(it.first);
                }
            }
        }

        std::uint16_t nearest_cover_id = hides[0].first->ID();
        float nearest_cover_distance =
            hides[0].first->Position().distance_to(DataBase::Storage::getInstance().getActor()->Position());

        for (const std::pair<int, std::pair<CScriptGameObject*, xr_map<std::uint32_t, CondlistData>>>& it : hides)
        {
            if (DataBase::Storage::getInstance()
                    .getStorage()
                    .at(it.second.first->ID())
                    .getClientObject()
                    ->inside(DataBase::Storage::getInstance().getActor()->Position()))
            {
                return it.second.first->ID();
            }

            float distance =
                it.second.first->Position().distance_to(DataBase::Storage::getInstance().getActor()->Position());

            if (distance < nearest_cover_distance)
            {
                if (!it.second.second.empty())
                {
                    xr_string picked_section_name =
                        XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(),
                            static_cast<CScriptGameObject*>(nullptr), it.second.second);
                    if (picked_section_name == "true")
                    {
                        nearest_cover_distance = distance;
                        nearest_cover_id = it.second.first->ID();
                    }
                }
                else
                {
                    nearest_cover_distance = distance;
                    nearest_cover_id = it.second.first->ID();
                }
            }
        }

        if (nearest_cover_id == hides[0].first->ID())
        {
            if (!hides[0].second.empty())
            {
                xr_string picked_section_name =
                    XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(),
                        static_cast<CScriptGameObject*>(nullptr), hides[0].second);

                if (!picked_section_name.empty() && picked_section_name != "true")
                    return 0;
            }
        }

        return nearest_cover_id;
    }

    return 0;
}

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
