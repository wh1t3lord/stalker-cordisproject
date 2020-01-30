#include "stdafx.h"
#include "Script_SoundManager.h"

CScriptIniFile story_ltx("misc\\sound_stories.ltx");


namespace Cordis
{
namespace Scripts
{
xr_map<xr_string, Script_SoundManager> Script_SoundManager::m_sound_managers;
}
} // namespace Cordis

namespace Cordis
{
namespace Scripts
{
Script_SoundManager::Script_SoundManager(const xr_string& id)
    : m_p_story(nullptr), m_last_playing_npc(0), m_storyteller_id(0), m_phrase_timeout(0), m_phrase_idle(0)
{
    this->m_id = id;
}

Script_SoundManager::~Script_SoundManager(void)
{
    if (this->m_p_story)
    {
        Msg("[Scripts/Script_SoundManager/~dtor()] deleting m_p_story");
        xr_delete(this->m_p_story);
    }
}

void Script_SoundManager::unregister_npc(const std::uint16_t npc_id) 
{
    Msg("[Scripts/Script_SoundManager/register_npc(npc_id)] UnRegister sm npc [%s]:[%s]", this->m_id.c_str(),
        std::to_string(npc_id).c_str());

    if (this->m_last_playing_npc == npc_id && XR_SOUND::getSoundDatabase().at(this->m_last_playing_npc))
    {
        if (this->m_p_story)
        {
            Msg("[Scripts/Script_SoundManager/unregister_npc(npc_id)] deleting m_p_story");
            xr_delete(this->m_p_story);
        }

        XR_SOUND::getSoundDatabase().at(this->m_last_playing_npc)->stop();
    }

    if (this->m_storyteller_id == npc_id)
        this->m_storyteller_id = 0;

    std::uint16_t id_to_remove = 0;
    bool is_exist = false;
    for (const std::uint16_t it : this->m_npc)
    {
        if (it == npc_id)
        {
            is_exist = true;
            break;
        }

        ++id_to_remove;
    }

    if (is_exist)
        this->m_npc.erase(this->m_npc.begin() + id_to_remove);
}

void Script_SoundManager::set_story(const xr_string& story_id_name) 
{
    if (this->m_p_story)
    {
        Msg("[Scripts/Script_SoundManager/set_story(story_id_name)] deleting m_p_story");
        xr_delete(this->m_p_story);
    }

    this->m_p_story = new Story(story_id_name);
}

bool Script_SoundManager::isFinished(void) const noexcept
{
    if (!this->m_p_story)
        return true;

    return this->m_p_story->isFinished();
}

void Script_SoundManager::update(void) 
{
    if (!this->m_p_story)
    {
        return;
    }

    if (XR_SOUND::getSoundDatabase().at(this->m_last_playing_npc))
    {
        if (DataBase::Storage::getInstance().getStorage().find(this->m_last_playing_npc) !=
            DataBase::Storage::getInstance().getStorage().end())
        {
            if (DataBase::Storage::getInstance()
                    .getStorage()
                    .at(this->m_last_playing_npc)
                    .getClientObject()
                    ->GetBestEnemy())
            {
                if (this->m_p_story)
                {
                    Msg("[Scripts/Script_SoundManager/update()] deleting m_p_story");
                    xr_delete(this->m_p_story);
                }

                XR_SOUND::getSoundDatabase().at(this->m_last_playing_npc)->stop(this->m_last_playing_npc);
            }
        }

        return;
    }

    std::uint32_t current_time = Globals::get_time_global();
    if (!this->m_phrase_timeout)
    {
        this->m_phrase_timeout = current_time;
    }

    if (current_time - this->m_phrase_timeout < this->m_phrase_idle)
    {
        return;
    }

    const std::tuple<xr_string, xr_string, xr_string>& next_phrase = this->m_p_story->getNextPhrase();

    if (std::get<0>(next_phrase).empty() && std::get<1>(next_phrase).empty() && std::get<2>(next_phrase).empty())
        return;

    std::uint32_t npc_id = 0;
    if (this->m_npc.empty())
        return;

    if (std::get<_kTupleGetWho>(next_phrase) == "teller")
    {
        if (!this->m_storyteller_id)
            this->choose_random_storyteller();

        npc_id = this->m_storyteller_id;
    }
    else if (std::get<_kTupleGetWho>(next_phrase) == "reaction")
    {
        std::uint32_t teller_id = 0;

        for (const std::uint32_t it : this->m_npc)
        {
            if (it == this->m_storyteller_id)
            {
                break;
            }
            ++teller_id;
        }

        if (this->m_npc.size() >= 2)
        {
            std::uint32_t id =
                Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(0, (this->m_npc.size() - 1));

            if (id >= teller_id)
                ++id;

            npc_id = this->m_npc[id];
        }
        else
        {
            npc_id = this->m_npc[0];
        }
    }
    else if (std::get<_kTupleGetWho>(next_phrase) == "reaction_all")
    {
        std::uint32_t _npc_id = 0;
        for (const std::uint32_t it : this->m_npc)
        {
            if (it != this->m_storyteller_id)
            {
                xr_string faction_name;
                XR_SOUND::set_sound_play(it, std::get<_kTupleGetTheme>(next_phrase), faction_name, 0);
                _npc_id = it;
            }
        }

        this->m_last_playing_npc = _npc_id;
        this->m_phrase_timeout = 0;
        this->m_phrase_idle = boost::lexical_cast<std::uint32_t>(std::get<_kTupleGetTimeout>(next_phrase)) * 1000;
        return;
    }
    else
    {
        npc_id =
            this->m_npc[Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(0, this->m_npc.size() - 1)];
    }

    if (!npc_id ||
        DataBase::Storage::getInstance().getStorage().find(npc_id) ==
            DataBase::Storage::getInstance().getStorage().end())
        return;

    if (DataBase::Storage::getInstance().getStorage().at(npc_id).getClientObject()->GetBestEnemy() &&
        XR_SOUND::getSoundDatabase().at(npc_id))
    {
        if (this->m_p_story)
        {
            Msg("[Scripts/Script_SoundManager/update()] deleting m_p_story");
            xr_delete(this->m_p_story);
        }

        XR_SOUND::getSoundDatabase().at(npc_id)->stop(npc_id);
        return;
    }

    Msg("[Scripts/Script_SoundManager/update()] speaking %s %s %s", this->m_id.c_str(), std::to_string(npc_id).c_str(),
        std::get<_kTupleGetTheme>(next_phrase).c_str());

    this->m_last_playing_npc = npc_id;

    if (std::get<_kTupleGetTheme>(next_phrase) != "nil")
    {
        if (this->m_p_story && this->m_p_story->getIDName() == "squad_counter_attack")
        {
            CSE_ALifeDynamicObject* p_server_npc = ai().alife().objects().object(npc_id);
            if (p_server_npc)
            {
                Script_SE_SimulationSquad* p_server_squad = Globals::get_object_squad(p_server_npc->ID);
                // Lord: доделать когда будет task_manager
            }
        }
    }

    this->m_phrase_timeout = 0;
    this->m_phrase_idle = boost::lexical_cast<std::uint32_t>(std::get<_kTupleGetTimeout>(next_phrase)) * 1000;
}

Story::Story(const xr_string& story_id_name)
{
    if (!story_ltx.section_exist(story_id_name.c_str()))
    {
        R_ASSERT2(false, "There is no story in sound_stories.ltx");
    }

    std::uint32_t line_count = story_ltx.line_count(story_id_name.c_str());
    const char* id_name;
    const char* value_name;

    // Lord: проверить на итерацию до конца ли доходит
    for (std::uint32_t i = 0; i < line_count - 1; ++i)
    {
        if (story_ltx.r_line(story_id_name.c_str(), i, &id_name, &value_name))
        {
            xr_vector<xr_string> data = Globals::Utils::parse_names(value_name);

            if (data.size() < 3)
            {
                R_ASSERT2(false, "something is not right, check your configuration file please!");
            }

            if (data[0] == "teller" || data[0] == "reaction" || data[0] == "reaction_all")
            {
                R_ASSERT2(false, "wrong first field!");
            }

            this->m_replics.push_back(std::make_tuple(data[0], data[1], data[2]));
        }
    }

    this->m_id_name = story_id_name;
    this->m_max_phrase_count = line_count - 1;
    this->m_next_phrase = 0;
}

Story::~Story(void) {}

} // namespace Scripts
} // namespace Cordis
