#include "stdafx.h"
#include "Script_Task.h"

namespace Cordis
{
namespace Scripts
{
xr_string task_functor_condlist(const xr_string& task_id_name, const xr_string& field_name, const xr_string& value_name)
{
    return XR_LOGIC::pick_section_from_condlist(Globals::get_story_object("actor"), (CScriptGameObject*)(nullptr),
        XR_LOGIC::parse_condlist_by_script_object("task", "task_condlist", value_name));
}

xr_string task_functor_zat_b29_adv_title(
    const xr_string& task_id_name, const xr_string& field_name, const xr_string& value_name)
{
    CScriptGameObject* p_client_actor = Globals::get_story_object("actor");
    xr_string generate_string = "zat_b29_simple_bring_title_";
    xr_string result = "zat_b29_simple_bring_title_";
    for (std::uint8_t i = 16; i <= 23; ++i)
    {
        if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopBringTable().at(i).c_str()) &&
            p_client_actor->GetObjectByName(Script_GlobalHelper::getInstance().getZatB29AfTable().at(i).c_str()))
        {
            result += std::to_string(i).c_str();
            break;
        }
        else if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopBringTable().at(i).c_str()))
        {
            result = "zat_b29_simple_find_title_";
            result += std::to_string(i).c_str();
            break;
        }
    }

    return result;
}

xr_string zat_b29_adv_descr(const xr_string& task_id_name, const xr_string& field_name, const xr_string& value_name)
{
    xr_string result_name;
    CScriptGameObject* const p_actor = Globals::get_story_object("actor");
    bool is_af = false;
    for (std::uint8_t i = 16; i <= 23; ++i)
    {
        if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopBringTable().at(i).c_str()) &&
            p_actor->GetObjectByName(Script_GlobalHelper::getInstance().getZatB29AfTable().at(i).c_str()))
        {
            is_af = true;
            result_name = "zat_b29_simple_bring_text_5";
            if (Globals::has_alife_info("zat_b29_stalker_rival_1_found_af") &&
                Globals::has_alife_info("zat_b29_first_rival_taken_out") && is_af)
            {
                return result_name;
            }
            else if (Globals::has_alife_info("zat_b29_stalker_rival_2_found_af") &&
                Globals::has_alife_info("zat_b29_second_rival_taken_out") && is_af)
            {
                return result_name;
            }
            else if (Globals::has_alife_info("zat_b29_linker_take_af_from_rival"))
            {
                result_name = "zat_b29_simple_bring_text_4";
            }
            else if (Globals::has_alife_info("zat_b29_stalkers_rivals_found_af"))
            {
                result_name = "zat_b29_simple_bring_text_3";
            }
            else if (Globals::has_alife_info("zat_b29_rivals_search") &&
                Globals::has_alife_info("zat_b29_exclusive_conditions"))
            {
                result_name = "zat_b29_simple_bring_text_1";
            }
            else if (Globals::has_alife_info("zat_b29_rivals_search"))
            {
                result_name = "zat_b29_simple_bring_text_2";
            }

            break;
        }
        else if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopBringTable().at(i).c_str()))
        {
            result_name = "zat_b29_simple_find_text_5";

            if (Globals::has_alife_info("zat_b29_stalker_rival_1_found_af") &&
                Globals::has_alife_info("zat_b29_first_rival_taken_out") && is_af)
            {
                return result_name;
            }
            else if (Globals::has_alife_info("zat_b29_stalker_rival_2_found_af") &&
                Globals::has_alife_info("zat_b29_second_rival_taken_out") && is_af)
            {
                return result_name;
            }
            else if (Globals::has_alife_info("zat_b29_linker_take_af_from_rival"))
            {
                result_name = "zat_b29_simple_find_text_4";
            }
            else if (Globals::has_alife_info("zat_b29_stalkers_rivals_found_af"))
            {
                result_name = "zat_b29_simple_find_text_3";
            }
            else if (Globals::has_alife_info("zat_b29_rivals_search") &&
                Globals::has_alife_info("zat_b29_exclusive_conditions"))
            {
                result_name = "zat_b29_simple_find_text_1";
            }
            else if (Globals::has_alife_info("zat_b29_rivals_search"))
            {
                result_name = "zat_b29_simple_find_text_2";
            }

            break;
        }
    }

    return result_name;
}

std::uint16_t target_condlist(const xr_string& id_name, const xr_string& field_name, const xr_string& value_name)
{
    if (value_name.empty())
    {
        Msg("[Scripts/target_condlist(id_name, field_name, value_name)] WARNING: value_name.empty() == true! Return "
            "...");
        return 0;
    }

    xr_map<std::uint32_t, CondlistData> parse_condlist =
        XR_LOGIC::parse_condlist_by_script_object("task", "task_condlist", value_name);

    xr_string picked_section_name = XR_LOGIC::pick_section_from_condlist(
        Globals::get_story_object("actor"), static_cast<CScriptGameObject*>(nullptr), parse_condlist);

    if (picked_section_name.empty())
    {
        Msg("[Scripts/target_condlist(id_name, field_name, value_name)] WARNING: picked_section_name.empty() == true! "
            "Return ...");
        return 0;
    }

    std::uint16_t target_object_id = Globals::get_story_object_id(picked_section_name);

    if (!target_object_id)
    {
        Msg("[Scripts/target_condlist(id_name, field_name, value_name)] WARNING: target_object_id == 0! Return ...");
        return 0;
    }

    return target_object_id;
}

std::uint16_t zat_b29_adv_target(const xr_string& id_name, const xr_string& field_name, const xr_string& value_name)
{
    xr_string target_object_id_name = "zat_a2_stalker_barmen";
    CScriptGameObject* const p_actor = Globals::get_story_object("actor");
    xr_string artefact_name;

    for (std::uint8_t i = 16; i <= 23; ++i)
    {
        if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopBringTable().at(i).c_str()) &&
            p_actor->GetObjectByName(Script_GlobalHelper::getInstance().getZatB29AfTable().at(i).c_str()))
        {
            artefact_name = Script_GlobalHelper::getInstance().getZatB29AfTable().at(i);
            break;
        }
    }

    if (!Globals::has_alife_info("zat_b29_linker_take_af_from_rival") &&
        Globals::has_alife_info("zat_b29_stalkers_rivals_found_af"))
    {
        if (Globals::has_alife_info("zat_b29_stalker_rival_1_found_af"))
        {
            if (!Globals::has_alife_info("zat_b29_first_rival_taken_out"))
            {
                if (Globals::has_alife_info("zat_b29_exclusive_conditions"))
                {
                    target_object_id_name = "zat_b29_stalker_rival_1";
                }
                else
                {
                    target_object_id_name = "zat_b29_stalker_rival_default_1";
                }
            }
            else if (artefact_name.empty())
            {
                if (Globals::has_alife_info("zat_b29_exclusive_conditions"))
                {
                    target_object_id_name = "zat_b29_stalker_rival_1";
                }
                else
                {
                    target_object_id_name = "zat_b29_stalker_rival_default_1";
                }
            }
        }
        else if (Globals::has_alife_info("zat_b29_stalker_rival_2_found_af"))
        {
            if (!Globals::has_alife_info("zat_b29_second_rival_taken_out"))
            {
                if (Globals::has_alife_info("zat_b29_exclusive_conditions"))
                {
                    target_object_id_name = "zat_b29_stalker_rival_2";
                }
                else
                {
                    target_object_id_name = "zat_b29_stalker_rival_default_2";
                }
            }
            else if (artefact_name.empty())
            {
                if (Globals::has_alife_info("zat_b29_exclusive_conditions"))
                {
                    target_object_id_name = "zat_b29_stalker_rival_2";
                }
                else
                {
                    target_object_id_name = "zat_b29_stalker_rival_default_2";
                }
            }
        }

        return Globals::get_story_object_id(target_object_id_name);
    }

    if (!artefact_name.empty())
        return Globals::get_story_object_id(target_object_id_name);
    
    Msg("[Scripts/zat_b29_adv_target(id_name, field_name, value_name)] WARNING: return 0");
    return 0;
}

} // namespace Scripts
} // namespace Cordis

xr_map<xr_string, Cordis::AnyCallable<std::uint16_t>> task_functors_id;
xr_map<xr_string, Cordis::AnyCallable<xr_string>> task_functors_name;

namespace Cordis
{
namespace Scripts
{
Script_Task::Script_Task(CScriptIniFile* p_ini, const xr_string& task_id_name)
    : m_p_ini(p_ini), m_task_id_name(task_id_name), m_status_name("normal"), m_current_title_id(0),
      m_current_target_id(0), m_current_description_id(0)
{
    if (!this->m_p_ini)
    {
        R_ASSERT2(false, "it can't be, can't initialize this class!!!!!!");
    }

    xr_string title_name = Globals::Utils::cfg_get_string(this->m_p_ini, task_id_name, "title");
    if (title_name.empty())
        title_name = "TITLE_DOESNT_EXIST";

    this->m_title_name = title_name;

    xr_string title_function_name = Globals::Utils::cfg_get_string(this->m_p_ini, task_id_name, "title_functor");
    if (title_function_name.empty())
        title_function_name = "condlist";

    this->m_title_functor_name = title_function_name;

    xr_string description_name = Globals::Utils::cfg_get_string(this->m_p_ini, task_id_name, "descr");
    if (description_name.empty())
        description_name = "DESCR_DOESNT_EXIST";

    this->m_description_name = description_name;

    xr_string description_functor_name = Globals::Utils::cfg_get_string(this->m_p_ini, task_id_name, "descr_functor");

    if (description_functor_name.empty())
        description_functor_name = "condlist";

    this->m_description_functor_name = description_functor_name;

    xr_string target_name = Globals::Utils::cfg_get_string(this->m_p_ini, task_id_name, "target");

    if (target_name.empty())
        target_name = "DESCR_DOESNT_EXIST";

    this->m_target_name = target_name;

    xr_string target_functor_name = Globals::Utils::cfg_get_string(this->m_p_ini, task_id_name, "target_functor");

    if (target_functor_name.empty())
        target_functor_name = "target_condlist";

    this->m_target_functor_name = target_functor_name;

    xr_string icon_name = Globals::Utils::cfg_get_string(this->m_p_ini, task_id_name, "icon");
    if (icon_name.empty())
        icon_name = "ui_pda2_mtask_overlay";

    this->m_icon_name = icon_name;

    this->m_priority = static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(this->m_p_ini, task_id_name, "prior"));
    this->m_is_storyline = Globals::Utils::cfg_get_bool(this->m_p_ini, task_id_name, "storyline");
    std::uint32_t i = 0;
    xr_string generate_section = "condlist_";
    while (this->m_p_ini->line_exist(
        task_id_name.c_str(), xr_string(generate_section).append(std::to_string(i).c_str()).c_str()))
    {
        this->m_condlist.push_back(XR_LOGIC::parse_condlist_by_script_object("task_manager", "condlist",
            this->m_p_ini->r_string(
                task_id_name.c_str(), xr_string(generate_section).append(std::to_string(i).c_str()).c_str())));
        ++i;
    }

    this->m_on_init_condlist = XR_LOGIC::parse_condlist_by_script_object(
        "task_manager", "condlist", Globals::Utils::cfg_get_string(this->m_p_ini, task_id_name, "on_init"));
    this->m_on_complete_condlist = XR_LOGIC::parse_condlist_by_script_object(
        "task_manager", "condlist", Globals::Utils::cfg_get_string(this->m_p_ini, task_id_name, "on_complete"));
    this->m_on_reversed_condlist = XR_LOGIC::parse_condlist_by_script_object(
        "task_manager", "condlist", Globals::Utils::cfg_get_string(this->m_p_ini, task_id_name, "on_reversed"));
    this->m_on_reward_money = XR_LOGIC::parse_condlist_by_script_object(
        "task_manager", "condlist", Globals::Utils::cfg_get_string(this->m_p_ini, task_id_name, "reward_money"));
    this->m_on_reward_item = XR_LOGIC::parse_condlist_by_script_object(
        "task_manager", "condlist", Globals::Utils::cfg_get_string(this->m_p_ini, task_id_name, "reward_item"));
    this->m_community_relation_delta_fail = static_cast<std::uint32_t>(
        Globals::Utils::cfg_get_number(this->m_p_ini, task_id_name, "community_relation_delta_fail"));
    this->m_community_relation_delta_complete = static_cast<std::uint32_t>(
        Globals::Utils::cfg_get_number(this->m_p_ini, task_id_name, "community_relation_delta_complete"));
}

Script_Task::~Script_Task(void) {}

} // namespace Scripts
} // namespace Cordis
