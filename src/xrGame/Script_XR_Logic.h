#pragma once

#include "xrCore/xr_ini.h"
#include "xrServer_Objects_ALife.h"
#include "Script_XR_Gulag.h"

namespace Cordis
{
namespace Scripts
{
namespace XR_LOGIC
{
constexpr const char* XR_LOGIC_CUSTOMDATA = "<customdata>";
constexpr const char* XR_LOGIC_CLIENT_SERVER_ARGUMENTS = "client_server";
constexpr const char* XR_LOGIC_CLIENT_CLIENT_ARGUMENTS = "client_client";

inline CInifile configure_schemes(CScriptGameObject* npc, const CInifile& ini, const xr_string& ini_filename,
    unsigned int stype, const xr_string& section_logic, const xr_string& gulag_name)
{
    if (!npc)
    {
        R_ASSERT2(false, "Object was null!");
        return CInifile("system.ltx");
    }

    std::uint16_t npc_id = npc->ID();
    DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage()[npc_id];

    if (storage.m_active_section.size())
    {
        Script_LogicManager::getInstance().all_deactivate(storage[storage.m_active_scheme].m_actions, npc);
    }

    CInifile actual_ini = ini;
    xr_string actual_ini_filename;

    if (!ini.section_exist(section_logic.c_str()))
    {
        if (!gulag_name.size())
        {
            actual_ini_filename = ini_filename;
        }
        else
        {
            Msg("[Scripts/XR_LOGIC/configure_schemes] ERROR: object '%s': unable to find section '%s' in '%s'",
                npc->Name(), section_logic.c_str(), ini_filename.c_str());
            R_ASSERT(false);

            return CInifile("system.ltx");
        }
    }
    else
    {
        xr_string filename = Globals::Utils::cfg_get_string(&ini, section_logic, "cfg", npc, false);
        if (filename.size())
        {
            actual_ini_filename = filename;
            actual_ini = CInifile(filename.c_str());

            if (!actual_ini.section_exist(section_logic.c_str()))
            {
                Msg("object: %s configuratuion file [%s] NOT FOUND or section [logic] isn't assigned ", npc->Name(),
                    filename.c_str());
                R_ASSERT(false);
                return CInifile("system.ltx");
            }
        }
        else
        {
            if (stype == Globals::STYPE_STALKER || stype == Globals::STYPE_MOBILE)
            {
            }
        }
    }

    // Lord: доделать!
    return CInifile("а это убрать и написать нормальный аргумент.ltx");
}

inline CInifile get_customdata_or_ini_file(CScriptGameObject* npc, const xr_string& filename)
{
    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return CInifile("system.ltx");
    }

    DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage()[npc->ID()];
    if (filename == XR_LOGIC_CUSTOMDATA)
    {
        CScriptIniFile* file = npc->spawn_ini();
        if (!file)
        {
            CInifile ini(file->fname());
            return ini;
        }
        else
            return CInifile("[[scripts\\dummy.ltx]]");
    }
    else if (!filename.find('*'))
    {
        if (storage.m_job_ini.size())
            return CInifile(storage.m_job_ini.c_str());

        return XR_GULAG::loadLtx(filename.substr(filename.find('*') + 1));
    }
}

inline void intialize_job(CScriptGameObject* object, DataBase::Storage_Data& storage, const bool& loaded,
    CScriptGameObject* actor, const std::uint16_t& stype)
{
    if (!loaded)
    {
        xr_string ini_filename = XR_LOGIC_CUSTOMDATA;
        CInifile ini
    }
}

// @ Нужно ли здесь template
template <typename... Arguments>
inline void issue_events(CScriptGameObject* npc, DataBase::Storage_Data& storage, Arguments... arguments)
{
    // @ Lord: реализовать!!!!!!!
    return;
}

inline void parse_infopotions(xr_map<std::uint32_t, CondlistData::CondlistValues>& data, xr_string& buffer)
{
    if (!buffer.size())
    {
        // Msg("[Scripts/XR_LOGIC/parse_infoportions_data] was null infoportion");
        return;
    }

    std::string mask_rejection = "+-~!=}%"; // @ }% если у нас там только одно выражение
    std::string maked_string = "";
    std::uint32_t total_it = 0;
    CondlistData::CondlistValues values;
    for (std::uint32_t it = 0; it < buffer.size(); ++it)
    {
        if (buffer[it] == '{' || buffer[it] == '%')
            continue;

        switch (buffer[it])
        {
        case '+':
        {
            std::uint32_t z = it;
            z += 1;
            while (mask_rejection.find(buffer[z]) == std::string::npos)
            {
                maked_string += buffer[z];
                ++z;
            }
            values.m_infopotion_name = maked_string;
            values.m_required = true;
            data[total_it] = values;
            ++total_it;
            --z;
            it = z;
            maked_string.clear();
            break;
        }
        case '-':
        {
            std::uint32_t z = it;
            z += 1;
            while (mask_rejection.find(buffer[z]) == std::string::npos)
            {
                maked_string += buffer[z];
                ++z;
            }
            values.m_infopotion_name = maked_string;
            values.m_required = false;
            data[total_it] = values;
            ++total_it;
            --z;
            it = z;
            maked_string.clear();
            break;
        }
        case '=':
        {
            std::uint32_t z = it;
            std::string params = "";
            z += 1;
            while (mask_rejection.find(buffer[z]) == std::string::npos)
            {
                if (buffer[z] == '(')
                {
                    std::uint32_t z1 = z;
                    ++z1;
                    while (buffer[z1] != ')')
                    {
                        params += buffer[z1];
                        ++z1;
                    }
                    z = z1;
                    break;
                }
                maked_string += buffer[z];
                ++z;
            }
            values.m_function_name = maked_string;
            values.m_params = params;
            values.m_expected = true;
            data[total_it] = values;
            ++total_it;
            --z;
            it = z;
            maked_string.clear();
            break;
        }
        case '~':
        {
            std::uint32_t z = it;
            z += 1;
            while (mask_rejection.find(buffer[z]) == std::string::npos)
            {
                maked_string += buffer[z];
                ++z;
            }
            values.m_prob = atoi(maked_string.c_str());
            data[total_it] = values;
            ++total_it;
            --z;
            it = z;
            maked_string.clear();
            break;
        }
        case '!':
        {
            std::uint32_t z = it;
            std::string params = "";
            z += 1;
            while (mask_rejection.find(buffer[z]) == std::string::npos)
            {
                if (buffer[z] == '(')
                {
                    std::uint32_t z1 = z;
                    while (buffer[z1] != ')')
                    {
                        params += buffer[z1];
                        ++z1;
                    }
                    z = z1;
                    break;
                }
                maked_string += buffer[z];
                ++z;
            }
            values.m_function_name = maked_string;
            values.m_params = params;
            values.m_expected = false;
            data[total_it] = values;
            ++total_it;
            --z;
            it = z;
            maked_string.clear();
            break;
        }
        }
    }
}

inline bool parse_condlistdata(xr_map<std::uint32_t, CondlistData>& data, xr_vector<CondlistParsingData>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Can't be null buffer!");
        return false;
    }

    for (std::uint32_t i = 0; i < buffer.size(); ++i)
    {
        data[i].m_text_name = buffer[i].m_text_name;
        parse_infopotions(data[i].m_infop_check, buffer[i].m_infocheck_name);
        parse_infopotions(data[i].m_infop_set, buffer[i].m_infoset_name);
    }

    return true;
}

// @ Lord: doesn't have any differences between parse_condlist_by_script_object, but first argument is script object :D
inline xr_map<std::uint32_t, CondlistData> parse_condlist_by_server_object(
    /*CSE_ALifeObject* npc,*/ const xr_string& section, const xr_string& field, const xr_string& source)
{
    //     if (!npc)
    //     {
    //         //   R_ASSERT2(false, "object is null!");
    //         // Lord: without any assertion cuz first argument may be nullptr
    //         Msg("[Script/XR_LOGIC/parse_condlist]: Warning 'npc' was nullptr!");
    //         /*        return xr_map<std::uint32_t, CondlistData>();*/
    //     }

    xr_map<std::uint32_t, CondlistData> result;
    xr_vector<xr_string> vector_sections;
    xr_vector<xr_string> vector_info_check;
    xr_vector<xr_string> vector_info_set;
    xr_string current_check = "";
    xr_string current_set = "";
    xr_string current_section = "";
    std::uint32_t it = 0;
    std::uint32_t total_it = 0;
    bool was_found_check = false;
    bool was_found_set = false;
    bool was_found_section = false;
    std::uint8_t counter_percent_symbol = 0;
    xr_string mask_symbols = "%{}, qwertyuioplkjhgfdsamnbvcxz1234567890-+=~!_():";

    xr_vector<CondlistParsingData> buffer;
    CondlistParsingData sub_data;

    for (char it : source)
    {
        if (mask_symbols.find(it) == std::string::npos)
        {
            R_ASSERT2(false, "Incorrect symbol. Please check your file!");
            break;
        }

        if (isspace(it))
            continue;

        if (it == ',')
        {
            if (!was_found_section)
                sub_data.m_text_name = "nil";

            buffer.push_back(sub_data);
            sub_data.Clear();
            continue;
        }

        if (it == '{')
        {
            if (current_section.size())
            {
                //		vector_sections.push_back(current_section);
                sub_data.m_text_name = current_section;
                current_section.clear();
                was_found_section = false;
            }

            if (was_found_set)
            {
                R_ASSERT2(false, "It can't be! You're lose percent for your set infoportion! Check your file!");
                break;
            }

            if (was_found_check)
            {
                R_ASSERT2(false, "Found duplicate!");
                break;
            }

            was_found_check = true;
            current_check += it;
            continue;
        }

        if (it == '}')
        {
            if (!was_found_check)
            {
                R_ASSERT2(false,
                    "Found right bracket without left bracket. No entry to check infoportion! Check your file!!!");
                break;
            }

            was_found_check = false;
            current_check += it;
            //		vector_info_check.push_back(current_check);
            sub_data.m_infocheck_name = current_check;
            current_check.clear();
            continue;
        }

        if (was_found_check)
        {
            current_check += it;
            continue;
        }

        if (it == '%')
        {
            if (current_section.size())
            {
                //	vector_sections.push_back(current_section);
                sub_data.m_text_name = current_section;
                current_section.clear();
                was_found_section = false;
            }

            if (was_found_check)
            {
                R_ASSERT2(false, "Incorrect sentence of set infoportion. Can't be it has symbol bracket in!");
                break;
            }

            // @ We met more two time the '%' symbol error in parsing!!!!
            if (counter_percent_symbol > 2)
            {
                R_ASSERT2(
                    false, "Incorrect sentence of set infoportion. More two symbols of percent!!! Check your file!");
                break;
            }

            ++counter_percent_symbol;

            if (counter_percent_symbol < 2)
            {
                was_found_set = true;
                current_set += it;
                continue;
            }
            else if (counter_percent_symbol == 2)
            {
                was_found_set = false;
                counter_percent_symbol = 0;
                current_set += it;
                //	vector_info_set.push_back(current_set);
                sub_data.m_infoset_name = current_set;
                current_set.clear();
                continue;
            }
        }

        if (was_found_set)
        {
            if (!was_found_section)
                was_found_section = true;

            current_set += it;
            continue;
        }

        if (!was_found_set && !was_found_check)
        {
            switch (it)
            {
            case '+':
            {
                R_ASSERT2(false, "You're lose bracket or percent symbol!");
                break;
            }
            case '-':
            {
                R_ASSERT2(false, "You're lose bracket or percent symbol!");
                break;
            }
            case '=':
            {
                R_ASSERT2(false, "You're lose bracket or percent symbol!");
                break;
            }
            case '~':
            {
                R_ASSERT2(false, "You're lose bracket or percent symbol!");
                break;
            }
            case '!':
            {
                R_ASSERT2(false, "You're lose bracket or percent symbol!");
                break;
            }
            }

            current_section += it;
            continue;
        }
        else
        {
            R_ASSERT2(
                false, "Incorrect parsing. Check your file! Can't parse section betwen two infoportion's sentences");
            break;
        }

        ++total_it;
    }

    // @ Added last sub_data, cuz in loop we can't add at last it : source!
    buffer.push_back(sub_data);
    sub_data.Clear();

    parse_condlistdata(result, buffer);

    return result;
}

// @ Lord: doesn't have any differences between parse_condlist_by_server_object, but first argument is CSE object :D
inline xr_map<std::uint32_t, CondlistData> parse_condlist_by_script_object(
    /*CScriptGameObject* npc,*/ const xr_string& section, const xr_string& field, const xr_string& source)
{
    //     if (!npc)
    //         Msg("[Scripts/XR_LOGIC/parse_condlist_by_script_object] npc was nullptr!");

    xr_map<std::uint32_t, CondlistData> result;
    xr_vector<xr_string> vector_sections;
    xr_vector<xr_string> vector_info_check;
    xr_vector<xr_string> vector_info_set;
    xr_string current_check = "";
    xr_string current_set = "";
    xr_string current_section = "";
    std::uint32_t it = 0;
    std::uint32_t total_it = 0;
    bool was_found_check = false;
    bool was_found_set = false;
    bool was_found_section = false;
    std::uint8_t counter_percent_symbol = 0;
    xr_string mask_symbols = "%{}, qwertyuioplkjhgfdsamnbvcxz1234567890-+=~!_():";

    xr_vector<CondlistParsingData> buffer;
    CondlistParsingData sub_data;

    for (char it : source)
    {
        if (mask_symbols.find(it) == std::string::npos)
        {
            R_ASSERT2(false, "Incorrect symbol. Please check your file!");
            break;
        }

        if (isspace(it))
            continue;

        if (it == ',')
        {
            if (!was_found_section)
                sub_data.m_text_name = "nil";

            buffer.push_back(sub_data);
            sub_data.Clear();
            continue;
        }

        if (it == '{')
        {
            if (current_section.size())
            {
                //		vector_sections.push_back(current_section);
                sub_data.m_text_name = current_section;
                current_section.clear();
                was_found_section = false;
            }

            if (was_found_set)
            {
                R_ASSERT2(false, "It can't be! You're lose percent for your set infoportion! Check your file!");
                break;
            }

            if (was_found_check)
            {
                R_ASSERT2(false, "Found duplicate!");
                break;
            }

            was_found_check = true;
            current_check += it;
            continue;
        }

        if (it == '}')
        {
            if (!was_found_check)
            {
                R_ASSERT2(false,
                    "Found right bracket without left bracket. No entry to check infoportion! Check your file!!!");
                break;
            }

            was_found_check = false;
            current_check += it;
            //		vector_info_check.push_back(current_check);
            sub_data.m_infocheck_name = current_check;
            current_check.clear();
            continue;
        }

        if (was_found_check)
        {
            current_check += it;
            continue;
        }

        if (it == '%')
        {
            if (current_section.size())
            {
                //	vector_sections.push_back(current_section);
                sub_data.m_text_name = current_section;
                current_section.clear();
                was_found_section = false;
            }

            if (was_found_check)
            {
                R_ASSERT2(false, "Incorrect sentence of set infoportion. Can't be it has symbol bracket in!");
                break;
            }

            // @ We met more two time the '%' symbol error in parsing!!!!
            if (counter_percent_symbol > 2)
            {
                R_ASSERT2(
                    false, "Incorrect sentence of set infoportion. More two symbols of percent!!! Check your file!");
                break;
            }

            ++counter_percent_symbol;

            if (counter_percent_symbol < 2)
            {
                was_found_set = true;
                current_set += it;
                continue;
            }
            else if (counter_percent_symbol == 2)
            {
                was_found_set = false;
                counter_percent_symbol = 0;
                current_set += it;
                //	vector_info_set.push_back(current_set);
                sub_data.m_infoset_name = current_set;
                current_set.clear();
                continue;
            }
        }

        if (was_found_set)
        {
            if (!was_found_section)
                was_found_section = true;

            current_set += it;
            continue;
        }

        if (!was_found_set && !was_found_check)
        {
            switch (it)
            {
            case '+':
            {
                R_ASSERT2(false, "You're lose bracket or percent symbol!");
                break;
            }
            case '-':
            {
                R_ASSERT2(false, "You're lose bracket or percent symbol!");
                break;
            }
            case '=':
            {
                R_ASSERT2(false, "You're lose bracket or percent symbol!");
                break;
            }
            case '~':
            {
                R_ASSERT2(false, "You're lose bracket or percent symbol!");
                break;
            }
            case '!':
            {
                R_ASSERT2(false, "You're lose bracket or percent symbol!");
                break;
            }
            }

            current_section += it;
            continue;
        }
        else
        {
            R_ASSERT2(
                false, "Incorrect parsing. Check your file! Can't parse section betwen two infoportion's sentences");
            break;
        }

        ++total_it;
    }

    // @ Added last sub_data, cuz in loop we can't add at last it : source!
    buffer.push_back(sub_data);
    sub_data.Clear();

    parse_condlistdata(result, buffer);

    return result;
}

inline xr_string pick_section_from_condlist(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* npc, const xr_map<std::uint32_t, CondlistData>& condlist)
{
    // Lord: доделать
    if (!actor)
    {
        R_ASSERT2(false, "object is null!");
        return xr_string("");
    }

    if (!npc)
    {
        R_ASSERT2(false, "object is null!");
        return xr_string("");
    }

    std::uint32_t value = 0; // idk what does it mean. Translate this to normal
    bool is_infoportion_conditions_met = false;

    for (const std::pair<std::uint32_t, CondlistData>& it : condlist)
    {
        // Изначально считаем, что все условия переключения удовлетворены
        is_infoportion_conditions_met = true;
        for (const std::pair<std::uint32_t, CondlistData::CondlistValues>& it_infoportion_check :
            it.second.m_infop_check)
        {
            if (it_infoportion_check.second.m_prob)
            {
                if (!value)
                {
                    value = Globals::Script_RandomInt::getInstance().Generate(1, 100);
                }

                if (it_infoportion_check.second.m_prob < value)
                {
                    // Инфорпоршень есть, но он не должен присутствовать
                    is_infoportion_conditions_met = false;
                    break;
                }
            }
            else if (it_infoportion_check.second.m_function_name.size())
            {
                if (Script_GlobalHelper::getInstance().m_registered_functions_xr_conditions.find(
                        it_infoportion_check.second.m_function_name) ==
                    Script_GlobalHelper::getInstance().m_registered_functions_xr_conditions.end())
                {
                    // @ Если мы ничего не нашли (Lord: проверить)
                    Msg("Object: [%s] - Function: %s doesn't registered in Singleton Script_GlobalHelper in function "
                        "Script_GlobalHelper::RegisterFunctionsFromAnotherFiles!!!! ",
                        npc->s_name, it_infoportion_check.second.m_function_name.c_str());
                    R_ASSERT(false);
                }

                if (it_infoportion_check.second.m_params.size())
                {
                    ///
                    // Parsing params
                    ///

                    xr_string buffer = it_infoportion_check.second.m_params;

                    if (buffer.find(':') == xr_string::npos)
                    {
                        Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(actor, npc, condlist)] Using function from "
                            "XR_CONDITION file and a function uses one argument.");

                        xr_string& argument = buffer;

                        Script_GlobalHelper::getInstance().getRegisteredFunctionsXRCondition()[it_infoportion_check.second
                                                                                     .m_function_name](actor, npc, argument);
                    }
                }
            }
        }
    }

    return xr_string("");
}
// Lord: doesn't uses delete this 
// inline xr_string pick_section_from_condlist(
//     CSE_ALifeCreatureActor* actor, CSE_ALifeDynamicObject* npc, const xr_map<std::uint32_t, CondlistData>& condlist)
// {
//     // Lord: доделать
//     if (!actor)
//     {
//         R_ASSERT2(false, "object is null!");
//         return xr_string("");
//     }
// 
//     if (!npc)
//     {
//         R_ASSERT2(false, "object is null!");
//         return xr_string("");
//     }
// 
//     return xr_string("");
// }

inline xr_string pick_section_from_condlist(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_map<std::uint32_t, CondlistData>& condlist)
{
    // Lord: доделать!
    if (!actor)
    {
        R_ASSERT2(false, "object is null!");
        return xr_string("");
    }

    if (!npc)
    {
        R_ASSERT2(false, "object is null!");
        return xr_string("");
    }

    return xr_string("");
}

} // namespace XR_LOGIC
} // namespace Scripts
} // namespace Cordis
