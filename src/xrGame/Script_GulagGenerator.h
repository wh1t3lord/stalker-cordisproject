#pragma once

#include "Script_GlobalHelper.h"
#include "Script_XR_Logic.h"
#include "Script_XR_Gulag.h"
#include "Script_XR_Condition.h"
#include "Script_SE_SmartTerrain.h"

namespace Cordis
{
namespace Scripts
{
class Script_SE_SmartTerrain;

namespace GulagGenerator
{
inline static xr_string& getLtx(void) noexcept
{
    static xr_string instance;
    return instance;
}

inline std::pair<xr_vector<JobData>, xr_vector<JobDataExclusive>> load_job(Script_SE_SmartTerrain* smart)
{
    if (!smart)
    {
        R_ASSERT2(false, "object was null!");
        return std::pair<xr_vector<JobData>, xr_vector<JobDataExclusive>>();
    }

    Msg("[Scripts/GulagGenerator/load_job(smart)] LOAD JOB %s", smart->name_replace());
    xr_string global_name = smart->name_replace();
    getLtx().clear();
    getLtx() += "[meet@generic_lager]\n";
    getLtx() += "close_distance 		= {=is_wounded} 0, 2\n";
    getLtx() +=
        "close_anim 			= {=is_wounded} nil, {!is_squad_commander} nil, {=actor_has_weapon} "
        "threat_na, talk_default\n";
    getLtx() +=
        "close_snd_hello 		= {=is_wounded} nil, {!is_squad_commander} nil, {=actor_enemy} nil, "
        "{=actor_has_weapon} meet_hide_weapon, meet_hello\n";
    getLtx() +=
        "close_snd_bye 			= {=is_wounded} nil, {!is_squad_commander} nil, {=actor_enemy} nil, "
        "{=actor_has_weapon} nil, meet_hello\n";
    getLtx() += "close_victim 			= {=is_wounded} nil, {!is_squad_commander} nil, actor\n";
    getLtx() += "far_distance 			= 0\n";
    getLtx() += "far_anim 				= nil\n";
    getLtx() += "far_snd 				= nil\n";
    getLtx() += "far_victim 			= nil\n";
    getLtx() +=
        "use					= {=is_wounded} false, {!is_squad_commander} false, {=actor_enemy} false, {=has_enemy} "
        "false, {=actor_has_weapon} false, {=dist_to_actor_le(3)} true, false\n";
    getLtx() +=
        "snd_on_use 			= {=is_wounded} nil, {=actor_enemy} nil, {!is_squad_commander} "
        "meet_use_no_talk_leader, {=actor_has_weapon} meet_use_no_weapon, {=has_enemy} meet_use_no_fight, "
        "{=dist_to_actor_le(3)} meet_use_no_default, nil\n";
    getLtx() += "meet_dialog			= nil\n";
    getLtx() += "abuse					= {=has_enemy} false, true\n";
    getLtx() += "trade_enable			= true\n";
    getLtx() += "allow_break			= true\n";
    getLtx() += "use_text				= nil\n";

    getLtx() += "[meet@generic_animpoint]\n";
    getLtx() += "close_distance 		= 0\n";
    getLtx() += "close_anim 			= {!is_squad_commander} nil, nil\n";
    getLtx() += "close_snd_hello 		= {!is_squad_commander} nil, nil\n";
    getLtx() += "close_snd_bye 			= {!is_squad_commander} nil, nil\n";
    getLtx() += "close_victim 			= {!is_squad_commander} nil, nil\n";
    getLtx() += "far_distance 			= 0\n";
    getLtx() += "far_anim 				= nil\n";
    getLtx() += "far_snd 				= nil\n";
    getLtx() += "far_victim 			= nil\n";
    getLtx() +=
        "use					= {=is_wounded} false, {!is_squad_commander} false, {=actor_enemy} false, {=has_enemy} "
        "false, {=actor_has_weapon} false, {=dist_to_actor_le(3)} true, false\n";
    getLtx() +=
        "snd_on_use 			= {=is_wounded} nil, {=actor_enemy} nil, {!is_squad_commander} "
        "meet_use_no_talk_leader, {=actor_has_weapon} meet_use_no_weapon, {=has_enemy} meet_use_no_fight, "
        "{=dist_to_actor_le(3)} meet_use_no_default, nil\n";
    getLtx() += "meet_dialog			= nil\n";
    getLtx() += "abuse					= {=has_enemy} false, true\n";
    getLtx() += "trade_enable			= true\n";
    getLtx() += "allow_break			= true\n";
    getLtx() += "meet_on_talking 		= false\n";
    getLtx() += "use_text				= nil\n";

    std::pair<xr_vector<JobData>, xr_vector<JobDataExclusive>> all_jobs;
    JobData stalker_jobs;
    stalker_jobs.m_precondition_is_monster = false;
    stalker_jobs.m_priority = 60;

    std::pair<std::uint32_t, xr_vector<JobData_SubData>> stalker_generic_point;
    stalker_generic_point.first = 3; // prior

    for (std::uint8_t i = 0; i < 20; ++i)
    {
        xr_string name = global_name;
        xr_string job_ltx_data = "";
        name += "_point_";
        name += std::to_string(i).c_str();

        JobData_SubData data;
        //         data.first = 3; // prior
        //         data.second.first = "logic@"; // section
        //         data.second.first += name;
        //         data.second.second = GULAG_POINT_JOB;
        data.m_priority = 3;
        data.m_job_id.first = "logic@";
        data.m_job_id.first += name;
        data.m_job_id.second += Globals::GulagGenerator::kGulagJobPoint;

        stalker_generic_point.second.push_back(data);

        job_ltx_data += "[logic@";
        job_ltx_data += name;
        job_ltx_data += "]\n";
        job_ltx_data += "active = cover@";
        job_ltx_data += name;
        job_ltx_data += "\n";
        job_ltx_data += "[cover@";
        job_ltx_data += name;
        job_ltx_data += "]\n";
        job_ltx_data += "meet = meet@generic_lager\n";
        job_ltx_data += "smart = ";
        job_ltx_data += global_name;
        job_ltx_data += "\n";
        job_ltx_data += "radius_min = 3\n";
        job_ltx_data += "radius_max = 8\n";
        job_ltx_data += "use_attack_direction = false\n";
        job_ltx_data += "anim = {!npc_community(zombied)} sit, guard\n";

        if (smart->getDefenceRestirctor().size())
        {
            job_ltx_data += "out_restr = ";
            job_ltx_data += smart->getDefenceRestirctor();
            job_ltx_data += "\n";
        }

        if (smart->getBaseOnActorControl())
        {
            if (smart->getBaseOnActorControl()->getIgnoreZoneName().size())
            {
                job_ltx_data += "combat_ignore_cond = {=npc_in_zone(smart.base_on_actor_control.ignore_zone)} true \n";
                job_ltx_data += "combat_ignore_keep_when_attacked = true \n";
            }
        }

        getLtx() += job_ltx_data;
    }

    stalker_jobs.m_jobs.push_back(stalker_generic_point);
#pragma region SURGE MANAGMENT
    std::pair<std::uint32_t, xr_vector<JobData_SubData>> stalker_surge;
    stalker_surge.first = 50;

    std::uint8_t it = 1;
    xr_string patrol_path_name = global_name;
    patrol_path_name += "_surge_";
    patrol_path_name += std::to_string(it).c_str();
    patrol_path_name += "_walk";
    while (Globals::patrol_path_exists(patrol_path_name.c_str()))
    {
        JobData_SubData data;
        xr_string waypoint_name = global_name;
        waypoint_name += "_surge_";
        waypoint_name += std::to_string(it).c_str();
        waypoint_name += "_walk";

        data.m_priority = 50;
        data.m_job_id.first = "logic@";
        data.m_job_id.first += waypoint_name;
        data.m_job_id.second += Globals::GulagGenerator::kGulagJobPath;
        data.m_function = [](CSE_ALifeDynamicObject* server_object, Script_SE_SmartTerrain* smart,
                              const std::pair<xr_string, xr_map<std::uint32_t, CondlistData>>& params,
                              const NpcInfo& npc_info) -> bool {
            if (!server_object)
                Msg("[Scripts/GulagGenerator/load_job(smart)] WARNING: server_object was null!");

            if (!smart)
                Msg("[Scripts/GulagGenerator/load_job(smart)] WARNING: smart was null!");

            return XR_CONDITION::is_surge_started();
        };

        stalker_surge.second.push_back(data);

        xr_string job_ltx_data = "[logic@";
        job_ltx_data += waypoint_name;
        job_ltx_data += "]\n";
        job_ltx_data += "active = walker@";
        job_ltx_data += waypoint_name;
        job_ltx_data += "\n";
        job_ltx_data += "[walker@";
        job_ltx_data += waypoint_name;
        job_ltx_data += "]\n";
        job_ltx_data += "sound_idle = state\n";
        job_ltx_data += "use_camp = true\n";
        job_ltx_data += "meet = meet@generic_lager\n";
        job_ltx_data += "path_walk = surge_";
        job_ltx_data += std::to_string(it).c_str();
        job_ltx_data += "_walk\n";
        job_ltx_data += "def_state_standing = guard\n";
        job_ltx_data += "def_state_moving = patrol\n";

        xr_string some_point_name = global_name;
        some_point_name += "_surge_";
        some_point_name += std::to_string(it).c_str();
        some_point_name += "_look";
        if (Globals::patrol_path_exists(some_point_name.c_str()))
        {
            job_ltx_data += "path_look = surge_";
            job_ltx_data += std::to_string(it).c_str();
            job_ltx_data += "_look\n";
        }

        if (smart->getDefenceRestirctor().size())
        {
            job_ltx_data += "out_restr = ";
            job_ltx_data += smart->getDefenceRestirctor();
            job_ltx_data += "\n";
        }

        if (smart->getBaseOnActorControl())
        {
            if (smart->getBaseOnActorControl()->getIgnoreZoneName().size() &&
                XR_GULAG::is_job_in_restrictor(
                    smart, smart->getBaseOnActorControl()->getIgnoreZoneName(), waypoint_name))
            {
                job_ltx_data += "combat_ignore_cond = {=npc_in_zone(";
                job_ltx_data += smart->getBaseOnActorControl()->getIgnoreZoneName();
                job_ltx_data += ")} true \n";
                job_ltx_data += "combat_ignore_keep_when_attacked = true \n";
            }
        }

        getLtx() += job_ltx_data;
        ++it;
        patrol_path_name.clear();
        patrol_path_name = global_name;
        patrol_path_name += "_surge_";
        patrol_path_name += std::to_string(it).c_str();
        patrol_path_name += "_walk";
    }

    if (it > 1)
        stalker_jobs.m_jobs.push_back(stalker_surge);

#pragma endregion

#pragma region SLEEP MANAGMENT
    std::pair<std::uint32_t, xr_vector<JobData_SubData>> stalker_sleep;
    stalker_sleep.first = 10;
    std::uint32_t it_sleep = 1;

    xr_string patrol_sleep_point_name = global_name;
    patrol_sleep_point_name += "_sleep_";
    patrol_sleep_point_name += std::to_string(it_sleep).c_str();
    while (Globals::patrol_path_exists(patrol_sleep_point_name.c_str()))
    {
        xr_string waypoint_name = global_name;
        waypoint_name += "_sleep_";
        waypoint_name += std::to_string(it).c_str();

        JobData_SubData data;
        data.m_priority = 10;
        data.m_job_id.first = "logic@";
        data.m_job_id.first += waypoint_name;
        data.m_job_id.second += Globals::GulagGenerator::kGulagJobPath;
        data.m_function_params.first = waypoint_name;
        data.m_function = [](CSE_ALifeDynamicObject* server_object, Script_SE_SmartTerrain* smart,
                              const std::pair<xr_string, xr_map<std::uint32_t, CondlistData>>& params,
                              const NpcInfo& npc_info) -> bool {
            if (!server_object)
            {
                R_ASSERT2(false, "object was null!");
                return false;
            }

            if (!smart)
            {
                R_ASSERT2(false, "object was null!");
                return false;
            }

            CSE_ALifeHumanAbstract* server_object_human = server_object->cast_human_abstract();
            if (!server_object_human)
            {
                R_ASSERT2(false, "Unsuccessful cast!");
                return false;
            }

            if (server_object_human->CommunityName() == "zombied")
                return false;

            if (!Globals::in_time_interval(21, 7))
                return false;

            if (smart->getSmartAlarmTime() == 0)
            {
                return true;
            }

            if (!smart->getSafeRestrictor().size())
            {
                return true;
            }

            return XR_GULAG::is_job_in_restrictor(smart, smart->getSafeRestrictor(), params.first);
        };

        stalker_sleep.second.push_back(data);

        xr_string job_ltx_data = "[logic@";
        job_ltx_data += waypoint_name;
        job_ltx_data += "]\n";
        job_ltx_data += "active = sleeper@";
        job_ltx_data += waypoint_name;
        job_ltx_data += "\n";
        job_ltx_data += "[sleeper@";
        job_ltx_data += waypoint_name;
        job_ltx_data += "]\n";
        job_ltx_data += "path_main = sleep_";
        job_ltx_data += std::to_string(it_sleep).c_str();
        job_ltx_data += "\n";

        if (smart->getSafeRestrictor().size() &&
            XR_GULAG::is_job_in_restrictor(smart, smart->getSafeRestrictor(), waypoint_name))
        {
            job_ltx_data += "invulnerable = {=npc_in_zone(";
            job_ltx_data += smart->getSafeRestrictor();
            job_ltx_data += ")} true \n";
        }

        if (smart->getDefenceRestirctor().size())
        {
            job_ltx_data += "out_restr = ";
            job_ltx_data += smart->getDefenceRestirctor();
            job_ltx_data += "\n";
        }

        if (smart->getBaseOnActorControl())
        {
            if (smart->getBaseOnActorControl()->getIgnoreZoneName().size() &&
                XR_GULAG::is_job_in_restrictor(
                    smart, smart->getBaseOnActorControl()->getIgnoreZoneName(), waypoint_name))
            {
                job_ltx_data += "combat_ignore_cond = {=npc_in_zone(";
                job_ltx_data += smart->getBaseOnActorControl()->getIgnoreZoneName();
                job_ltx_data += ")} true \n";
                job_ltx_data += "combat_ignore_keep_when_attacked = true \n";
            }
        }

        getLtx() += job_ltx_data;
        ++it_sleep;
        patrol_sleep_point_name = global_name;
        patrol_sleep_point_name += "_sleep_";
        patrol_sleep_point_name += std::to_string(it_sleep).c_str();
    }

    if (it_sleep > 1)
        stalker_jobs.m_jobs.push_back(stalker_sleep);
#pragma endregion

#pragma region COLLECTOR HANDLING
    std::pair<std::uint32_t, xr_vector<JobData_SubData>> stalker_collector;
    stalker_collector.first = 25;
    std::uint32_t it_collector = 1;
    xr_string patrol_collector_point_name = global_name;
    patrol_collector_point_name += "_collector_";
    patrol_collector_point_name += std::to_string(it_collector).c_str();
    patrol_collector_point_name += "_walk";

    while (Globals::patrol_path_exists(patrol_collector_point_name.c_str()))
    {
        xr_string waypoint_name = global_name;
        waypoint_name += "_collector_";
        waypoint_name += std::to_string(it_collector).c_str();
        waypoint_name += "_walk";

        JobData_SubData data;
        data.m_priority = 25;
        data.m_job_id.first = "logic@";
        data.m_job_id.first += waypoint_name;
        data.m_job_id.second = Globals::GulagGenerator::kGulagJobPath;
        data.m_function = [](CSE_ALifeDynamicObject* server_object, Script_SE_SmartTerrain* smart,
                              const std::pair<xr_string, xr_map<std::uint32_t, CondlistData>>& params,
                              const NpcInfo& npc_info) -> bool {
            if (!server_object)
            {
                R_ASSERT2(false, "object was null!");
                return false;
            }

            if (!smart)
            {
                R_ASSERT2(false, "object was null!");
                return false;
            }

            CSE_ALifeHumanAbstract* server_object_human = server_object->cast_human_abstract();
            if (!server_object_human)
            {
                R_ASSERT2(false, "Unsuccessful cast!");
                return false;
            }

            if (server_object_human->CommunityName() == "zombied")
                return false;

            const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(server_object->ID);

            if (!storage.getClientObject())
                return false;

            CScriptGameObject* npc = storage.getClientObject();

            // @ Lord: проверить данный ли метод используется????
            if (npc->GetObjectByName("detector_simple"))
                return true;

            if (npc->GetObjectByName("detector_advanced"))
                return true;

            if (npc->GetObjectByName("detector_elite"))
                return true;

            if (npc->GetObjectByName("detector_scientific"))
                return true;

            return false;
        };

        stalker_collector.second.push_back(data);

        xr_string job_ltx_data = "[logic@";
        job_ltx_data += waypoint_name;
        job_ltx_data += "]\n";
        job_ltx_data += "active = walker@";
        job_ltx_data += waypoint_name;
        job_ltx_data += "\n";
        job_ltx_data += "[walker@";
        job_ltx_data += waypoint_name;
        job_ltx_data += "]\n";
        job_ltx_data += "sound_idle = state\n";
        job_ltx_data += "meet = meet@generic_lager\n";
        job_ltx_data += "path_walk = collector_";
        job_ltx_data += std::to_string(it_collector).c_str();
        job_ltx_data += "_walk\n";
        job_ltx_data += "def_state_standing = guard\n";
        job_ltx_data += "def_state_moving = patrol\n";

        xr_string sub_point_name = global_name;
        sub_point_name += "_collector_";
        sub_point_name += std::to_string(it_collector).c_str();
        sub_point_name += "_look";
        if (Globals::patrol_path_exists(sub_point_name.c_str()))
        {
            job_ltx_data += "path_look = collector_";
            job_ltx_data += std::to_string(it_collector).c_str();
            job_ltx_data += "_look\n";
        }

        if (smart->getSafeRestrictor().size() &&
            XR_GULAG::is_job_in_restrictor(smart, smart->getSafeRestrictor(), waypoint_name))
        {
            job_ltx_data += "invulnerable = {=npc_in_zone(";
            job_ltx_data += smart->getSafeRestrictor();
            job_ltx_data += ")} true \n";
        }

        if (smart->getDefenceRestirctor().size())
        {
            job_ltx_data += "out_restr = ";
            job_ltx_data += smart->getDefenceRestirctor();
            job_ltx_data += "\n";
        }

        if (smart->getBaseOnActorControl())
        {
            if (smart->getBaseOnActorControl()->getIgnoreZoneName().size() &&
                XR_GULAG::is_job_in_restrictor(
                    smart, smart->getBaseOnActorControl()->getIgnoreZoneName(), waypoint_name))
            {
                job_ltx_data += "combat_ignore_cond = {=npc_in_zone(";
                job_ltx_data += smart->getBaseOnActorControl()->getIgnoreZoneName();
                job_ltx_data += ")} true \n";
                job_ltx_data += "combat_ignore_keep_when_attacked = true \n";
            }
        }

        getLtx() += job_ltx_data;
        ++it_collector;
        patrol_collector_point_name = global_name;
        patrol_collector_point_name += "_collector_";
        patrol_collector_point_name += std::to_string(it_collector).c_str();
        patrol_collector_point_name += "_walk";
    }

    if (it_collector > 1)
        stalker_jobs.m_jobs.push_back(stalker_collector);
#pragma endregion

#pragma region WALKER HANDLING
    std::pair<std::uint32_t, xr_vector<JobData_SubData>> stalker_walker;
    stalker_walker.first = 15;
    std::uint32_t it_walker = 1;
    xr_string patrol_walker_point_name = global_name;
    patrol_walker_point_name += "_walker_";
    patrol_walker_point_name += std::to_string(it_walker).c_str();
    patrol_walker_point_name += "_walk";
    while (Globals::patrol_path_exists(patrol_walker_point_name.c_str()))
    {
        xr_string waypoint_name = global_name;
        waypoint_name += "_walker_";
        waypoint_name += std::to_string(it_walker).c_str();
        waypoint_name += "_walk";

        JobData_SubData data;
        data.m_priority = 15;
        data.m_job_id.first = "logic@";
        data.m_job_id.first += waypoint_name;
        data.m_function_params.first = waypoint_name;
        data.m_function = [](CSE_ALifeDynamicObject* server_object, Script_SE_SmartTerrain* smart,
                              const std::pair<xr_string, xr_map<std::uint32_t, CondlistData>>& params,
                              const NpcInfo& npc_info) -> bool {
            if (!server_object)
            {
                R_ASSERT2(false, "object was null!");
                return false;
            }

            if (!smart)
            {
                R_ASSERT2(false, "object was null!");
                return false;
            }

            // @ Lord: проверить данное условие, вообще оно может выполняться, когда в оригинале было AlarmTime == nil
            if (smart->getSmartAlarmTime() == 0)
                return true;

            if (!smart->getSafeRestrictor().size())
                return true;

            return XR_GULAG::is_job_in_restrictor(smart, smart->getSafeRestrictor(), params.first);
        };

        stalker_walker.second.push_back(data);

        xr_string job_ltx_data = "[logic@";
        job_ltx_data += waypoint_name;
        job_ltx_data += "]\n";
        job_ltx_data += "active = walker@";
        job_ltx_data += waypoint_name;
        job_ltx_data += "\n";
        job_ltx_data += "[walker@";
        job_ltx_data += waypoint_name;
        job_ltx_data += "]\n";
        job_ltx_data += "sound_idle = state\n";
        job_ltx_data += "meet = meet@generic_lager\n";
        job_ltx_data += "path_walk = walker_";
        job_ltx_data += std::to_string(it_walker).c_str();
        job_ltx_data += "_walk\n";
        job_ltx_data += "def_state_standing = guard\n";
        job_ltx_data += "def_state_moving = patrol\n";

        xr_string sub_point_name = global_name;
        sub_point_name += "_walker_";
        sub_point_name += std::to_string(it_walker).c_str();
        sub_point_name += "_look";

        if (Globals::patrol_path_exists(sub_point_name.c_str()))
        {
            job_ltx_data += "path_look = walker_";
            job_ltx_data += std::to_string(it_walker).c_str();
            job_ltx_data += "_look\n";
        }

        if (smart->getSafeRestrictor().size() &&
            XR_GULAG::is_job_in_restrictor(smart, smart->getSafeRestrictor(), waypoint_name))
        {
            job_ltx_data += "invulnerable = {=npc_in_zone(";
            job_ltx_data += smart->getSafeRestrictor();
            job_ltx_data += ")} true\n";
        }

        if (smart->getDefenceRestirctor().size())
        {
            job_ltx_data += "out_restr = ";
            job_ltx_data += smart->getDefenceRestirctor();
            job_ltx_data += "\n";
        }

        if (smart->getBaseOnActorControl())
        {
            if (smart->getBaseOnActorControl()->getIgnoreZoneName().size() &&
                XR_GULAG::is_job_in_restrictor(
                    smart, smart->getBaseOnActorControl()->getIgnoreZoneName(), waypoint_name))
            {
                job_ltx_data += "combat_ignore_cond = {=npc_in_zone(";
                job_ltx_data += smart->getBaseOnActorControl()->getIgnoreZoneName();
                job_ltx_data += ")} true \n";
                job_ltx_data += "combat_ignore_keep_when_attacked = true \n";
            }
        }

        getLtx() += job_ltx_data;
        ++it_walker;
        patrol_walker_point_name = global_name;
        patrol_walker_point_name += "_walker_";
        patrol_walker_point_name += std::to_string(it_walker).c_str();
        patrol_walker_point_name += "_walk";
    }

    if (it_walker > 1)
        stalker_jobs.m_jobs.push_back(stalker_walker);
#pragma endregion

#pragma region PATROL HANDLING
    std::pair<std::uint32_t, xr_vector<JobData_SubData>> stalker_patrol;
    stalker_patrol.first = 20;
    std::uint32_t it_patrol = 1;
    xr_string patrol_patrol_point_name = global_name;
    patrol_patrol_point_name += "_patrol_";
    patrol_patrol_point_name += std::to_string(it_patrol).c_str();
    patrol_patrol_point_name += "_walk";

    while (Globals::patrol_path_exists(patrol_patrol_point_name.c_str()))
    {
        xr_string waypoint_name = global_name;
        waypoint_name += "_patrol_";
        waypoint_name += std::to_string(it_patrol);
        waypoint_name += "_walk";
        std::uint8_t job_count = 3;

        for (std::uint8_t i = 0; i < job_count; ++i)
        {
            JobData_SubData data;
            data.m_priority = 20;
            data.m_job_id.first = "logic@";
            data.m_job_id.first += waypoint_name.c_str();
            data.m_job_id.second = Globals::GulagGenerator::kGulagJobPath;
            data.m_function_params.first = waypoint_name;
            data.m_function = [](CSE_ALifeDynamicObject* server_object, Script_SE_SmartTerrain* smart,
                                  const std::pair<xr_string, xr_map<std::uint32_t, CondlistData>>& params,
                                  const NpcInfo& npc_info) -> bool {
                if (!server_object)
                {
                    R_ASSERT2(false, "object was null!");
                    return false;
                }

                if (!smart)
                {
                    R_ASSERT2(false, "object was null!");
                    return false;
                }

                if (!strcmp(server_object->cast_human_abstract()->CommunityName(), "zombied"))
                    return false;

                if (smart->getSmartAlarmTime() == 0)
                    return true;

                if (!smart->getSafeRestrictor().size())
                    return true;

                return XR_GULAG::is_job_in_restrictor(smart, smart->getSafeRestrictor(), params.first);
            };

            stalker_patrol.second.push_back(data);
        }

        xr_string job_ltx_data = "[logic@";
        job_ltx_data += waypoint_name.c_str();
        job_ltx_data += "]\n";
        job_ltx_data += "active = patrol@";
        job_ltx_data += waypoint_name.c_str();
        job_ltx_data += "]\n";
        job_ltx_data += "[patrol@";
        job_ltx_data += waypoint_name.c_str();
        job_ltx_data += "]\n";
        job_ltx_data += "meet = meet@generic_lager\n";
        job_ltx_data += "formation = back\n";
        job_ltx_data += "path_walk = patrol_";
        job_ltx_data += std::to_string(it_patrol).c_str();
        job_ltx_data += "_walk\n";
        job_ltx_data += "on_signal = end| %=search_gulag_job%\n";

        xr_string sub_point_name = global_name;
        sub_point_name += "_patrol_";
        sub_point_name += std::to_string(it_patrol).c_str();
        sub_point_name += "_look\n";
        if (Globals::patrol_path_exists(sub_point_name.c_str()))
        {
            job_ltx_data += "path_look = patrol_";
            job_ltx_data += std::to_string(it_patrol).c_str();
            job_ltx_data += "_look\n";
        }

        if (smart->getSafeRestrictor().size() &&
            XR_GULAG::is_job_in_restrictor(smart, smart->getSafeRestrictor(), waypoint_name))
        {
            job_ltx_data += "invulnerable = {=npc_in_zone(";
            job_ltx_data += smart->getSafeRestrictor().c_str();
            job_ltx_data += ")} true \n";
        }

        if (smart->getDefenceRestirctor().size())
        {
            job_ltx_data += "out_restr = ";
            job_ltx_data += smart->getDefenceRestirctor().c_str();
            job_ltx_data += "\n";
        }

        getLtx() += job_ltx_data;
        ++it_patrol;
        patrol_patrol_point_name = global_name;
        patrol_patrol_point_name += "_patrol_";
        patrol_patrol_point_name += std::to_string(it_patrol).c_str();
        patrol_patrol_point_name += "_walk";
    }

    if (it_patrol > 1)
        stalker_jobs.m_jobs.push_back(stalker_patrol);
#pragma endregion

#pragma region XR_ANIMPOINT HANDLING
    std::uint32_t it_xranimpoint = 1;
    std::pair<std::uint32_t, xr_vector<JobData_SubData>> stalker_xranimpoint; // @ Lord: как приоритет??
    stalker_xranimpoint.first = 50;
    xr_string patrol_xranimpoint_point_name = global_name;
    patrol_xranimpoint_point_name += "_animpoint_";
    patrol_xranimpoint_point_name += std::to_string(it_xranimpoint).c_str();

        while (Script_GlobalHelper::getInstance().getGameRegisteredServerSmartCovers().find(
                   patrol_xranimpoint_point_name) !=
            Script_GlobalHelper::getInstance().getGameRegisteredServerSmartCovers().end())
        {
            xr_string waypoint_name = global_name;
            waypoint_name += "_animpoint_";
            waypoint_name += std::to_string(it_xranimpoint).c_str();

            JobData_SubData data;

            data.m_priority = 15;
            data.m_job_id.first = "logic@";
            data.m_job_id.first += waypoint_name;
            data.m_job_id.second = Globals::GulagGenerator::kGulagJobSmartCover;
            data.m_function = [](CSE_ALifeDynamicObject* server_object, Script_SE_SmartTerrain* smart,
                                  const std::pair<xr_string, xr_map<std::uint32_t, CondlistData>>& params,
                                  const NpcInfo& npc_info) -> bool {
                if (!server_object)
                {
                    R_ASSERT2(false, "object was null!");
                    return false;
                }

                CSE_ALifeHumanAbstract* server_human = server_object->cast_human_abstract();
                if (!server_human)
                {
                    R_ASSERT2(false, "object was null!");
                    return false;
                }

                if (!strcmp(server_human->CommunityName(), "zombied"))
                    return false;

                return true;
            };

            // @ Lord: в оригинале пушиться сразу же в stalker_jobs, проверить этот момент!
            stalker_xranimpoint.second.push_back(data);

            xr_string job_ltx_data = "[logic@";
            job_ltx_data += waypoint_name;
            job_ltx_data += "]\n";
            job_ltx_data += "active = animpoint@";
            job_ltx_data += waypoint_name;
            job_ltx_data += "]\n";
            job_ltx_data += "[animpoint@";
            job_ltx_data += waypoint_name;
            job_ltx_data += "]\n";
            job_ltx_data += "meet = meet@generic_animpoint\n";
            job_ltx_data += "cover_name = ";
            job_ltx_data += waypoint_name;
            job_ltx_data += "\n";

            if (smart->getDefenceRestirctor().size())
            {
                job_ltx_data += "out_restr = ";
                job_ltx_data += smart->getDefenceRestirctor();
                job_ltx_data += "\n";
            }

            if (smart->getSafeRestrictor().size() &&
                XR_GULAG::is_job_in_restrictor(smart, smart->getSafeRestrictor(), waypoint_name))
            {
                job_ltx_data += "invulnerable = {=npc_in_zone(";
                job_ltx_data += smart->getSafeRestrictor();
                job_ltx_data += ")} true \n";
            }

            if (smart->getBaseOnActorControl())
            {
                if (smart->getBaseOnActorControl()->getIgnoreZoneName().size())
                {
                    job_ltx_data += "combat_ignore_cond = {=npc_in_zone(";
                    job_ltx_data += smart->getBaseOnActorControl()->getIgnoreZoneName();
                    job_ltx_data += ")} true \n";
                    job_ltx_data += "combat_ignore_keep_when_attacked = true \n";
                }
            }

            getLtx() += job_ltx_data;
            ++it_xranimpoint;
            patrol_xranimpoint_point_name = global_name;
            patrol_xranimpoint_point_name += "_animpoint_";
            patrol_xranimpoint_point_name += std::to_string(it_xranimpoint).c_str();
        }
    

    if (it_xranimpoint >= 1)
        stalker_jobs.m_jobs.push_back(stalker_xranimpoint);

#pragma endregion

#pragma region GUARD Handling
    std::pair<std::uint32_t, xr_vector<JobData_SubData>> stalker_guard;
    stalker_guard.first = 25;
    std::uint32_t it_guard = 1;
    xr_string patrol_guard_point_name = global_name;
    patrol_guard_point_name += "_guard_";
    patrol_guard_point_name += std::to_string(it_guard);
    patrol_guard_point_name += "_walk";

    while (Globals::patrol_path_exists(patrol_guard_point_name.c_str()))
    {
        xr_string waypoint_name = global_name;
        waypoint_name += "_guard_";
        waypoint_name += std::to_string(it_guard).c_str();
        waypoint_name += "_walk";

        JobData_SubData data;
        data.m_priority = 25;
        data.m_job_id.first = "logic@";
        data.m_job_id.first += waypoint_name;
        data.m_job_id.second = Globals::GulagGenerator::kGulagJobPath;
        data.m_function_params.first = waypoint_name;
        data.m_function = [](CSE_ALifeDynamicObject* server_object, Script_SE_SmartTerrain* smart,
                              const std::pair<xr_string, xr_map<std::uint32_t, CondlistData>>& params,
                              const NpcInfo& npc_info) -> bool {
            if (!smart)
            {
                R_ASSERT2(false, "object was null!");
                return false;
            }

            if (smart->getSmartAlarmTime() == 0)
                return true;

            if (!smart->getSafeRestrictor().size())
                return true;

            return XR_GULAG::is_job_in_restrictor(smart, smart->getSafeRestrictor(), params.first);
        };

        stalker_guard.second.push_back(data);

        xr_string job_ltx_data = "[logic@";
        job_ltx_data += waypoint_name;
        job_ltx_data += "]\n";
        job_ltx_data += "active = walker@";
        job_ltx_data += waypoint_name;
        job_ltx_data += "\n";
        job_ltx_data += "[walker@";
        job_ltx_data += waypoint_name;
        job_ltx_data += "]\n";
        job_ltx_data += "meet = meet@generic_lager\n";
        job_ltx_data += "path_walk = guard_";
        job_ltx_data += std::to_string(it_guard).c_str();
        job_ltx_data += "_walk\n";
        job_ltx_data += "path_look = guard_";
        job_ltx_data += std::to_string(it_guard).c_str();
        job_ltx_data += "_look\n";

        if (smart->getSafeRestrictor().size() &&
            XR_GULAG::is_job_in_restrictor(smart, smart->getSafeRestrictor(), waypoint_name))
        {
            job_ltx_data += "invulnerable = {=npc_in_zone(";
            job_ltx_data += smart->getSafeRestrictor();
            job_ltx_data += ")} true \n";
        }

        if (smart->getDefenceRestirctor().size())
        {
            job_ltx_data += "out_restr = ";
            job_ltx_data += smart->getDefenceRestirctor();
            job_ltx_data += "\n";
        }

        xr_string job1_ltx_data = "[walker1@";
        job1_ltx_data += waypoint_name;
        job1_ltx_data += "]\n";
        job1_ltx_data += "meet = meet@generic_lager\n";
        job1_ltx_data += "path_walk = guard_";
        job1_ltx_data += std::to_string(it_guard).c_str();
        job1_ltx_data += "_walk\n";
        job1_ltx_data += "path_look = guard_";
        job1_ltx_data += std::to_string(it_guard).c_str();
        job1_ltx_data += "_look\n";
        job1_ltx_data += "def_state_standing = wait_na\n";
        job1_ltx_data += "on_info = {!is_obj_on_job(logic@follower_";
        job1_ltx_data += waypoint_name;
        job1_ltx_data += ":3)} walker@";
        job1_ltx_data += waypoint_name;
        job1_ltx_data += "\n";
        job1_ltx_data += "on_info2 = {=distance_to_obj_on_job_le(logic@follower_";
        job1_ltx_data += waypoint_name;
        job1_ltx_data += ":3)} remark@";
        job1_ltx_data += waypoint_name;
        job1_ltx_data += "\n";

        if (smart->getSafeRestrictor().size() &&
            XR_GULAG::is_job_in_restrictor(smart, smart->getSafeRestrictor(), waypoint_name))
        {
            job1_ltx_data += "invulnerable = {=npc_in_zone(";
            job1_ltx_data += smart->getSafeRestrictor();
            job1_ltx_data += ")} true \n";
        }

        if (smart->getDefenceRestirctor().size())
        {
            job1_ltx_data += "out_restr = ";
            job1_ltx_data += smart->getDefenceRestirctor();
            job1_ltx_data += "\n";
        }

        job1_ltx_data += "[remark@";
        job1_ltx_data += smart->getDefenceRestirctor();
        job1_ltx_data += "]\n";
        job1_ltx_data += "anim = wait_na\n";
        job1_ltx_data += "target = logic@follower_";
        job1_ltx_data += waypoint_name;
        job1_ltx_data += "\n";

        if (smart->getDefenceRestirctor().size())
        {
            job1_ltx_data += "out_restr = ";
            job1_ltx_data += smart->getDefenceRestirctor();
            job1_ltx_data += "\n";
        }

        data.m_priority = 24;
        data.m_job_id.first = "logic@follower_";
        data.m_job_id.first += waypoint_name;
        data.m_job_id.second = Globals::GulagGenerator::kGulagJobPath;
        data.m_function_params.first = waypoint_name;
        data.m_function = [](CSE_ALifeDynamicObject* server_object, Script_SE_SmartTerrain* smart,
                              const std::pair<xr_string, xr_map<std::uint32_t, CondlistData>>& params,
                              const NpcInfo& npc_info) -> bool {
            return npc_info.m_need_job == (xr_string("logic@").append(params.first.c_str()));
        };

        stalker_guard.second.push_back(data);

        xr_string follower_ltx = "[logic@follower_";
        follower_ltx += waypoint_name;
        follower_ltx += "]\n";
        follower_ltx += "active = walker@follow_";
        follower_ltx += waypoint_name;
        follower_ltx += "\n";
        follower_ltx += "[walker@follow_";
        follower_ltx += waypoint_name;
        follower_ltx += "]\n";
        follower_ltx += "meet = meet@generic_lager\n";
        follower_ltx += "path_walk = guard_";
        follower_ltx += std::to_string(it_guard).c_str();
        follower_ltx += "_walk\n";
        follower_ltx += "path_look = guard_";
        follower_ltx += std::to_string(it_guard).c_str();
        follower_ltx += "_look\n";
        follower_ltx += "on_info = {=distance_to_obj_on_job_le(logic@";
        follower_ltx += waypoint_name;
        follower_ltx += ":3)} remark@follower_";
        follower_ltx += waypoint_name;
        follower_ltx += "\n";

        if (smart->getDefenceRestirctor().size())
        {
            follower_ltx += "out_restr = ";
            follower_ltx += smart->getDefenceRestirctor();
            follower_ltx += "\n";
        }

        follower_ltx += "[remark@follower_";
        follower_ltx += waypoint_name;
        follower_ltx += "]\n";
        follower_ltx += "anim = wait_na\n";
        follower_ltx += "target = logic@";
        follower_ltx += waypoint_name;
        follower_ltx += "\n";
        follower_ltx += "on_timer = 2000 | %=switch_to_desired_job%\n";

        if (smart->getDefenceRestirctor().size())
        {
            follower_ltx += "out_restr = ";
            follower_ltx += smart->getDefenceRestirctor();
            follower_ltx += "\n";
        }

        getLtx() += job_ltx_data;
        getLtx() += job1_ltx_data;
        getLtx() += follower_ltx;
        ++it_guard;
        patrol_guard_point_name = global_name;
        patrol_guard_point_name += "_guard_";
        patrol_guard_point_name += std::to_string(it_guard);
        patrol_guard_point_name += "_walk";
    }

    if (it_guard > 1)
        stalker_jobs.m_jobs.push_back(stalker_guard);
#pragma endregion

#pragma region SNIPER handling
    std::pair<std::uint32_t, xr_vector<JobData_SubData>> stalker_sniper;
    stalker_sniper.first = 30;
    std::uint32_t it_sniper = 1;
    xr_string patrol_sniper_point_name = global_name;
    patrol_sniper_point_name += "_sniper_";
    patrol_sniper_point_name += std::to_string(it_sniper).c_str();
    patrol_sniper_point_name += "_walk";

    while (Globals::patrol_path_exists(patrol_sniper_point_name.c_str()))
    {
        xr_string waypoint_name = global_name;
        waypoint_name += "_sniper_";
        waypoint_name += std::to_string(it_sniper).c_str();
        waypoint_name += "_walk";

        JobData_SubData data;
        data.m_priority = 30;
        data.m_job_id.first = "logic@";
        data.m_job_id.first += waypoint_name;
        data.m_job_id.second = Globals::GulagGenerator::kGulagJobPath;
        data.m_function_params.first = waypoint_name;
        data.m_function = [](CSE_ALifeDynamicObject* server_object, Script_SE_SmartTerrain* smart,
                              const std::pair<xr_string, xr_map<std::uint32_t, CondlistData>>& params,
                              const NpcInfo& npc_info) -> bool {
            if (!server_object)
            {
                R_ASSERT2(false, "object was null!");
                return false;
            }

            CSE_ALifeHumanAbstract* server_human = server_object->cast_human_abstract();

            if (!server_human)
            {
                R_ASSERT2(false, "object was null!");
                return false;
            }

            if (!strcmp(server_human->CommunityName(), "zombied"))
                return false;

            return Globals::is_accessible_job(server_object, params.first);
        };

        stalker_sniper.second.push_back(data);

        xr_string job_ltx_data = "[logic@";
        job_ltx_data += waypoint_name;
        job_ltx_data += "]\n";
        job_ltx_data += "active = camper@";
        job_ltx_data += waypoint_name;
        job_ltx_data += "\n";
        job_ltx_data += "[camper@";
        job_ltx_data += waypoint_name;
        job_ltx_data += "]\n";
        job_ltx_data += "meet = meet@generic_lager\n";
        job_ltx_data += "path_walk = sniper_";
        job_ltx_data += std::to_string(it_sniper).c_str();
        job_ltx_data += "_walk\n";
        job_ltx_data += "path_look = sniper_";
        job_ltx_data += std::to_string(it_sniper).c_str();
        job_ltx_data += "_look\n";
        job_ltx_data += "sniper = true\n";
        job_ltx_data += "def_state_cmapering = hide\n";
        job_ltx_data += "def_state_campering_fire = hide_fire\n";

        if (smart->getDefenceRestirctor().size())
        {
            job_ltx_data += "out_restr = ";
            job_ltx_data += smart->getDefenceRestirctor();
            job_ltx_data += ",";
            job_ltx_data += Globals::get_job_restrictor(waypoint_name);
        }

        getLtx() += job_ltx_data;
        ++it_sniper;
        patrol_sniper_point_name = global_name;
        patrol_sniper_point_name += "_sniper_";
        patrol_sniper_point_name += std::to_string(it_sniper).c_str();
        patrol_sniper_point_name += "_walk";
    }

    if (it_sniper > 1)
        stalker_jobs.m_jobs.push_back(stalker_sniper);
#pragma endregion

#pragma region CAMPER Handling
    std::pair<std::uint32_t, xr_vector<JobData_SubData>> stalker_camper;
    stalker_camper.first = 45;
    std::uint32_t it_camper = 1;
    xr_string patrol_camper_point_name = global_name;
    patrol_camper_point_name += "_camper_";
    patrol_camper_point_name += std::to_string(it_camper).c_str();
    patrol_camper_point_name += "_walk";

    while (Globals::patrol_path_exists(patrol_camper_point_name.c_str()))
    {
        xr_string waypoint_name = global_name;
        waypoint_name += "_camper_";
        waypoint_name += std::to_string(it_camper).c_str();
        waypoint_name += "_walk";

        JobData_SubData data;
        data.m_priority = 45;
        data.m_job_id.first = "logic@";
        data.m_job_id.first += waypoint_name;
        data.m_job_id.second = Globals::GulagGenerator::kGulagJobPath;
        data.m_function_params.first = waypoint_name;
        data.m_function = [](CSE_ALifeDynamicObject* server_object, Script_SE_SmartTerrain* smart,
                              const std::pair<xr_string, xr_map<std::uint32_t, CondlistData>>& params,
                              const NpcInfo& npc_info) -> bool {
            if (!server_object)
            {
                R_ASSERT2(false, "object was null!");
                return false;
            }

            return Globals::is_accessible_job(server_object, params.first);
        };

        stalker_camper.second.push_back(data);

        xr_string job_ltx_data = "[logic@";
        job_ltx_data += waypoint_name;
        job_ltx_data += "]\n";
        job_ltx_data += "active = camper@";
        job_ltx_data += waypoint_name;
        job_ltx_data += "\n";
        job_ltx_data += "[camper@";
        job_ltx_data += waypoint_name;
        job_ltx_data += "]\n";
        job_ltx_data += "meet = meet@generic_lager\n";
        job_ltx_data += "radius = 0";
        job_ltx_data += "path_walk = camper_";
        job_ltx_data += std::to_string(it_camper).c_str();
        job_ltx_data += "_walk\n";
        job_ltx_data += "def_state_moving = rush\n";
        job_ltx_data += "def_state_campering = hide\n";
        job_ltx_data += "def_state_campering_fire = hide_fire\n";

        xr_string sub_point_name = global_name;
        sub_point_name += "_camper_";
        sub_point_name += std::to_string(it_camper).c_str();
        sub_point_name += "_look";
        if (Globals::patrol_path_exists(sub_point_name.c_str()))
        {
            job_ltx_data += "path_look = camper_";
            job_ltx_data += std::to_string(it_camper).c_str();
            job_ltx_data += "_look\n";
        }

        if (smart->getDefenceRestirctor().size())
        {
            job_ltx_data += "out_restr = ";
            job_ltx_data += smart->getDefenceRestirctor();
            job_ltx_data += ",";
            job_ltx_data += Globals::get_job_restrictor(waypoint_name);
            job_ltx_data += "\n";
        }

        getLtx() += job_ltx_data;
        ++it_camper;
        patrol_camper_point_name = global_name;
        patrol_camper_point_name += "_camper_";
        patrol_camper_point_name += std::to_string(it_camper).c_str();
        patrol_camper_point_name += "_walk";
    }

    if (it_camper > 1)
        stalker_jobs.m_jobs.push_back(stalker_camper);
#pragma endregion

    all_jobs.first.push_back(stalker_jobs);

#pragma region MOB HOME
    JobData monster_jobs;
    monster_jobs.m_precondition_is_monster = true;
    monster_jobs.m_priority = 50;

    std::pair<std::uint32_t, xr_vector<JobData_SubData>> monster_mob_home;
    monster_mob_home.first = 50; // Lord: какой приоритет?
    for (std::uint8_t i = 1; i < 21; ++i)
    {
        xr_string patrol_mob_home_point_name = global_name;
        patrol_mob_home_point_name += "_home_";
        patrol_mob_home_point_name += std::to_string(i).c_str();
        std::uint32_t home_min_radius = 10;
        std::uint32_t home_middle_radius = 20;
        std::uint32_t home_max_radius = 70;

        JobData_SubData data;
        data.m_priority = 40;
        data.m_job_id.first = "logic@";
        data.m_job_id.first += patrol_mob_home_point_name;
        data.m_job_id.second = Globals::GulagGenerator::kGulagJobPoint;

        monster_mob_home.second.push_back(data);

        xr_string job_ltx_data = "[logic@";
        job_ltx_data += patrol_mob_home_point_name;
        job_ltx_data += "]\n";
        job_ltx_data += "active = mob_home@";
        job_ltx_data += patrol_mob_home_point_name;
        job_ltx_data += "\n";
        job_ltx_data += "[mob_home@";
        job_ltx_data += patrol_mob_home_point_name;
        job_ltx_data += "]\n";
        job_ltx_data += "gulag_point = true\n";
        job_ltx_data += "home_min_radius = ";
        job_ltx_data += std::to_string(home_min_radius).c_str();
        job_ltx_data += "\n";
        job_ltx_data += "home_mid_radius = ";
        job_ltx_data += std::to_string(home_middle_radius).c_str();
        job_ltx_data += "\n";
        job_ltx_data += "home_max_radius = ";
        job_ltx_data += std::to_string(home_max_radius).c_str();
        job_ltx_data += "\n";

        if (smart->getDefenceRestirctor().size())
        {
            job_ltx_data += "out_restr = ";
            job_ltx_data += smart->getDefenceRestirctor();
            job_ltx_data += "\n";
        }

        getLtx() += job_ltx_data;
    }

    monster_jobs.m_jobs.push_back(monster_mob_home);
#pragma endregion

    all_jobs.first.push_back(monster_jobs);

    return all_jobs;
}

inline void add_exclusive_job(const xr_string& section_name, const xr_string& work_field_name,
    CScriptIniFile& smart_ini, std::pair<xr_vector<JobData>, xr_vector<JobDataExclusive>>& all_jobs)
{
    if (!section_name.size())
    {
        R_ASSERT2(false, "string can't be null!");
        return;
    }

    if (!work_field_name.size())
    {
        R_ASSERT2(false, "string can't be empty!");
        return;
    }

    xr_string work_name = Globals::Utils::cfg_get_string(&smart_ini, section_name, work_field_name);

    if (!work_name.size())
    {
        R_ASSERT2(false, "a string can't be empty!");
        return;
    }

    xr_string ini_path_name = "scripts\\";
    ini_path_name += work_name;

    if (!FS.exist("$game_config$", ini_path_name.c_str()))
    {
        Msg("There is no configuration file [%s]", ini_path_name.c_str());
        R_ASSERT(false);
        return;
    }

    CScriptIniFile& job_ini_file = CScriptIniFile(ini_path_name.c_str());
    xr_string job_online_name =
        Globals::Utils::cfg_get_string(&job_ini_file, (xr_string("logic@").append(work_field_name)), "job_online");
    xr_string active_section_name =
        Globals::Utils::cfg_get_string(&job_ini_file, (xr_string("logic@").append(work_field_name)), "active");
    xr_string job_suitable_name =
        Globals::Utils::cfg_get_string(&job_ini_file, (xr_string("logic@").append(work_field_name)), "suitable");
    xr_string scheme_name = Globals::Utils::get_scheme_by_section(active_section_name);
    xr_string job_type_name = Script_GlobalHelper::getInstance().getJobTypesByScheme().at(scheme_name);
    std::uint32_t new_priority = static_cast<std::uint32_t>(
        Globals::Utils::cfg_get_number(&job_ini_file, (xr_string("logic@").append(work_field_name)), "prior"));
    bool is_monster =
        Globals::Utils::cfg_get_bool(&job_ini_file, (xr_string("logic@").append(work_field_name)), "monster_job");

    if (scheme_name == Globals::GulagGenerator::kGulagJobNameMobHome)
        if (Globals::Utils::cfg_get_bool(&job_ini_file, active_section_name, "gulag_point"))
            job_type_name = Globals::GulagGenerator::kGulagJobPoint;

    if (!job_suitable_name.size())
    {
        JobDataExclusive data;
        data.m_priority = new_priority;
        data.m_is_precondition_monster = is_monster;
        data.m_job_id.m_section_name = xr_string("logic@").append(work_field_name);
        data.m_job_id.m_ini_path_name = ini_path_name;
        data.m_job_id.m_job_type = job_type_name;
        data.m_job_id.m_online_name = job_online_name;
        data.m_job_id.m_ini_file = job_ini_file;

        all_jobs.second.push_back(data);
        return;
    }

    xr_map<std::uint32_t, CondlistData> condlist_data = XR_LOGIC::parse_condlist_by_server_object(
        (xr_string("logic@").append(work_field_name)), "suitable", job_suitable_name);

    JobDataExclusive data;
    data.m_priority = new_priority;
    data.m_is_precondition_monster = is_monster;
    data.m_job_id.m_section_name = xr_string("logic@").append(work_field_name);
    data.m_job_id.m_ini_path_name = ini_path_name;
    data.m_job_id.m_job_type = job_type_name;
    data.m_job_id.m_online_name = job_online_name;
    data.m_job_id.m_ini_file = job_ini_file;
    data.m_function_params.second = condlist_data;
    data.m_function = [](CSE_ALifeDynamicObject* server_object, Script_SE_SmartTerrain* smart,
                          const std::pair<xr_string, xr_map<std::uint32_t, CondlistData>>& params) -> bool {
        xr_string result = XR_LOGIC::pick_section_from_condlist(
            DataBase::Storage::getInstance().getActor(), server_object, params.second);

        if (result == "false" || !result.size())
            return false;

        return true;
    };

    all_jobs.second.push_back(data);

    // Затычка для оффлайна, когда невозможно проверить прекондишн
    JobDataExclusive data1;
    data1.m_priority = -1;
    data1.m_is_precondition_monster = is_monster;
    data1.m_job_id.m_section_name = xr_string("logic@").append(work_field_name);
    data1.m_job_id.m_job_type = job_type_name;
    data1.m_job_id.m_ini_file = job_ini_file;

    all_jobs.second.push_back(data1);
}

} // namespace GulagGenerator
} // namespace Scripts
} // namespace Cordis

#include "Script_SE_SmartTerrain.h"
