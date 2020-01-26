#include "stdafx.h"
#include "Script_CampData.h"

namespace Cordis
{
namespace Scripts
{
constexpr std::uint32_t kNpcRoleNone = 0;
constexpr std::uint32_t kNpcRoleListener = 1;
constexpr std::uint32_t kNpcRoleDirector = 2;

bool sr_camp_idle_precondition(Script_CampData* const p_camp) { return true; }
bool sr_camp_harmonica_precondition(Script_CampData* const p_camp)
{
    if (!p_camp)
    {
        Msg("[Scripts/sr_camp_harmonica_precondition(p_camp)] WARNING: p_camp == nullptr! return false");
        return false;
    }

    if (!p_camp->getHarmonicas().empty())
    {
        if (p_camp->getNpcs().size() > 1)
        {
            DataBase::Storage_Scheme* p_storage_scheme = nullptr;
            CScriptGameObject* p_client_object = nullptr;
            for (const std::pair<std::uint16_t, std::pair<xr_string, xr_map<xr_string, std::uint32_t>>>& it :
                p_camp->getNpcs())
            {
                if (DataBase::Storage::getInstance().getStorage().find(it.first) !=
                    DataBase::Storage::getInstance().getStorage().end())
                {
                    p_client_object = DataBase::Storage::getInstance().getStorage().at(it.first).getClientObject();
                    if (!DataBase::Storage::getInstance().getStorage().at(it.first).getActiveSchemeName().empty())
                    {
                        if (DataBase::Storage::getInstance().getStorage().at(it.first).getSchemes().find(DataBase::Storage::getInstance().getStorage().at(it.first).getActiveSchemeName()) !=
                            DataBase::Storage::getInstance().getStorage().at(it.first).getSchemes().end())
                        {
                            p_storage_scheme =
                                DataBase::Storage::getInstance().getStorage().at(it.first).getSchemes().at(
                                    DataBase::Storage::getInstance().getStorage().at(it.first).getActiveSchemeName());
                        }
                    }
                }

                const xr_map<xr_string, std::uint32_t>& role_states = it.second.second;

                if ((role_states.find("harmonica") != role_states.end()) &&
                    (role_states.at("harmonica") == kNpcRoleDirector) &&
                    (p_storage_scheme &&
                        p_storage_scheme->getBaseActionName() == p_storage_scheme->getDescriptionName()) &&
                    (p_client_object && !XR_MEET::is_meet(p_client_object)))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool sr_camp_guitar_precondition(Script_CampData* const p_camp)
{
    if (!p_camp)
    {
        Msg("[Scripts/sr_camp_guitar_precondition(p_camp)] WARNING: p_camp == nullptr! Return false");
        return false;
    }

    if (!p_camp->getGuitars().empty())
    {
        if (p_camp->getNpcs().size() > 1)
        {
            for (const std::pair<std::uint16_t, std::pair<xr_string, xr_map<xr_string, std::uint32_t>>>& it :
                p_camp->getNpcs())
            {
                DataBase::Storage_Scheme* p_storage_scheme = nullptr;
                CScriptGameObject* p_npc = nullptr;

                if (DataBase::Storage::getInstance().getStorage().find(it.first) !=
                    DataBase::Storage::getInstance().getStorage().end())
                {
                    const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(it.first);
                    p_npc = storage.getClientObject();

                    if (!storage.getActiveSchemeName().empty() &&
                        (storage.getSchemes().find(storage.getActiveSchemeName()) != storage.getSchemes().end()))
                    {
                        p_storage_scheme = storage.getSchemes().at(storage.getActiveSchemeName());
                    }
                }

                if ((it.second.second.at("guitar") == kNpcRoleDirector) &&
                    (p_storage_scheme &&
                        (p_storage_scheme->getBaseActionName() == p_storage_scheme->getDescriptionName())) &&
                    (p_npc && !XR_MEET::is_meet(p_npc)))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool sr_camp_story_precondition(Script_CampData* const p_camp)
{
    if (!p_camp)
    {
        Msg("[Scripts/sr_camp_story_precondition(p_camp)] WARNING: p_camp == nullptr! Return false");
        return false;
    }

    if (!p_camp->getStories().empty())
    {
        std::uint32_t count_not_meet_npc = 0;

        CScriptGameObject* p_npc = nullptr;
        for (const std::pair<std::uint16_t, std::pair<xr_string, xr_map<xr_string, std::uint32_t>>>& it :
            p_camp->getNpcs())
        {
            if (DataBase::Storage::getInstance().getStorage().find(it.first) !=
                DataBase::Storage::getInstance().getStorage().end())
            {
                p_npc = DataBase::Storage::getInstance().getStorage().at(it.first).getClientObject();
            }

            if (p_npc && !XR_MEET::is_meet(p_npc))
            {
                ++count_not_meet_npc;
            }
        }

        if (count_not_meet_npc > 1)
            return true;
    }

    return false;
}

} // namespace Scripts
} // namespace Cordis

namespace Cordis
{
namespace Scripts
{
Script_CampData::Script_CampData(CScriptGameObject* p_client_object, CScriptIniFile* p_ini)
    : m_p_object(p_client_object), m_p_ini(p_ini), m_director_id(0), m_active_state_name("idle"),
      m_p_sound_manager(&Script_SoundManager::getSoundManager(
          xr_string("camp").append(std::to_string(this->m_p_object->ID()).c_str()))),
      m_is_sound_manager_started(true), m_active_state_time(0), m_timeout(0), m_idle_talker_id(0)
{
    if (!this->m_p_ini)
    {
        R_ASSERT2(false, "it can't be, so initialize your m_p_ini!");
    }

    this->m_stories = Globals::Utils::parse_names(Globals::Utils::cfg_get_string(this->m_p_ini, "camp", "stories"));
    this->m_guitars =
        Globals::Utils::parse_names(Globals::Utils::cfg_get_string(this->m_p_ini, "camp", "guitar_themes"));
    this->m_harmonicas =
        Globals::Utils::parse_names(Globals::Utils::cfg_get_string(this->m_p_ini, "camp", "harmonica_themes"));

    xr_map<xr_string, std::uint32_t> transitions;
    transitions.insert({"harmonica", 30});
    transitions.insert({"guitar", 30});
    transitions.insert({"story", 40});
    Script_CampStateData idle_data =
        Script_CampStateData(30000, 40000, 0, transitions, "idle", sr_camp_idle_precondition);
    transitions.clear();

    transitions.insert({"idle", 100});
    transitions.insert({"harmonica", 0});
    transitions.insert({"guitar", 0});
    transitions.insert({"story", 0});
    Script_CampStateData harmonica_data =
        Script_CampStateData(10000, 11000, 3000, transitions, "listen", sr_camp_harmonica_precondition);
    transitions.clear();

    transitions.insert({"idle", 100});
    transitions.insert({"harmonica", 0});
    transitions.insert({"guitar", 0});
    transitions.insert({"story", 0});
    Script_CampStateData guitar_data =
        Script_CampStateData(10000, 11000, 4500, transitions, "listen", sr_camp_guitar_precondition);
    transitions.clear();

    transitions.insert({"idle", 100});
    transitions.insert({"harmonica", 0});
    transitions.insert({"guitar", 0});
    transitions.insert({"story", 0});
    Script_CampStateData story_data =
        Script_CampStateData(10000, 11000, 0, transitions, "listen", sr_camp_story_precondition);
    transitions.clear();

    this->m_states["idle"] = idle_data;
    this->m_states["harmonica"] = harmonica_data;
    this->m_states["guitar"] = guitar_data;
    this->m_states["story"] = story_data;


}

Script_CampData::~Script_CampData(void) {}

} // namespace Scripts
} // namespace Cordis
