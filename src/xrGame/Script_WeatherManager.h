#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_WeatherManager
{
private:
    Script_WeatherManager(void)
        : m_wfx_time(0.0f), m_update_time(0), m_is_forced_weather_change_on_time_change(false), m_last_hour(0)
    {
        this->m_p_ini = new CScriptIniFile("environment\\dynamic_weather_graphs.ltx");
    }

public:
    inline static Script_WeatherManager& getInstance(void) noexcept
    {
        static Script_WeatherManager instance;
        return instance;
    }

    ~Script_WeatherManager(void);
    Script_WeatherManager(const Script_WeatherManager&) = delete;
    Script_WeatherManager& operator=(const Script_WeatherManager&) = delete;
    Script_WeatherManager(Script_WeatherManager&&) = delete;
    Script_WeatherManager& operator=(Script_WeatherManager&&) = delete;

    inline void forced_weather_change(void) { this->m_is_forced_weather_change_on_time_change = true; }
    inline const xr_string& getWeatherFXName(void) const noexcept { return this->m_weather_fx_name; }

    void reset(void);
    void update(void);
    void select_weather(bool is_now);
    Script_WeatherData init_by_graph(const xr_map<xr_string, xr_string>& graph, const xr_string& weather_name);
    const xr_map<xr_string, xr_string>& get_graph_by_name(const xr_string& name);
    void set_state_as_string(const xr_string& data_name);
    xr_string get_state_as_string(void);

    void load(NET_Packet& packet);
    void save(NET_Packet& packet);

private:
    bool m_is_forced_weather_change_on_time_change;
    std::uint32_t m_update_time;
    std::uint32_t m_last_hour;
    float m_wfx_time;
    CScriptIniFile* m_p_ini;
    xr_map<xr_string, xr_map<xr_string, xr_string>> m_graphs;
    xr_map<std::uint32_t, CondlistData> m_weather_list;
    xr_map<xr_string, Script_WeatherData> m_state;
    xr_string m_level_name;
    xr_string m_weather_fx_name;
};
} // namespace Scripts
} // namespace Cordis
