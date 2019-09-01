#pragma once

namespace Cordis
{
namespace Scripts
{
class Story;

class StoryLtx
{
    friend class Story;
    friend class Script_SoundManager;

private:
    inline static CScriptIniFile& getLtx(void) noexcept
    {
        static CScriptIniFile ini("misc\\sound_stories.ltx");
        return ini;
    }
};

class Script_SoundManager
{
    friend class Story;

public:
    // @ Lord - don't use implicit throught only getSoundManager()!
    Script_SoundManager(const xr_string& id);
    Script_SoundManager(void) {}
    ~Script_SoundManager(void);

    inline static Script_SoundManager& getSoundManager(const xr_string& id)
    {
        m_sound_managers[id] = Script_SoundManager(id);
        return m_sound_managers[id];
    }

    inline void register_npc(const std::uint16_t& npc_id)
    {
        // Lord: In-Game logging make it
        Msg("[Script] Register sm npc [%s]:[%s]", this->m_id.c_str(), std::to_string(npc_id).c_str());
        this->m_npc.push_back(npc_id);
    }

    inline void unregister_npc(const std::uint16_t& npc_id)
    {
        Msg("[Script] UnRegister sm npc [%s]:[%s]", this->m_id.c_str(), std::to_string(npc_id).c_str());

        if (this->m_last_playing_npc == npc_id)
        {
        }
    }

    inline void set_storyteller(const std::uint16_t& npc_id) {}

    inline void set_story(const xr_string& story_id) {}

private:
    std::uint16_t m_last_playing_npc;
    static xr_map<xr_string, Script_SoundManager> m_sound_managers;
    xr_vector<std::uint16_t> m_npc;
    xr_string m_id;
};

class Story
{
public:
    Story(const xr_string& story_id);
    ~Story(void);
};
} // namespace Scripts
} // namespace Cordis
