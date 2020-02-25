#include "stdafx.h"
#include "Script_WeatherManager.h"

namespace Cordis
{
namespace Scripts
{
xr_string get_next_state(const xr_map<xr_string, xr_string>& graph, const xr_string& state_name)
{
    float sum = 0.0f;
    for (const std::pair<xr_string, xr_string>& it : graph)
    {
        sum += boost::lexical_cast<float>(it.second);
    }

    float rnd = Globals::Script_RandomFloat::getInstance().Generate() * sum;
    xr_string next_state_name;

    for (const std::pair<xr_string, xr_string>& it : graph)
    {
        next_state_name = it.first;
        rnd -= boost::lexical_cast<float>(it.second);
        if (rnd <= 0.0f)
            break;
    }

    return next_state_name;
}

Script_WeatherManager::~Script_WeatherManager(void)
{
    if (this->m_p_ini)
    {
        xr_delete(this->m_p_ini);
    }
}

void Script_WeatherManager::reset(void)
{
    Msg("[Scripts/Script_WeatherManager/reset()] is called!");
    CScriptIniFile ini("game.ltx");
    xr_string weather_name = Globals::Utils::cfg_get_string(ini, Globals::Game::level::get_name(), "weathers");

    if (weather_name.empty())
        weather_name = "[default]";

    xr_string postprocess_name = Globals::Utils::cfg_get_string(ini, Globals::Game::level::get_name(), "postprocess");

    if (!postprocess_name.empty())
    {
        Msg("[Scripts/Script_WeatherManager/reset()] level %s | postprocess %s",
            Globals::Game::level::get_name().c_str(), postprocess_name.c_str());
        Globals::Game::level::add_pp_effector(postprocess_name.c_str(), 999, true);
    }
    else
    {
        Msg("[Scripts/Script_WeatherManager/reset()] level %s | postprocess none",
            Globals::Game::level::get_name().c_str());
        Globals::Game::level::remove_pp_effector(999);
    }

    if (weather_name != "[default]")
    {
        this->m_weather_list =
            XR_LOGIC::parse_condlist_by_script_object(Globals::Game::level::get_name(), "weather", "[default]");
    }
    else
    {
        this->m_weather_list =
            XR_LOGIC::parse_condlist_by_script_object(Globals::Game::level::get_name(), "weather", weather_name);
    }

    this->select_weather(true);
    this->m_last_hour = Globals::get_time_hours();
}

void Script_WeatherManager::update(void)
{
    if (Globals::Game::level::is_wfx_playing())
    {
        this->m_weather_fx_name = Globals::Game::level::get_weather();
    }
    else
        this->m_weather_fx_name.clear();

    if (this->m_last_hour != Globals::get_time_hours())
    {
        this->m_last_hour = Globals::get_time_hours();
        for (std::pair<const xr_string, Script_WeatherData>& it : this->m_state)
        {
            it.second.setCurrentStateName(it.second.getNextStateName());
            it.second.setNextStateName(get_next_state(it.second.getGraph(), it.second.getCurrentStateName()));
        }

        this->select_weather(false);
    }
}

void Script_WeatherManager::select_weather(bool is_now)
{
    xr_string weather_name = XR_LOGIC::pick_section_from_condlist(
        DataBase::Storage::getInstance().getActor(), DataBase::Storage::getInstance().getActor(), this->m_weather_list);
    const xr_map<xr_string, xr_string>& graph = this->get_graph_by_name(weather_name);
    xr_string weather_section_name;

    if (graph.empty())
    {
        this->m_state[weather_name].Clear();
        weather_section_name = weather_name;
    }
    else
    {
        if (this->m_state[weather_name].isEmpty() || this->m_state[weather_name].getGraphName() != weather_name)
        {
            this->m_state[weather_name] = this->init_by_graph(graph, weather_name);
        }

        Script_WeatherData& storage = this->m_state.at(weather_name);
        weather_section_name = "default_";
        weather_section_name.append(storage.getCurrentStateName());
    }

    if (is_now)
    {
        this->m_last_hour = Globals::get_time_hours();
    }

    if (this->m_is_forced_weather_change_on_time_change)
    {
        this->m_is_forced_weather_change_on_time_change = false;
    }

    if (this->m_weather_fx_name.empty())
    {
        Globals::Game::level::set_weather(weather_name.c_str(), is_now);
    }
    else
    {
        Globals::Game::level::start_weather_fx_from_time(this->m_weather_fx_name.c_str(), this->m_wfx_time);
    }
}

Script_WeatherData Script_WeatherManager::init_by_graph(
    const xr_map<xr_string, xr_string>& graph, const xr_string& weather_name)
{
    xr_string current_state_name = get_next_state(graph, "");
    xr_string next_state_name = get_next_state(graph, current_state_name);
    Script_WeatherData data;
    data.setCurrentStateName(current_state_name);
    data.setNextStateName(next_state_name);
    data.setGraph(graph);
    data.setGraphName(weather_name);
    return data;
}

const xr_map<xr_string, xr_string>& Script_WeatherManager::get_graph_by_name(const xr_string& name)
{
    if (this->m_graphs.find(name) == this->m_graphs.end())
        this->m_graphs[name] = Globals::parse_ini_section_to_array(this->m_p_ini, name);

    return this->m_graphs.at(name);
}

void Script_WeatherManager::set_state_as_string(const xr_string& data_name)
{
    xr_string copy_data_name = data_name;
    boost::algorithm::trim(copy_data_name);
    boost::regex rgx("[^;]+");
    boost::sregex_token_iterator iter(copy_data_name.begin(), copy_data_name.end(), rgx);

    boost::sregex_token_iterator end;
    for (; iter != end; ++iter)
    {
        xr_string temporary = iter->str().c_str();
        boost::algorithm::trim(temporary);
        xr_string graph_name;
        xr_string current_state_name;
        xr_string next_state_name;

        current_state_name = temporary.substr(temporary.find("=") + 1);
        current_state_name.erase(current_state_name.rfind(","));
        next_state_name = temporary.substr(temporary.rfind(",") + 1);
        graph_name = temporary;
        graph_name.erase(temporary.find("="));
        boost::trim(next_state_name);
        boost::trim(current_state_name);
        boost::trim(graph_name);

        const xr_map<xr_string, xr_string>& graph = this->get_graph_by_name(graph_name);
        if (graph.empty())
        {
            // Empty code here
        }
        else
        {
            Script_WeatherData data;
            data.setCurrentStateName(current_state_name);
            data.setGraph(graph);
            data.setGraphName(graph_name);
            data.setNextStateName(next_state_name);
            this->m_state[graph_name] = data;
        }
    }
}

xr_string Script_WeatherManager::get_state_as_string(void)
{
    xr_string result_name;
    xr_vector<xr_string> temporary;
    for (std::pair<const xr_string, Script_WeatherData>& it : this->m_state)
    {
        xr_string generated_data_name;
        generated_data_name = it.second.getGraphName();
        generated_data_name.append("=");
        generated_data_name.append(it.second.getCurrentStateName());
        generated_data_name.append(",");
        generated_data_name.append(it.second.getNextStateName());
        generated_data_name.append(";");
        temporary.push_back(generated_data_name);
    }
    temporary.back().pop_back();

    for (const xr_string& it : temporary)
    {
        result_name += it;
    }

    return result_name;
}

void Script_WeatherManager::load(NET_Packet& packet)
{
    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Load, false, "Script_WeatherManager");
    Msg("[Scripts/Script_WeatherManager/load(packet)] is loading!");
    xr_string state_string;
    packet.r_stringZ(state_string);

    if (state_string.empty())
        Msg("[Scripts/Script_WeatherManager/load(packet)] state_string is empty!");

    this->set_state_as_string(state_string);
    this->m_update_time = packet.r_u32();
    xr_string weather_fx_name;
    packet.r_stringZ(weather_fx_name);
    if (weather_fx_name != "nil")
    {
        this->m_weather_fx_name = weather_fx_name;
        this->m_wfx_time = packet.r_float();
    }
    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Load, true, "Script_WeatherManager");
}

void Script_WeatherManager::save(NET_Packet& packet)
{
    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, false, "Script_WeatherManager");

    Msg("[Scripts/Script_WeatherManager/save(packet)] is saving!");

    if (this->get_state_as_string().empty())
        Msg("[Scripts/Script_WeatherManager/save(packet)] state_name is empty!");
    
    packet.w_stringZ(this->get_state_as_string().c_str());
    packet.w_u32(this->m_update_time);
    packet.w_stringZ(this->m_weather_fx_name.c_str());

    if (!this->m_weather_fx_name.empty())
        packet.w_float(Globals::Game::level::get_wfx_time());

    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, true, "Script_WeatherManager");
}

} // namespace Scripts
} // namespace Cordis
