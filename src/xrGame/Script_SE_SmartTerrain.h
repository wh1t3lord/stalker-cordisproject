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
struct JobData_SubData;
struct JobDataExclusive;

// @ For smart terrain stuff
struct JobData_SubData
{
    std::uint16_t m_npc_id = 0; // @ Используется для выдачи работы НПС если 0 то работа свободна!
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
        /*
                this->m_priority = Globals::kUnsignedInt32Undefined;
                this->m_job_index = Globals::kUnsignedInt32Undefined;*/
        this->m_job_id.first.clear();
        this->m_job_id.second.clear();
        /*
                this->m_function_params.first.clear();
                this->m_function_params.second.clear();
                this->m_function = nullptr;*/
    }
};

struct NpcInfo
{ // @ Lord: пока что не вся заполнена!
    bool m_is_monster;
    bool m_begin_job;
	int m_job_prioprity;
	int m_stype;
    std::uint32_t m_job_id;
    CSE_ALifeDynamicObject* m_server_object; // @ Lord: определить где оно именно удаляется в итоге
	JobData_SubData* m_job_link1 = nullptr;
	JobDataExclusive* m_job_link2 = nullptr;
    xr_string m_need_job;
    // Как бы у нас может быть только одна работа, и учитываем это всегда то есть одно из m_job_link будет отлично от
    // nullptr



    inline void clear(void)
    {
        this->m_is_monster = false;
        this->m_begin_job = false;
        this->m_stype = Globals::kUnsignedInt32Undefined;
        this->m_job_prioprity = Globals::kUnsignedInt32Undefined;
        this->m_server_object = nullptr;
        this->m_need_job.clear();
        this->m_job_link1 = nullptr;
        this->m_job_link2 = nullptr;
    }
};

struct JobData
{
    bool m_precondition_is_monster;
    std::uint32_t m_priority;
    // std::pair<m_prior, std::pair<m_prior, std::pair<m_section, m_job_type>>>
    xr_vector<std::pair<std::uint32_t, xr_vector<JobData_SubData>>> m_jobs;
};

// WARNING!
// Don't use it directly! using JobDataExclusive* data structure!
struct JobDataExclusive_SubData
{
    ~JobDataExclusive_SubData(void)
    {
        if (!m_has_previous)
            if (this->m_ini_file)
                xr_delete(this->m_ini_file);
    }

    inline void setDeallocationChecker(const xr_string& comparator) noexcept
    {
        this->m_has_previous = (this->m_section_name == comparator);
    }

    inline void clear(void) noexcept
    {
        this->m_section_name.clear();
        this->m_job_type.clear();
    }

    CScriptIniFile* m_ini_file;
    xr_string m_section_name;
    xr_string m_online_name;
    xr_string m_job_type;
    xr_string m_ini_path_name;

private:
    bool m_has_previous = false; // @ system value for deallocation, don't touch it
};

struct JobDataExclusive
{
    ~JobDataExclusive(void) {}

    bool m_is_precondition_monster;
    std::uint16_t m_npc_id; // @ Используется для выдачи работы если 0 работа свободна!
    std::uint32_t m_priority;
    std::uint32_t m_job_index = Globals::kUnsignedInt32Undefined;
    std::pair<xr_string, xr_map<std::uint32_t, CondlistData>> m_function_params;
    std::function<bool(CSE_ALifeDynamicObject*, Script_SE_SmartTerrain*,
        const std::pair<xr_string, xr_map<std::uint32_t, CondlistData>>& params)>
        m_function;
    JobDataExclusive_SubData m_job_id;
};

// Lord: проверить выравнивание!!!!!!
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
    virtual void STATE_Read(NET_Packet& packet, u16 size);
    virtual void STATE_Write(NET_Packet& packet);
    virtual void register_npc(CSE_ALifeMonsterAbstract* object);
    virtual void unregister_npc(CSE_ALifeMonsterAbstract* object);
    virtual CALifeSmartTerrainTask* task(CSE_ALifeMonsterAbstract* object);
    void update(void) override;
    bool target_precondition(CSE_ALifeObject* squad, bool is_need_to_dec_population);

    inline NpcInfo fill_npc_info(CSE_ALifeDynamicObject* server_object)
    {
        if (!server_object)
        {
            R_ASSERT2(false, "object was null!");
            return NpcInfo();
        }

        NpcInfo data;

        data.m_is_monster = Globals::IsStalker(server_object, 0) ? false : true;
        data.m_server_object = server_object;
        data.m_need_job = "nil"; // LorD: проверить будет ли дропать nil, если будет то найти и исправить когда это будет, чтобы все "nil" просто проверялись всегда как .empty()
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
    inline bool IsDisabled(void) const noexcept { return this->m_is_disabled; }

    inline xr_string getDefenceRestirctor(void) noexcept { return this->m_defence_restictor; }
    inline xr_string getAttackRestrictor(void) noexcept { return this->m_attack_restrictor; }
    inline xr_string getSafeRestrictor(void) noexcept { return this->m_safe_restirctor; }
    inline Script_SmartTerrainControl* getBaseOnActorControl(void) noexcept
    {
        return this->m_base_on_actor_control.get();
    }
    inline xrTime& getSmartAlarmTime(void) noexcept { return this->m_smart_alarm_time; }
    inline xr_map<std::uint32_t, xrTime>& getDeadTime(void) noexcept { return this->m_dead_time; }
    inline std::uint16_t getIDNPCOnJob(const xr_string& job_name) noexcept
    {
        return this->m_npc_by_job_section[job_name];
    }
    inline std::uint16_t getSquadID(void) noexcept { return this->m_squad_id; }
    inline xr_string getSpawnPointName(void) noexcept { return this->m_spawn_point_name; }
    inline std::uint32_t getStaydSquadQuan(void) noexcept { return this->m_stayed_squad_quan; }
    inline xr_map<std::uint32_t, NpcInfo>& getNpcInfo(void) noexcept
    {
        return this->m_npc_info;
    } // @ Lord: const return!!!
    inline xr_map<std::uint32_t, JobDataSmartTerrain*>& getJobData(void) noexcept { return this->m_job_data; }
    inline CALifeSmartTerrainTask* getAlifeSmartTerrainTask(void) { return this->m_smart_alife_task.get(); }
    inline CScriptIniFile* getIni(void) const noexcept { return this->m_ini.get(); }
    inline const xr_map<xr_string, std::uint8_t>& getAlreadySpawned(void) const noexcept
    {
        return this->m_already_spawned;
    }

    inline void setAlreadySpawned(const xr_string& section_name, const std::uint8_t value) noexcept
    {
        if (section_name.empty())
        {
            Msg("[Scripts/Script_SE_SmartTerrain/setAlreadySpawned(section_name, value)] WARNING: section_name.empty() "
                "== true! Can't assign!");
            return;
        }

        this->m_already_spawned[section_name] = value;
    }

    inline const xr_map<std::uint32_t, CSE_ALifeDynamicObject*>& getArrivingNpc(void) const { return this->m_arriving_npc; }
    inline void setArrivingNpc(const std::uint32_t id, CSE_ALifeDynamicObject* const p_object)
    {
        if (id >= Globals::kUnsignedInt16Undefined)
        {
#ifdef DEBUG
            MESSAGE("Something wrong with your id!");
#endif // DEBUG
            return;
        }

        this->m_arriving_npc[id] = p_object;
    }
#pragma endregion

#pragma region Cordis Setters
    inline void setDefenceRestrictor(const xr_string& string) noexcept { this->m_defence_restictor = string; }
    inline void setAttackRestrictor(const xr_string& string) noexcept { this->m_attack_restrictor = string; }
    inline void setSafeRestrictor(const xr_string& string) noexcept { this->m_safe_restirctor = string; }
    inline void setStaydSquadQuan(const std::uint32_t& value) noexcept { this->m_stayed_squad_quan = value; }
    inline void set_alarm(void) noexcept { this->m_smart_alarm_time = Globals::Game::get_game_time(); }
#pragma endregion

    bool am_i_reached(Script_SE_SimulationSquad* squad);

    void read_params(void);
    void on_after_reach(Script_SE_SimulationSquad* squad);
    void on_reach_target(Script_SE_SimulationSquad* squad);
    void clear_dead(CSE_ALifeDynamicObject* server_object);
    void hide(void);
    void check_respawn_params(xr_string& params);
    void select_npc_job(NpcInfo& npc_info);
    void switch_to_desired_job(CScriptGameObject* const p_npc);
    void setup_logic(CScriptGameObject* const p_npc);
    void init_npc_after_load(void);
    float evaluate_prior(Script_SE_SimulationSquad* const p_squad);
private:
    void show(void);
    void load_jobs(void);
    void update_jobs(void);

private:
    bool m_is_initialized;
    bool m_is_registered;
    bool m_is_smart_showed_spot;
    bool m_is_need_init_npc;
    bool m_is_disabled;
    bool m_is_respawn_point;
    bool m_is_mutant_lair;
    bool m_is_no_mutant;
    bool m_is_respawn_only_smart;
    bool m_is_campfires_on;
    std::uint16_t m_squad_id;
    std::uint16_t m_respawn_radius;
    std::uint32_t m_max_population;
    std::uint32_t m_arrive_distance;
    std::uint32_t m_population;
    std::uint32_t m_stayed_squad_quan;
    std::uint32_t m_show_time;
    std::uint32_t m_check_time;
    CScriptIniFile* m_ltx;
    xrTime m_smart_alarm_time;
    std::pair<xr_vector<JobData>, xr_vector<JobDataExclusive*>>
        m_jobs; // @ Состоит из двух векторов впервый заносятся stalker_jobs, и monster_jobs, во второй вектор заносятся
                // exclusive работы
    std::unique_ptr<CALifeSmartTerrainTask> m_smart_alife_task;
    //  Script_SmartTerrainControl* m_base_on_actor_control;
    std::unique_ptr<Script_SmartTerrainControl> m_base_on_actor_control;
    std::unique_ptr<CScriptIniFile> m_ini;
    xr_map<std::uint32_t, xrTime> m_dead_time;
    xr_map<xr_string, std::uint16_t> m_npc_by_job_section;
    // pair<vector_spawn_squads_name, condlist_spawn_num>!
    xr_map<xr_string, std::pair<xr_vector<xr_string>, xr_map<std::uint32_t, CondlistData>>> m_respawn_params;
    xr_map<xr_string, std::uint8_t> m_already_spawned;
    xr_map<std::uint32_t, CSE_ALifeDynamicObject*> m_arriving_npc;
    xr_map<std::uint32_t, NpcInfo> m_npc_info;
    xr_map<std::uint32_t, JobDataSmartTerrain*> m_job_data;
    xr_map<std::uint32_t, CondlistData> m_respawn_sector_condlist;
    xr_vector<CSE_ALifeDynamicObject*> m_npc_to_register;
    xrTime m_last_respawn_update;
    xr_string m_smart_level;
    xr_string m_defence_restictor;
    xr_string m_attack_restrictor;
    xr_string m_safe_restirctor;
    xr_string m_spawn_point_name;
    xr_string m_smart_showed_spot_name;
    xr_string m_simulation_type_name;
    xr_string m_player_name;
    xr_string m_fobidden_point_name;
    xr_string m_traveller_actor_path_name;
    xr_string m_traveller_squad_path_name;
    xr_string m_ltx_name;
};
} // namespace Scripts
} // namespace Cordis
