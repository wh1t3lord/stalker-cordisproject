#pragma once

#include "Script_SmartTerrainControl.h"
/*#include "Script_GulagGenerator.h"*/

namespace Cordis
{
namespace Scripts
{
class Script_SE_SimulationSquad;
}
} // namespace Cordis

namespace Cordis
{
namespace Scripts
{
struct NpcInfo;

struct JobData_SubData
{
    std::uint32_t m_priority;
    std::uint32_t m_job_index = Globals::kUnsignedInt32Undefined; // @ Lord: делаем так специально, потом пересмотреть
                                                                  // архитектуру Script_SE_SmartTerrain и GulagGenerator
    // @ Section | Job type
    std::pair<xr_string, xr_string> m_job_id;
    std::pair<xr_string, xr_map<std::uint32_t, CondlistData>> m_function_params;
    std::function<bool(CSE_ALifeDynamicObject*, Script_SE_SmartTerrain*,
        const std::pair<xr_string, xr_map<std::uint32_t, CondlistData>>& params, const NpcInfo&)>
        m_function;

    inline void clear(void)
    {
        this->m_priority = Globals::kUnsignedInt32Undefined;
        this->m_job_index = Globals::kUnsignedInt32Undefined;
        this->m_job_id.first.clear();
        this->m_job_id.second.clear();
        this->m_function_params.first.clear();
        this->m_function_params.second.clear();
        this->m_function = nullptr;
    }
};

struct NpcInfo
{ // @ Lord: пока что не вся заполнена!
    bool m_is_monster;
    bool m_begin_job;
    std::uint32_t m_stype;
    std::uint32_t m_job_prioprity;
    CSE_ALifeDynamicObject* m_server_object; // @ Lord: определить где оно именно удаляется в итоге
    xr_string m_need_job;
    JobData_SubData m_job_link;

    inline void clear(void)
    {
        this->m_is_monster = false;
        this->m_begin_job = false;
        this->m_stype = Globals::kUnsignedInt32Undefined;
        this->m_job_prioprity = Globals::kUnsignedInt32Undefined;
        this->m_server_object = nullptr;
        this->m_need_job.clear();
        this->m_job_link.clear();
    }
};

struct JobData
{
    bool m_precondition_is_monster;
    std::uint32_t m_priority;
    // std::pair<m_prior, std::pair<m_prior, std::pair<m_section, m_job_type>>>
    xr_vector<std::pair<std::uint32_t, xr_vector<JobData_SubData>>> m_jobs;
};

struct JobDataExclusive_SubData
{
    CScriptIniFile m_ini_file = CScriptIniFile("system.ltx");
    xr_string m_section_name;
    xr_string m_online_name;
    xr_string m_job_type;
    xr_string m_ini_path_name;
};

struct JobDataExclusive
{
    bool m_is_precondition_monster;
    std::uint32_t m_priority;
    std::pair<xr_string, xr_map<std::uint32_t, CondlistData>> m_function_params;
    std::function<bool(CSE_ALifeDynamicObject*, Script_SE_SmartTerrain*,
        const std::pair<xr_string, xr_map<std::uint32_t, CondlistData>>& params)>
        m_function;
    JobDataExclusive_SubData m_job_id;
};

struct JobDataSmartTerrain
{
    std::uint32_t m_priority;
    CALifeSmartTerrainTask* m_alife_task = nullptr;
    // @ First - section | Second - job_type (that taking from gulag_general as JobData_SubData::m_job_id respectively)
    std::pair<xr_string, xr_string> m_job_id;
};

class Script_SE_SmartTerrain : public CSE_ALifeSmartZone
{
    using inherited = CSE_ALifeSmartZone;

public:
    Script_SE_SmartTerrain(LPCSTR section);
    virtual ~Script_SE_SmartTerrain(void);

    virtual Script_SE_SmartTerrain* cast_script_se_smartterrain(void) { return this; }
    virtual void on_before_register(void);
    virtual void on_register(void);
    virtual void on_unregister(void);
    inline NpcInfo fill_npc_info(CSE_ALifeDynamicObject* server_object)
    {
        if (!server_object)
        {
            R_ASSERT2(false, "object was null!");
            return NpcInfo();
        }

        NpcInfo data;

        data.m_is_monster = Globals::IsStalker(server_object, 0);
        data.m_server_object = server_object;
        data.m_need_job = "nil";
        data.m_job_prioprity = -1;
        data.m_begin_job = false;

        if (data.m_is_monster)
            data.m_stype = Globals::kSTypeMobile;
        else
            data.m_stype = Globals::kSTypeStalker;

        return data;
    }

    inline void refresh_script_logic(const std::uint32_t& object_id)
    {
        CSE_ALifeDynamicObject* server_object = ai().alife().objects().object(object_id);
        std::uint16_t stype = Globals::kSTypeMobile;

        if (Globals::IsStalker(server_object, 0))
            stype = Globals::kSTypeStalker;
    }

    inline void refresh(void) { this->show(); }

#pragma region Cordis Getters
    inline xr_string getDefenceRestirctor(void) noexcept { return this->m_defence_restictor; }
    inline xr_string getAttackRestrictor(void) noexcept { return this->m_attack_restrictor; }
    inline xr_string getSafeRestrictor(void) noexcept { return this->m_safe_restirctor; }
    inline Script_SmartTerrainControl* getBaseOnActorControl(void) noexcept { return this->m_base_on_actor_control; }
    inline xrTime& getSmartAlarmTime(void) noexcept { return this->m_smart_alarm_time; }
    inline xr_map<std::uint32_t, xrTime>& getDeadTime(void) noexcept { return this->m_dead_time; }
    inline std::uint32_t getIDNPCOnJob(const xr_string& job_name) noexcept
    {
        return this->m_npc_by_job_section[job_name];
    }
    inline std::uint16_t getSquadID(void) noexcept { return this->m_squad_id; }
    inline CInifile& getIni(void) noexcept { return this->spawn_ini(); }
    inline xr_string getSpawnPointName(void) noexcept { return this->m_spawn_point_name; }
    inline std::uint32_t getStaydSquadQuan(void) noexcept { return this->m_stayed_squad_quan; }
    inline xr_map<std::uint32_t, NpcInfo>& getNpcInfo(void) noexcept { return this->m_npc_info; }
    inline xr_map<std::uint32_t, JobDataSmartTerrain>& getJobData(void) noexcept { return this->m_job_data; }
    inline CALifeSmartTerrainTask* getAlifeSmartTerrainTask(void) { return this->m_smart_alife_task.get();
    }
#pragma endregion

#pragma region Cordis Setters
    inline void setDefenceRestrictor(const xr_string& string) noexcept { this->m_defence_restictor = string; }
    inline void setAttackRestrictor(const xr_string& string) noexcept { this->m_attack_restrictor = string; }
    inline void setSafeRestrictor(const xr_string& string) noexcept { this->m_safe_restirctor = string; }
    inline void setStaydSquadQuan(const std::uint32_t& value) noexcept { this->m_stayed_squad_quan = value; }
#pragma endregion

    void read_params(void);
    void on_after_reach(Script_SE_SimulationSquad* squad);
    void on_reach_target(Script_SE_SimulationSquad* squad);
    void clear_dead(CSE_ALifeDynamicObject* server_object);
    void hide(void);

private:
    void show(void);
    void load_jobs(void);

private:
    bool m_is_initialized;
    bool m_is_registered;
    bool m_is_smart_showed_spot;
    bool m_is_need_init_npc;
    std::uint16_t m_squad_id;
    std::uint32_t m_population;
    std::uint32_t m_stayed_squad_quan;
    std::uint32_t m_show_time;
    xrTime m_smart_alarm_time;
    std::unique_ptr<CALifeSmartTerrainTask> m_smart_alife_task;
    Script_SmartTerrainControl* m_base_on_actor_control;
    xr_map<std::uint32_t, xrTime> m_dead_time;
    xr_map<xr_string, std::uint32_t> m_npc_by_job_section;
    xr_map<std::uint32_t, CSE_ALifeDynamicObject*> m_arriving_npc;
    xr_map<std::uint32_t, NpcInfo> m_npc_info;
    xr_map<std::uint32_t, JobDataSmartTerrain> m_job_data;
    xr_vector<CSE_ALifeDynamicObject*> m_npc_to_register;
    xr_string m_smart_level;
    xr_string m_defence_restictor;
    xr_string m_attack_restrictor;
    xr_string m_safe_restirctor;
    xr_string m_spawn_point_name;
    xr_string m_smart_showed_spot_name;
    xr_string m_simulation_type_name;
    xr_string m_player_name;
};
} // namespace Scripts
} // namespace Cordis
