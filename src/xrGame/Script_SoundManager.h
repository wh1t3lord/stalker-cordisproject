#pragma once

namespace Cordis
{
namespace Scripts
{
class Story;
constexpr std::uint32_t _kTupleGetWho = 0;
constexpr std::uint32_t _kTupleGetTheme = 1;
constexpr std::uint32_t _kTupleGetTimeout = 2;

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
        Msg("[Scripts/Script_SoundManager/register_npc(npc_id)] Register sm npc [%s]:[%s]", this->m_id.c_str(),
            std::to_string(npc_id).c_str());
        this->m_npc.push_back(npc_id);
    }

    void unregister_npc(const std::uint16_t npc_id);

    inline void set_storyteller(const std::uint16_t npc_id) { this->m_storyteller_id = npc_id; }

    void set_story(const xr_string& story_id_name);

    bool isFinished(void) const noexcept;

    void update(void);

    inline void choose_random_storyteller(void) noexcept
    {
        this->m_storyteller_id =
            this->m_npc[Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(0, (this->m_npc.size() - 1))];
    }

private:
    std::uint16_t m_last_playing_npc;
    std::uint16_t m_storyteller_id;
    std::uint32_t m_phrase_timeout;
    std::uint32_t m_phrase_idle;
    Story* m_p_story;
    static xr_map<xr_string, Script_SoundManager> m_sound_managers;
    xr_vector<std::uint16_t> m_npc;
    xr_string m_id;
};

class Story
{
public:
    Story(const xr_string& story_id_name);
    ~Story(void);

    inline bool isFinished(void) const noexcept { return (this->m_next_phrase > this->m_max_phrase_count); }
    inline void reset_story(void) noexcept { this->m_next_phrase = 0; }
    inline const std::tuple<xr_string, xr_string, xr_string>& getNextPhrase(void) noexcept
    {
        ++this->m_next_phrase;
        return this->m_replics[this->m_next_phrase];
    }
    inline const xr_string& getIDName(void) const noexcept { return this->m_id_name; }

private:
    std::uint32_t m_next_phrase;
    std::uint32_t m_max_phrase_count;
    xr_vector<std::tuple<xr_string, xr_string, xr_string>> m_replics;
    xr_string m_id_name;
};
} // namespace Scripts
} // namespace Cordis
