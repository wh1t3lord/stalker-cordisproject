#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_CampData
{
public:
    Script_CampData(void) = delete;
    Script_CampData(CScriptGameObject* p_client_object, CScriptIniFile* p_ini);
    ~Script_CampData(void);

    struct Script_CampStateData
    {
    public:
        Script_CampStateData(void) {}
        Script_CampStateData(const std::uint32_t min_time, const std::uint32_t max_time, const std::uint32_t timeout,
            const xr_map<xr_string, std::uint32_t>& transitions, const xr_string& general_state_name,
            const std::function<bool(Script_CampData* const)>& function)
            : m_min_time(min_time), m_max_time(max_time), m_timeout(timeout), m_transitions(transitions),
              m_general_state_name(general_state_name), m_precondition(function)
        {
        }

        ~Script_CampStateData(void) {}

        inline std::uint32_t getMinTime(void) const noexcept { return this->m_min_time; }
        inline void setMinTime(const std::uint32_t value) noexcept { this->m_min_time = value; }

        inline std::uint32_t getMaxTime(void) const noexcept { return this->m_max_time; }
        inline void setMaxTime(const std::uint32_t value) noexcept { this->m_max_time = value; }

        inline std::uint32_t getTimeOut(void) const noexcept { return this->m_timeout; }
        inline void setTimeOut(const std::uint32_t value) noexcept { this->m_timeout = value; }

        inline const xr_map<xr_string, std::uint32_t>& getTransitions(void) const noexcept
        {
            return this->m_transitions;
        }

        inline const xr_string& getGeneralStateName(void) const noexcept { return this->m_general_state_name; }
        inline void setGeneralStateName(const xr_string& state_name) noexcept
        {
            if (state_name.empty())
            {
                Msg("[Scripts/Script_CampData/Script_CampStateData/setGeneralStateName(state_name)] WARNING: "
                    "state_name.empty() == true! You are trying to set an empty string, return ...");
                return;
            }

            this->m_general_state_name = state_name;
        }

        inline bool CallPrecondition(Script_CampData* const p_camp)
        {
            if (!this->m_precondition)
            {
                Msg("[Scripts/Script_CampData/Script_CampStateData/CallPrecondition(p_camp)] WARNING: p_camp == "
                    "nullptr! You can't call precondition return false");
                return false;
            }

            return this->m_precondition(p_camp);
        }

    private:
        std::uint32_t m_min_time;
        std::uint32_t m_max_time;
        std::uint32_t m_timeout;
        xr_map<xr_string, std::uint32_t> m_transitions;
        xr_string m_general_state_name;
        std::function<bool(Script_CampData* const)> m_precondition;
    };

    void update(void);

        inline const xr_vector<xr_string>& getHarmonicas(void) const noexcept
    {
        return this->m_harmonicas;
    }
    inline const xr_vector<xr_string>& getGuitars(void) const noexcept { return this->m_guitars; }
    inline const xr_vector<xr_string>& getStories(void) const noexcept { return this->m_stories; }

    inline const xr_map<std::uint16_t, std::pair<xr_string, xr_map<xr_string, std::uint32_t>>>& getNpcs(void) const
        noexcept
    {
        return this->m_npcs;
    }

private:
    bool m_is_sound_manager_started;
    std::uint16_t m_director_id;
    std::uint16_t m_idle_talker_id;
    std::uint32_t m_timeout;
    std::uint32_t m_active_state_time;
    Script_SoundManager* m_p_sound_manager;
    CScriptIniFile* m_p_ini;
    CScriptGameObject* m_p_object;
    // pair.first -> state_name
    // pair.second -> role_name = state_value;
    xr_map<std::uint16_t, std::pair<xr_string, xr_map<xr_string, std::uint32_t>>> m_npcs;
    xr_map<xr_string, Script_CampStateData> m_states;
    xr_vector<xr_string> m_stories;
    xr_vector<xr_string> m_guitars;
    xr_vector<xr_string> m_harmonicas;
    xr_string m_active_state_name;
};
} // namespace Scripts
} // namespace Cordis
