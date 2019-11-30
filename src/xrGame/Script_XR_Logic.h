#pragma once

#include "xrCore/xr_ini.h"
#include "xrServer_Objects_ALife.h"
#include "Script_XR_Gulag.h"
#include "Script_GlobalHelper.h"
#include "Script_LogicManager.h"
namespace Cordis
{
namespace Scripts
{
namespace XR_LOGIC
{
constexpr const char* XR_LOGIC_CUSTOMDATA = "<customdata>";
constexpr const char* XR_LOGIC_CLIENT_SERVER_ARGUMENTS = "_client_server";
constexpr const char* XR_LOGIC_SERVER_SERVER_ARGUMENTS = "_server";
constexpr const char* XR_LOGIC_CLIENT_CLIENT_ARGUMENTS = "_client";
constexpr const char* XR_LOGIC_TEXT_NEVER = "never";
constexpr const char* kXRLogicReturnTypeSuccessfulName = "true";
constexpr const char* kXRLogicReturnTypeBadName = "false";

inline CScriptIniFile configure_schemes(CScriptGameObject* npc, CScriptIniFile& ini, const xr_string& ini_filename,
    unsigned int stype, const xr_string& section_logic, const xr_string& gulag_name)
{
    if (!npc)
    {
        R_ASSERT2(false, "Object was null!");
        return CScriptIniFile("system.ltx");
    }

    std::uint16_t npc_id = npc->ID();
    const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(npc_id);

    if (storage.getActiveSchemeName().size())
    {
        /* Когда дойдёшь до данной реализации функции удали этот комментарий
                Script_LogicManager::getInstance().all_deactivate(
                    storage.getData().at(storage.getActiveSchemeName()).getActions(), npc);*/
    }

    CScriptIniFile actual_ini = ini;
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

            return CScriptIniFile("system.ltx");
        }
    }
    else
    {
        xr_string filename = Globals::Utils::cfg_get_string(&ini, section_logic, "cfg", npc, false);
        if (filename.size())
        {
            actual_ini_filename = filename;
            actual_ini = CScriptIniFile(filename.c_str());

            if (!actual_ini.section_exist(section_logic.c_str()))
            {
                Msg("object: %s configuratuion file [%s] NOT FOUND or section [logic] isn't assigned ", npc->Name(),
                    filename.c_str());
                R_ASSERT(false);
                return CScriptIniFile("system.ltx");
            }
        }
        else
        {
            if (stype == Globals::kSTypeStalker || stype == Globals::kSTypeMobile)
            {
            }
        }
    }

    // Lord: доделать!
    return CScriptIniFile("а это убрать и написать нормальный аргумент.ltx");
}

inline CScriptIniFile get_customdata_or_ini_file(CScriptGameObject* npc, const xr_string& filename)
{
    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return CScriptIniFile("system.ltx");
    }

    const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(npc->ID());
    if (filename == XR_LOGIC_CUSTOMDATA)
    {
        CScriptIniFile* file = npc->spawn_ini();
        if (!file)
        {
            return *file;
        }
        else
            return CScriptIniFile("[[scripts\\dummy.ltx]]");
    }
    else if (!filename.find('*'))
    {
        if (storage.getJobIniName().size())
            return CScriptIniFile(storage.getJobIniName().c_str());

        return *XR_GULAG::loadLtx(filename.substr(filename.find('*') + 1));
    }

    return CScriptIniFile(filename.c_str());
}

inline void intialize_job(CScriptGameObject* object, DataBase::Storage_Data& storage, const bool& loaded,
    CScriptGameObject* actor, const std::uint16_t& stype)
{
    // Lord: доделать!
    if (!loaded)
    {
        xr_string ini_filename = XR_LOGIC_CUSTOMDATA;
    }
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
            while (buffer[z] && (mask_rejection.find(buffer[z]) == std::string::npos))
            {
                maked_string += buffer[z];
                ++z;
            }
            values.m_infopotion_name = maked_string;
            values.m_required = true;
            data[total_it] = values;
            values.clear();
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
            while (buffer[z] && (mask_rejection.find(buffer[z]) == std::string::npos))
            {
                maked_string += buffer[z];
                ++z;
            }
            values.m_infopotion_name = maked_string;
            values.m_required = false;
            data[total_it] = values;
            values.clear();
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
            while (buffer[z] && (mask_rejection.find(buffer[z]) == std::string::npos))
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
            values.clear();
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
            while (buffer[z] && (mask_rejection.find(buffer[z]) == std::string::npos))
            {
                maked_string += buffer[z];
                ++z;
            }
            values.m_prob = atoi(maked_string.c_str());
            data[total_it] = values;
            values.clear();
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
            while (buffer[z] && (mask_rejection.find(buffer[z]) == std::string::npos))
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
            values.clear();
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
            else
            {
                if (current_section.size())
                    sub_data.m_text_name = current_section;
                else
                {
                    R_ASSERT2(false, "it can't be!");
                    sub_data.m_text_name = "nil";
                }
            }
            was_found_section = false;
            buffer.push_back(sub_data);
            sub_data.Clear();
            current_section.clear();
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

            was_found_section = true;
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
    if (current_section.size())
        sub_data.m_text_name = current_section;

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
// Lord: исправить pick_section_from_condlist!!!!
// Не правильно считывает {} false, true <- вот это вообще почему-то не учитывается
// Сделать полноценную проверку в {} и аналогично %%, чтобы не первая попавшаяся сущность анализировалось а целый блок,
// то есть {-info_portion =my_function}, в данном случае =my_function тоже анализируется!!!!!!!!
inline xr_string pick_section_from_condlist(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* npc, const xr_map<std::uint32_t, CondlistData>& condlist)
{
    // Lord: доделать
    if (!actor)
    {
        //  R_ASSERT2(false, "object is null!");
        //  return xr_string("");
        Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(client_actor, server_npc, condlist)] WARNING: client_actor = "
            "nullptr! client_actor is null!");
    }

    if (!npc)
    {
        //   R_ASSERT2(false, "object is null!");
        //   return xr_string("");
        Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(client_actor, server_npc, condlist)] WARNING: server_npc = "
            "nullptr! server_npc is null!");
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
                xr_string calling_function_name = it_infoportion_check.second.m_function_name;
                calling_function_name += XR_LOGIC_SERVER_SERVER_ARGUMENTS;

                if (Script_GlobalHelper::getInstance().getRegisteredFunctionsXRCondition().find(
                        calling_function_name) ==
                    Script_GlobalHelper::getInstance().getRegisteredFunctionsXRCondition().end())
                {
                    // @ Если мы ничего не нашли (Lord: проверить)
                    Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(actor, npc, condlist)] Object: [%s] - Function: "
                        "%s doesn't registered in Singleton Script_GlobalHelper in function "
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
                    xr_vector<xr_string> argument_buffer;
                    boost::regex expr{"\\w+"};
                    boost::regex_token_iterator<std::string::iterator> it{buffer.begin(), buffer.end(), expr};
                    boost::regex_token_iterator<std::string::iterator> end;

                    std::uint8_t argument_counter = 0;
                    while (it != end)
                    {
                        ++argument_counter;
                        Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(actor, npc, condlist)] Argument #%d: %s",
                            argument_counter, it->str().c_str());
                        argument_buffer.push_back(it->str().c_str());
                        ++it;
                    }

                    //                     if (buffer.find(':') == xr_string::npos)
                    //                     {
                    Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(actor, npc, condlist)] Total arguments count: %d",
                        argument_buffer.size());

                    /*                    xr_string& argument = buffer;*/

                    if (Script_GlobalHelper::getInstance().getRegisteredFunctionsXRCondition()[calling_function_name].
                        operator()<CScriptGameObject*, CSE_ALifeDynamicObject*, const xr_vector<xr_string>&>(
                            actor, npc, argument_buffer))
                    {
                        if (!it_infoportion_check.second.m_expected)
                        {
                            is_infoportion_conditions_met = false;
                            break;
                        }
                    }
                    else
                    {
                        if (it_infoportion_check.second.m_expected)
                        {
                            is_infoportion_conditions_met = false;
                            break;
                        }
                    }
                    /*                    }*/
                    /*
                                        else
                                        {
                                            // Lord: убедиться что сам аргумент не может быть нулём!!!!
                                            xr_string argument2 = buffer.substr(buffer.rfind(':') + 1);
                                            xr_string argument1 = buffer.erase(buffer.find(':'));
                                            int argument2_number = atoi(argument2.c_str());
                                            if (!argument2_number)
                                            {
                                                if (Script_GlobalHelper::getInstance()
                                                        .getRegisteredFunctionsXRCondition()[calling_function_name](
                                                            actor, npc, argument1, argument2))
                                                {
                                                    if (!it_infoportion_check.second.m_expected)
                                                    {
                                                        is_infoportion_conditions_met = false;
                                                        break;
                                                    }
                                                }
                                                else
                                                {
                                                    if (it_infoportion_check.second.m_expected)
                                                    {
                                                        is_infoportion_conditions_met = false;
                                                        break;
                                                    }
                                                }
                                            }

                                            else
                                            {
                                                if (Script_GlobalHelper::getInstance()
                                                        .getRegisteredFunctionsXRCondition()[calling_function_name](
                                                            actor, npc, argument1, argument2_number))
                                                {
                                                    if (!it_infoportion_check.second.m_expected)
                                                    {
                                                        is_infoportion_conditions_met = false;
                                                        break;
                                                    }
                                                }
                                                else
                                                {
                                                    if (it_infoportion_check.second.m_expected)
                                                    {
                                                        is_infoportion_conditions_met = false;
                                                        break;
                                                    }
                                                }
                                            }
                                        }*/
                }
                else
                { // no params

                    if (Script_GlobalHelper::getInstance()
                            .getRegisteredFunctionsXRCondition()[calling_function_name]
                            .getArgumentsCount() == 3)
                    {
                        // no params, but overloaded third argument can not used
                        xr_vector<xr_string> argument_buffer;

                        if (Script_GlobalHelper::getInstance()
                                .getRegisteredFunctionsXRCondition()[calling_function_name]
                                .
                                operator()<CScriptGameObject*, CSE_ALifeDynamicObject*, const xr_vector<xr_string>&>(
                                    actor, npc, argument_buffer))
                        {
                            if (!it_infoportion_check.second.m_expected)
                            {
                                is_infoportion_conditions_met = false;
                                break;
                            }
                        }
                        else
                        {
                            if (it_infoportion_check.second.m_expected)
                            {
                                is_infoportion_conditions_met = false;
                                break;
                            }
                        }
                    }
                    else if (Script_GlobalHelper::getInstance()
                                 .getRegisteredFunctionsXRCondition()[calling_function_name]
                                 .getArgumentsCount() == 2)
                    { // no params, but 2 arguments
                        if (Script_GlobalHelper::getInstance()
                                .getRegisteredFunctionsXRCondition()[calling_function_name]
                                .
                                operator()<CScriptGameObject*, CSE_ALifeDynamicObject*>(actor, npc))
                        {
                            if (!it_infoportion_check.second.m_expected)
                            {
                                is_infoportion_conditions_met = false;
                                break;
                            }
                        }
                        else
                        {
                            if (it_infoportion_check.second.m_expected)
                            {
                                is_infoportion_conditions_met = false;
                                break;
                            }
                        }
                    }
                    else if (Script_GlobalHelper::getInstance()
                                 .getRegisteredFunctionsXRCondition()[calling_function_name]
                                 .getArgumentsCount() == 0)
                    {
                        if (Script_GlobalHelper::getInstance()
                                .getRegisteredFunctionsXRCondition()[calling_function_name]())
                        {
                            if (!it_infoportion_check.second.m_expected)
                            {
                                is_infoportion_conditions_met = false;
                                break;
                            }
                        }
                        else
                        {
                            if (it_infoportion_check.second.m_expected)
                            {
                                is_infoportion_conditions_met = false;
                                break;
                            }
                        }
                    }
                }
            }
            else if (Globals::has_alife_info(it_infoportion_check.second.m_infopotion_name.c_str()))
            {
                if (!it_infoportion_check.second.m_required)
                {
                    Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(actor, npc, condlist)] CANCELLED: actor has "
                        "infoportion '%s', which is NOT needed [%s]",
                        it_infoportion_check.second.m_infopotion_name,
                        std::to_string(Globals::has_alife_info(it_infoportion_check.second.m_infopotion_name.c_str()))
                            .c_str());
                    is_infoportion_conditions_met = false;
                    break;
                }
                else
                {
                    Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(actor, npc, condlist)] PASSED: actor has "
                        "infoportion '%s', which is needed [%s]",
                        it_infoportion_check.second.m_infopotion_name,
                        std::to_string(Globals::has_alife_info(it_infoportion_check.second.m_infopotion_name.c_str()))
                            .c_str());
                }
            }
            else if (!Globals::has_alife_info(it_infoportion_check.second.m_infopotion_name.c_str()))
            {
                if (it_infoportion_check.second.m_required)
                {
                    Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(actor, npc, condlist)] CANCELLED: actor has NO "
                        "infop '%s', which is needed [%s]",
                        it_infoportion_check.second.m_infopotion_name,
                        std::to_string(Globals::has_alife_info(it_infoportion_check.second.m_infopotion_name.c_str()))
                            .c_str());
                    is_infoportion_conditions_met = false;
                    break;
                }
                else
                {
                    Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(actor, npc, condlist)] PASSED: actor has NO "
                        "infop '%s', which is not needed [%s]",
                        it_infoportion_check.second.m_infopotion_name.c_str(),
                        std::to_string(Globals::has_alife_info(it_infoportion_check.second.m_infopotion_name.c_str()))
                            .c_str());
                }
            }
        }

        if (is_infoportion_conditions_met)
        {
            for (std::pair<std::uint32_t, CondlistData::CondlistValues> it_infoportion_set : it.second.m_infop_set)
            {
                if (!DataBase::Storage::getInstance().getActor())
                {
                    R_ASSERT2(false,
                        "TRYING TO SET INFOPORTION SET WHEN ACTOR IS NULL! IT CAN'T BE YOU ARE TRYING TO USE WHEN "
                        "ACTOR IS NOT INITIALIZE, LATER USE IT");
                }

                if (it_infoportion_set.second.m_function_name.size())
                {
                    xr_string calling_function_name = it_infoportion_set.second.m_function_name;
                    calling_function_name += XR_LOGIC_CLIENT_SERVER_ARGUMENTS;

                    if (Script_GlobalHelper::getInstance().getRegisteredFunctionsXRCondition().find(
                            calling_function_name) ==
                        Script_GlobalHelper::getInstance().getRegisteredFunctionsXRCondition().end())
                    {
                        Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(actor, npc, condlist)] object '%s': "
                            "pick_section_from_condlist: function '%s' is "
                            "not defined in xr_effects",
                            npc->s_name, it_infoportion_set.second.m_function_name.c_str());
                        R_ASSERT(false);
                    }

                    if (it_infoportion_set.second.m_params.size())
                    {
                        if (it_infoportion_set.second.m_params.find(':') == xr_string::npos)
                        {
                            xr_string argument = it_infoportion_set.second.m_params;
                            Script_GlobalHelper::getInstance().getRegisteredFunctionsXREffects()[calling_function_name](
                                actor, npc, argument);
                        }
                        else
                        {
                            xr_string buffer = it_infoportion_set.second.m_params;
                            // Lord: убедиться что сам аргумент не может быть нулём!!!!
                            xr_string argument2 = buffer.substr(buffer.rfind(':') + 1);

                            xr_string argument1 = buffer.erase(buffer.find(':'));
                            int argument2_number = atoi(argument2.c_str());
                            if (!argument2_number)
                                Script_GlobalHelper::getInstance()
                                    .getRegisteredFunctionsXREffects()[calling_function_name](
                                        actor, npc, argument1, argument2);
                            else
                                Script_GlobalHelper::getInstance()
                                    .getRegisteredFunctionsXREffects()[calling_function_name](
                                        actor, npc, argument1, argument2_number);
                        }
                    }
                }
                else if (it_infoportion_set.second.m_required)
                {
                    if (!Globals::has_alife_info(it_infoportion_set.second.m_infopotion_name.c_str()))
                    {
                        CScriptGameObject* client_actor = DataBase::Storage::getInstance().getActor();
                        if (client_actor)
                            client_actor->GiveInfoPortion(it_infoportion_set.second.m_infopotion_name.c_str());
                        else
                            Msg("[XR_LOGIC/pick_section_from_condlist(server_actor, server_npc, condlist)] WARNING: "
                                "Can not set an infoportion [%s] for actor, because "
                                "DataBase::Storage::getInstane().getActor() == nullptr!",
                                it_infoportion_set.second.m_infopotion_name.c_str());
                    }
                }
                else if (!it_infoportion_set.second.m_required)
                {
                    if (Globals::has_alife_info(it_infoportion_set.second.m_infopotion_name.c_str()))
                    {
                        CScriptGameObject* client_actor = DataBase::Storage::getInstance().getActor();
                        if (client_actor)
                            client_actor->DisableInfoPortion(it_infoportion_set.second.m_infopotion_name.c_str());
                        else
                            Msg("[XR_LOGIC/pick_section_from_condlist(server_actor, server_npc, condlist)] WARNING: "
                                "Can not set an infoportion [%s] for actor, because "
                                "DataBase::Storage::getInstane().getActor() == nullptr!",
                                it_infoportion_set.second.m_infopotion_name.c_str());
                    }
                }
            }

            if (it.second.m_text_name == XR_LOGIC_TEXT_NEVER)
            {
                return xr_string("");
            }
            else
            {
                return it.second.m_text_name;
            }
        }
    }

    return xr_string("");
}

inline xr_string pick_section_from_condlist(
    CSE_ALifeDynamicObject* actor, CSE_ALifeDynamicObject* npc, const xr_map<std::uint32_t, CondlistData>& condlist)
{
    // Lord: доделать
    if (!actor)
    {
        //         R_ASSERT2(false, "object is null!");
        //         return xr_string("");
        Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(server_actor, server_npc, condlist)] WARNING: server_actor = "
            "nullptr! server_actor is null!");
    }

    if (!npc)
    {
        //         R_ASSERT2(false, "object is null!");
        //         return xr_string("");
        Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(server_actor, server_npc, condlist)] WARNING: server_npc = "
            "nullptr! server_npc is null!");
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
                xr_string calling_function_name = it_infoportion_check.second.m_function_name;
                calling_function_name += XR_LOGIC_SERVER_SERVER_ARGUMENTS;

                if (Script_GlobalHelper::getInstance().getRegisteredFunctionsXRCondition().find(
                        calling_function_name) ==
                    Script_GlobalHelper::getInstance().getRegisteredFunctionsXRCondition().end())
                {
                    // @ Если мы ничего не нашли (Lord: проверить)
                    Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(actor, npc, condlist)] Object: [%s] - Function: "
                        "%s doesn't registered in Singleton Script_GlobalHelper in function "
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
                    xr_vector<xr_string> argument_buffer;
                    boost::regex expr{"\\w+"};
                    boost::regex_token_iterator<std::string::iterator> it{buffer.begin(), buffer.end(), expr};
                    boost::regex_token_iterator<std::string::iterator> end;

                    std::uint8_t argument_counter = 0;
                    while (it != end)
                    {
                        ++argument_counter;
                        Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(actor, npc, condlist)] Argument #%d: %s",
                            argument_counter, it->str().c_str());
                        argument_buffer.push_back(it->str().c_str());
                        ++it;
                    }

                    //                     if (buffer.find(':') == xr_string::npos)
                    //                     {
                    Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(actor, npc, condlist)] Total arguments count: %d",
                        argument_buffer.size());

                    /*                    xr_string& argument = buffer;*/

                    if (Script_GlobalHelper::getInstance().getRegisteredFunctionsXRCondition()[calling_function_name].
                        operator()<CSE_ALifeDynamicObject*, CSE_ALifeDynamicObject*, const xr_vector<xr_string>&>(
                            actor, npc, argument_buffer))
                    {
                        if (!it_infoportion_check.second.m_expected)
                        {
                            is_infoportion_conditions_met = false;
                            break;
                        }
                    }
                    else
                    {
                        if (it_infoportion_check.second.m_expected)
                        {
                            is_infoportion_conditions_met = false;
                            break;
                        }
                    }
                    /*                    }*/
                    /*
                                        else
                                        {
                                            // Lord: убедиться что сам аргумент не может быть нулём!!!!
                                            xr_string argument2 = buffer.substr(buffer.rfind(':') + 1);
                                            xr_string argument1 = buffer.erase(buffer.find(':'));
                                            int argument2_number = atoi(argument2.c_str());
                                            if (!argument2_number)
                                            {
                                                if (Script_GlobalHelper::getInstance()
                                                        .getRegisteredFunctionsXRCondition()[calling_function_name](
                                                            actor, npc, argument1, argument2))
                                                {
                                                    if (!it_infoportion_check.second.m_expected)
                                                    {
                                                        is_infoportion_conditions_met = false;
                                                        break;
                                                    }
                                                }
                                                else
                                                {
                                                    if (it_infoportion_check.second.m_expected)
                                                    {
                                                        is_infoportion_conditions_met = false;
                                                        break;
                                                    }
                                                }
                                            }

                                            else
                                            {
                                                if (Script_GlobalHelper::getInstance()
                                                        .getRegisteredFunctionsXRCondition()[calling_function_name](
                                                            actor, npc, argument1, argument2_number))
                                                {
                                                    if (!it_infoportion_check.second.m_expected)
                                                    {
                                                        is_infoportion_conditions_met = false;
                                                        break;
                                                    }
                                                }
                                                else
                                                {
                                                    if (it_infoportion_check.second.m_expected)
                                                    {
                                                        is_infoportion_conditions_met = false;
                                                        break;
                                                    }
                                                }
                                            }
                                        }*/
                }
                else
                { // no params

                    if (Script_GlobalHelper::getInstance()
                            .getRegisteredFunctionsXRCondition()[calling_function_name]
                            .getArgumentsCount() == 3)
                    {
                        // no params, but overloaded third argument can not used
                        xr_vector<xr_string> argument_buffer;

                        if (Script_GlobalHelper::getInstance()
                                .getRegisteredFunctionsXRCondition()[calling_function_name]
                                .
                                operator()<CSE_ALifeDynamicObject*, CSE_ALifeDynamicObject*,
                                    const xr_vector<xr_string>&>(actor, npc, argument_buffer))
                        {
                            if (!it_infoportion_check.second.m_expected)
                            {
                                is_infoportion_conditions_met = false;
                                break;
                            }
                        }
                        else
                        {
                            if (it_infoportion_check.second.m_expected)
                            {
                                is_infoportion_conditions_met = false;
                                break;
                            }
                        }
                    }
                    else if (Script_GlobalHelper::getInstance()
                                 .getRegisteredFunctionsXRCondition()[calling_function_name]
                                 .getArgumentsCount() == 2)
                    { // no params, but 2 arguments
                        if (Script_GlobalHelper::getInstance()
                                .getRegisteredFunctionsXRCondition()[calling_function_name]
                                .
                                operator()<CSE_ALifeDynamicObject*, CSE_ALifeDynamicObject*>(actor, npc))
                        {
                            if (!it_infoportion_check.second.m_expected)
                            {
                                is_infoportion_conditions_met = false;
                                break;
                            }
                        }
                        else
                        {
                            if (it_infoportion_check.second.m_expected)
                            {
                                is_infoportion_conditions_met = false;
                                break;
                            }
                        }
                    }
                    else if (Script_GlobalHelper::getInstance()
                                 .getRegisteredFunctionsXRCondition()[calling_function_name]
                                 .getArgumentsCount() == 0)
                    {
                        if (Script_GlobalHelper::getInstance()
                                .getRegisteredFunctionsXRCondition()[calling_function_name]())
                        {
                            if (!it_infoportion_check.second.m_expected)
                            {
                                is_infoportion_conditions_met = false;
                                break;
                            }
                        }
                        else
                        {
                            if (it_infoportion_check.second.m_expected)
                            {
                                is_infoportion_conditions_met = false;
                                break;
                            }
                        }
                    }
                }
            }
            else if (Globals::has_alife_info(it_infoportion_check.second.m_infopotion_name.c_str()))
            {
                if (!it_infoportion_check.second.m_required)
                {
                    Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(actor, npc, condlist)] CANCELLED: actor has "
                        "infoportion '%s', which is NOT needed [%s]",
                        it_infoportion_check.second.m_infopotion_name,
                        std::to_string(Globals::has_alife_info(it_infoportion_check.second.m_infopotion_name.c_str()))
                            .c_str());
                    is_infoportion_conditions_met = false;
                    break;
                }
                else
                {
                    Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(actor, npc, condlist)] PASSED: actor has "
                        "infoportion '%s', which is needed [%s]",
                        it_infoportion_check.second.m_infopotion_name,
                        std::to_string(Globals::has_alife_info(it_infoportion_check.second.m_infopotion_name.c_str()))
                            .c_str());
                }
            }
            else if (!Globals::has_alife_info(it_infoportion_check.second.m_infopotion_name.c_str()))
            {
                if (it_infoportion_check.second.m_required)
                {
                    Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(actor, npc, condlist)] CANCELLED: actor has NO "
                        "infop '%s', which is needed [%s]",
                        it_infoportion_check.second.m_infopotion_name,
                        std::to_string(Globals::has_alife_info(it_infoportion_check.second.m_infopotion_name.c_str()))
                            .c_str());
                    is_infoportion_conditions_met = false;
                    break;
                }
                else
                {
                    Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(actor, npc, condlist)] PASSED: actor has NO "
                        "infop '%s', which is not needed [%s]",
                        it_infoportion_check.second.m_infopotion_name.c_str(),
                        std::to_string(Globals::has_alife_info(it_infoportion_check.second.m_infopotion_name.c_str()))
                            .c_str());
                }
            }
        }

        if (is_infoportion_conditions_met)
        {
            for (std::pair<std::uint32_t, CondlistData::CondlistValues> it_infoportion_set : it.second.m_infop_set)
            {
                if (!DataBase::Storage::getInstance().getActor())
                {
                    R_ASSERT2(false, "TRYING TO SET INFOPORTION SET WHEN ACTOR IS NULL!");
                }

                if (it_infoportion_set.second.m_function_name.size())
                {
                    xr_string calling_function_name = it_infoportion_set.second.m_function_name;
                    calling_function_name += XR_LOGIC_CLIENT_SERVER_ARGUMENTS;

                    if (Script_GlobalHelper::getInstance().getRegisteredFunctionsXRCondition().find(
                            calling_function_name) ==
                        Script_GlobalHelper::getInstance().getRegisteredFunctionsXRCondition().end())
                    {
                        Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(actor, npc, condlist)] object '%s': "
                            "pick_section_from_condlist: function '%s' is "
                            "not defined in xr_effects",
                            npc->s_name, it_infoportion_set.second.m_function_name.c_str());
                        R_ASSERT(false);
                    }

                    if (it_infoportion_set.second.m_params.size())
                    {
                        if (it_infoportion_set.second.m_params.find(':') == xr_string::npos)
                        {
                            xr_string argument = it_infoportion_set.second.m_params;
                            Script_GlobalHelper::getInstance().getRegisteredFunctionsXREffects()[calling_function_name](
                                actor, npc, argument);
                        }
                        else
                        {
                            xr_string buffer = it_infoportion_set.second.m_params;
                            // Lord: убедиться что сам аргумент не может быть нулём!!!!
                            xr_string argument2 = buffer.substr(buffer.rfind(':') + 1);

                            xr_string argument1 = buffer.erase(buffer.find(':'));
                            int argument2_number = atoi(argument2.c_str());
                            if (!argument2_number)
                                Script_GlobalHelper::getInstance()
                                    .getRegisteredFunctionsXREffects()[calling_function_name](
                                        actor, npc, argument1, argument2);
                            else
                                Script_GlobalHelper::getInstance()
                                    .getRegisteredFunctionsXREffects()[calling_function_name](
                                        actor, npc, argument1, argument2_number);
                        }
                    }
                }
                else if (it_infoportion_set.second.m_required)
                {
                    if (!Globals::has_alife_info(it_infoportion_set.second.m_infopotion_name.c_str()))
                    {
                        CScriptGameObject* client_actor = DataBase::Storage::getInstance().getActor();
                        if (client_actor)
                            client_actor->GiveInfoPortion(it_infoportion_set.second.m_infopotion_name.c_str());
                        else
                            Msg("[XR_LOGIC/pick_section_from_condlist(server_actor, server_npc, condlist)] WARNING: "
                                "Can not set an infoportion [%s] for actor, because "
                                "DataBase::Storage::getInstane().getActor() == nullptr!",
                                it_infoportion_set.second.m_infopotion_name.c_str());
                    }
                }
                else if (!it_infoportion_set.second.m_required)
                {
                    if (Globals::has_alife_info(it_infoportion_set.second.m_infopotion_name.c_str()))
                    {
                        CScriptGameObject* client_actor = DataBase::Storage::getInstance().getActor();
                        if (client_actor)
                            client_actor->DisableInfoPortion(it_infoportion_set.second.m_infopotion_name.c_str());
                        else
                            Msg("[XR_LOGIC/pick_section_from_condlist(server_actor, server_npc, condlist)] WARNING: "
                                "Can not set an infoportion [%s] for actor, because "
                                "DataBase::Storage::getInstane().getActor() == nullptr!",
                                it_infoportion_set.second.m_infopotion_name.c_str());
                    }
                }
            }

            if (it.second.m_text_name == XR_LOGIC_TEXT_NEVER)
            {
                return xr_string("");
            }
            else
            {
                return it.second.m_text_name;
            }
        }
    }

    return xr_string("");
}

inline xr_string pick_section_from_condlist(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_map<std::uint32_t, CondlistData>& condlist)
{
    // Lord: доделать
    if (!actor)
    {
        Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(client_actor, client_npc, condlist)] WARNING: client_actor = "
            "nullptr! client_actor is null!");
        //         R_ASSERT2(false, "object is null!");
        //         return xr_string("");
    }

    if (!npc)
    {
        //         R_ASSERT2(false, "object is null!");
        //         return xr_string("");
        Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(client_actor, client_npc, condlist)] WARNING: client_npc = "
            "nullptr! client_npc is null!");
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
                xr_string calling_function_name = it_infoportion_check.second.m_function_name;
                calling_function_name += XR_LOGIC_SERVER_SERVER_ARGUMENTS;

                if (Script_GlobalHelper::getInstance().getRegisteredFunctionsXRCondition().find(
                        calling_function_name) ==
                    Script_GlobalHelper::getInstance().getRegisteredFunctionsXRCondition().end())
                {
                    // @ Если мы ничего не нашли (Lord: проверить)
                    Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(actor, npc, condlist)] Object: [%s] - Function: "
                        "%s doesn't registered in Singleton Script_GlobalHelper in function "
                        "Script_GlobalHelper::RegisterFunctionsFromAnotherFiles!!!! ",
                        npc->Name(), it_infoportion_check.second.m_function_name.c_str());
                    R_ASSERT(false);
                }

                if (it_infoportion_check.second.m_params.size())
                {
                    ///
                    // Parsing params
                    ///
                    xr_string buffer = it_infoportion_check.second.m_params;
                    xr_vector<xr_string> argument_buffer;
                    boost::regex expr{"\\w+"};
                    boost::regex_token_iterator<std::string::iterator> it{buffer.begin(), buffer.end(), expr};
                    boost::regex_token_iterator<std::string::iterator> end;

                    std::uint8_t argument_counter = 0;
                    while (it != end)
                    {
                        ++argument_counter;
                        Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(actor, npc, condlist)] Argument #%d: %s",
                            argument_counter, it->str().c_str());
                        argument_buffer.push_back(it->str().c_str());
                        ++it;
                    }

                    //                     if (buffer.find(':') == xr_string::npos)
                    //                     {
                    Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(actor, npc, condlist)] Total arguments count: %d",
                        argument_buffer.size());

                    /*                    xr_string& argument = buffer;*/

                    if (Script_GlobalHelper::getInstance().getRegisteredFunctionsXRCondition()[calling_function_name].
                        operator()<CScriptGameObject*, CScriptGameObject*, const xr_vector<xr_string>&>(
                            actor, npc, argument_buffer))
                    {
                        if (!it_infoportion_check.second.m_expected)
                        {
                            is_infoportion_conditions_met = false;
                            break;
                        }
                    }
                    else
                    {
                        if (it_infoportion_check.second.m_expected)
                        {
                            is_infoportion_conditions_met = false;
                            break;
                        }
                    }
                    /*                    }*/
                    /*
                                        else
                                        {
                                            // Lord: убедиться что сам аргумент не может быть нулём!!!!
                                            xr_string argument2 = buffer.substr(buffer.rfind(':') + 1);
                                            xr_string argument1 = buffer.erase(buffer.find(':'));
                                            int argument2_number = atoi(argument2.c_str());
                                            if (!argument2_number)
                                            {
                                                if (Script_GlobalHelper::getInstance()
                                                        .getRegisteredFunctionsXRCondition()[calling_function_name](
                                                            actor, npc, argument1, argument2))
                                                {
                                                    if (!it_infoportion_check.second.m_expected)
                                                    {
                                                        is_infoportion_conditions_met = false;
                                                        break;
                                                    }
                                                }
                                                else
                                                {
                                                    if (it_infoportion_check.second.m_expected)
                                                    {
                                                        is_infoportion_conditions_met = false;
                                                        break;
                                                    }
                                                }
                                            }

                                            else
                                            {
                                                if (Script_GlobalHelper::getInstance()
                                                        .getRegisteredFunctionsXRCondition()[calling_function_name](
                                                            actor, npc, argument1, argument2_number))
                                                {
                                                    if (!it_infoportion_check.second.m_expected)
                                                    {
                                                        is_infoportion_conditions_met = false;
                                                        break;
                                                    }
                                                }
                                                else
                                                {
                                                    if (it_infoportion_check.second.m_expected)
                                                    {
                                                        is_infoportion_conditions_met = false;
                                                        break;
                                                    }
                                                }
                                            }
                                        }*/
                }
                else
                { // no params

                    if (Script_GlobalHelper::getInstance()
                            .getRegisteredFunctionsXRCondition()[calling_function_name]
                            .getArgumentsCount() == 3)
                    {
                        // no params, but overloaded third argument can not used
                        xr_vector<xr_string> argument_buffer;

                        if (Script_GlobalHelper::getInstance()
                                .getRegisteredFunctionsXRCondition()[calling_function_name]
                                .
                                operator()<CScriptGameObject*, CScriptGameObject*, const xr_vector<xr_string>&>(
                                    actor, npc, argument_buffer))
                        {
                            if (!it_infoportion_check.second.m_expected)
                            {
                                is_infoportion_conditions_met = false;
                                break;
                            }
                        }
                        else
                        {
                            if (it_infoportion_check.second.m_expected)
                            {
                                is_infoportion_conditions_met = false;
                                break;
                            }
                        }
                    }
                    else if (Script_GlobalHelper::getInstance()
                                 .getRegisteredFunctionsXRCondition()[calling_function_name]
                                 .getArgumentsCount() == 2)
                    { // no params, but 2 arguments
                        if (Script_GlobalHelper::getInstance()
                                .getRegisteredFunctionsXRCondition()[calling_function_name]
                                .
                                operator()<CScriptGameObject*, CScriptGameObject*>(actor, npc))
                        {
                            if (!it_infoportion_check.second.m_expected)
                            {
                                is_infoportion_conditions_met = false;
                                break;
                            }
                        }
                        else
                        {
                            if (it_infoportion_check.second.m_expected)
                            {
                                is_infoportion_conditions_met = false;
                                break;
                            }
                        }
                    }
                    else if (Script_GlobalHelper::getInstance()
                                 .getRegisteredFunctionsXRCondition()[calling_function_name]
                                 .getArgumentsCount() == 0)
                    {
                        if (Script_GlobalHelper::getInstance()
                                .getRegisteredFunctionsXRCondition()[calling_function_name]())
                        {
                            if (!it_infoportion_check.second.m_expected)
                            {
                                is_infoportion_conditions_met = false;
                                break;
                            }
                        }
                        else
                        {
                            if (it_infoportion_check.second.m_expected)
                            {
                                is_infoportion_conditions_met = false;
                                break;
                            }
                        }
                    }
                }
            }
            else if (Globals::has_alife_info(it_infoportion_check.second.m_infopotion_name.c_str()))
            {
                if (!it_infoportion_check.second.m_required)
                {
                    Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(actor, npc, condlist)] CANCELLED: actor has "
                        "infoportion '%s', which is NOT needed [%s]",
                        it_infoportion_check.second.m_infopotion_name,
                        std::to_string(Globals::has_alife_info(it_infoportion_check.second.m_infopotion_name.c_str()))
                            .c_str());
                    is_infoportion_conditions_met = false;
                    break;
                }
                else
                {
                    Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(actor, npc, condlist)] PASSED: actor has "
                        "infoportion '%s', which is needed [%s]",
                        it_infoportion_check.second.m_infopotion_name,
                        std::to_string(Globals::has_alife_info(it_infoportion_check.second.m_infopotion_name.c_str()))
                            .c_str());
                }
            }
            else if (!Globals::has_alife_info(it_infoportion_check.second.m_infopotion_name.c_str()))
            {
                if (it_infoportion_check.second.m_required)
                {
                    Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(actor, npc, condlist)] CANCELLED: actor has NO "
                        "infop '%s', which is needed [%s]",
                        it_infoportion_check.second.m_infopotion_name,
                        std::to_string(Globals::has_alife_info(it_infoportion_check.second.m_infopotion_name.c_str()))
                            .c_str());
                    is_infoportion_conditions_met = false;
                    break;
                }
                else
                {
                    Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(actor, npc, condlist)] PASSED: actor has NO "
                        "infop '%s', which is not needed [%s]",
                        it_infoportion_check.second.m_infopotion_name.c_str(),
                        std::to_string(Globals::has_alife_info(it_infoportion_check.second.m_infopotion_name.c_str()))
                            .c_str());
                }
            }
        }

        if (is_infoportion_conditions_met)
        {
            for (std::pair<std::uint32_t, CondlistData::CondlistValues> it_infoportion_set : it.second.m_infop_set)
            {
                if (!DataBase::Storage::getInstance().getActor())
                {
                    R_ASSERT2(false, "TRYING TO SET INFOPORTION SET WHEN ACTOR IS NULL!");
                }

                if (it_infoportion_set.second.m_function_name.size())
                {
                    xr_string calling_function_name = it_infoportion_set.second.m_function_name;
                    calling_function_name += XR_LOGIC_CLIENT_SERVER_ARGUMENTS;

                    if (Script_GlobalHelper::getInstance().getRegisteredFunctionsXRCondition().find(
                            calling_function_name) ==
                        Script_GlobalHelper::getInstance().getRegisteredFunctionsXRCondition().end())
                    {
                        Msg("[Scripts/XR_LOGIC/pick_section_from_condlist(actor, npc, condlist)] object '%s': "
                            "pick_section_from_condlist: function '%s' is "
                            "not defined in xr_effects",
                            npc->Name(), it_infoportion_set.second.m_function_name.c_str());
                        R_ASSERT(false);
                    }

                    if (it_infoportion_set.second.m_params.size())
                    {
                        if (it_infoportion_set.second.m_params.find(':') == xr_string::npos)
                        {
                            xr_string argument = it_infoportion_set.second.m_params;
                            Script_GlobalHelper::getInstance().getRegisteredFunctionsXREffects()[calling_function_name](
                                actor, npc, argument);
                        }
                        else
                        {
                            xr_string buffer = it_infoportion_set.second.m_params;
                            // Lord: убедиться что сам аргумент не может быть нулём!!!!
                            xr_string argument2 = buffer.substr(buffer.rfind(':') + 1);

                            xr_string argument1 = buffer.erase(buffer.find(':'));
                            int argument2_number = atoi(argument2.c_str());
                            if (!argument2_number)
                                Script_GlobalHelper::getInstance()
                                    .getRegisteredFunctionsXREffects()[calling_function_name](
                                        actor, npc, argument1, argument2);
                            else
                                Script_GlobalHelper::getInstance()
                                    .getRegisteredFunctionsXREffects()[calling_function_name](
                                        actor, npc, argument1, argument2_number);
                        }
                    }
                }
                else if (it_infoportion_set.second.m_required)
                {
                    if (!Globals::has_alife_info(it_infoportion_set.second.m_infopotion_name.c_str()))
                    {
                        CScriptGameObject* client_actor = DataBase::Storage::getInstance().getActor();
                        if (client_actor)
                            client_actor->GiveInfoPortion(it_infoportion_set.second.m_infopotion_name.c_str());
                        else
                            Msg("[XR_LOGIC/pick_section_from_condlist(server_actor, server_npc, condlist)] WARNING: "
                                "Can not set an infoportion [%s] for actor, because "
                                "DataBase::Storage::getInstane().getActor() == nullptr!",
                                it_infoportion_set.second.m_infopotion_name.c_str());
                    }
                }
                else if (!it_infoportion_set.second.m_required)
                {
                    if (Globals::has_alife_info(it_infoportion_set.second.m_infopotion_name.c_str()))
                    {
                        CScriptGameObject* client_actor = DataBase::Storage::getInstance().getActor();
                        if (client_actor)
                            client_actor->DisableInfoPortion(it_infoportion_set.second.m_infopotion_name.c_str());
                        else
                            Msg("[XR_LOGIC/pick_section_from_condlist(server_actor, server_npc, condlist)] WARNING: "
                                "Can not set an infoportion [%s] for actor, because "
                                "DataBase::Storage::getInstane().getActor() == nullptr!",
                                it_infoportion_set.second.m_infopotion_name.c_str());
                    }
                }
            }

            if (it.second.m_text_name == XR_LOGIC_TEXT_NEVER)
            {
                return xr_string("");
            }
            else
            {
                return it.second.m_text_name;
            }
        }
    }

    return xr_string("");
}

inline void pstor_load_all(CScriptGameObject* client_object, NET_Packet& packet)
{
    const std::uint16_t& npc_id = client_object->ID();
    std::uint32_t total_count = packet.r_u32();

    for (std::uint32_t i = 0; i < total_count; ++i)
    {
        xr_string varname;
        packet.r_stringZ(varname);

        std::uint8_t pstor_type = packet.r_u8();
        if (pstor_type == Globals::kPstorTypeNumber)
        {
            DataBase::Storage::getInstance().setPStorNumber(
                npc_id, varname, static_cast<std::uint8_t>(packet.r_float()));
        }
        else if (pstor_type == Globals::kPstorTypeString)
        {
            xr_string result;
            packet.r_stringZ(result);
            DataBase::Storage::getInstance().setPStorString(npc_id, varname, result);
        }
        else if (pstor_type == Globals::kPstorTypeBoolean)
        {
            DataBase::Storage::getInstance().setPStorBool(npc_id, varname, packet.r_u8() ? true : false);
        }
        else
        {
            R_ASSERT2(false, "can't reached!");
            return;
        }
    }
}

inline bool pstor_retrieve_bool(CScriptGameObject* object, const xr_string& varname)
{
    if (!object)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!varname.size())
    {
        R_ASSERT2(false, "can't be an empty string!");
        return false;
    }

    if (DataBase::Storage::getInstance().getStorage().at(object->ID()).getPStor().at(varname).IsInitializedBool())
    {
        return DataBase::Storage::getInstance().getStorage().at(object->ID()).getPStor().at(varname).getBool();
    }

    Msg("[Scripts/XR_LOGIC/pstor_retrieve_bool(object, varname)] WARNING: returns a default value, becasue can't find "
        "a value by string index -> [%s]",
        varname.c_str());

    return false;
}

inline std::uint8_t pstor_retrieve_number(CScriptGameObject* object, const xr_string& varname)
{
    if (!object)
    {
        R_ASSERT2(false, "object was null!");
        return std::uint8_t(0);
    }

    if (!varname.size())
    {
        R_ASSERT2(false, "can't be an empty string!");
        return std::uint8_t(0);
    }

    if (DataBase::Storage::getInstance().getStorage().at(object->ID()).getPStor().at(varname).IsInitializedNumber())
    {
        return DataBase::Storage::getInstance().getStorage().at(object->ID()).getPStor().at(varname).getNumber();
    }

    Msg("[Scripts/XR_LOGIC/pstor_retrieve_number(object, varname)] WARNING: returning a default value, because can't "
        "find current value by string index -> [%s]",
        varname.c_str());

    return std::uint8_t(0);
}

inline xr_string pstor_retrieve_string(CScriptGameObject* object, const xr_string& varname)
{
    if (!object)
    {
        R_ASSERT2(false, "object was null!");
        return xr_string();
    }

    if (!varname.size())
    {
        R_ASSERT2(false, "can't be an empty string!");
        return xr_string();
    }

    if (DataBase::Storage::getInstance().getStorage().at(object->ID()).getPStor().at(varname).IsInitializedString())
    {
        return DataBase::Storage::getInstance().getStorage().at(object->ID()).getPStor().at(varname).getString();
    }

    Msg("[Scripts/XR_LOGIC/pstor_retrieve_string(object, varname)] WARNING: return a default value, because can't find "
        "a value by string index -> [%s]",
        varname.c_str());

    return xr_string();
}

inline void pstor_store(CScriptGameObject* object, const xr_string& varname, const bool value)
{
    if (!object)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    if (!varname.size())
    {
        R_ASSERT2(false, "can't be an empty!");
        return;
    }

    DataBase::Storage::getInstance().setPStorBool(object->ID(), varname, value);
}

inline void pstor_store(CScriptGameObject* object, const xr_string& varname, const std::uint8_t value)
{
    if (!object)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    if (!varname.size())
    {
        R_ASSERT2(false, "can't be an empty string!");
        return;
    }

    DataBase::Storage::getInstance().setPStorNumber(object->ID(), varname, value);
}

inline void pstor_store(CScriptGameObject* object, const xr_string& varname, const xr_string& value)
{
    if (!object)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    if (!varname.size())
    {
        R_ASSERT2(false, "can't be an empty string!");
        return;
    }

    DataBase::Storage::getInstance().setPStorString(object->ID(), varname, value);
}

inline void load_object(CScriptGameObject* client_object, NET_Packet& packet)
{
    if (!client_object)
    {
        R_ASSERT2(false, "can't load an empty object!");
        return;
    }
    const std::uint16_t& npc_id = client_object->ID();
    xr_string load_marker_name = "object";
    load_marker_name += client_object->Name();
    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Load, false, load_marker_name.c_str());

    xr_string job_ini_name;
    packet.r_stringZ(job_ini_name);

    xr_string ini_filename;
    packet.r_stringZ(ini_filename);

    xr_string section_logic_name;
    packet.r_stringZ(section_logic_name);

    xr_string active_section_name;
    packet.r_stringZ(active_section_name);

    if (active_section_name.empty() || active_section_name == "")
        active_section_name = "nil";

    xr_string gulag_name;
    packet.r_stringZ(gulag_name);

    DataBase::Storage::getInstance().setStorageJobIniName(npc_id, job_ini_name);
    DataBase::Storage::getInstance().setStorageLoadedInifilename(npc_id, ini_filename);
    DataBase::Storage::getInstance().setStorageLoadedSectionLogicName(npc_id, section_logic_name);
    DataBase::Storage::getInstance().setStorageLoadedActiveSectionName(npc_id, active_section_name);
    DataBase::Storage::getInstance().setStorageLoadedGulagName(npc_id, gulag_name);

    DataBase::Storage::getInstance().setStorageActivationTime(npc_id, packet.r_s32() + Globals::get_time_global());
    DataBase::Storage::getInstance().setStorageActivationGameTime(npc_id, Globals::Utils::r_CTime(packet));

    pstor_load_all(client_object, packet);

    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Load, true, load_marker_name.c_str());
}

inline void save_object(CScriptGameObject* client_object, NET_Packet& packet)
{
    if (!client_object)
    {
        R_ASSERT2(false, "can't use an empty object here!");
        return;
    }

    xr_string save_marker_name = "object";
    save_marker_name += client_object->Name();
    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, false, save_marker_name.c_str());

    const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(client_object->ID());

    packet.w_stringZ(storage.getJobIniName().c_str());
    packet.w_stringZ(storage.getInifilename().c_str());
    packet.w_stringZ(storage.getSectionLogicName().c_str());
    packet.w_stringZ(storage.getActiveSectionName().c_str());
    packet.w_stringZ(storage.getGulagName().c_str());

    std::int32_t activation_time = storage.getActivationTime();

    if (!activation_time)
        activation_time = 0;

    packet.w_s32(activation_time - Globals::get_time_global());

    Globals::Utils::w_CTime(packet, const_cast<xrTime&>(storage.getActivationGameTime()));

    if (!storage.getActiveSchemeName().empty())
    {
        // Lord: доделать
    }

    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, true, save_marker_name.c_str());
}

inline void mob_capture(CScriptGameObject* client_mob, const bool is_reset_actions, const xr_string& scheme_name)
{
    if (!client_mob)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    if (is_reset_actions)
    {
        Globals::reset_action(client_mob, scheme_name);
    }
    else
    {
        if (!client_mob->GetScriptControl())
        {
            client_mob->SetScriptControl(true, scheme_name.c_str());
        }
    }
}

inline bool is_mob_captured(CScriptGameObject* p_client_object)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return false;
    }

    return p_client_object->GetScriptControl();
}

inline void assign_storage_and_bind(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
{
    DataBase::Storage_Scheme storage;
    if (DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().find(scheme_name) ==
        DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().end())

    {
        storage.setClientObject(p_client_object);
        DataBase::Storage::getInstance().setStorageScheme(p_client_object->ID(), scheme_name, storage);
    }
}

inline void mob_release(CScriptGameObject* const p_client_object, const xr_string& scheme_name)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    if (scheme_name.empty())
    {
        R_ASSERT2(false, "string can't be empty here");
        return;
    }

    if (p_client_object->GetScriptControl())
    {
        p_client_object->SetScriptControl(false, scheme_name.c_str());
    }
}

inline LogicData cfg_get_condlist(CScriptIniFile* const p_ini, const xr_string& section_name,
    const xr_string& field_name, CScriptGameObject* const p_npc)
{
    LogicData result;
    if (!p_ini)
    {
        R_ASSERT2(false, "object is null!");
        return result;
    }

    xr_string data_name = Globals::Utils::cfg_get_string(p_ini, section_name, field_name);
    if (data_name.empty())
        return result;

    xr_vector<xr_string> params = Globals::Utils::parse_params(data_name);
    if (params.empty())
    {
        R_ASSERT2(false, "can't be mepty!");
        return result;
    }
    
    result.setFieldName(field_name);
    result.setCondlist(parse_condlist_by_script_object(section_name, field_name, params[0]));

    return result;
}

inline void try_switch_to_another_section(
    CScriptGameObject* p_client_object, DataBase::Storage_Scheme& storage, CScriptGameObject* p_client_actor)
{
    if (!p_client_actor)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    const xr_string& logic_name = storage.getLogicName();
    std::uint16_t npc_id = p_client_object->ID();

    if (logic_name.empty())
    {
        R_ASSERT2(false, "can't find script swtiching information in storage");
        return;
    }

    bool is_switched = false;
}

} // namespace XR_LOGIC
} // namespace Scripts
} // namespace Cordis
