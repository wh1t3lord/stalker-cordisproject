#pragma once

#include "Script_GlobalHelper.h"
#include "alife_registry_container_composition.h"
#include "Script_GulagGenerator.h"
#include "Script_StoryObject.h"
#include "game_sv_single.h"

namespace Cordis
{
namespace Scripts
{
class Script_SimulationBoard;
}
} // namespace Cordis

namespace Cordis
{
namespace Scripts
{
namespace Globals
{
namespace Utils
{
inline xr_string cfg_get_string(CScriptIniFile* char_ini, const xr_string& section, const xr_string& field,
    bool mandatory, const xr_string& gulag_name)
{
    if (section.size() &&
        (char_ini->section_exist(section.c_str()) && char_ini->line_exist(section.c_str(), field.c_str())))
    {
        xr_string result = "";
        result.append(gulag_name);
        result.append("_");
        result.append(char_ini->r_string(section.c_str(), field.c_str()));
        if (gulag_name.size())
        {
            if (result == "nil")
                result.clear();

            return result;
        }
        else
        {
            result = char_ini->r_string(section.c_str(), field.c_str());

            if (result == "nil")
                result.clear();

            return result;
        }
    }

    if (!mandatory)
        return "";

    MESSAGEW("Attempt to read a non-existant string field %s in section %s", field.c_str(), section.c_str());
    R_ASSERT(false);

    return "";
}

inline xr_string cfg_get_string(const CInifile* char_ini, const xr_string& section, const xr_string& field,
    bool mandatory, const xr_string& gulag_name)
{
    if (section.size() &&
        (char_ini->section_exist(section.c_str()) && char_ini->line_exist(section.c_str(), field.c_str())))
    {
        xr_string result = "";
        result.append(gulag_name);
        result.append("_");
        result.append(char_ini->r_string(section.c_str(), field.c_str()));
		if (gulag_name.size())
		{
			if (result == "nil")
				result.clear();

			return result;
		}
		else
		{
			result = char_ini->r_string(section.c_str(), field.c_str());

			if (result == "nil")
				result.clear();

			return result;
		}
    }

    if (!mandatory)
        return "";

    MESSAGEW("Attempt to read a non-existant string field %s in section %s", field.c_str(), section.c_str());
    R_ASSERT(false);

    return "";
}

inline xr_string cfg_get_string(
    CInifile& char_ini, const xr_string& section, const xr_string& field, bool mandatory, const xr_string& gulag_name)
{
    if (section.size() &&
        (char_ini.section_exist(section.c_str()) && char_ini.line_exist(section.c_str(), field.c_str())))
    {
        xr_string result = "";
        result.append(gulag_name);
        result.append("_");
        result.append(char_ini.r_string(section.c_str(), field.c_str()));
		if (gulag_name.size())
		{
			if (result == "nil")
				result.clear();

			return result;
		}
		else
		{
			result = char_ini.r_string(section.c_str(), field.c_str());

			if (result == "nil")
				result.clear();

			return result;
		}
    }

    if (!mandatory)
        return "";

    MESSAGEW("Attempt to read a non-existant string field %s in section %s", field.c_str(), section.c_str());
    R_ASSERT(false);

    return "";
}

inline float graph_distance(const std::uint16_t& gamevertexid_1, const std::uint16_t& gamevertexid_2)
{
    return ai()
        .game_graph()
        .vertex(gamevertexid_1)
        ->game_point()
        .distance_to(ai().game_graph().vertex(gamevertexid_2)->game_point());
}

inline bool cfg_get_bool(const CInifile* char_ini, const xr_string& section, const xr_string& field,
    CScriptGameObject* object, bool mandatory)
{
    xr_string object_name;

    if (!section.size() || !field.size())
    {
        R_ASSERT2(false, "string can't be null");
        return false;
    }

    if (!object)
        object_name = "Unknown Object";
    else
        object_name = object->Name();

    if (char_ini->section_exist(section.c_str()) && char_ini->line_exist(section.c_str(), field.c_str()))
    {
        bool result = char_ini->r_bool(section.c_str(), field.c_str());
        return result;
    }

    if (!mandatory)
    {
        return false;
    }

    MESSAGEW("ERROR object %s attempt to read a non-existant boolean field %s in section %s", object_name.c_str(),
        field.c_str(), section.c_str());
    R_ASSERT(false);
    return false;
}

inline bool cfg_get_bool(const CInifile* char_ini, const xr_string& section, const xr_string& field,
    CSE_ALifeDynamicObject* object, bool mandatory)
{
    xr_string object_name;

    if (!section.size() || !field.size())
    {
        R_ASSERT2(false, "string can't be null");
        return false;
    }

    if (!object)
        object_name = "Unknown Object";
    else
        object_name = object->name();

    if (char_ini->section_exist(section.c_str()) && char_ini->line_exist(section.c_str(), field.c_str()))
    {
        bool result = char_ini->r_bool(section.c_str(), field.c_str());
        return result;
    }

    if (!mandatory)
    {
        return false;
    }

    Msg("[Script]: ERROR object %s attempt to read a non-existant boolean field %s in section %s", object_name.c_str(),
        field.c_str(), section.c_str());
    R_ASSERT(false);
    return false;
}

inline bool cfg_get_bool(CScriptIniFile* char_ini, const xr_string& section, const xr_string& field,
    CScriptGameObject* object, bool mandatory)
{
    xr_string object_name;

    if (!section.size() || !field.size())
    {
        R_ASSERT2(false, "string can't be null");
        return false;
    }

    if (!object)
        object_name = "Unknown Object";
    else
        object_name = object->Name();

    if (char_ini->section_exist(section.c_str()) && char_ini->line_exist(section.c_str(), field.c_str()))
    {
        bool result = char_ini->r_bool(section.c_str(), field.c_str());
        return result;
    }

    if (!mandatory)
    {
        return false;
    }

    Msg("[Script]: ERROR object %s attempt to read a non-existant boolean field %s in section %s", object_name.c_str(),
        field.c_str(), section.c_str());
    R_ASSERT(false);
    return false;
}

inline bool cfg_get_bool(CScriptIniFile* char_ini, const xr_string& section, const xr_string& field,
    CSE_ALifeDynamicObject* object, bool mandatory)
{
    xr_string object_name;

    if (!section.size() || !field.size())
    {
        R_ASSERT2(false, "string can't be null");
        return false;
    }

    if (!object)
        object_name = "Unknown Object";
    else
        object_name = object->name();

    if (char_ini->section_exist(section.c_str()) && char_ini->line_exist(section.c_str(), field.c_str()))
    {
        bool result = char_ini->r_bool(section.c_str(), field.c_str());
        return result;
    }

    if (!mandatory)
    {
        return false;
    }

    Msg("[Script]: ERROR object %s attempt to read a non-existant boolean field %s in section %s", object_name.c_str(),
        field.c_str(), section.c_str());
    R_ASSERT(false);
    return false;
}

inline float cfg_get_number(const CInifile* char_ini, const xr_string& section, const xr_string& field,
    CScriptGameObject* object, bool mandatory)
{
    xr_string object_name;

    if (!section.size() || !field.size())
    {
        R_ASSERT2(false, "string can't be null!");
        return 0.0f;
    }

    if (char_ini->section_exist(section.c_str()) && char_ini->line_exist(section.c_str(), field.c_str()))
        return char_ini->r_float(section.c_str(), field.c_str());

    Msg("[Script] cfg_get_number has returned a default value");
    return 0.0f;
}

inline float cfg_get_number(const CInifile* char_ini, const xr_string& section, const xr_string& field,
    CSE_ALifeDynamicObject* object, bool mandatory)
{
    xr_string object_name;

    if (!section.size() || !field.size())
    {
        R_ASSERT2(false, "string can't be null!");
        return 0.0f;
    }

    if (char_ini->section_exist(section.c_str()) && char_ini->line_exist(section.c_str(), field.c_str()))
        return char_ini->r_float(section.c_str(), field.c_str());

    Msg("[Script] cfg_get_number has returned a default value");
    return 0.0f;
}

inline float cfg_get_number(CScriptIniFile* char_ini, const xr_string& section, const xr_string& field,
    CScriptGameObject* object, bool mandatory)
{
    xr_string object_name;

    if (!section.size() || !field.size())
    {
        R_ASSERT2(false, "string can't be null!");
        return 0.0f;
    }

    if (char_ini->section_exist(section.c_str()) && char_ini->line_exist(section.c_str(), field.c_str()))
        return char_ini->r_float(section.c_str(), field.c_str());

    Msg("[Script] cfg_get_number has returned a default value");
    return 0.0f;
}

inline float cfg_get_number(CScriptIniFile* char_ini, const xr_string& section, const xr_string& field,
    CSE_ALifeDynamicObject* object, bool mandatory)
{
    xr_string object_name;

    if (!section.size() || !field.size())
    {
        R_ASSERT2(false, "string can't be null!");
        return 0.0f;
    }

    if (char_ini->section_exist(section.c_str()) && char_ini->line_exist(section.c_str(), field.c_str()))
        return char_ini->r_float(section.c_str(), field.c_str());

    Msg("[Script] cfg_get_number has returned a default value");
    return 0.0f;
}

// xr_string cfg_get_string(const CInifile& char_ini, const xr_string& section, const xr_string& field,
//     bool mandatory = false, const xr_string& gulag_name = "")
// {
//     if (section.size() &&
//         (char_ini.section_exist(section.c_str()) && char_ini.line_exist(section.c_str(), field.c_str())))
//     {
//         xr_string result = "";
//         result.append(gulag_name);
//         result.append("_");
//         result.append(char_ini.r_string(section.c_str(), field.c_str()));
//         if (gulag_name.size())
//             return result;
//         else
//             return char_ini.r_string(section.c_str(), field.c_str());
//     }
//
//     if (!mandatory)
//         return "";
//
//     Msg("Attempt to read a non-existant string field %s in section %s", field.c_str(), section.c_str());
//     R_ASSERT(false);
//
//     return "";
// }
inline xr_string get_scheme_by_section(xr_string& data)
{
    if (!data.size())
    {
        R_ASSERT2(false, "string can't be empty!");
        return xr_string("sr_idle");
    }

    xr_string result = data;
    if (result.find('@') == xr_string::npos)
    {
        //    R_ASSERT2(false, "invalid string!");
        Msg("[Scripts/Globals/Utils/get_scheme_by_section(data)] Does it right logic section name %s ?", data.c_str());
        return result;
    }

    return result.erase(result.find('@'));
}

inline xr_vector<xr_string> parse_names(const xr_string& buffer)
{
    xr_vector<xr_string> result;
    if (!buffer.size())
    {
        Msg("[Scripts/Globals/Utils/parse_names(buffer)] WARNING: buffer.size() = 0! You are trying to parse an empty "
            "string! Return empty vector");
        return result;
    }

    const char* pattern = "[^,]+";
    boost::regex regex(pattern);

    boost::sregex_iterator it(buffer.begin(), buffer.end(), regex);
    boost::sregex_iterator end;

    for (; it != end; ++it)
    {
        xr_string value_name = it->str().c_str();
        boost::algorithm::trim(value_name);

        result.push_back(value_name);
    }

    return result;
}

inline void r_2nums(const CInifile& spawn_ini, const xr_string& section_name, const xr_string& line_name,
    std::uint32_t& number_min, std::uint32_t& number_max)
{
    if (spawn_ini.line_exist(section_name.c_str(), line_name.c_str()))
    {
        xr_vector<xr_string> buffer = parse_names(spawn_ini.r_string(section_name.c_str(), line_name.c_str()));
        std::uint32_t count = buffer.size();

        if (count == 0)
            return;

        if (count == 1)
        {
            number_min = atoi(buffer[0].c_str());
        }
        else if (count > 1)
        {
            number_min = atoi(buffer[0].c_str());
            number_max = atoi(buffer[1].c_str());
        }
    }
}

// first -> section | second -> prob
inline xr_vector<std::pair<xr_string, float>> parse_spawns(const xr_string& buffer)
{
    xr_vector<std::pair<xr_string, float>> result;

    if (!buffer.size())
    {
        R_ASSERT2(false, "string can't be empty!");
        return result;
    }

    xr_vector<xr_string> buffer_names = parse_names(buffer);

    std::uint32_t count = buffer_names.size() - 1;

    std::uint32_t it = 0;

    while (it <= count)
    {
        std::pair<xr_string, std::uint32_t> data;
        data.first = buffer_names[it];

        // Lord: наверное лучше сделать через try
        if (buffer_names[it + 1].size() && it + 1 <= count)
        {
            std::uint32_t value = atoi(buffer_names[it + 1].c_str());

            if (value)
            {
                data.second = value;
                it += 2;
            }
            else
            {
                data.second = 1.0f;
                it += 1;
            }
        }
        else
        {
            data.second = 1.0f;
            it += 1;
        }

        result.push_back(data);
    }

    return result;
}

inline bool is_npc_in_zone(CScriptGameObject* npc, CScriptGameObject* zone)
{
    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!zone)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    return zone->inside(npc->Position());
}

inline bool is_npc_in_zone(CSE_ALifeDynamicObject* server_object, CScriptGameObject* zone)
{
    if (!server_object)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!zone)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    return zone->inside(server_object->Position());
}

inline xrTime r_CTime(NET_Packet& packet)
{
    int Y = packet.r_u8();
    if (Y == Globals::kUnsignedInt8Undefined)
    {
        Msg("[Scripts/Globals/Utils/r_CTime(packet)] WARNING: bad initialize time from net_packet!");
        return xrTime();
    }

    if (Y != 0)
    {
        xrTime time = xrTime();
        std::uint8_t mounth = packet.r_u8();
        std::uint8_t day = packet.r_u8();
        std::uint8_t hour = packet.r_u8();
        std::uint8_t minute = packet.r_u8();
        std::uint8_t second = packet.r_u8();
        std::uint16_t milisecond = packet.r_u16();
        time.set(Y + 2000, mounth, day, hour, minute, second, milisecond);

        return time;
    }
    else
    {
        Msg("[Scripts/Globals/Utils/r_CTime(packet)] WARNING: can't initialize time from net_packet!");
        return xrTime();
    }
}

inline xrTime r_CTime(IReader& packet)
{
    int Y = packet.r_u8();
    if (Y == Globals::kUnsignedInt8Undefined)
    {
        Msg("[Scripts/Globals/Utils/r_CTime(packet)] WARNING: bad initialize time from IReader!");
        return xrTime();
    }

    if (Y != 0)
    {
        xrTime time = xrTime();
        std::uint8_t mounth = packet.r_u8();
        std::uint8_t day = packet.r_u8();
        std::uint8_t hour = packet.r_u8();
        std::uint8_t minute = packet.r_u8();
        std::uint8_t second = packet.r_u8();
        std::uint16_t milisecond = packet.r_u16();
        time.set(Y + 2000, mounth, day, hour, minute, second, milisecond);

        return time;
    }
    else
    {
        Msg("[Scripts/Globals/Utils/r_CTime(packet)] WARNING: can't initialize time from IReader!");
        return xrTime();
    }
}

inline void w_CTime(NET_Packet& packet, xrTime& time)
{
    if (time == 0)
    {
        Msg("[Scripts/Globals/Utils/w_CTime(packet, time)] WARNING: invalid xrTime structure, writing "
            "std::uint8_t(-1). Return...");
        packet.w_u8(Globals::kUnsignedInt8Undefined);
        return;
    }

    if (!(time == xrTime()))
    {
        std::uint32_t year = 0;
        std::uint32_t mounth = 0;
        std::uint32_t day = 0;
        std::uint32_t hour = 0;
        std::uint32_t minute = 0;
        std::uint32_t second = 0;
        std::uint32_t milisecond = 0;

        time.get(year, mounth, day, hour, minute, second, milisecond);

        packet.w_u8(year - 2000);
        packet.w_u8(mounth);
        packet.w_u8(day);
        packet.w_u8(hour);
        packet.w_u8(minute);
        packet.w_u8(second);
        packet.w_u16(milisecond);
    }
    else
    {
        Msg("[Scripts/Globals/Utils/w_CTime(packet, time)] WARNING: time equals default constructor xrTime()!");
        packet.w_u8(0);
    }
}

inline CondlistWaypoints::CondlistWayPointsData parse_waypoint_data(
    const xr_string& name, const Flags32& waypoint_flags, const xr_string& waypoint_name)
{
    CondlistWaypoints::CondlistWayPointsData result;

    result.setFlags(waypoint_flags);

    if (waypoint_name.find('|') == xr_string::npos)
    {
        return result;
    }

    boost::regex regex("\\w+=\\w+");
    boost::sregex_iterator i(waypoint_name.begin(), waypoint_name.end(), regex);
    boost::sregex_iterator j;

    // Строгая проверка, ибо в последующем в move_mgr и mob_walker не будет вылета
    // а понять трудно почему при a="название_анимации" не будет ничего работать
    for (; i != j; ++i)
    {
        xr_string data = i->str().c_str();
        xr_string field_name = data;
        field_name.erase(field_name.find('='));

        if (field_name.empty())
        {
            R_ASSERT2(false, "Incorrect data before '=', check your waypoint description!");
            return result;
        }

        xr_string value = data.substr(data.rfind('=') + 1);
        if (value.empty())
        {
            R_ASSERT2(false, "Incorrect data after '=', check your waypoint description!");
            return result;
        }

        result.setData(field_name, value);
    }

    return result;
}

inline CondlistWaypoints path_parse_waypoints(const xr_string& path_name)
{
    if (path_name.empty())
    {
        Msg("[Scripts/Globals/Utils/path_parse_waypoints(path_name)] WARNING: path_name.empty() == true! Return empty "
            "object ...");
        return CondlistWaypoints();
    }

    CondlistWaypoints result;

    CPatrolPathParams patrol = CPatrolPathParams(path_name.c_str());
    std::uint32_t count = patrol.count();

    // Lord: проверить правильно ли был составлен цикл на итерации
    for (std::uint32_t i = 0; i < count; ++i)
    {
        result.setData(parse_waypoint_data(path_name, patrol.flags(i), patrol.name(i)));
    }

    return result;
}

inline CondlistWaypoints path_parse_waypoints_argumentlist(const xr_string& path_name, const std::uint32_t point_count,
    const xr_vector<std::pair<std::uint32_t, xr_string>>& data)
{
    if (path_name.empty())
    {
        Msg("[Scripts/Globals/Utils/path_parse_waypoints(path_name)] WARNING: path_name.empty() == true! Return empty "
            "object ...");
        return CondlistWaypoints();
    }

    CondlistWaypoints result;

    CPatrolPathParams patrol = CPatrolPathParams(path_name.c_str());
    std::uint32_t count = patrol.count();

    if (count != point_count)
    {
        Msg("[Scripts/Globals/Utils/path_parse_waypoints_argumentlist(path_name, point_count, data)] WARNING: count doesn't equal point_count! Return ...");
        return CondlistWaypoints();
    }

    // Lord: проверить правильно ли был составлен цикл на итерации
    for (std::uint32_t i = 0; i < count; ++i)
    {
        Flags32 flags;
        flags.assign(data[i].first);
        result.setData(parse_waypoint_data(path_name, flags, data[i].second));
    }

    return result;
}

bool is_stalker_at_waypoint(
    CScriptGameObject* p_client_object, CPatrolPathParams& patrol_path, const std::uint32_t path_point)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return false;
    }

    const Fvector& entity_position = p_client_object->Position();

    float distance = entity_position.distance_to_sqr(patrol_path.point(path_point));
    if (distance <= 0.13f)
        return true;

    return false;
}

inline xr_vector<xr_string> parse_params(const xr_string& params)
{
    xr_vector<xr_string> result;
    if (params.empty())
    {
        Msg("[Scripts/Globals/Utils/parse_params(params)] params.empty() == true! Return empty vector");
        return result;
    }

    boost::regex rgx("\\w+|[^\\|\\[\\]]+");
    boost::sregex_token_iterator iter(params.begin(), params.end(), rgx);
    boost::sregex_token_iterator end;
    for (; iter != end; ++iter)
    {
        xr_string temporary = iter->str().c_str();
        boost::algorithm::trim(temporary);

        if (!temporary.empty())
            result.push_back(temporary);
    }

    return result;
}

inline std::uint32_t send_to_nearest_accessible_vertex(
    CScriptGameObject* const p_client_object, const std::uint32_t vertex_id)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return 0;
    }

    std::uint32_t _detected_vertex = Globals::kUnsignedInt32Undefined;
    if (!p_client_object->accessible_vertex_id(vertex_id))
    {
        Fvector result;
        _detected_vertex = p_client_object->accessible_nearest(Game::level::vertex_position(vertex_id), result);
    }

    p_client_object->set_dest_level_vertex_id(
        (_detected_vertex == Globals::kUnsignedInt32Undefined) ? vertex_id : _detected_vertex);

    return (_detected_vertex == Globals::kUnsignedInt32Undefined) ? vertex_id : _detected_vertex;
}

inline xr_map<std::uint32_t, xr_map<std::uint32_t, CondlistData>> parse_data_1v(
    CScriptGameObject* const p_client_object, const xr_string& buffer_name)
{
    xr_map<std::uint32_t, xr_map<std::uint32_t, CondlistData>> result;

    if (buffer_name.empty())
    {
        Msg("[Scripts/Globals/Utils/parse_data_1v(p_client_object, buffer_name)] WARNING: buffer_name.empty() == true! "
            "Can't parse an empty string return ...");
        return result;
    }

    boost::regex rgx("\\w+|[^\\|\\[\\]]+");
    boost::sregex_token_iterator iter(buffer_name.begin(), buffer_name.end(), rgx);
    boost::sregex_token_iterator end;
    bool is_condlist_found = false;
    xr_string previous_data;
    for (; iter != end; ++iter)
    {
        xr_string temporary = iter->str().c_str();
        boost::algorithm::trim(temporary);
        if (temporary.empty())
            continue;

        if (!is_condlist_found)
        {
            previous_data = temporary;
            is_condlist_found = true;
            continue;
        }

        if (atoi(temporary.c_str()) != 0)
        {
            R_ASSERT2(false, "can't be it must be a string or condlist!!!");
        }

        result[atoi(previous_data.c_str())] =
            XR_LOGIC::parse_condlist_by_script_object(previous_data, temporary, temporary);
        is_condlist_found = false;
    }

    return result;
}

inline xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>> parse_data(const xr_string& buffer_name)
{
    xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>> result;

    if (buffer_name.empty())
    {
        MESSAGEWR("string is empty can't parse data");
        return result;
    }

	boost::regex rgx("\\w+|[^\\|\\[\\]]+");
	boost::sregex_token_iterator iter(buffer_name.begin(), buffer_name.end(), rgx);
	boost::sregex_token_iterator end;
	bool is_condlist_found = false;
    bool is_first = true;
	xr_string previous_data;
    std::uint32_t index = 0;
	for (; iter != end; ++iter)
	{
		xr_string temporary = iter->str().c_str();
		boost::algorithm::trim(temporary);
        if (temporary.empty())
        {
            R_ASSERT2(false, "it can't be! Check your string data");
            return result;
        }

        if (is_first)
        {
            if ((atoi(temporary.c_str()) == 0) && (temporary != "0"))
            {
                R_ASSERT2(false, "first value must be integer or number not a real string!");
                return result;
            }
            is_first = false;
        }

		if (!is_condlist_found)
		{
			previous_data = temporary;
			is_condlist_found = true;
			continue;
		}

		if (atoi(temporary.c_str()) != 0)
		{
			R_ASSERT2(false, "can't be it must be a string or condlist!!!");
		}

        std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>> data;
        std::get<0>(data) = atoi(previous_data.c_str());
        if (temporary.find('@') != xr_string::npos)
        {
            xr_string state_name = temporary.substr(0, temporary.find('@'));
            xr_string sound_name = temporary.substr(temporary.rfind('@') + 1);

            std::get<1>(data) = XR_LOGIC::parse_condlist_by_script_object(previous_data, state_name, state_name);
            std::get<2>(data) = XR_LOGIC::parse_condlist_by_script_object(previous_data, sound_name, sound_name);
        }
        else
        {
            std::get<1>(data) = XR_LOGIC::parse_condlist_by_script_object(previous_data, temporary, temporary);
        }

        result[index] = data;
		is_condlist_found = false;
        ++index;
	}
}

} // namespace Utils

namespace Game
{
inline bool has_active_tutotial(void) { return (!!Script_GlobalHelper::getInstance().getGlobalTutorial()); }

void start_tutorial(LPCSTR tutorial_name)
{
    if (!tutorial_name)
    {
        R_ASSERT2(false, "string can't be empty!!");
    }

    if (!Script_GlobalHelper::getInstance().getGlobalTutorial())
    {
        CUISequencer* instance = new CUISequencer();
        Script_GlobalHelper::getInstance().setGlobalTutorial(instance);
        Script_GlobalHelper::getInstance().getGlobalTutorial()->Start(tutorial_name);
    }
}

void stop_tutorial(void)
{
    if (Script_GlobalHelper::getInstance().getGlobalTutorial())
        Script_GlobalHelper::getInstance().getGlobalTutorial()->Stop();
}

inline LPCSTR translate_string(LPCSTR str) { return *StringTable().translate(str); }
inline LPCSTR translate_string(const xr_string& string) { return translate_string(string.c_str()); }
inline xrTime get_game_time(void) noexcept { return get_time_struct(); }
inline const CGameGraph* get_game_graph() { return &GEnv.AISpace->game_graph(); }
inline CSE_Abstract* alife_create(
    const xr_string& section, const Fvector& position, std::uint32_t level_vertex_id, std::uint16_t game_vertex_id)
{
    CALifeSimulator* alife = const_cast<CALifeSimulator*>(ai().get_alife());

    VERIFY(alife);

    CSE_Abstract* server_instance =
        alife->spawn_item(section.c_str(), position, level_vertex_id, game_vertex_id, std::uint16_t(-1));

    if (server_instance)
        Msg("[Scripts/Globals/Game/alife_create(section_name, position, level_vertex_id, game_vertex_id)] "
            "server_instance is created name -> [%s]\n section_name -> [%s]\n x -> [%f] y -> [%f] z -> [%f] "
            "level_vertex_id -> [%d] game_vertex_id [%d]",
            server_instance->name_replace(), server_instance->name(), position.x, position.y, position.z,
            level_vertex_id, game_vertex_id);

    return server_instance;
}

inline CSE_Abstract* alife_create(const xr_string& section, const Fvector& position,
    const std::uint32_t level_vertex_id, const std::uint16_t game_vertex_id, const std::uint16_t parent_id)
{
    if (parent_id == kUnsignedInt16Undefined)
        return alife_create(section, position, level_vertex_id, game_vertex_id);

    CSE_ALifeDynamicObject* object = ai().alife().objects().object(parent_id, true);
    if (!object)
    {
        Msg("! invalid parent id [%d] specified", parent_id);
        return (0);
    }

    CALifeSimulator* alife = const_cast<CALifeSimulator*>(ai().get_alife());

    if (!object->m_bOnline)
        return (alife->spawn_item(section.c_str(), position, level_vertex_id, game_vertex_id, parent_id));

    NET_Packet packet;
    packet.w_begin(M_SPAWN);
    packet.w_stringZ(section.c_str());

    CSE_Abstract* item =
        alife->spawn_item(section.c_str(), position, level_vertex_id, game_vertex_id, parent_id, false);
    item->Spawn_Write(packet, FALSE);
    alife->server().FreeID(item->ID, 0);
    F_entity_Destroy(item);

    ClientID clientID;
    clientID.set(0xffff);

    u16 dummy;
    packet.r_begin(dummy);
    VERIFY(dummy == M_SPAWN);
    return (alife->server().Process_spawn(packet, clientID));
}

inline void alife_release(CSE_Abstract* object, bool no_assert)
{
    // VERIFY(self);
    //	self->release						(object,true);

    THROW(object);
    CSE_ALifeObject* alife_object = smart_cast<CSE_ALifeObject*>(object);
    THROW(alife_object);
    if (!alife_object->m_bOnline)
    {
        CALifeSimulator* p_alife = const_cast<CALifeSimulator*>(ai().get_alife());

        if (p_alife)
            p_alife->release(object, no_assert);

        return;
    }

    // awful hack, for stohe only
    NET_Packet packet;
    packet.w_begin(M_EVENT);
    packet.w_u32(Level().timeServer());
    packet.w_u16(GE_DESTROY);
    packet.w_u16(object->ID);
    Level().Send(packet, net_flags(TRUE, TRUE));
}

inline xr_string get_squad_relation_to_actor_by_id(const std::uint16_t& squad_id)
{
    Script_SE_SimulationSquad* squad = ai().alife().objects().object(squad_id)->cast_script_se_simulationsquad();

    if (!squad)
    {
        Msg("[Scripts/Globals/Game/get_squad_relation_to_actor_by_id(squad_id)] No scuh squad %s in board",
            std::to_string(squad_id).c_str());
        R_ASSERT(false);
        return "";
    }

    int goodwill = 0;
    int npc_count = 0;

    CScriptGameObject* actor = DataBase::Storage::getInstance().getActor();

    for (AssociativeVector<std::uint16_t, CSE_ALifeMonsterAbstract*>::const_iterator it =
             squad->squad_members().begin();
         it != squad->squad_members().end(); ++it)
    {
        CScriptGameObject* client_object = nullptr;
        if (DataBase::Storage::getInstance().getStorage().find(it->first) !=
            DataBase::Storage::getInstance().getStorage().end())
            client_object = DataBase::Storage::getInstance().getStorage().at(it->first).getClientObject();

        if (client_object && actor)
        {
            goodwill += client_object->GetAttitude(actor);
            npc_count += 1;
        }
    }

    if (npc_count)
    {
        int delta = goodwill / npc_count;

        if (delta <= kRelationKoeffEnemy)
            return kRelationsTypeEnemy;

        if (delta >= kRelationKoeffFriend)
            return kRelationsTypeFriends;

        if (delta < kRelationKoeffFriend && delta > kRelationKoeffEnemy)
            return kRelationsTypeNeutral;
    }

    return kRelationsTypeEnemy;
}

inline std::uint32_t time_global(void) noexcept { return Device.dwTimeGlobal; }

namespace level
{
inline Fvector vertex_position(u32 level_vertex_id) { return (ai().level_graph().vertex_position(level_vertex_id)); }
inline bool is_level_present(void) { return (!!g_pGameLevel); }
inline u32 vertex_id(Fvector position) { return (ai().level_graph().vertex_id(position)); }
inline float rain_factor(void) { return (g_pGamePersistent->Environment().CurrentEnv->rain_density); }
inline CScriptGameObject* get_object_by_id(u16 id)
{
    CGameObject* pGameObject = smart_cast<CGameObject*>(Level().Objects.net_Find(id));
    if (!pGameObject)
        return nullptr;

    return pGameObject->lua_game_object();
}

inline xr_string get_name(void) { return (*Level().name()); }
inline void map_remove_object_spot(const std::uint16_t& id, LPCSTR spot_type)
{
    Level().MapManager().RemoveMapLocation(spot_type, id);
}

inline void map_add_object_spot(const std::uint16_t& id, LPCSTR spot_type, LPCSTR text)
{
    CMapLocation* ml = Level().MapManager().AddMapLocation(spot_type, id);
    if (xr_strlen(text))
    {
        ml->SetHint(text);
    }
}

inline std::uint16_t map_has_object_spot(const std::uint16_t& id, LPCSTR spot_type)
{
    return Level().MapManager().HasMapLocation(spot_type, id);
}

inline void map_add_object_spot_ser(const std::uint16_t& id, LPCSTR spot_type, LPCSTR text)
{
    CMapLocation* ml = Level().MapManager().AddMapLocation(spot_type, id);
    if (xr_strlen(text))
        ml->SetHint(text);

    ml->SetSerializable(true);
}

inline void map_change_spot_hint(const std::uint16_t& id, LPCSTR spot_type, LPCSTR text)
{
    CMapLocation* ml = Level().MapManager().GetMapLocation(spot_type, id);
    if (!ml)
        return;
    ml->SetHint(text);
}
inline void add_pp_effector(LPCSTR fn, int id, bool cyclic)
{
    CPostprocessAnimator* pp = new CPostprocessAnimator(id, cyclic);
    pp->Load(fn);
    Actor()->Cameras().AddPPEffector(pp);
}
inline float get_snd_volume(void) noexcept { return psSoundVFactor; }
inline void set_snd_volume(const float value) noexcept
{
    psSoundVFactor = value;
    clamp(psSoundVFactor, 0.0f, 1.0f);
}
inline void spawn_phantom(const Fvector& position)
{
    Level().spawn_item("m_phantom", position, u32(-1), u16(-1), false);
}
inline void remove_pp_effector(int id)
{
    CPostprocessAnimator* pp = smart_cast<CPostprocessAnimator*>(Actor()->Cameras().GetPPEffector((EEffectorPPType)id));

    if (pp)
        pp->Stop(1.0f);
}
inline void set_pp_effector_factor(int id, float f, float f_sp)
{
    CPostprocessAnimator* pp = smart_cast<CPostprocessAnimator*>(Actor()->Cameras().GetPPEffector((EEffectorPPType)id));

    if (pp)
        pp->SetDesiredFactor(f, f_sp);
}
inline void set_pp_effector_factor(int id, float f)
{
    CPostprocessAnimator* pp = smart_cast<CPostprocessAnimator*>(Actor()->Cameras().GetPPEffector((EEffectorPPType)id));

    if (pp)
        pp->SetCurrentFactor(f);
}
inline float add_cam_effector(LPCSTR fn, int id, bool cyclic, LPCSTR cb_func)
{
    CAnimatorCamEffectorScriptCB* e = new CAnimatorCamEffectorScriptCB(cb_func);
    e->SetType((ECamEffectorType)id);
    e->SetCyclic(cyclic);
    e->Start(fn);
    Actor()->Cameras().AddCamEffector(e);
    return e->GetAnimatorLength();
}
inline float add_cam_effector2(LPCSTR fn, int id, bool cyclic, LPCSTR cb_func, float cam_fov)
{
    CAnimatorCamEffectorScriptCB* e = new CAnimatorCamEffectorScriptCB(cb_func);
    e->m_bAbsolutePositioning = true;
    e->m_fov = cam_fov;
    e->SetType((ECamEffectorType)id);
    e->SetCyclic(cyclic);
    e->Start(fn);
    Actor()->Cameras().AddCamEffector(e);
    return e->GetAnimatorLength();
}
inline void remove_cam_effector(int id) { Actor()->Cameras().RemoveCamEffector((ECamEffectorType)id); }
inline void show_weapon(bool b) { psHUD_Flags.set(HUD_WEAPON_RT2, b); }
inline void disable_input(void)
{
    g_bDisableAllInput = true;
    Msg("[Scripts/Globals/Game/level/disable_input()] input disabled");
}

inline void enable_input(void)
{
    g_bDisableAllInput = false;
    Msg("input enabled");
}
inline void hide_indicators_safe(void)
{
    if (CurrentGameUI())
    {
        CurrentGameUI()->ShowGameIndicators(false);
        CurrentGameUI()->ShowCrosshair(false);

        CurrentGameUI()->OnExternalHideIndicators();
    }
    psActorFlags.set(AF_GODMODE_RT, TRUE);
}
inline void show_indicators(void)
{
    if (CurrentGameUI())
    {
        CurrentGameUI()->ShowGameIndicators(true);
        CurrentGameUI()->ShowCrosshair(true);
    }
    psActorFlags.set(AF_GODMODE_RT, FALSE);
}
inline void add_complex_effector(LPCSTR section, int id) { AddEffector(Actor(), id, section); }
inline void remove_complex_effector(int id) { RemoveEffector(Actor(), id); }
inline float get_time_factor(void) { return (Level().GetGameTimeFactor()); }
LPCSTR get_weather(void) { return *g_pGamePersistent->Environment().GetWeather(); }
void set_weather(pcstr const weather_name, const bool forced)
{
    if (!Device.editor())
        g_pGamePersistent->Environment().SetWeather(weather_name, forced);
}

bool set_weather_fx(pcstr const weather_name)
{
    if (!Device.editor())
        return g_pGamePersistent->Environment().SetWeatherFX(weather_name);

    return false;
}
inline bool is_wfx_playing(void) { return (g_pGamePersistent->Environment().IsWFXPlaying()); }
inline float get_wfx_time(void) { return (g_pGamePersistent->Environment().wfx_time); }
inline void stop_weather_fx(void) { g_pGamePersistent->Environment().StopWFX(); }
inline void set_time_factor(const float time_factor)
{
    if (!OnServer() || Device.editor())
        return;

    Level().Server->GetGameState()->SetGameTimeFactor(time_factor);
}
inline bool start_weather_fx_from_time(pcstr const weather_name, const float time)
{
    if (!Device.editor())
        return g_pGamePersistent->Environment().StartWeatherFXFromTime(weather_name, time);

    return false;
}
} // namespace level
} // namespace Game

namespace GameRelations
{
inline bool check_all_squad_members(const xr_string& squad_name, const xr_string& goodwill_name)
{
    if (!squad_name.size())
    {
        R_ASSERT2(false, "string can't be empty!");
        return false;
    }

    if (!goodwill_name.size())
    {
        R_ASSERT2(false, "string can't be empty!");
        return false;
    }

    Script_SE_SimulationSquad* squad =
        ai().alife().objects().object(get_story_object_id(squad_name))->cast_script_se_simulationsquad();
    if (squad && DataBase::Storage::getInstance().getActor())
    {
        for (AssociativeVector<std::uint16_t, CSE_ALifeMonsterAbstract*>::const_iterator it =
                 squad->squad_members().begin();
             it != squad->squad_members().end(); ++it)
        {
            bool is_goodwill = false;
            CScriptGameObject* object = DataBase::Storage::getInstance().getStorage().at(it->first).getClientObject();
            if (goodwill_name == Globals::kRelationsTypeEnemy)
            {
                if (object)
                {
                    Msg("[Scripts/Globals/GameRelations/check_all_squad_members(squad_name, goodwill_name)] a "
                        "member [%d] is an enemy",
                        it->first);
                    is_goodwill = (object->GetAttitude(DataBase::Storage::getInstance().getActor()) <=
                        Globals::kRelationKoeffEnemy);
                }
            }
            else
            {
                if (object)
                {
                    Msg("[Scripts/Globals/GameRelations/check_all_squad_members(squad_name, goodwill_name)] a "
                        "member [%d] is a friend",
                        it->first);
                    is_goodwill = (object->GetAttitude(DataBase::Storage::getInstance().getActor()) >=
                        Globals::kRelationKoeffFriend);
                }
            }

            if (is_goodwill)
                return true;
        }
    }

    return false;
}

inline int community_goodwill(LPCSTR community_name, int entity_id)
{
    CHARACTER_COMMUNITY community;
    community.set(community_name);

    return RELATION_REGISTRY().GetCommunityGoodwill(community.index(), std::uint16_t(entity_id));
}

inline int get_community_relation(const xr_string& faction, const xr_string& faction_to)
{
    if (!faction.size())
    {
        R_ASSERT2(false, "can't be an empty string!");
        return 0;
    }

    if (!faction_to.size())
    {
        R_ASSERT2(false, "can't be an empty string!");
        return 0;
    }

    CHARACTER_COMMUNITY community_from;
    community_from.set(faction.c_str());
    CHARACTER_COMMUNITY community_to;
    community_to.set(faction_to.c_str());

    return RELATION_REGISTRY().GetCommunityRelation(community_from.index(), community_to.index());
}

inline std::uint32_t get_npcs_relation(CScriptGameObject* const p_npc1, CScriptGameObject* const p_npc2)
{ 
    if (!p_npc1 || !p_npc2)
    {
        Msg("[Scripts/Globals/GameRelations/get_npcs_relation(p_npc1, p_npc2)] WARNING: some of arguemnts is null Return ...");
        return ALife::ERelationType::eRelationTypeDummy;  
    }


    return p_npc1->GetRelationType(p_npc2); 
}

inline void set_npc_sympathy(CScriptGameObject* npc, float new_sympathy)
{
    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    if (new_sympathy < 0.0f)
        new_sympathy = 0.0f;

    if (new_sympathy > 1.0f)
        new_sympathy = 1.0f;

    npc->SetSympathy(new_sympathy);
}

inline void set_npcs_relation(
    CScriptGameObject* client_from_object1, CScriptGameObject* client_to_object2, xr_string& new_relation_name)
{
    if (!client_from_object1)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    if (!client_to_object2)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    if (!new_relation_name.c_str())
    {
        Msg("[Scripts/Globals/GameRelations/set_npcs_relation(server_object, client_object, new_relation_name)] "
            "WARNING: new_relation_name was an empty string! Set default value => [%s]",
            kRelationsTypeNeutral);
        new_relation_name = kRelationsTypeNeutral;
    }

    int goodwill = 0;

    if (new_relation_name == kRelationsTypeEnemy)
        goodwill = -1000;
    else if (new_relation_name == kRelationsTypeFriends)
        goodwill = 1000;

    RELATION_REGISTRY().ForceSetGoodwill(client_from_object1->ID(), client_to_object2->ID(), goodwill);
}

inline void set_npcs_relation(
    const std::uint16_t& server_from_object_id, const std::uint16_t& server_to_object_id, xr_string& new_relation_name)
{
    if (!new_relation_name.c_str())
    {
        Msg("[Scripts/Globals/GameRelations/set_npcs_relation(server_object, client_object, new_relation_name)] "
            "WARNING: new_relation_name was an empty string! Set default value => [%s]",
            kRelationsTypeNeutral);
        new_relation_name = kRelationsTypeNeutral;
    }

    int goodwill = 0;

    if (new_relation_name == kRelationsTypeEnemy)
        goodwill = -1000;
    else if (new_relation_name == kRelationsTypeFriends)
        goodwill = 1000;

    RELATION_REGISTRY().ForceSetGoodwill(server_from_object_id, server_to_object_id, goodwill);
}

inline void set_npcs_relation(CSE_ALifeMonsterAbstract* server_from_object, CSE_ALifeMonsterAbstract* server_to_object,
    xr_string& new_relation_name)
{
    if (!server_from_object)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    if (!server_to_object)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    if (!new_relation_name.c_str())
    {
        Msg("[Scripts/Globals/GameRelations/set_npcs_relation(server_object, client_object, new_relation_name)] "
            "WARNING: new_relation_name was an empty string! Set default value => [%s]",
            kRelationsTypeNeutral);
        new_relation_name = kRelationsTypeNeutral;
    }

    int goodwill = 0;

    if (new_relation_name == kRelationsTypeEnemy)
        goodwill = -1000;
    else if (new_relation_name == kRelationsTypeFriends)
        goodwill = 1000;

    RELATION_REGISTRY().ForceSetGoodwill(server_from_object->ID, server_to_object->ID, goodwill);
}

inline bool is_factions_enemies(const xr_string& faction, const xr_string& faction_to)
{
    if (!faction.size())
    {
        R_ASSERT2(false, "can't be an empty string!");
        return false;
    }

    if (!faction_to.size())
    {
        R_ASSERT2(false, "can't be an empty string!");
        return false;
    }

    if (faction != "none" && faction_to != "none")
    {
        return get_community_relation(faction, faction_to) <= kRelationKoeffEnemy;
    }

    return false;
}

inline bool is_factions_friends(const xr_string& faction, const xr_string& faction_to)
{
    if (!faction.size())
    {
        R_ASSERT2(false, "can't be an empty string!");
        return false;
    }

    if (!faction_to.size())
    {
        R_ASSERT2(false, "can't be an empty string!");
        return false;
    }

    if (faction != "none" && faction_to != "none")
    {
        return get_community_relation(faction, faction_to) >= kRelationKoeffFriend;
    }

    return false;
}

inline void set_squad_goodwill(const xr_string& squad_id_name, const xr_string& goodwill_name)
{
    Script_SE_SimulationSquad* p_squad = get_story_squad(squad_id_name);
    if (!p_squad)
    {
        std::uint16_t squad_id = static_cast<std::uint16_t>(atoi(squad_id_name.c_str()));
        if (squad_id == 0)
        {
            Msg("[Scripts/Globals/GameRelations/set_squad_goowill(squad_id_name, goodwill_name)] WARNING: There is no "
                "squad! Return ...");
            return;
        }
        else
        {
            p_squad = ai().alife().objects().object(squad_id)->cast_script_se_simulationsquad();
        }
    }

    if (p_squad)
    {
        p_squad->set_squad_relation(goodwill_name);
    }

    Msg("[Scripts/Globals/GameRelations/set_squad_goodwill(squad_id_name, goodwill_name)] WARNING: can't find squad "
        "anyway check your argument -> %s",
        squad_id_name.c_str());
}

inline void set_squad_goodwill_to_npc(
    CScriptGameObject* const p_npc, const xr_string& squad_id_name, const xr_string& goodwill_name)
{
    int goodwill = 0;
    if (goodwill_name == Globals::kRelationsTypeEnemy)
        goodwill = Globals::kRelationKoeffEnemy;
    else if (goodwill_name == Globals::kRelationsTypeFriends)
        goodwill = Globals::kRelationKoeffFriend;

    Script_SE_SimulationSquad* p_squad = Globals::get_story_squad(squad_id_name);
    if (!p_squad)
    {
        std::uint16_t squad_id = static_cast<std::uint16_t>(atoi(squad_id_name.c_str()));
        if (squad_id == 0)
        {
            Msg("[Scripts/Globals/set_squad_goodwill_to_npc(p_npc, squad_id_name, goodwill_name)] WARNING: can't find "
                "any squad by %s Return ...",
                squad_id_name.c_str());
            return;
        }

        p_squad = ai().alife().objects().object(squad_id)->cast_script_se_simulationsquad();
    }

    if (p_squad)
    {
        for (AssociativeVector<std::uint16_t, CSE_ALifeMonsterAbstract*>::const_iterator it =
                 p_squad->squad_members().begin();
             it != p_squad->squad_members().end(); ++it)
        {
            if (p_npc)
            {
                RELATION_REGISTRY().ForceSetGoodwill(it->second->ID, p_npc->ID(), goodwill);
                RELATION_REGISTRY().ForceSetGoodwill(ai().alife().objects().object(p_npc->ID())->ID,
                    ai().alife().objects().object(p_npc->ID())->ID, goodwill);
            }
        }
    }

    Msg("[Scripts/Globals/set_squad_goodwill_to_npc(p_npc, squad_id_name, goodwill_name)] WARNING: can't find any "
        "squad by %s Return ...",
        squad_id_name.c_str());
}

inline void change_factions_community_num(const xr_string& community_name, const std::uint16_t npc_id, const int delta)
{
    if (community_name.empty())
    {
        Msg("[Scripts/Globals/GameRelations/change_factions_community_num(community_name, npc_id, delta)] WARNING: "
            "community_name.empty() == true! Return ...");
        return;
    }

    if (community_name == "none")
    {
        Msg("[Scripts/Globals/GameRelations/change_factions_community_num(community_name, npc_id, delta)] WARNING: "
            "community_name == \"none\" Return ...");
        return;
    }

    if (!npc_id)
    {
        Msg("[Scripts/Globals/GameRelations/change_factions_community_num(community_name, npc_id, delta)] WARNING: "
            "npc_id == 0! Return ...");
        return;
    }

    CHARACTER_COMMUNITY character;
    character.set(community_name.c_str());

    RELATION_REGISTRY().ChangeCommunityGoodwill(character.index(), npc_id, delta);
}

inline std::uint32_t get_relation_id_by_name(const xr_string& relation_name)
{
    std::uint32_t result = 0;
    if (relation_name.empty())
    {
#ifdef DEBUG
        MESSAGEWR("You passed an empty argument can't find anything!");
#endif // DEBUG
        return result;
    }


    if (Script_GlobalHelper::getInstance().getRegisteredRelations().empty())
    {
#ifdef DEBUG
        MESSAGEWR("Early (late) calling!");
#endif // DEBUG
        return result;
    }

    if (Script_GlobalHelper::getInstance().getRegisteredRelations().find(relation_name) == Script_GlobalHelper::getInstance().getRegisteredRelations().end())
    {
#ifdef DEBUG
        MESSAGEWR("can't find relation id by name %s", relation_name.c_str());
#endif // DEBUG

        return result;
    }

    result = Script_GlobalHelper::getInstance().getRegisteredRelations().at(relation_name);

    return result;
}

} // namespace GameRelations

inline bool predicate_const_true(std::uint16_t, bool) { return true; }
inline bool predicate_animpoint_bread(std::uint16_t npc_id, bool)
{
    if (DataBase::Storage::getInstance().getStorage().at(npc_id).getClientObject())
    {
        if (DataBase::Storage::getInstance().getStorage().at(npc_id).getClientObject()->get_visual_name())
        {
            if (Script_GlobalHelper::getInstance().getRegisteredEatableVisuals().at(
                    DataBase::Storage::getInstance().getStorage().at(npc_id).getClientObject()->get_visual_name()) &&
                DataBase::Storage::getInstance().getStorage().at(npc_id).getClientObject()->GetObjectByName("bread"))
                return true;
        }
    }

    return false;
}

inline bool predicate_animpoint_kolbasa(std::uint16_t npc_id, bool)
{
    if (DataBase::Storage::getInstance().getStorage().at(npc_id).getClientObject())
    {
        if (DataBase::Storage::getInstance().getStorage().at(npc_id).getClientObject()->get_visual_name())
        {
            if (Script_GlobalHelper::getInstance().getRegisteredEatableVisuals().at(
                    DataBase::Storage::getInstance().getStorage().at(npc_id).getClientObject()->get_visual_name()) &&
                DataBase::Storage::getInstance().getStorage().at(npc_id).getClientObject()->GetObjectByName("kolbasa"))
                return true;
        }
    }

    return false;
}

inline bool predicate_animpoint_vodka(std::uint16_t npc_id, bool)
{
    if (DataBase::Storage::getInstance().getStorage().at(npc_id).getClientObject())
    {
        if (DataBase::Storage::getInstance().getStorage().at(npc_id).getClientObject()->get_visual_name())
        {
            if (Script_GlobalHelper::getInstance().getRegisteredEatableVisuals().at(
                    DataBase::Storage::getInstance().getStorage().at(npc_id).getClientObject()->get_visual_name()) &&
                DataBase::Storage::getInstance().getStorage().at(npc_id).getClientObject()->GetObjectByName("vodka"))
                return true;
        }
    }

    return false;
}

inline bool predicate_animpoint_energy(std::uint16_t npc_id, bool)
{
    if (DataBase::Storage::getInstance().getStorage().at(npc_id).getClientObject())
    {
        if (DataBase::Storage::getInstance().getStorage().at(npc_id).getClientObject()->get_visual_name())
        {
            if (Script_GlobalHelper::getInstance().getRegisteredEatableVisuals().at(
                    DataBase::Storage::getInstance().getStorage().at(npc_id).getClientObject()->get_visual_name()) &&
                DataBase::Storage::getInstance().getStorage().at(npc_id).getClientObject()->GetObjectByName(
                    "energy_drink"))
                return true;
        }
    }

    return false;
}

inline bool predicate_animpoint_guitar(std::uint16_t npc_id, bool is_in_camp)
{
    if (DataBase::Storage::getInstance().getStorage().at(npc_id).getClientObject() && is_in_camp)
    {
        if (DataBase::Storage::getInstance().getStorage().at(npc_id).getClientObject()->GetObjectByName("guitar_a"))
            return true;
    }

    return false;
}

inline bool predicate_animpoint_harmonica(std::uint16_t npc_id, bool is_in_camp)
{
    if (DataBase::Storage::getInstance().getStorage().at(npc_id).getClientObject() && is_in_camp)
    {
        if (DataBase::Storage::getInstance().getStorage().at(npc_id).getClientObject()->get_visual_name())
        {
            if (Script_GlobalHelper::getInstance().getRegisteredEatableVisuals().at(
                    DataBase::Storage::getInstance().getStorage().at(npc_id).getClientObject()->get_visual_name()) &&
                DataBase::Storage::getInstance().getStorage().at(npc_id).getClientObject()->GetObjectByName(
                    "harmonica_a"))
                return true;
        }
    }

    return false;
}

// Lord: Реализовать когда будет XR_GULAG
inline bool predicate_animpoint_weapon(std::uint16_t npc_id, bool) { return false; }

inline std::uint32_t vertex_in_direction(
    const std::uint32_t& level_vertex_id, Fvector& direction, const float& max_distance)
{
    direction.normalize_safe();
    direction.mul(max_distance);
    Fvector start_position = ai().level_graph().vertex_position(level_vertex_id);
    Fvector finish_position = Fvector(start_position).add(direction);
    u32 result = u32(-1);
    ai().level_graph().farthest_vertex_in_direction(level_vertex_id, start_position, finish_position, result, 0);
    return (ai().level_graph().valid_vertex_id(result) ? result : level_vertex_id);
}

inline float yaw(const Fvector& vector1, const Fvector& vector2)
{
    return (acos((vector1.x * vector2.x) + (vector1.z * vector2.z)) /
        (sqrt((vector1.x * vector1.x) + (vector1.z * vector1.z)) *
            sqrt((vector2.x * vector2.x) + (vector2.z * vector2.z))));
}

inline float yaw_degree(const Fvector& vector1, const Fvector& vector2)
{
    return (acos((vector1.x * vector2.x) + (vector1.z * vector2.z)) /
               (sqrt((vector1.x * vector1.x) + (vector2.z * vector2.z)) *
                   sqrt((vector2.x * vector2.x) + (vector2.z * vector2.z)))) *
        57.2957f;
}

// @ 2D
inline Fvector vector_cross(const Fvector& vector1, const Fvector& vector2)
{
    return Fvector().set(vector1.y * vector2.z - vector1.z * vector2.y, vector1.z * vector2.x - vector1.x * vector2.z,
        vector1.x * vector2.y - vector1.y * vector2.x);
}

inline Fvector vector_rotate_y(Fvector& vector, float& angle)
{
    // Lord: если это константа то определить что это
    angle *= 0.017453292519943295769236907684886f;
    float cos_result = cos(angle);
    float sin_result = sin(angle);

    return Fvector().set(
        vector.x * cos_result - vector.z * sin_result, vector.y, vector.x * sin_result + vector.z * cos_result);
}

inline float distance_between(CScriptGameObject* object1, CScriptGameObject* object2)
{
    if (!object1)
    {
        R_ASSERT2(false, "object is null!");
        return 0.0f;
    }

    if (!object2)
    {
        R_ASSERT2(false, "object is null!");
        return 0.0f;
    }

    return object1->Position().distance_to(object2->Position());
}

inline float distance_between(CScriptGameObject* object1, CSE_ALifeDynamicObject* server_object2)
{
    if (!object1)
    {
        R_ASSERT2(false, "object is null!");
        return 0.0f;
    }

    if (!server_object2)
    {
        R_ASSERT2(false, "object is null!");
        return 0.0f;
    }

    return object1->Position().distance_to(server_object2->Position());
}

inline float distance_between(CSE_ALifeDynamicObject* server_object1, CSE_ALifeDynamicObject* server_object2)
{
    if (!server_object1)
    {
        R_ASSERT2(false, "object is null!");
        return 0.0f;
    }

    if (!server_object2)
    {
        R_ASSERT2(false, "object is null!");
        return 0.0f;
    }

    return server_object1->Position().distance_to(server_object2->Position());
}

inline float distance_between(CSE_ALifeDynamicObject* server_object1, CScriptGameObject* object2)
{
    if (!server_object1)
    {
        R_ASSERT2(false, "object is null!");
        return 0.0f;
    }

    if (!object2)
    {
        R_ASSERT2(false, "object is null!");
        return 0.0f;
    }

    return server_object1->Position().distance_to(object2->Position());
}

inline std::uint16_t get_story_object_id(const xr_string& object_id_name)
{
    if (!object_id_name.size())
    {
        R_ASSERT2(false, "can't be empty!");
        return Globals::kUnsignedInt16Undefined;
    }

    return Script_StoryObject::getInstance().get(object_id_name);
}

inline xr_string get_object_story_id(const std::uint16_t object_id)
{
    return Script_StoryObject::getInstance().get_story_id(object_id);
}

inline Script_SE_SimulationSquad* get_story_squad(const xr_string& object_id_name)
{
    if (!object_id_name.size())
    {
        R_ASSERT2(false, "string can't be empty!");
        return nullptr;
    }

    Script_SE_SimulationSquad* server_object = nullptr;

    const std::uint16_t& squad_id = get_story_object_id(object_id_name);

    server_object = ai().alife().objects().object(squad_id)->cast_script_se_simulationsquad();

    return server_object;
}

inline Script_SE_SimulationSquad* get_object_squad(const std::uint16_t& object_id)
{
    CSE_ALifeDynamicObject* server_object = ai().alife().objects().object(object_id);

    if (server_object)
    {
        if (server_object->cast_monster_abstract()->m_group_id != Globals::kUnsignedInt16Undefined)
            return ai()
                .alife()
                .objects()
                .object(server_object->cast_monster_abstract()->m_group_id)
                ->cast_script_se_simulationsquad();
    }

    return nullptr;
}

inline bool is_npc_in_actor_frustrum(CScriptGameObject* npc)
{
    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!DataBase::Storage::getInstance().getActor())
    {
        Msg("[Scripts/Globals/is_npc_in_actor_frustrum(npc)] DataBase's actor is null! Early calling!!!");
        return false;
    }

    Fvector& actor_direction = Device.vCameraDirection;

    Fvector npc_direction = npc->Position();
    npc_direction.sub(DataBase::Storage::getInstance().getActor()->Position());
    float yaw = yaw_degree(actor_direction, npc_direction);

    return yaw < 35.0f;
}

inline bool is_npc_in_actor_frustrum(CSE_ALifeDynamicObject* server_npc)
{
    if (!server_npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!DataBase::Storage::getInstance().getActor())
    {
        Msg("[Scripts/Globals/is_npc_in_actor_frustrum(server_npc)] DataBase's actor is null! Early calling!!!");
        return false;
    }

    Fvector& actor_direciton = Device.vCameraDirection;

    Fvector npc_direction = server_npc->Position();
    npc_direction.sub(DataBase::Storage::getInstance().getActor()->Position());
    float yaw = yaw_degree(actor_direciton, npc_direction);

    return yaw < 35.0f;
}

inline CScriptGameObject* get_story_object(const xr_string& object_id)
{
    CScriptGameObject* result = nullptr;
    if (object_id.empty())
    {
        R_ASSERT2(false, "can't be empty!");
        return result;
    }

    const std::uint16_t& obj_id = Script_StoryObject::getInstance().get(object_id);

    if (!obj_id)
        return result;

    result = DataBase::Storage::getInstance().getStorage().at(obj_id).getClientObject();

    if (result)
    {
        Msg("[Scripts/Globals/get_story_object(object_id_name)] returned by DataBase!");
        return result;
    }
    else
    {
        result = Game::level::get_object_by_id(obj_id);
        if (result)
        {
            Msg("[Scripts/Globals/get_story_object(object_id_name)] returned by "
                "Game::level::get_object_by_id(obj_id)!");
            return result;
        }
    }

    return nullptr;
}

inline int get_script_clsid(const CLASS_ID& clsid) { return object_factory().script_clsid(clsid); }
inline bool patrol_path_exists(LPCSTR patrol_path) { return (!!ai().patrol_paths().path(patrol_path, true)); }

inline void load_sound(void)
{
    CScriptIniFile sound_ini = CScriptIniFile("misc\\script_sound.ltx");

    if (!sound_ini.section_exist("list"))
    {
        R_ASSERT2(false, "There is no list. Please check your config (misc\\script_sound.ltx)!");
    }

    std::uint32_t count_line = sound_ini.line_count("list");
    xr_string id, value, category;

    for (int i = 0; i < count_line; ++i)
    {
        xr_string section;
        xr_string value;
        pcstr s;
        pcstr v;
        bool result = !!sound_ini.r_line("list", i, &s, &v);
        if (result)
        {
            section = s;
            value = v;

            xr_string sound_type_name = Globals::Utils::cfg_get_string(&sound_ini, section, "type");

            if (sound_type_name == "npc")
                Script_SoundThemeDataBase::getInstance().getTheme()[section] = new Script_SoundNPC(sound_ini, section);
            else if (sound_type_name == "actor")
                Script_SoundThemeDataBase::getInstance().getTheme()[section] =
                    new Script_SoundActor(sound_ini, section);
            else if (sound_type_name == "3d")
                Script_SoundThemeDataBase::getInstance().getTheme()[section] =
                    new Script_SoundObject(sound_ini, section);
            else if (sound_type_name == "looped")
                Script_SoundThemeDataBase::getInstance().getTheme()[section] =
                    new Script_SoundLooped(sound_ini, section);
        }
    }
}

inline void init_npc_sound(CScriptGameObject* npc)
{
    for (std::pair<xr_string, Script_ISoundEntity*> it : Script_SoundThemeDataBase::getInstance().getTheme())
    {
        if (it.second)
        {
            if (it.second->getSoundType() == SCRIPTSOUNDTYPE_NPC)
            {
                if (it.second->getAvailCommunities()[character_community(npc)])
                {
                    it.second->initialize_npc(npc);
                }
            }
        }
    }
}

inline bool has_alife_info(LPCSTR info_id)
{
    const KNOWN_INFO_VECTOR* known_info =
        ai().alife().registry<CALifeAbstractRegistry<u16, KNOWN_INFO_VECTOR>>(NULL).object(0, true);

    if (!known_info)
    {
        //   R_ASSERT2(false, "object in null!");
        Msg("[Scripts/Globals/has_alife_info(info_id_name)] known_info = nullptr! Return false!");
        return false;
    }

    if (!known_info->size())
        return (false);

    if (std::find_if(known_info->begin(), known_info->end(), CFindByIDPred(info_id)) == known_info->end())
        return (false);

    return (true);
}

inline bool has_alife_info(const xr_string& info_id_name)
{
    return has_alife_info(info_id_name.c_str());
}

inline int get_general_goodwill_between(const std::uint16_t& from, const std::uint16_t& to)
{
    CHARACTER_GOODWILL presonal_goodwill = RELATION_REGISTRY().GetGoodwill(from, to);
    VERIFY(presonal_goodwill != NO_GOODWILL);

    CSE_ALifeTraderAbstract* from_obj = smart_cast<CSE_ALifeTraderAbstract*>(ai().alife().objects().object(from));
    CSE_ALifeTraderAbstract* to_obj = smart_cast<CSE_ALifeTraderAbstract*>(ai().alife().objects().object(to));

    if (!from_obj || !to_obj)
    {
        //         GEnv.ScriptEngine->script_log(LuaMessageType::Error,
        //             "RELATION_REGISTRY::get_general_goodwill_between  : cannot convert obj to
        //             CSE_ALifeTraderAbstract!");
        R_ASSERT2(false, "object is null!");
        return (0);
    }
    CHARACTER_GOODWILL community_to_obj_goodwill = RELATION_REGISTRY().GetCommunityGoodwill(from_obj->Community(), to);
    CHARACTER_GOODWILL community_to_community_goodwill =
        RELATION_REGISTRY().GetCommunityRelation(from_obj->Community(), to_obj->Community());

    return presonal_goodwill + community_to_obj_goodwill + community_to_community_goodwill;
}

inline bool check_squad_for_enemies(CSE_ALifeOnlineOfflineGroup* squad)
{
    if (!squad)
    {
        R_ASSERT2(false, "object is null!");
        return false;
    }

    for (std::pair<std::uint16_t, CSE_ALifeMonsterAbstract*> it : squad->squad_members())
    {
        if (it.second)
        {
            if (get_general_goodwill_between(it.first, ai().alife().graph().actor()->ID))
                return true;
        }
    }

    return false;
}

inline bool IsMonster(CScriptGameObject* object, int class_id = 0)
{
    if (!object)
    {
        Msg("[Scripts/Globals/IsMonster(object, class_id)] WARNING: trying to use class_id = %d", class_id);
    }

    int result = class_id ? class_id : object->clsid();

    return (Script_GlobalHelper::getInstance().getMonsterClasses().at(result) == true);
}

inline bool IsMonster(CSE_ALifeDynamicObject* server_object, int class_id = 0)
{
    if (!server_object)
    {
        Msg("[Scripts/Globals/IsMonster(server_object, class_id)] WARNING: trying to use class_id = %d", class_id);
    }

    int result = class_id ? class_id : server_object->m_script_clsid;

    return (Script_GlobalHelper::getInstance().getMonsterClasses().at(result) == true);
}

inline bool IsStalker(CScriptGameObject* object, int class_id = 0)
{
    if (!object)
    {
        Msg("[Scripts/Globals/IsStalker(object, class_id)] WARNING: trying to use class_id = %d", class_id);
    }

    int result = class_id ? class_id : object->clsid();

    return ((Script_GlobalHelper::getInstance().getStalkerClasses().find(result) ==
                Script_GlobalHelper::getInstance().getStalkerClasses().end()) ?
            false :
            true);
}

inline bool IsStalker(CSE_ALifeDynamicObject* server_object, int class_id = 0)
{
    if (!server_object)
    {
        Msg("[Scripts/Globals/IsStalker(server_object, class_id)] WARNING: trying to use class_id = %d", class_id);
    }

    int result = class_id ? class_id : server_object->m_script_clsid;

    return ((Script_GlobalHelper::getInstance().getStalkerClasses().find(result) ==
                Script_GlobalHelper::getInstance().getStalkerClasses().end()) ?
            false :
            true);
}

inline bool IsArtefact(CScriptGameObject* object, int class_id = 0)
{
    if (!object)
    {
        Msg("[Scripts/Globals/IsArtefact(object, class_id)] WARNING: trying to use class_id = %d", class_id);
    }

    int result = class_id ? class_id : object->clsid();

    return ((Script_GlobalHelper::getInstance().getArtefactClasses().find(result) ==
                Script_GlobalHelper::getInstance().getArtefactClasses().end()) ?
            false :
            true);
}

inline bool IsWeapon(CScriptGameObject* object, int class_id)
{
    if (!object)
    {
        Msg("[Scripts/Globals/IsWeapon(object, class_id)] WARNING: trying to use class_id = %d", class_id);
    }

    int result = class_id ? class_id : object->clsid();

    return ((Script_GlobalHelper::getInstance().getWeaponClasses().find(result) ==
                Script_GlobalHelper::getInstance().getWeaponClasses().end()) ?
            false :
            true);
}

inline xr_string character_community(CScriptGameObject* object)
{
    if (!object)
    {
        R_ASSERT2(false, "object in null!");
        return "monster";
    }

    if (IsStalker(object))
    {
        return object->CharacterCommunity();
    }

    return "monster";
}

inline void change_team_squad_group(CSE_ALifeDynamicObject* server_object, const std::uint8_t& team,
    const std::uint8_t& group, const std::uint8_t& squad)
{
    if (!server_object)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    CSE_ALifeCreatureAbstract* server_creature = server_object->cast_creature_abstract();

    if (!server_creature)
    {
        R_ASSERT2(false, "bad casting check your server_object!!!");
        return;
    }
    CScriptGameObject* client_object = nullptr;
    if (DataBase::Storage::getInstance().getStorage().size())
        client_object = DataBase::Storage::getInstance().getStorage().at(server_object->ID).getClientObject();

    if (!client_object)
    {
        server_creature->s_team = team;
        server_creature->s_squad = squad;
        server_creature->s_group = group;
    }
    else
    {
        client_object->ChangeTeam(team, squad, group);
    }
}

inline bool is_level_present(void) { return (!!g_pGameLevel); }

inline bool is_device_paused(void) { return !!Device.Paused(); }

inline void set_device_paused(bool b) { Device.Pause(b, TRUE, FALSE, "set_device_paused_script"); }

inline const CInifile* get_system_ini(void) { return (pSettings); }

inline static xr_map<xr_string, unsigned int>& SaveMarkers(void) noexcept
{
    static xr_map<xr_string, unsigned int> save_markers;
    return save_markers;
}

inline static xr_map<xr_string, xr_string>& Schemes(void) noexcept
{
    static xr_map<xr_string, xr_string> schemes;
    return schemes;
}

inline static xr_map<xr_string, unsigned int>& STypes(void) noexcept
{
    static xr_map<xr_string, unsigned int> stypes;
    return stypes;
}

inline static xr_map<xr_string, Cordis::AnyCallable<void>>& getXREffects_Functions(void) noexcept
{
    static xr_map<xr_string, Cordis::AnyCallable<void>> functions;
    return functions;
}

inline void load_scheme(const xr_string& filename, const xr_string& scheme, unsigned int stype)
{
    Schemes()[scheme] = filename;
    STypes()[scheme] = stype;
}

inline void start_game_callback(void)
{
    // Loads schemes Lord: переименовать соответвующие наименования скриптов, но если _G[schemes[scheme]] -> нужно
    // расписывать через case, потому что строка будет указывать на namespace или getInstance() самого класса То есть
    // switch (scheme_name) { case "Script_StoryObject": Script_StoryObject::getInstance().DoSomething(); etc... };
    load_scheme("xr_danger", "danger", kSTypeStalker);
    load_scheme("xr_gather_items", "gather_items", kSTypeStalker);
    load_scheme("xr_abuse", "abuse", kSTypeStalker);
    load_scheme("xr_walker", "walker", kSTypeStalker);
    load_scheme("xr_remark", "remark", kSTypeStalker);
    load_scheme("xr_cover", "cover", kSTypeStalker);
    load_scheme("xr_sleeper", "sleeper", kSTypeStalker);
    load_scheme("xr_kamp", "kamp", kSTypeStalker);
    load_scheme("xr_camper", "camper", kSTypeStalker);
    load_scheme("xr_meet", "meet", kSTypeStalker);
    load_scheme("xr_help_wounded", "help_wounded", kSTypeStalker);
    load_scheme("xr_corpse_detection", "corpse_detection", kSTypeStalker);
    load_scheme("xr_combat", "combat", kSTypeStalker);
    load_scheme("xr_death", "death", kSTypeStalker);
    load_scheme("xr_hit", "hit", kSTypeStalker);
    load_scheme("xr_wounded", "wounded", kSTypeStalker);
    load_scheme("xr_meet", "actor_dialogs", kSTypeStalker);
    load_scheme("xr_combat_ignore", "combat_ignore", kSTypeStalker);
    load_scheme("xr_combat_zombied", "combat_zombied", kSTypeStalker);
    load_scheme("xr_patrol", "patrol", kSTypeStalker);
    load_scheme("xr_smartcover", "smartcover", kSTypeStalker);
    load_scheme("xr_companion", "companion", kSTypeStalker);
    load_scheme("xr_animpoint", "animpoint", kSTypeStalker);
    load_scheme("xr_reach_task", "reach_task", kSTypeStalker);

    load_scheme("mob_remark", "mob_remark", kSTypeMobile);
    load_scheme("mob_walker", "mob_walker", kSTypeMobile);
    load_scheme("mob_combat", "mob_combat", kSTypeMobile);
    load_scheme("mob_death", "mob_death", kSTypeMobile);
    load_scheme("mob_jump", "mob_jump", kSTypeMobile);
    load_scheme("mob_home", "mob_home", kSTypeMobile);

    load_scheme("ph_door", "ph_door", kSTypeItem);
    load_scheme("ph_idle", "ph_idle", kSTypeItem);
    load_scheme("ph_hit", "ph_hit", kSTypeItem);
    load_scheme("ph_on_hit", "ph_on_hit", kSTypeItem);
    load_scheme("ph_button", "ph_button", kSTypeItem);
    load_scheme("ph_code", "ph_code", kSTypeItem);
    load_scheme("ph_death", "ph_on_death", kSTypeItem);
    load_scheme("ph_minigun", "ph_minigun", kSTypeItem);
    // load_scheme("ph_target","ph_target",kSTypeItem);
    load_scheme("ph_oscillate", "ph_oscillate", kSTypeItem);

    load_scheme("heli_move", "heli_move", kSTypeHelicopter);

    load_scheme("sr_no_weapon", "sr_no_weapon", kSTypeRestrictor);
    load_scheme("sr_teleport", "sr_teleport", kSTypeRestrictor);
    load_scheme("sr_idle", "sr_idle", kSTypeRestrictor);
    load_scheme("sr_light", "sr_light", kSTypeRestrictor);
    load_scheme("sr_timer", "sr_timer", kSTypeRestrictor);
    load_scheme("sr_psy_antenna", "sr_psy_antenna", kSTypeRestrictor);
    load_scheme("sr_postprocess", "sr_postprocess", kSTypeRestrictor);
    load_scheme("sr_particle", "sr_particle", kSTypeRestrictor);
    load_scheme("sr_cutscene", "sr_cutscene", kSTypeRestrictor);
    // load_scheme("sr_bloodsucker","sr_bloodsucker",kSTypeRestrictor);
    load_scheme("sr_monster", "sr_monster", kSTypeRestrictor);
    // load_scheme("sr_robbery","sr_robbery",kSTypeRestrictor);
    // load_scheme("sr_survival","sr_survival",kSTypeRestrictor);
    load_scheme("sr_crow_spawner", "sr_crow_spawner", kSTypeRestrictor);
    // load_scheme("sr_shooting","sr_shooting",kSTypeRestrictor);
    // load_scheme("sr_recoveritem","sr_recoveritem",kSTypeRestrictor);
    load_scheme("sr_silence", "sr_silence", kSTypeRestrictor);
    load_scheme("sr_deimos", "sr_deimos", kSTypeRestrictor);

    Script_GlobalHelper::getInstance();

    Msg("[Scripts/Globals/start_game_callback()] was called!");
}

void system_deallocation(void)
{
    Msg("[Scripts/Globals/system_deallocation()] disconnect from server!");
    Script_GlobalHelper::getInstance().DeallocateDynamicLtx();
    Script_SimulationBoard::getInstance().Deallocate();
    DataBase::Storage::getInstance().Deallocate();
    Script_StoryObject::getInstance().Deallocate();
    Script_SimulationObjects::getInstance().Deallocate();
    Script_SRLightManager::getInstance().Deallocate();
}

inline void set_save_marker(NET_Packet& packet, const xr_string& mode, bool check, const xr_string& prefix)
{
    xr_string result = "_";
    result.append(prefix);

    if (check)
    {
        if (SaveMarkers()[result] == 0)
        {
            R_ASSERT3(false, "Trying to check without marker ", result.c_str());
        }

        if (mode == kSaveMarkerMode_Save)
        {
            unsigned int delta = packet.w_tell() - SaveMarkers()[result];
            // Lord: переделать в In-Game логгирование
            xr_string info = "";
            info.append(result);
            info.append(":SAVE DELTA: ");
            Log(info.c_str(), delta);

            if (delta >= 8000)
            {
                // Lord: переделать в In-Game логгирование
                Log("WARNING! Maybe this is problem save point");
            }

            if (delta >= 10240)
            {
                // empty code here
                // R_ASSERT2(false, "You are saving too much");
            }
            packet.w_u16(delta);
        }
        else
        {
            const unsigned int c_delta = packet.r_tell() - SaveMarkers()[result];
            unsigned int delta = packet.r_u16();

            if (delta != c_delta)
            {
                Msg("INCORRECT LOAD [%s].[%d][%d]", result.c_str(), delta, c_delta);
                R_ASSERT(false);
            }
            else
            {
                xr_string info = "";
                info.append(result);
                info.append(": LOAD DELTA: %d");
                Msg(info.c_str(), delta);
            }
        }
        return;
    }

    if (mode == kSaveMarkerMode_Save)
    {
        xr_string info = "";
        info.append(result);
        info.append(": set save marker: %d");
        Msg(info.c_str(), packet.w_tell());
        SaveMarkers()[result] = packet.w_tell();

        if (packet.w_tell() > 16000)
        {
            R_ASSERT2(false, "You are trying save too much");
        }
    }
    else
    {
        xr_string info = "";
        info.append(result);
        info.append(": set load marker: %d");
        Msg(info.c_str(), packet.r_tell());
        SaveMarkers()[result] = packet.r_tell();
    }
}

inline void set_save_marker(IReader& packet, const xr_string& mode, bool check, const xr_string& prefix)
{
    xr_string result = "_";
    result.append(prefix);

    if (check)
    {
        if (SaveMarkers()[result] == 0)
        {
            R_ASSERT3(false, "Trying to check without marker ", result.c_str());
        }

        if (mode == kSaveMarkerMode_Save)
        {
            R_ASSERT2(false, "you can't use in that fuction save section!!!!");
        }
        else
        {
            const unsigned int c_delta = packet.tell() - SaveMarkers()[result];
            unsigned int delta = packet.r_u16();

            if (delta != c_delta)
            {
                Msg("INCORRECT LOAD [%s].[%d][%d]", result.c_str(), delta, c_delta);
                R_ASSERT(false);
            }
            else
            {
                xr_string info = "";
                info.append(result);
                info.append(": LOAD DELTA: %d");
                Msg(info.c_str(), delta);
            }
        }
        return;
    }

    if (mode == kSaveMarkerMode_Save)
    {
        R_ASSERT2(false, "you can't use save section in this function!!!!!");
    }
    else
    {
        xr_string info = "";
        info.append(result);
        info.append(": set load marker: %d");
        Msg(info.c_str(), packet.tell());
        SaveMarkers()[result] = packet.tell();
    }
}

inline bool is_on_the_same_level(CSE_ALifeObject* object1, CSE_ALifeObject* object2)
{
    if (!object1 || !object2)
    {
        R_ASSERT2(false, "Object was null!");
        return false;
    }

    return (ai().game_graph().vertex(object1->m_tGraphID)->level_id() ==
        ai().game_graph().vertex(object2->m_tGraphID)->level_id());
}

inline float sim_dist_to(CSE_ALifeObject* object1, CSE_ALifeObject* object2)
{
    if (!object1 || !object2)
    {
        R_ASSERT2(false, "Object was null!");
        return 0.0f;
    }

    std::uint16_t gamevertexid_1 = object1->m_tGraphID;
    std::uint16_t gamevertexid_2 = object2->m_tGraphID;

    return Utils::graph_distance(gamevertexid_1, gamevertexid_2);
}

inline ESingleGameDifficulty get_game_difficulty() { return g_SingleGameDifficulty; }

inline std::uint32_t get_time_days()
{
    std::uint32_t year = 0, month = 0, day = 0, hours = 0, mins = 0, secs = 0, milisecs = 0;
    split_time((g_pGameLevel && Level().game) ? Level().GetGameTime() : ai().alife().time_manager().game_time(), year,
        month, day, hours, mins, secs, milisecs);
    return day;
}

inline std::uint32_t get_time_hours()
{
    std::uint32_t year = 0, month = 0, day = 0, hours = 0, mins = 0, secs = 0, milisecs = 0;
    split_time((g_pGameLevel && Level().game) ? Level().GetGameTime() : ai().alife().time_manager().game_time(), year,
        month, day, hours, mins, secs, milisecs);
    return hours;
}

inline std::uint32_t get_time_minutes()
{
    std::uint32_t year = 0, month = 0, day = 0, hours = 0, mins = 0, secs = 0, milisecs = 0;
    split_time((g_pGameLevel && Level().game) ? Level().GetGameTime() : ai().alife().time_manager().game_time(), year,
        month, day, hours, mins, secs, milisecs);
    return mins;
}

inline bool in_time_interval(const std::uint32_t& value1, const std::uint32_t& value2)
{
    std::uint32_t game_hours = get_time_hours();

    if (value1 >= value2)
    {
        return ((game_hours < value2) || (game_hours >= value1));
    }
    else
    {
        return ((game_hours < value2) && (game_hours >= value1));
    }

    return false;
}

inline xr_string get_job_restrictor(const xr_string& waypoint_name)
{
    if (!waypoint_name.size())
    {
        R_ASSERT2(false, "string can't be empty!");
        return false;
    }

    get_job_restrictor(waypoint_name.c_str());
}

inline xr_string get_job_restrictor(const char* waypoint_name)
{
    if (!waypoint_name)
    {
        R_ASSERT2(false, "string can't be empty!");
        return xr_string("");
    }

    const Fvector& position = CPatrolPathParams(waypoint_name).point(std::uint32_t(0));

    for (const std::pair<xr_string, CScriptGameObject*>& it :
        DataBase::Storage::getInstance().getGameRegisteredCombatSpaceRestrictors())
    {
        if (it.second)
        {
            if (it.second->inside(position))
            {
                return it.first;
            }
        }
    }

    return xr_string("");
}

inline bool is_accessible_job(CSE_ALifeDynamicObject* server_object, const xr_string& waypoint_name)
{
    if (!waypoint_name.size())
    {
        R_ASSERT2(false, "the string can't be empty!");
        return false;
    }

    return is_accessible_job(server_object, waypoint_name.c_str());
}

inline bool is_accessible_job(CSE_ALifeDynamicObject* server_object, const char* waypoint_name)
{
    if (!waypoint_name)
    {
        R_ASSERT2(false, "string can't be empty!");
        return false;
    }

    CScriptGameObject* object = DataBase::Storage::getInstance().getStorage().at(server_object->ID).getClientObject();

    if (!object)
    {
        R_ASSERT2(false, "object is null!");
        return false;
    }

    const Fvector& npc_position = object->Position();
    const Fvector& job_position = CPatrolPathParams(waypoint_name).point(std::uint32_t(0));
    bool is_npc_inside = false;

    for (const std::pair<xr_string, CScriptGameObject*>& it :
        DataBase::Storage::getInstance().getGameRegisteredCombatSpaceRestrictors())
    {
        if (it.second)
        {
            if (it.second->inside(npc_position))
            {
                is_npc_inside = true;
                if (it.second->inside(job_position))
                    return true;
            }
        }
    }

    return (!is_npc_inside);
}

inline CScriptIniFile* create_ini_file(LPCSTR ini_string)
{
    IReader reader((void*)ini_string, xr_strlen(ini_string));
    //    return ((CScriptIniFile*)new CInifile(&reader, FS.get_path("$game_config$")->m_Path));
    return new CScriptIniFile(&reader, FS.get_path("$game_config$")->m_Path);
}

inline std::uint32_t get_time_global(void) { return Device.dwTimeGlobal; }

inline void reset_action(CScriptGameObject* client_object, const xr_string& scheme_name)
{
    if (!client_object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    if (client_object->GetScriptControl())
        client_object->SetScriptControl(false, scheme_name.c_str());

    client_object->SetScriptControl(true, scheme_name.c_str());
}

inline void action(
    CScriptGameObject* client_object, CScriptMovementAction& move_action, CScriptActionCondition& condition_action)
{
    if (!client_object)
        return;

    CScriptEntityAction action;
    action.SetAction(move_action);
    action.SetAction(condition_action);

    client_object->AddAction(&action);
}

inline void action(CScriptGameObject* client_object, CScriptAnimationAction& animation_actoion,
    CScriptSoundAction& sound_action, CScriptActionCondition& condition_action)
{
    if (!client_object)
        return;

    CScriptEntityAction action;
    action.SetAction(animation_actoion);
    action.SetAction(sound_action);
    action.SetAction(condition_action);

    client_object->AddAction(&action);
}

inline void action(CScriptGameObject* client_object, CScriptMovementAction& move_action,
    CScriptAnimationAction& animation_action, CScriptSoundAction& sound_action,
    CScriptActionCondition& condition_action)
{
    if (!client_object)
        return;

    CScriptEntityAction action;
    action.SetAction(move_action);
    action.SetAction(animation_action);
    action.SetAction(sound_action);
    action.SetAction(condition_action);

    client_object->AddAction(&action);
}

inline void action(CScriptGameObject* client_object, CScriptAnimationAction& animation_action,
    CScriptActionCondition& condition_action)
{
    if (!client_object)
        return;

    CScriptEntityAction action;
    action.SetAction(animation_action);
    action.SetAction(condition_action);

    client_object->AddAction(&action);
}

inline void action(CScriptGameObject* p_client_object, CScriptAnimationAction& animation_action,
    CScriptWatchAction& look_action, CScriptActionCondition& condition_action)
{
    if (!p_client_object)
        return;

    CScriptEntityAction action;
    action.SetAction(animation_action);
    action.SetAction(look_action);
    action.SetAction(condition_action);

    p_client_object->AddAction(&action);
}

inline void action(
    CScriptGameObject* client_object, CScriptMovementAction& move_action, CScriptSoundAction& sound_action)
{
    if (!client_object)
        return;

    CScriptEntityAction action;
    action.SetAction(move_action);
    action.SetAction(sound_action);

    client_object->AddAction(&action);
}

inline void action(
    CScriptGameObject* p_client_object, CScriptWatchAction& look_action, CScriptActionCondition& condition_action)
{
    if (!p_client_object)
        return;

    CScriptEntityAction action;
    action.SetAction(look_action);
    action.SetAction(condition_action);

    p_client_object->AddAction(&action);
}

inline std::uint32_t choose_look_point(
    CPatrolPathParams* patrol_look, const CondlistWaypoints& path_look_info, const Flags32& search_for)
{
    if (!patrol_look)
    {
        R_ASSERT2(false, "object is null!");
        return std::uint32_t();
    }

    std::uint32_t points_found_total_weight = 0;
    std::uint32_t founded_point_index = Globals::kUnsignedInt32Undefined;
    std::uint32_t r = 0; // @ ??? how to name this variable, idk but GSC code
    std::uint32_t number_equal_points = 0;
    std::uint32_t point_look_weight = 0;
    for (std::uint32_t look_index = 0; look_index < patrol_look->count(); ++look_index)
    {
        Flags32 this_val = path_look_info.getData().at(look_index).getFlags();
        if (this_val.equal(search_for))
        {
            ++number_equal_points;

            xr_string point_look_weight_name = path_look_info.getData().at(look_index).getValue("p");
            if (!point_look_weight_name.empty())
                point_look_weight = atoi(point_look_weight_name.c_str());
            else
                point_look_weight = 100;

            points_found_total_weight += point_look_weight;
            r = Globals::Script_RandomInt::getInstance().Generate(std::uint32_t(1), points_found_total_weight);

            if (r <= point_look_weight)
                founded_point_index = look_index;
        }
    }

    return founded_point_index;
}

inline bool is_vector_nil(const Fvector& data) { return (fis_zero(data.x) && fis_zero(data.y) && fis_zero(data.z)); }

inline CScriptActionPlanner* get_script_action_planner(CScriptGameObject* obj)
{
    if (!obj)
    {
        R_ASSERT2(false, "object is null!");
        return nullptr;
    }

    return (obj->action_planner<CScriptActionPlanner>());
}

inline CScriptActionPlanner* cast_planner(CScriptActionBase* action)
{
    return (smart_cast<CScriptActionPlanner*>(action));
}

inline bool is_strappable_weapon(CScriptGameObject* const p_client_object)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return false;
    }

    xr_string section_name = p_client_object->Section();
    return get_system_ini()->line_exist(section_name.c_str(), "strap_bone0");
}

inline bool is_look_object_type(CScriptGameObject* const p_client_object, Script_StateManager* const p_state_manager)
{
    if (!p_state_manager)
    {
        R_ASSERT2(false, "it can't be!");
        return false;
    }

    if (!p_client_object)
    {
        R_ASSERT2(false, "it can't be");
        return false;
    }

    if (Script_GlobalHelper::getInstance().getLookDirectionStates().find(p_state_manager->getTargetStateName()) !=
        Script_GlobalHelper::getInstance().getLookDirectionStates().end())
        if (Script_GlobalHelper::getInstance().getLookDirectionStates().at(p_state_manager->getTargetStateName()))
            return true;

    return !(Script_GlobalHelper::getInstance()
                 .getStateLibrary()
                 .at(p_state_manager->getTargetStateName())
                 .getAnimationName()
                 .empty());
}

inline void look_at_object(CScriptGameObject* const p_client_object, Script_StateManager* const p_state_manager)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "it can'be!");
        return;
    }

    if (!p_state_manager)
    {
        R_ASSERT2(false, "it can't be!");
        return;
    }

    p_state_manager->setPointObjectDirection(is_look_object_type(p_client_object, p_state_manager));

    if (p_state_manager->isPointObjectDirection())
    {
        p_client_object->set_sight(
            Game::level::get_object_by_id(p_state_manager->getLookObject()->ID()), true, false, false);
    }
    else
    {
        p_client_object->set_sight(Game::level::get_object_by_id(p_state_manager->getLookObject()->ID()), true, true);
    }
}

inline std::uint32_t look_position_type(
    CScriptGameObject* const p_client_object, Script_StateManager* const p_state_manager)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "it can't be!");
        return 0;
    }

    if (!p_state_manager)
    {
        Msg("[Scripts/Globals/look_position_type(p_client_object, p_state_manager)] wARNING: p_state_manager == "
            "nullptr! Return ...");
        return SightManager::eSightTypePathDirection;
    }

    if (Script_GlobalHelper::getInstance()
            .getStateLibrary()
            .at(p_state_manager->getTargetStateName())
            .getDirectionType())
    {
        return Script_GlobalHelper::getInstance()
            .getStateLibrary()
            .at(p_state_manager->getTargetStateName())
            .getDirectionType();
    }

    if (!p_state_manager->getActionPlanner()
             ->evaluator(p_state_manager->getProperties().at("movement_stand"))
             .evaluate())
    {
        if (is_vector_nil(p_state_manager->getLookPosition()))
            return SightManager::eSightTypeDirection;

        return SightManager::eSightTypePathDirection;
    }

    if (is_vector_nil(p_state_manager->getLookPosition()))
        return SightManager::eSightTypeDirection;

    return SightManager::eSightTypeCover;
}

inline void turn(CScriptGameObject* const p_client_object, Script_StateManager* const p_state_manager)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "it can't be!");
        return;
    }

    if (!p_state_manager)
    {
        R_ASSERT2(false, "it can't be!");
        return;
    }

    p_state_manager->setPointObjectDirection(is_look_object_type(p_client_object, p_state_manager));

    if (is_vector_nil(p_state_manager->getLookPosition()) &&
        Game::level::get_object_by_id(p_state_manager->getLookObject()->ID()))
    {
        look_at_object(p_client_object, p_state_manager);
    }
    else if (is_vector_nil(p_state_manager->getLookPosition()))
    {
        Fvector direction = Fvector().sub(p_state_manager->getLookPosition(), p_client_object->Position());

        if (p_state_manager->isPointObjectDirection())
            direction.y = 0.0f;

        direction.normalize();

        if (is_vector_nil(direction))
        {
            Msg("[Scripts/Globals/turn(p_client_object, p_state_manager)] WARNING: You are trying to set a wrong "
                "direction!");
            p_state_manager->setLookPosition(
                Fvector().set((p_client_object->Position().x + p_client_object->Direction().x),
                    (p_client_object->Position().y + p_client_object->Direction().y),
                    (p_client_object->Position().z + p_client_object->Direction().z)));
            direction = p_client_object->Direction();
        }

        p_client_object->set_sight(SightManager::eSightTypeDirection, direction, true);
    }
}

inline std::uint32_t get_idle_state(const xr_string& target_state_name)
{
    if (target_state_name.empty())
        return MonsterSpace::eObjectActionIdle;

    const StateLibData& data = Script_GlobalHelper::getInstance().getStateLibrary().at(target_state_name);

    if (data.getMentalType() == eMentalStateDanger && data.getMovementType() == eMovementTypeStand &&
        data.getAnimationName().empty())
    {
        return MonsterSpace::eObjectActionAim1;
    }

    return MonsterSpace::eObjectActionIdle;
}

inline CScriptGameObject* const get_weapon(CScriptGameObject* const p_client_object, const xr_string& target_state_name)
{
    if (target_state_name.empty())
    {
        R_ASSERT2(false, "string is empty not right!");
        return nullptr;
    }

    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return nullptr;
    }

    const std::uint16_t item_slot_id =
        Script_GlobalHelper::getInstance().getStateLibrary().at(target_state_name).getWeaponSlot();

    if (!item_slot_id)
        return p_client_object->best_weapon();
    else
        return p_client_object->item_in_slot(item_slot_id);
}

inline void set_state(CScriptGameObject* const p_client_object, const xr_string& state_name,
    StateManagerCallbackData& callback, const std::uint32_t timeout,
    std::pair<Fvector, CScriptGameObject* const> target, const StateManagerExtraData& extra)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "can't assign state manager!");
        return;
    }

    DataBase::Storage::getInstance().setStorageStateManagerSetState(
        p_client_object->ID(), state_name, callback, timeout, target, extra);
}

// inline std::uint32_t get_level_id(void) { return (ai().game_graph().level().level_id()); }

inline LPCSTR get_level_name(int level_id)
{
    LPCSTR result = *ai().game_graph().header().level((GameGraph::_LEVEL_ID)level_id).name();
    return (result);
}

inline void update_logic(CScriptGameObject* const p_object)
{
    if (!p_object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    bool is_object_alive = p_object->Alive();
    const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(p_object->ID());
    CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();
    DataBase::Storage_Scheme* p_storage_combat = storage.getSchemes().at("combat");

    if (is_object_alive && !storage.getActiveSchemeName().empty())
    {
        bool is_switched = false;
        CScriptActionPlanner* p_planner = Globals::get_script_action_planner(p_object);

        if (p_planner->initialized() &&
            p_planner->current_action_id() == StalkerDecisionSpace::eWorldOperatorCombatPlanner)
        {
            const DataBase::Data_Overrides& overrides = storage.getOverrides();

            if (!overrides.isEmpty())
            {
                if (!overrides.getOnCombat().IsEmpty())
                {
                    XR_LOGIC::pick_section_from_condlist(p_actor, p_object, overrides.getOnCombat().getCondlist());
                }

                if (p_storage_combat && !p_storage_combat->getLogic().empty())
                {
                    if (!XR_LOGIC::try_switch_to_another_section(p_object, *p_storage_combat, p_actor))
                    {
                        if (!overrides.getCombatType().IsEmpty())
                        {
                            XR_COMBAT::set_combat_type(p_object, p_actor, overrides.getCombatType().getCondlist());
                        }
                    }
                    else
                    {
                        is_switched = true;
                    }
                }
            }
            else
            {
                XR_COMBAT::set_combat_type(p_object, p_actor, p_storage_combat->getXRCombatCombatTypeCondlist());
            }

            if (!is_switched)
            {
                XR_LOGIC::try_switch_to_another_section(
                    p_object, *storage.getSchemes().at(storage.getActiveSchemeName()), p_actor);
            }
        }
    }
    else
    {
        XR_COMBAT::set_combat_type(p_object, p_actor, p_storage_combat->getXRCombatCombatTypeCondlist());
    }
}

inline void change_anomalies_names(void)
{
    if (has_alife_info("jup_b32_scanner_reward"))
    {
        for (const PDA_ChangeObjectData& it : Script_GlobalHelper::getInstance().getPDAChangeObjects())
        {
            if (has_alife_info(it.getGroupName().c_str()) && !it.isEnabled())
            {
                PDA_ChangeObjectData& _it = const_cast<PDA_ChangeObjectData&>(it);
                _it.setEnabled(true);
            }
        }
    }

    if (Globals::Game::level::get_name() != "jupiter")
        return;

    for (const PDA_ChangeObjectData& it : Script_GlobalHelper::getInstance().getPDAChangeObjects())
    {
        if (it.isEnabled())
        {
            std::uint16_t object_id = Globals::get_story_object_id(it.getTargetName());

            if (object_id && Globals::Game::level::map_has_object_spot(object_id, "primary_object"))
            {
                Globals::Game::level::map_remove_object_spot(object_id, "primary_object");
            }

            xr_string hint_name = Globals::Game::translate_string(it.getHintName().c_str());
            hint_name += "\\n";
            hint_name += "\\n"; // Lord: должно ли быть так а не через один \ ????

            bool is_has_af = false;
            xr_vector<xr_string> artefacts;
            is_has_af = XR_CONDITION::is_anomaly_has_artefact(
                DataBase::Storage::getInstance().getActor(), nullptr, {it.getZoneName()}, artefacts);

            if (is_has_af)
            {
                hint_name += Globals::Game::translate_string("st_jup_b32_has_af");

                for (const xr_string& it : artefacts)
                {
                    xr_string translate_data_name = "st_";
                    translate_data_name += it;
                    translate_data_name += "_name";
                    hint_name += "\\n";
                    hint_name += Globals::Game::translate_string(translate_data_name.c_str());
                }
            }
            else
            {
                hint_name += Globals::Game::translate_string("st_jup_b32_no_af");
            }

            if (object_id && (Globals::Game::level::map_has_object_spot(object_id, "primary_object") == 0))
                Globals::Game::level::map_add_object_spot(object_id, "primary_object", hint_name.c_str());
        }
    }
}

inline bool isImportantSave(void) { return !!psActorFlags.test(AF_IMPORTANT_SAVE); }

inline void set_inactivate_input_time(const std::uint32_t delta)
{
    DataBase::Storage::getInstance().setStorageDisableInputTime(
        DataBase::Storage::getInstance().getActor()->ID(), Globals::Game::get_game_time());
    DataBase::Storage::getInstance().setStorageDisableInputIdle(
        DataBase::Storage::getInstance().getActor()->ID(), delta);

    Globals::Game::level::disable_input();
}

inline xr_map<xr_string, xr_string> parse_ini_section_to_array(
    CScriptIniFile* const p_ini, const xr_string& section_name)
{
    xr_map<xr_string, xr_string> result;

    if (p_ini && !section_name.empty() && p_ini->section_exist(section_name.c_str()))
    {
        const char* id_name;
        const char* value_name;
        for (std::uint32_t i = 0; i < p_ini->line_count(section_name.c_str()); ++i)
        {
            if (p_ini->r_line(section_name.c_str(), i, &id_name, &value_name))
            {
                xr_string copy_id_name = id_name;
                xr_string copy_value_name = value_name;
                boost::trim(copy_id_name);
                boost::trim(copy_value_name);

                result[copy_id_name] = copy_value_name;
            }
        }
    }

    if (result.empty())
        Msg("[Scripts/Globals/parse_ini_section_to_array(p_ini, section_name)] WARNING: parsed data is empty!");

    return result;
}

inline void start_surge(void)
{
    if (Script_SurgeManager::getInstance().get_nearest_cover())
    {
        Script_SurgeManager::getInstance().start(true);
    }
    else
    {
        Msg("[Scripts/Globals/start_surge()] WARNING: can't obtain nearest cover and can't start manually!");
    }
}

inline void stop_surge(void)
{
    if (Script_SurgeManager::getInstance().IsStarted())
        Script_SurgeManager::getInstance().end_surge(true);
}

inline void change_game_time(std::uint32_t days, std::uint32_t hours, std::uint32_t mins)
{
    game_sv_Single* tpGame = smart_cast<game_sv_Single*>(Level().Server->GetGameState());
    if (tpGame && ai().get_alife())
    {
        u32 value = days * 86400 + hours * 3600 + mins * 60;
        float fValue = static_cast<float>(value);
        value *= 1000; // msec
        g_pGamePersistent->Environment().ChangeGameTime(fValue);
        tpGame->alife().time_manager().change_game_time(value);
    }
}

inline void stop_play_sound(CScriptGameObject* const p_npc)
{
    if (!p_npc)
    {
        MESSAGEWR("p_npc == nullptr!");
        return;
    }

    if (p_npc->Alive())
    {
        p_npc->set_sound_mask(Globals::kUnsignedInt32Undefined);
        p_npc->set_sound_mask(0);
    }
}

inline bool is_npc_in_combat(CScriptGameObject* const p_npc)
{
    if (!p_npc)
    {
        MESSAGEWR("p_npc == nullptr!");
        return false;
    }

    CScriptActionPlanner* const p_planner = get_script_action_planner(p_npc);

    if (!p_planner->initialized())
        return false;
    
    CScriptActionPlanner::_action_id_type current_action_id = p_planner->current_action_id();

    return (current_action_id == StalkerDecisionSpace::eWorldOperatorCombatPlanner || current_action_id == StalkerDecisionSpace::eWorldOperatorPostCombatWait);
}

inline void unstrap_weapon(
    CScriptGameObject* const p_npc, CScriptGameObject* const p_not_used, const xr_vector<xr_string>& not_used)
{
    if (!p_npc)
    {
        Msg("[Scripts/Globals/unstrap_weapon(p_npc)] WARNING: p_npc == nullptr! Return ...");
        return;
    }

    const xr_vector<xr_string> weapon_table = {
        "pri_a15_wpn_svu", "pri_a15_wpn_wincheaster1300", "pri_a15_wpn_ak74u", "pri_a15_wpn_ak74"};

    const xr_vector<xr_string> unstrapped = {"pri_a15_wpn_svu_unstrapped", "pri_a15_wpn_wincheaster1300_unstrapped",
        "pri_a15_wpn_ak74u_unstrapped", "pri_a15_wpn_ak74_unstrapped"};
    std::uint32_t index = 0;
    CScriptGameObject* p_item = nullptr;
    for (const xr_string& it : weapon_table)
    {
        p_item = p_npc->GetObjectByName(it.c_str());
        if (p_item)
        {
            break;
        }

        ++index;
    }

    if (p_item == nullptr)
    {
        Msg("[Scripts/Globals/unstrap_weapon(p_npc)] WARNING: can't find item! Return ...");
        return;
    }

    p_item->attachable_item_load_attach(unstrapped.at(index).c_str());
}

inline void strap_weapon(
    CScriptGameObject* const p_npc, CScriptGameObject* const p_not_used, const xr_vector<xr_string>& not_used)
{
    if (!p_npc)
    {
        Msg("[Scripts/Globals/unstrap_weapon(p_npc)] WARNING: p_npc == nullptr! Return ...");
        return;
    }

    const xr_vector<xr_string> weapon_table = {
        "pri_a15_wpn_svu", "pri_a15_wpn_wincheaster1300", "pri_a15_wpn_ak74u", "pri_a15_wpn_ak74"};

    CScriptGameObject* p_item = nullptr;
    std::uint32_t index = 0;

    for (const xr_string& it : weapon_table)
    {
        p_item = p_npc->GetObjectByName(it.c_str());

        if (p_item)
        {
            break;
        }

        ++index;
    }

    if (p_item == nullptr)
    {
        Msg("[Scripts/Globals/unstrap_weapon(p_npc)] WARNING: p_item == nullptr! Return ...");
        return;
    }

    p_item->attachable_item_load_attach(weapon_table.at(index).c_str());
}

inline void lights_off(
    CScriptGameObject* const p_not_used_1, CScriptGameObject* const p_not_used_2, const xr_vector<xr_string>& not_used)
{
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("pri_a15_lights_off");
}

inline void lights_on(
    CScriptGameObject* const p_not_used_1, CScriptGameObject* const p_not_used_2, const xr_vector<xr_string>& not_used)
{
    DataBase::Storage::getInstance().getActor()->DisableInfoPortion("pri_a15_lights_off");
}

inline void end_scene(
    CScriptGameObject* const p_not_used_1, CScriptGameObject* const p_not_used_2, const xr_vector<xr_string>& not_used)
{
    DataBase::Storage::getInstance().getActor()->DisableInfoPortion("pri_a15_cutscene_end");
}

inline void break_fence(
    CScriptGameObject* const p_not_used_1, CScriptGameObject* const p_not_used_2, const xr_vector<xr_string>& not_used)
{
    if (DataBase::Storage::getInstance().getAnimationObjectsByName().find("pri_a15_door") == DataBase::Storage::getInstance().getAnimationObjectsByName().end())
    {
        Msg("[Scripts/Globals/break_fence()] WARNING: can't find object by 'pri_a15_door' Return ...");
        return;
    }

   Script_Binder_DoorLabx8* const p_binder = DataBase::Storage::getInstance().getAnimationObjectsByName().at("pri_a15_door");

   if (!p_binder)
   {
       Msg("[Scripts/Globals/break_fence()] WARNING: something is went wrong! Object is nullptr Return ...");
       return;
   }

   p_binder->animation_forward();
}

inline Script_CampData* get_current_camp(const Fvector& position) 
{
    for (const std::pair<std::uint16_t, std::pair<Script_CampData*, CScriptGameObject*>>& it : DataBase::Storage::getInstance().getCamps())
    {
        if (it.second.second->inside(position))
        {
            return it.second.first;
        }
    }

    return nullptr;
}

inline void start_guitar(CScriptGameObject* const p_npc) 
{
    if (!p_npc)
    {
        Msg("[Scripts/Globals/start_guitar(p_npc)] WARNING: p_npc == nullptr! Return ...");
        return;
    }

    std::uint16_t camp_id = DataBase::Storage::getInstance().getStorage().at(p_npc->ID()).getRegisteredCamp();

    if (!camp_id)
    {
        Msg("[Scripts/Globals/start_guitar(p_npc)] WARNING: can't find camp_id, it doesnt registered! Return ...");
        return;
    }

    Script_CampData* const p_camp = DataBase::Storage::getInstance().getCamps().at(camp_id).first;

    p_camp->getScriptSoundManager()->set_storyteller(p_camp->getDirectorID());
    p_camp->getScriptSoundManager()->set_story(p_camp->getGuitars()[Script_RandomInt::getInstance().Generate<std::uint32_t>(0, p_camp->getGuitars().size() - 1)]);
    p_camp->setSoundManagerStarted(true);
    p_camp->update();
}

inline void start_harmonica(CScriptGameObject* const p_npc) 
{
    if (!p_npc)
    {
        Msg("[Scripts/Globals/start_harmonica(p_npc)] WARNING: p_npc == nullptr! Return ...");
        return;
    }

    std::uint16_t camp_id = DataBase::Storage::getInstance().getStorage().at(p_npc->ID()).getRegisteredCamp();

    if (!camp_id)
    {
        Msg("[Scripts/Globals/start_harmonica(p_npc)] WARNING: can't find camp_id, it doesnt registered! Return ...");
        return;
    }

    Script_CampData* const p_camp = DataBase::Storage::getInstance().getCamps().at(camp_id).first;

    p_camp->getScriptSoundManager()->set_storyteller(p_camp->getDirectorID());
    p_camp->getScriptSoundManager()->set_story(
        p_camp->getHarmonicas()[Script_RandomInt::getInstance().Generate<std::uint32_t>(
            0, p_camp->getHarmonicas().size() - 1)]);
    p_camp->setSoundManagerStarted(true);
    p_camp->update();
}

inline bool is_npc_asleep(CScriptGameObject* const p_npc) 
{
    if (!p_npc)
    {
        Msg("[Scripts/Globals/is_npc_asleep(p_npc)] WARNING: p_npc == nullptr! Return ...");
        return false;
    }

    if (DataBase::Storage::getInstance().getStorage().find(p_npc->ID()) == DataBase::Storage::getInstance().getStorage().end())
    {
        Msg("[Scripts/Globals/is_npc_asleep(p_npc)] WARNING: SOMETHING IS WRONG THE NPC DIDN'T REGISTERED IN STORAGE YET! %d Return ...", p_npc->ID());
        return false;
    }
    const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(p_npc->ID());
    return (storage.getStateManager()->getAnimState()->getStates().getCurrentStateName() == "sleep");
}

inline void unlock_medkit(CScriptGameObject* const p_client_object) 
{
    if (!p_client_object)
    {
        MESSAGEWR("p_client_object == nullptr");
        return;
    }

    if (DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().at("wounded"))
    {
        DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().at("wounded")->getWoundedManager()->unlock_medkit();
    }
}

inline bool is_wounded(CScriptGameObject* const p_client_object) 
{
    if (!p_client_object)
    {
        MESSAGEWR("p_client_object == nullptr!");
        return false;
    }

    if (DataBase::Storage::getInstance().getStorage().find(p_client_object->ID()) ==
        DataBase::Storage::getInstance().getStorage().end())
    {
        MESSAGEWR("early calling of storage!");
        return false;
    }

    if (DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().at("wounded") == nullptr)
        return false;

    
    // Lord: проверить на использование nil
    return (DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().at("wounded")->getWoundedManager()->getStateName().empty() == false); 
}

inline void hit_callback(const std::uint16_t npc_id)
{
    if (DataBase::Storage::getInstance().getStorage().at(npc_id).getSchemes().at("wounded"))
        DataBase::Storage::getInstance()
            .getStorage()
            .at(npc_id)
            .getSchemes()
            .at("wounded")
            ->getWoundedManager()
            ->hit_callback();
}

inline bool is_heavy_wounded(const std::uint16_t npc_id) 
{
    if (DataBase::Storage::getInstance().getStorage().find(npc_id) == DataBase::Storage::getInstance().getStorage().end())
    {
        MESSAGEWR("early calling, storage maybe not prepared");
        return false;
    }

    if (npc_id == Globals::kUnsignedInt32Undefined)
    {
        MESSAGEWR("bad id check code!");
        return false;
    }

    if (DataBase::Storage::getInstance().getStorage().at(npc_id).getSchemes().at("wounded"))
        return (DataBase::Storage::getInstance().getStorage().at(npc_id).getSchemes().at("wounded")->getWoundedManager()->getStateName().empty() == false);

    return false;
}

inline bool is_psy_wounded_by_id(const std::uint16_t npc_id)
{
    if (DataBase::Storage::getInstance().getStorage().find(npc_id) == DataBase::Storage::getInstance().getStorage().end())
    {
        MESSAGEWR("early calling, storage maybe not prepared!");
        return false;
    }

    if (npc_id == Globals::kUnsignedInt32Undefined)
    {
        MESSAGEWR("bad id check code!");
        return false;
    }

    xr_vector<xr_string> states;
    states.emplace_back("psy_pain");
    states.emplace_back("psy_armed");
    states.emplace_back("psy_shoot");
    states.emplace_back("psycho_pain");
    states.emplace_back("psycho_shoot");

    DataBase::Storage_Scheme* p_storage = DataBase::Storage::getInstance().getStorage().at(npc_id).getSchemes().at("wounded");
    if (p_storage)
    {
        for (const xr_string& it : states)
        {
            if (p_storage->getWoundedManager()->getStateName() == it)
                return true;
        }
    }

    return false;
}

inline void turn_on_campfires_by_smart_name(const xr_string& name) 
{
    if (name.empty())
    {
        MESSAGEWR("invalid string!");
        return;
    }

    if (DataBase::Storage::getInstance().getCampfiresBySmartName().find(name) == DataBase::Storage::getInstance().getCampfiresBySmartName().end())
    {
        MESSAGEWR("can't find data by smart_terrain %s", name.c_str());
        return;
    }

    const xr_map<std::uint16_t, CZoneCampfire*>& smart_campfires =
        DataBase::Storage::getInstance().getCampfiresBySmartName().at(name);

    if (smart_campfires.empty())
    {
        MESSAGEWR("data is empty! Maybe it was not initialized correctly!");
        return;
    }

    for (const std::pair<std::uint16_t, CZoneCampfire*>& it : smart_campfires)
    {
        if (it.second)
            if (!it.second->is_on())
                it.second->turn_on_script();
    }
}

inline void turn_off_campfires_by_smart_name(const xr_string& name) 
{
    if (name.empty())
    {
        MESSAGEWR("invalid string!");
        return;
    }

    if (DataBase::Storage::getInstance().getCampfiresBySmartName().find(name) ==
        DataBase::Storage::getInstance().getCampfiresBySmartName().end())
    {
        MESSAGEWR("can't find data by smart_terrain %s", name.c_str());
        return;
    }

    const xr_map<std::uint16_t, CZoneCampfire*>& smart_campfires =
        DataBase::Storage::getInstance().getCampfiresBySmartName().at(name);

    if (smart_campfires.empty())
    {
        MESSAGEWR("data is empty! Maybe it was not initialized correctly!");
        return;
    }

    for (const std::pair<std::uint16_t, CZoneCampfire*>& it : smart_campfires)
    {
        if (it.second)
            if (it.second->is_on())
                it.second->turn_off_script();
    }
}

inline void setup_gulag_and_logic_on_spawn(CScriptGameObject* const p_client_object, const DataBase::Storage_Data& storage, const std::uint16_t stype, const bool loaded)
{
    if (p_client_object == nullptr)
    {
#ifdef DEBUG
        MESSAGEWR("Invalid object!");
#endif // DEBUG
        return;
    }

    CSE_ALifeMonsterAbstract* const p_monster = ai().alife().objects().object(p_client_object->ID())->cast_monster_abstract();
    if (p_monster)
    {
        std::uint16_t smart_terrain_id = p_monster->m_smart_terrain_id;
#ifdef DEBUG
        MESSAGE("client_object=%s smart_terrain_id=%d is_loaded=%s", p_client_object->Name(), smart_terrain_id, loaded ? "true" : false);
#endif // DEBUG

        if (smart_terrain_id && smart_terrain_id != Globals::kUnsignedInt16Undefined)
        {
            Script_SE_SmartTerrain* const p_smart_terrain = ai().alife().objects().object(smart_terrain_id)->cast_script_se_smartterrain();
            if (p_smart_terrain == nullptr)
            {
#ifdef DEBUG
                MESSAGEWR("can't cast to smart terrain!");
#endif // DEBUG
                return;
            }

            bool is_need_setup_logic = false;

            if (!loaded)
            {
				if (p_smart_terrain->getNpcInfo().find(p_client_object->ID()) != p_smart_terrain->getNpcInfo().end())
				{
					if (p_smart_terrain->getNpcInfo().at(p_client_object->ID()).m_begin_job)
					{
						is_need_setup_logic = true;
					}
					else
					{
						is_need_setup_logic = false;
					}
				}
            }

            if (is_need_setup_logic)
            {
                p_smart_terrain->setup_logic(p_client_object);
            }
            else
            {
                XR_LOGIC::intialize_job(p_client_object, storage, loaded, DataBase::Storage::getInstance().getActor(), stype);
            }
        }
        else
        {
            XR_LOGIC::intialize_job(p_client_object, storage, loaded, DataBase::Storage::getInstance().getActor(), stype);
        }
    }
    else
    {
        XR_LOGIC::intialize_job(p_client_object, storage, loaded, DataBase::Storage::getInstance().getActor(), stype);
    }
}

} // namespace Globals
} // namespace Scripts
} // namespace Cordis
