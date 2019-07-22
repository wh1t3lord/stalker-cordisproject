#pragma once

namespace Cordis
{
namespace Scripts
{
    // Lord: доделать данный manager
class Script_SurgeManager
{
private:
    Script_SurgeManager(void) {}

    // @ Where the surge manager is used
    enum
    {
        zaton,
        jupiter,
        pripyat
    };

public:
    inline static Script_SurgeManager& getInstance(void) noexcept
    {
        static Script_SurgeManager instance;
        return instance;
    }

    ~Script_SurgeManager(void);
    Script_SurgeManager(const Script_SurgeManager&) = delete;
    Script_SurgeManager& operator=(const Script_SurgeManager&) = delete;
    Script_SurgeManager(Script_SurgeManager&&) = delete;
    Script_SurgeManager& operator=(Script_SurgeManager&&) = delete;

    inline bool IsStarted(void) const noexcept { return this->m_started; }
    inline bool IsFinished(void) const noexcept { return this->m_finished; }

private:
    bool m_started;
    bool m_finished;
    bool m_time_forwarded;
    bool m_skip_message;
    bool m_task_given;
    bool m_effector_set;
    bool m_second_message_given;
    bool m_ui_disabled;
    bool m_blowout_sound;
    const std::uint32_t m_surge_time = 190;
    int m_count;
    xr_map<std::uint32_t, bool> m_levels_respawn;
    CInifile m_ini;
};
} // namespace Scripts
} // namespace Cordis
