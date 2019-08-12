#pragma once

namespace Cordis
{
namespace Scripts
{
namespace GulagGenerator
{
constexpr const char* GULAG_POINT_JOB = "point_job";
constexpr const char* GULAG_PATH_JOB = "path_job";

struct JobData
{
    struct SubData
    {
        std::uint32_t m_priority;
        // @ Section | Job type
        std::pair<xr_string, xr_string> m_job_id;
        std::pair<xr_string, xr_map<std::uint32_t, CondlistData>> m_function_params;
        std::function<bool(CSE_ALifeDynamicObject*, Script_SE_SmartTerrain*,
            const std::pair<xr_string, xr_map<std::uint32_t, CondlistData>>& params, const NpcInfo&)>
            m_function;
    };

    bool m_precondition_is_monster;
    std::uint32_t m_priority;
    // std::pair<m_prior, std::pair<m_prior, std::pair<m_section, m_job_type>>>
    xr_vector<std::pair<std::uint32_t, xr_vector<SubData>>> m_jobs;
};

inline static xr_string& getLtx(void) noexcept
{
    static xr_string instance;
    return instance;
}

inline bool load_job(Script_SE_SmartTerrain* smart)
{
    if (!smart)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    Msg("[Scripts/GulagGenerator/load_job(smart)] LOAD JOB %s", smart->name());
    xr_string global_name = smart->name();
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

    JobData stalker_jobs;
    stalker_jobs.m_precondition_is_monster = false;
    stalker_jobs.m_priority = 60;

    std::pair<std::uint32_t, xr_vector<JobData::SubData>> stalker_generic_point;
    stalker_generic_point.first = 3; // prior

    for (std::uint8_t i = 0; i < 20; ++i)
    {
        xr_string name = global_name;
        xr_string job_ltx_data = "";
        name += "_point_";
        name += std::to_string(i).c_str();

        JobData::SubData data;
        //         data.first = 3; // prior
        //         data.second.first = "logic@"; // section
        //         data.second.first += name;
        //         data.second.second = GULAG_POINT_JOB;
        data.m_priority = 3;
        data.m_job_id.first = "logic@";
        data.m_job_id.first += name;
        data.m_job_id.second += GULAG_POINT_JOB;

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

        if (smart->getBaseOnActorControl()->getIgnoreZoneName().size())
        {
            job_ltx_data += "combat_ignore_cond = {=npc_in_zone(smart.base_on_actor_control.ignore_zone)} true \n";
            job_ltx_data += "combat_ignore_keep_when_attacked = true \n";
        }

        getLtx() += job_ltx_data;
    }

    stalker_jobs.m_jobs.push_back(stalker_generic_point);
#pragma region SURGE MANAGMENT
    std::pair<std::uint32_t, xr_vector<JobData::SubData>> stalker_surge;
    stalker_surge.first = 50;

    std::uint8_t it = 1;
    xr_string patrol_path_name = global_name;
    patrol_path_name += "_surge_";
    patrol_path_name += std::to_string(it).c_str();
    patrol_path_name += "_walk";
    while (Globals::patrol_path_exists(patrol_path_name.c_str()))
    {
        JobData::SubData data;
        xr_string waypoint_name = global_name;
        waypoint_name += "_surge_";
        waypoint_name += std::to_string(it).c_str();
        waypoint_name += "_walk";

        data.m_priority = 50;
        data.m_job_id.first = "logic@";
        data.m_job_id.first += waypoint_name;
        data.m_job_id.second += GULAG_PATH_JOB;
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
        job_ltx_data += "meet = meet@generic_larger\n";
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

        // Lord: XR_GULAG::job_in_restrictor реализовать!!!
        if (smart->getBaseOnActorControl()->getIgnoreZoneName().size() /*&& XR_GULAG::job_in_restrictor()*/)
        {
            job_ltx_data += "combat_ignore_cond = {=npc_in_zone(";
            job_ltx_data += smart->getBaseOnActorControl()->getIgnoreZoneName();
            job_ltx_data += ")} true \n";
            job_ltx_data += "combat_ignore_keep_when_attacked = true \n";
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
    std::pair<std::uint32_t, xr_vector<JobData::SubData>> stalker_sleep;
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

        JobData::SubData data;
        data.m_priority = 10;
        data.m_job_id.first = "logic@";
        data.m_job_id.first += waypoint_name;
        data.m_job_id.second += GULAG_PATH_JOB;
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

            // @ Lord: реализовать здесь XR_GULAG::job_in_restrictor
            // return XR_GULAG::job_in_restrictor();
            return false;
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

        // @ Lord: XR_GULAG::job_in_restrictor implement
        if (smart->getSafeRestrictor().size() /*&& XR_GULAG::job_in_restrictor()*/)
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

        // @ Lord: XR_GULAG::job_in_restrictor implement
        if (smart->getBaseOnActorControl()->getIgnoreZoneName().size() /*&& XR_GULAG::job_in_restrictor()*/)
        {
            job_ltx_data += "combat_ignore_cond = {=npc_in_zone(";
            job_ltx_data += smart->getBaseOnActorControl()->getIgnoreZoneName();
            job_ltx_data += ")} true \n";
            job_ltx_data += "combat_ignore_keep_when_attacked = true \n";
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
    std::pair<std::uint32_t, xr_vector<JobData::SubData>> stalker_collector;
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

        JobData::SubData data;
        data.m_priority = 25;
        data.m_job_id.first = "logic@";
        data.m_job_id.first += waypoint_name;
        data.m_job_id.second = GULAG_PATH_JOB;
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

            DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage()[server_object->ID];

            if (!storage.m_object)
                return false;

            CScriptGameObject* npc = storage.m_object;

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

        // @ Lord: Implement XR_GULAG::job_in_restrictor();
        if (smart->getSafeRestrictor().size() /*&& XR_GULAG::job_in_restrictor()*/)
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

        // @ Lord: Implement XR_GULAG::job_in_restrictor();
        if (smart->getBaseOnActorControl()->getIgnoreZoneName().size() /*&& XR_GULAG::job_in_restrictor()*/)
        {
            job_ltx_data += "combat_ignore_cond = {=npc_in_zone(";
            job_ltx_data += smart->getBaseOnActorControl()->getIgnoreZoneName();
            job_ltx_data += ")} true \n";
            job_ltx_data += "combat_ignore_keep_when_attacked = true \n";
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


#pragma endregion
}

} // namespace GulagGenerator
} // namespace Scripts
} // namespace Cordis
