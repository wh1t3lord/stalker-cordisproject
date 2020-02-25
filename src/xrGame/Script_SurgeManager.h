#pragma once

namespace Cordis
{
namespace Scripts
{
constexpr std::uint32_t _kStartMinSurgeTime = 2 * 60 * 60;
constexpr std::uint32_t _kStartMaxSurgeTime = 4 * 60 * 60;
constexpr std::uint32_t _kMinSurgeTime = 12 * 60 * 60;
constexpr std::uint32_t _kMaxSurgeTime = 24 * 60 * 60;
constexpr int _kSurgeShockPPeffector = 1;
constexpr int _kEathquakeCamEffector = 2;
constexpr int _kSleepCamEffector = 3;
constexpr int _kSleepFadePPEffector = 4;

// Lord: доделать данный manager, и переписать некторые значения под code style!
class Script_SurgeManager
{
private:
    Script_SurgeManager(void)
        : m_ini("misc\\surge_manager.ltx"), m_is_started(false), m_is_finished(true), m_is_time_forwarded(false),
          m_is_skip_message(false), m_is_task_given(false), m_is_effector_set(false), m_is_second_message_given(false),
          m_is_ui_disabled(false), m_is_blowout_sound(false), m_last_surge_time(Globals::Game::get_game_time()),
          m_delta(Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(
              _kStartMinSurgeTime, _kStartMaxSurgeTime)),
          m_count(0), m_is_loaded(false)
    {
        this->m_levels_respawn["zaton"] = false;
        this->m_levels_respawn["jupiter"] = false;
        this->m_levels_respawn["pripyat"] = false;

        xr_string condlist_string = "true";
        if (this->m_ini.line_exist("settings", "condlist"))
        {
            condlist_string = this->m_ini.r_string("settings", "condlist");
        }

        this->m_condlist = XR_LOGIC::parse_condlist_by_script_object("surge_manager", "condlist", condlist_string);

        condlist_string = "false";
        if (this->m_ini.line_exist("settings", "survive"))
        {
            condlist_string = this->m_ini.r_string("settings", "survive");
        }

        this->m_survive_condlist =
            XR_LOGIC::parse_condlist_by_script_object("surge_manager", "survive_condlist", condlist_string);

        this->init_surge_covers();
    }

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

    inline bool IsStarted(void) const noexcept { return this->m_is_started; }
    inline bool IsFinished(void) const noexcept { return this->m_is_finished; }
    inline bool IsKillingAll(void) const noexcept { return (this->m_is_ui_disabled && this->m_is_started); }
    inline bool isUIDisabled(void) const noexcept { return this->m_is_ui_disabled; }
    inline void setSurgeMessage(const xr_string& text) noexcept { this->m_surge_message_name = text; }
    inline void setSurgeTaskSectionName(const xr_string& task_name) noexcept
    {
        this->m_surge_task_section_name = task_name;
    }
    inline bool isBlowoutSound(void) const noexcept { return this->m_is_blowout_sound; }
    inline void setSkipMessage(const bool value) noexcept { this->m_is_skip_message = value; }

    void init_surge_covers(void);
    std::uint16_t get_nearest_cover(void);
    void update(void);
    void load(IReader* packet);
    void save(NET_Packet& packet);
    void launch_rockets(void);
    void kill_all_unhided_after_actor_death(void);
    void kill_all_unhided(void);
    void give_surge_hide_task(void);
    void respawn_artefacts_and_replace_anomaly_zone(void);
    void end_surge(bool is_manual = false);
    void skip_surge(void);
    void start(const bool is_manual = false);

    static bool actor_in_cover(void) noexcept
    {
        std::uint16_t cover_id = Script_SurgeManager::getInstance().get_nearest_cover();
        if (cover_id &&
            DataBase::Storage::getInstance().getStorage().at(cover_id).getClientObject()->inside(
                DataBase::Storage::getInstance().getActor()->Position()))
            return true;

        return false;
    }

    static std::uint16_t get_task_target(void) noexcept
    {
        if (actor_in_cover())
            return 0;

        return Script_SurgeManager::getInstance().get_nearest_cover();
    }

    static void set_surge_message(const xr_string& text) noexcept
    {
        Script_SurgeManager::getInstance().setSurgeMessage(text);
    }

    static void set_surge_task(const xr_string& task_name) noexcept
    {
        Script_SurgeManager::getInstance().setSurgeTaskSectionName(task_name);
    }

    static bool is_killing_all(void) noexcept
    {
        if (Script_SurgeManager::getInstance().IsStarted() && Script_SurgeManager::getInstance().isUIDisabled())
            return true;

        return false;
    }

    static void resurrect_skip_message(void) { Script_SurgeManager::getInstance().setSkipMessage(false); }

    static bool sound_started(void) noexcept
    {
        return (Script_SurgeManager::getInstance().IsStarted() && Script_SurgeManager::getInstance().isUIDisabled());
    }

private:
    bool m_is_started;
    bool m_is_finished;
    bool m_is_loaded;
    bool m_is_time_forwarded;
    bool m_is_skip_message;
    bool m_is_task_given;
    bool m_is_effector_set;
    bool m_is_second_message_given;
    bool m_is_ui_disabled;
    bool m_is_blowout_sound;
    const std::uint32_t m_surge_time = 190;
    std::uint32_t m_delta;
    int m_count;
    xrTime m_inited_time;
    xrTime m_last_surge_time;
    xr_map<xr_string, bool> m_levels_respawn;
    xr_map<std::uint32_t, CondlistData> m_condlist;
    xr_map<std::uint32_t, CondlistData> m_survive_condlist;
    xr_map<int, std::pair<CScriptGameObject*, xr_map<std::uint32_t, CondlistData>>> m_covers;
    xr_string m_surge_message_name;
    xr_string m_surge_task_section_name;
    CScriptIniFile m_ini;
};
} // namespace Scripts
} // namespace Cordis
