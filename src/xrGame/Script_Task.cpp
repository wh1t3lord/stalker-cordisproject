#include "stdafx.h"
#include "Script_task.h"

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

std::uint16_t surge_task_target(const xr_string& id_name, const xr_string& field_name, const xr_string& value_name)
{
    return Script_SurgeManager::get_task_target();
}

std::uint16_t get_guider(const xr_string& level_name)
{
    xr_string current_name = Globals::Game::level::get_name();

    if (!Script_GlobalHelper::getInstance().getTaskGuidersByLevel().at(current_name).empty() &&
        !Script_GlobalHelper::getInstance().getTaskGuidersByLevel().at(current_name).at(level_name).empty())
        return Globals::get_story_object_id(
            Script_GlobalHelper::getInstance().getTaskGuidersByLevel().at(current_name).at(level_name));

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
    : m_p_ini(p_ini), m_task_id_name(task_id_name), m_status_name("normal"), m_current_target_id(0)
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
        title_function_name = "task_functor_condlist";

    this->m_title_functor_name = title_function_name;

    xr_string description_name = Globals::Utils::cfg_get_string(this->m_p_ini, task_id_name, "descr");
    if (description_name.empty())
        description_name = "DESCR_DOESNT_EXIST";

    this->m_description_name = description_name;

    xr_string description_functor_name = Globals::Utils::cfg_get_string(this->m_p_ini, task_id_name, "descr_functor");

    if (description_functor_name.empty())
        description_functor_name = "task_functor_condlist";

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

    if (task_functors_name.empty())
    {
        task_functors_name["task_functor_condlist"] = task_functor_condlist;
        task_functors_name["task_functor_zat_b29_adv_title"] = task_functor_zat_b29_adv_title;
        task_functors_name["zat_b29_adv_descr"] = zat_b29_adv_descr;
    }

    if (task_functors_id.empty())
    {
        task_functors_id["target_condlist"] = target_condlist;
        task_functors_id["zat_b29_adv_target"] = zat_b29_adv_target;
        task_functors_id["surge_task_target"] = surge_task_target;
    }

    this->m_current_title_name = task_functors_name.at(this->m_title_functor_name)
                                     .
                                     operator()<const xr_string&, const xr_string&, const xr_string&>(
                                         this->m_task_id_name, "title", this->m_title_name);
    this->m_current_description_name = task_functors_name.at(this->m_description_functor_name)
                                           .
                                           operator()<const xr_string&, const xr_string&, const xr_string&>(
                                               this->m_task_id_name, "descr", this->m_description_name);
    std::uint32_t time = 0;

    if (this->m_wait_time)
        time = this->m_wait_time;

    if (this->m_is_storyline)
    {
        if (time == 0)
        {
            this->m_spot_name = "storyline_task_location";
        }
        else
        {
            this->m_spot_name = "storyline_task_location_complex_timer";
        }
    }
    else
    {
        if (time == 0)
        {
            this->m_spot_name = "secondary_task_location";
        }
        else
        {
            this->m_spot_name = "secondary_task_location_complex_timer";
        }
    }

    this->m_current_target_id = task_functors_id.at(this->m_target_functor_name)
                                    .
                                    operator()<const xr_string&, const xr_string&, const xr_string&>(
                                        this->m_task_id_name, "target", this->m_target_name);

    this->m_is_send_update_news =
        Globals::Utils::cfg_get_bool(this->m_p_ini, this->m_task_id_name, "dont_send_update_news");
}

Script_Task::~Script_Task(void)
{
    /* Будет ли удалять сам GameTask.cpp:313, если да то удалить комментарий
        if (this->m_task)
        {
            xr_delete(this->m_task);
        }*/
}

void Script_Task::give_task(void)
{
    // Lord: проверить деаллокацию!
    CGameTask* task = new CGameTask();
    task->SetID_script(this->m_task_id_name.c_str());

    if (this->m_is_storyline)
        task->SetType_script(eTaskTypeStoryline);
    else
        task->SetType_script(eTaskTypeAdditional);

    task->SetTitle_script(this->m_current_title_name.c_str());
    task->SetDescription_script(this->m_current_description_name.c_str());
    task->SetPriority_script(this->m_priority);
    task->SetIconName_script(this->m_icon_name.c_str());
    // Lord: подумать как исправить
    task->AddCompleteFunc_script("task_complete");
    task->AddFailFunc_script("task_fail");

    XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(),
        DataBase::Storage::getInstance().getActor(), this->m_on_init_condlist);

    if (this->m_current_target_id)
    {
        task->SetMapLocation_script(this->m_spot_name.c_str());
        task->SetMapObjectID_script(this->m_current_target_id);

        if (this->m_is_storyline)
            Globals::Game::level::map_add_object_spot(this->m_current_target_id, "ui_storyline_task_blink", "");
        else
            Globals::Game::level::map_add_object_spot(this->m_current_target_id, "ui_secondary_task_blink", "");
    }

    std::uint32_t time = 0;

    if (this->m_wait_time)
        time = this->m_wait_time;

    this->m_status_name = "selected";
    this->m_initied_time = Globals::Game::get_game_time();

    DataBase::Storage::getInstance().getActor()->GiveTaskToActor(task, time * 10, false, time);
    this->m_task = task;
}

void Script_Task::check_task(void)
{
    std::uint32_t global_time = Globals::get_time_global();
    bool is_task_updated = false;

    if (this->m_check_time && !this->m_last_check_task_name.empty() && (global_time - this->m_check_time <= 50))
        return;

    if (!this->m_task)
    {
        if (DataBase::Storage::getInstance().getActor())
            this->m_task = DataBase::Storage::getInstance().getActor()->GetTask(this->m_task_id_name.c_str(), true);

        return;
    }

    this->m_check_time = global_time;

    xr_string title_name = task_functors_name.at(this->m_title_functor_name)
                               .
                               operator()<const xr_string&, const xr_string&, const xr_string&>(
                                   this->m_task_id_name, "title", this->m_title_name);

    if (this->m_current_title_name != title_name)
    {
        Msg("[Scripts/Script_Task/check_task()] updated due to title change from %s to %s",
            this->m_current_title_name.c_str(), title_name.c_str());
        is_task_updated = true;
        this->m_current_title_name = title_name;
        this->m_task->SetTitle_script(Globals::Game::translate_string(title_name.c_str()));
    }

    xr_string description_name = task_functors_name.at(this->m_description_functor_name)
                                     .
                                     operator()<const xr_string&, const xr_string&, const xr_string&>(
                                         this->m_task_id_name, "descr", this->m_description_name);

    if (this->m_current_description_name != description_name)
    {
        Msg("[Scripts/Script_Task/check_task()] updated due to description change from %s to %s",
            this->m_current_description_name.c_str(), description_name.c_str());
        is_task_updated = true;
        this->m_current_description_name = description_name;
        this->m_task->SetDescription_script(Globals::Game::translate_string(description_name.c_str()));
    }

    std::uint16_t target_id = task_functors_id.at(this->m_target_functor_name)
                                  .
                                  operator()<const xr_string&, const xr_string&, const xr_string&>(
                                      this->m_task_id_name, "target", this->m_target_name);
    this->check_level(target_id);

    if (this->m_current_target_id != target_id)
    {
        Msg("[Scripts/Script_Task/check_task()] udated due to target change from %d to %d", this->m_current_target_id,
            target_id);

        if (!this->m_current_target_id)
        {
            is_task_updated = true;
            this->m_task->ChangeMapLocation(this->m_spot_name.c_str(), target_id);
            if (this->m_is_storyline)
                Globals::Game::level::map_add_object_spot(target_id, "ui_storyline_task_blink", "");
            else
                Globals::Game::level::map_add_object_spot(target_id, "ui_secondary_task_blink", "");
        }
        else
        {
            if (!target_id)
            {
                this->m_task->RemoveMapLocations(false);
                is_task_updated = true;
            }
            else
            {
                if (this->m_is_storyline)
                    Globals::Game::level::map_add_object_spot(target_id, "ui_storyline_task_blink", "");
                else
                    Globals::Game::level::map_add_object_spot(target_id, "ui_secondary_task_blink", "");

                this->m_task->ChangeMapLocation(this->m_spot_name.c_str(), target_id);
                is_task_updated = true;
            }
        }

        this->m_current_target_id = target_id;
    }

    if (is_task_updated && !(this->m_is_send_update_news))
        Script_NewsManager::getInstance().SendTask(
            DataBase::Storage::getInstance().getActor(), "updated", this->m_task);

    for (const xr_map<std::uint32_t, CondlistData>& it : this->m_condlist)
    {
        xr_string picked_section_name = XR_LOGIC::pick_section_from_condlist(
            DataBase::Storage::getInstance().getActor(), DataBase::Storage::getInstance().getActor(), it);

        if (!picked_section_name.empty())
        {
            if (Script_GlobalHelper::getInstance().getTaskValidValues().find(picked_section_name) ==
                Script_GlobalHelper::getInstance().getTaskValidValues().end())
            {
                R_ASSERT2(false, "undefined keyword!");
            }

            this->m_last_check_task_name = picked_section_name;
            return;
        }
    }
}

void Script_Task::give_reward(void)
{
    XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(),
        DataBase::Storage::getInstance().getActor(), this->m_on_complete_condlist);

    xr_string money_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(),
        DataBase::Storage::getInstance().getActor(), this->m_on_reward_money);
    xr_string items_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(),
        DataBase::Storage::getInstance().getActor(), this->m_on_reward_item);

    CScriptGameObject* const p_npc = DataBase::Storage::getInstance().getInvetoryUpgradesVictim();

    if (!money_name.empty())
        relocate_money(p_npc, boost::lexical_cast<int>(money_name), "in");

    if (!items_name.empty())
    {
        xr_map<xr_string, std::uint32_t> ancillary_item_table;

        for (const xr_string& it : Globals::Utils::parse_names(items_name))
        {
            if (ancillary_item_table.find(it) == ancillary_item_table.end())
            {
                ancillary_item_table[it] = 1;
            }
            else
            {
                ancillary_item_table[it] = ancillary_item_table[it] + 1;
            }
        }

        for (const std::pair<xr_string, std::uint32_t>& it : ancillary_item_table)
        {
            relocate_item_section(p_npc, it.first, "in", it.second);
        }
    }
}

void Script_Task::deactivate_task(CGameTask* task)
{
    Msg("[Scripts/Script_Task/deactivate_task(task)] deactivate task %s", this->m_task_id_name.c_str());

    this->m_check_time = 0;

    if (this->m_last_check_task_name == "fail")
    {
        Script_NewsManager::getInstance().SendTask(DataBase::Storage::getInstance().getActor(), "fail", task);
    }
    else if (this->m_last_check_task_name == "reversed")
    {
        XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(),
            DataBase::Storage::getInstance().getActor(), this->m_on_reversed_condlist);
        Script_NewsManager::getInstance().SendTask(DataBase::Storage::getInstance().getActor(), "reversed", task);
    }

    this->m_last_check_task_name.clear();

    this->m_status_name = "normal";
}

void Script_Task::check_level(const std::uint16_t target_id)
{
    if (!DataBase::Storage::getInstance().getActor()->IsActiveTask(this->m_task) || !(target_id))
        return;

    // LorD: подумать над обработкой или сделать функцию которая определяет существует ли левел или нет!!!
    if (Globals::Game::level::get_name().empty())
        return;

    CSE_Abstract* const p_server_object = ai().alife().objects().object(target_id);

    if (p_server_object)
    {
        xr_string target_level_name = Globals::get_level_name(Globals::Game::get_game_graph()->vertex(p_server_object->cast_alife_dynamic_object()->m_tGraphID)->level_id());
        xr_string level_name = Globals::Game::level::get_name();

        if (target_level_name != level_name)
        {
            std::uint16_t guider_id = get_guider(target_level_name);
            if (!guider_id)
            {
                Msg("[Scripts/Script_Task/check_level(target_id)] WARNING: guider_id == 0! Return ...");
                return;
            }

            xr_string guider_spot_name;
            xr_string guider_spot2_name;

            if (this->m_is_storyline)
            {
                guider_spot_name = "storyline_task_on_guider";
                guider_spot2_name = "secondary_task_on_guider";
            }
            else
            {
                guider_spot_name = "secondary_task_on_guider";
                guider_spot2_name = "storyline_task_on_guider";
            }

            if (Globals::Game::level::map_has_object_spot(guider_id, guider_spot2_name.c_str()) != 0)
                Globals::Game::level::map_remove_object_spot(guider_id, guider_spot2_name.c_str());

            if (guider_id && Globals::Game::level::map_has_object_spot(guider_id, guider_spot_name.c_str()) != 0)
                Globals::Game::level::map_add_object_spot(guider_id, guider_spot_name.c_str(), "");
        }
        else
        {
            this->remove_guider_spot();
        }
    }
}

void Script_Task::remove_guider_spot(void)
{
    if (Script_GlobalHelper::getInstance().getTaskGuidersByLevel().find(Globals::Game::level::get_name()) ==
        Script_GlobalHelper::getInstance().getTaskGuidersByLevel().end())
        return;

    for (const std::pair<xr_string, xr_string>& it :
        Script_GlobalHelper::getInstance().getTaskGuidersByLevel().at(Globals::Game::level::get_name()))
    {
        std::uint16_t guider_id = Globals::get_story_object_id(it.second);

        if (guider_id)
        {
            if (Globals::Game::level::map_has_object_spot(guider_id, "storyline_task_on_guider") != 0)
            {
                Globals::Game::level::map_remove_object_spot(guider_id, "storyline_task_on_guider");
            }

            if (Globals::Game::level::map_has_object_spot(guider_id, "secondary_task_on_guider") != 0)
            {
                Globals::Game::level::map_remove_object_spot(guider_id, "secondary_task_on_guider");
            }
        }
    }
}

void Script_Task::save(NET_Packet& packet)
{
    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, false, "Script_Task");
    packet.w_u8(Script_GlobalHelper::getInstance().getTaskIDByStatus().at(this->m_status_name));
    Globals::Utils::w_CTime(packet, this->m_initied_time);
    packet.w_stringZ(this->m_current_title_name.c_str());
    packet.w_stringZ(this->m_current_description_name.c_str());
    packet.w_stringZ(std::to_string(this->m_current_target_id).c_str());
    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, true, "Script_Task");
}

void Script_Task::load(IReader& packet)
{
    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Load, false, "Script_Task");
    this->m_status_name = Script_GlobalHelper::getInstance().getTaskStatusByID().at(packet.r_u8());
    this->m_initied_time = Globals::Utils::r_CTime(packet);
    packet.r_stringZ(this->m_current_title_name);
    packet.r_stringZ(this->m_current_description_name);

    xr_string id_name;
    packet.r_stringZ(id_name);

    if (id_name.empty() || id_name == "nil") // LorD: проверить будет ли дропать nil, если будет то найти и исправить когда это будет, чтобы все "nil" просто проверялись всегда как .empty()
        this->m_current_target_id = 0;
    else
        this->m_current_target_id = boost::lexical_cast<std::uint16_t>(id_name);
   
    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Load, true, "Script_Task");
}

} // namespace Scripts
} // namespace Cordis
