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
Script_CampData::Script_CampData(CScriptGameObject* p_client_object, CScriptIniFile* p_ini, bool is_deallocate_ini)
    : m_p_object(p_client_object), m_p_ini(p_ini), m_director_id(0), m_active_state_name("idle"),
      m_p_sound_manager(&Script_SoundManager::getSoundManager(
          xr_string("camp").append(std::to_string(this->m_p_object->ID()).c_str()))),
      m_is_sound_manager_started(true), m_active_state_time(0), m_timeout(0), m_idle_talker_id(0), m_is_system_flag_deallocation(is_deallocate_ini)
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

Script_CampData::~Script_CampData(void) 
{
    if (this->m_is_system_flag_deallocation)
    {
        Msg("[Scripts/Script_CampData/~dtor()] deleting allocated this->m_p_ini!");
        xr_delete(this->m_p_ini);
    }
}

void Script_CampData::update(void) 
{
    if (!this->m_p_sound_manager->isFinished())
    {
        this->m_p_sound_manager->update();
        return;
    }

    if (this->m_is_sound_manager_started == false)
        return;
    
    if (this->m_idle_talker_id)
    {
        if (XR_SOUND::getSoundDatabase().at(this->m_idle_talker_id))
            return;
        else
            this->m_idle_talker_id = 0;
    }

    if (this->m_active_state_time < Globals::get_time_global())
    {
        this->set_next_state();
        this->get_director();

        if (this->m_director_id == 0)
        {
            this->m_active_state_name = "idle";

            for (std::pair<const std::uint16_t, std::pair<xr_string, xr_map<xr_string, std::uint32_t>>>& it : this->m_npcs)
            {
                it.second.first = this->m_active_state_name;
            }
        }

        this->m_is_sound_manager_started = false;

        for (std::pair<const std::uint16_t, std::pair<xr_string, xr_map<xr_string, std::uint32_t>>>& it : this->m_npcs)
        {
            if (DataBase::Storage::getInstance().getStorage().find(it.first) != DataBase::Storage::getInstance().getStorage().end())
            {
                const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(it.first);
                DataBase::Storage_Scheme* p_scheme = storage.getSchemes().at(storage.getActiveSchemeName());

                for (Script_ISchemeEntity* p_scheme_entity : p_scheme->getActions())
                {
                    p_scheme_entity->update(0.0f); // Lord: подумать какое значение передавать
                }
            }

            // Lord: доделать когда будет meet
        }
    }

    if (this->m_timeout && (this->m_timeout <= Globals::get_time_global()))
    {
        this->set_story();
        this->m_timeout = 0;
    }

    if (this->m_active_state_name == "idle")
    {
        std::uint32_t npc_count = 0;
        xr_vector<std::uint16_t> talkers;

        for (std::pair<const std::uint16_t, std::pair<xr_string, xr_map<xr_string, std::uint32_t>>>& it : this->m_npcs)
        {
            ++npc_count;
            talkers.push_back(it.first);
        }

        if (npc_count)
        {
            this->m_idle_talker_id = talkers[Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(0, talkers.size() - 1)];
            xr_string faction_name;
            XR_SOUND::set_sound_play(this->m_idle_talker_id, "state", faction_name, 0);
        }
    }
}

void Script_CampData::set_next_state(void) 
{
    const xr_map<xr_string, std::uint32_t>& transitions = this->m_states.at(this->m_active_state_name).getTransitions();
    std::uint32_t random_value = Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(1, 100);

    for (const std::pair<xr_string, std::uint32_t>& it : transitions)
    {
        if (random_value < it.second)
        {
            if (this->m_states.at(it.first).CallPrecondition(this))
            {
                this->m_active_state_name = it.first;
                break;
            }
        }
        else
        {
            random_value -= it.second;
        }
    }

    for (std::pair<const std::uint16_t, std::pair<xr_string, xr_map<xr_string, std::uint32_t>>>& it : this->m_npcs)
    {
        it.second.first = this->m_active_state_name;
    }

    this->m_active_state_time = Globals::get_time_global() +
        Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(
            this->m_states.at(this->m_active_state_name).getMinTime(),
            this->m_states.at(this->m_active_state_name).getMaxTime());

    this->m_timeout = Globals::get_time_global() + this->m_states.at(this->m_active_state_name).getTimeOut();
}

void Script_CampData::get_director(void) 
{
    if (this->m_active_state_name == "idle")
    {
        this->m_director_id = 0;
        return;
    }

    xr_vector<std::uint16_t> directors;
    std::uint32_t npc_count = 0;

    for (std::pair<const std::uint16_t, std::pair<xr_string, xr_map<xr_string, std::uint32_t>>>& it : this->m_npcs)
    {
        ++npc_count;
        if (DataBase::Storage::getInstance().getStorage().find(it.first) != DataBase::Storage::getInstance().getStorage().end())
        {
            const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(it.first);

            if (!storage.getActiveSchemeName().empty() && (storage.getSchemes().find(storage.getActiveSchemeName()) != storage.getSchemes().end()))
            {
                DataBase::Storage_Scheme* const p_scheme = storage.getSchemes().at(storage.getActiveSchemeName());
                CScriptGameObject* const p_npc = storage.getClientObject();

                if (it.second.second.at(this->m_active_state_name) == kNpcRoleDirector && p_scheme && p_scheme->getBaseActionName() == p_scheme->getDescriptionName())
                {
                    directors.push_back(it.first);
                }
            }
        }
    }

    if (npc_count == 0)
    {
        this->m_director_id = 0;
        return;
    }

    if (directors.empty())
    {
        return;
    }
    else if (directors.size() == 1)
    {
        this->m_director_id = directors[0];
    }
    else 
    {
        this->m_director_id =
            directors[Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(0, directors.size() - 1)];
    }
}

void Script_CampData::set_story(void) 
{
    if (this->m_active_state_name == "story")
    {
        this->m_p_sound_manager->set_storyteller(this->m_director_id);
        this->m_p_sound_manager->set_story(
            this->m_stories[Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(
                0, this->m_stories.size() - 1)]);
        this->m_is_sound_manager_started = true;
    }
    else if (this->m_active_state_name == "idle")
        this->m_is_sound_manager_started = true;
}

void Script_CampData::get_camp_action(const std::uint16_t npc_id, xr_string& state_name, bool& is_director) 
{
    if (!npc_id)
    {
        Msg("[Scripts/Script_CampData/get_camp_action(npc_id, state_name, is_director)] WARNING: npc_id == 0! Return ...");
        state_name.clear();
        is_director = false;
        return;
    }

    if (this->m_npcs.find(npc_id) == this->m_npcs.end())
    {
        Msg("[Scripts/Script_CampData/get_camp_action(npc_id, state_name, is_director)] WARNING: can't find npc by id %d", npc_id);
        state_name.clear();
        is_director = false;
        return;
    }

    state_name = this->m_npcs.at(npc_id).first;
    is_director = (this->m_director_id == npc_id);
}

void Script_CampData::register_npc(const std::uint16_t npc_id) 
{
    this->m_npcs[npc_id].first = this->m_active_state_name;
    DataBase::Storage::getInstance().setStorageRegisteredCamp(npc_id, this->m_p_object->ID());

    for (const std::pair<xr_string, Script_CampStateData>& it : this->m_states)
    {
        std::uint32_t role = this->get_npc_role(npc_id, it.first);

        if (role == kNpcRoleNone)
        {
            Msg("[Scripts/Script_CampData/register_npc(npc_id)] WARNING: wrong role for npc with id in camp %d %s", npc_id, this->m_p_object->Name());
            return;
        }

        this->m_npcs[npc_id].second[it.first] = role;
    }

    this->m_p_sound_manager->register_npc(npc_id);

    const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(npc_id);
    DataBase::Storage_Scheme* p_scheme = storage.getSchemes().at(storage.getActiveSchemeName());
    
    for (Script_ISchemeEntity* p_scheme_entity : p_scheme->getActions())
    {
        if (p_scheme_entity->isActionSubscribed())
            p_scheme_entity->update(0.0f); // Lord: подумать какое значение передавать
    }
}

void Script_CampData::unregister_npc(const std::uint16_t npc_id) 
{
    if (this->m_director_id == npc_id)
    {
        this->m_is_sound_manager_started = true;
        this->m_active_state_time = 0;
        this->m_director_id = 0;

        this->m_active_state_name = "idle";
     
        for (std::pair<const std::uint16_t, std::pair<xr_string, xr_map<xr_string, std::uint32_t>>>& it : this->m_npcs)
        {
            it.second.first = this->m_active_state_name;
        }
    }

    DataBase::Storage::getInstance().setStorageRegisteredCamp(npc_id, 0);
    this->m_p_sound_manager->unregister_npc(npc_id);
}

std::uint32_t Script_CampData::get_npc_role(const std::uint16_t npc_id, const xr_string& state_name)
{
    if (DataBase::Storage::getInstance().getStorage().find(npc_id) == DataBase::Storage::getInstance().getStorage().end())
    {
        Msg("[Scripts/Script_CampData/get_npc_role(npc_id, state)] WARNING: can't find storage by id %d Return ...", npc_id);
        return kNpcRoleNone;
    }

    const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(npc_id);
    DataBase::Storage_Scheme* const p_scheme = storage.getSchemes().at(storage.getActiveSchemeName());

    if (!p_scheme)
    {
        Msg("[Scripts/Script_CampData/get_npc_role(npc_id, state)] WARNING: p_scheme == nullptr! Return ...");
        return kNpcRoleNone;
    }

    const xr_vector<std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>>& npc_actions =
        p_scheme->getApprovedActions();

    xr_string description_name = p_scheme->getDescriptionName();

    if (state_name == "harmonica" || state_name == "guitar")
    {
        description_name.append("_");
        description_name.append(state_name);

        for (const std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>& it : npc_actions)
        {
            if (it.second == description_name)
                return kNpcRoleDirector;
        }

        return kNpcRoleListener;
    }
    else if(state_name == "story")
    {
        for (const std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>& it : npc_actions)
        {
            if (it.second == description_name || it.second == xr_string(description_name).append("_weapon"))
                return kNpcRoleDirector;
        }

        return kNpcRoleListener;
    }

    return kNpcRoleNone;
}

} // namespace Scripts
} // namespace Cordis
