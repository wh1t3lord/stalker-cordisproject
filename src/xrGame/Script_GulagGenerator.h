#pragma once

namespace Cordis
{
namespace Scripts
{
namespace GulagGenerator
{
constexpr const char* GULAG_POINT_JOB = "point_job";

struct JobData
{
    bool m_precondition_is_monster;
    std::uint32_t m_priority;
    // std::pair<m_prior, std::pair<m_prior, std::pair<m_section, m_job_type>>>
    std::pair<std::uint32_t, xr_vector<std::pair<std::uint32_t, std::pair<xr_string, xr_string>>>> m_jobs;
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

    std::pair<std::uint32_t, xr_vector<std::pair<std::uint32_t, std::pair<xr_string, xr_string>>>>
        stalker_generic_point;
    stalker_generic_point.first = 3; // prior

    for (std::uint8_t i = 0; i < 20; ++i)
    {
        xr_string name = global_name;
        xr_string job_ltx_data = "";
        name += "_point_";
        name += std::to_string(i).c_str();

        std::pair<std::uint32_t, std::pair<xr_string, xr_string>> data;
        data.first = 3; // prior
        data.second.first = "logic@"; // section
        data.second.first += name;
        data.second.second = GULAG_POINT_JOB;

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
    }
}

} // namespace GulagGenerator
} // namespace Scripts
} // namespace Cordis
