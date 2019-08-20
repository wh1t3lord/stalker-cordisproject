#pragma once

#include "Script_SmartTerrainControl.h"

namespace Cordis
{
namespace Scripts
{
struct NpcInfo
{ // @ Lord: пока что не вся заполнена!
    bool m_is_monster;
    bool m_begin_job;
    std::uint32_t m_stype;
    std::uint32_t m_job_prioprity;
    std::uint32_t m_job_id;
    CSE_ALifeDynamicObject* m_server_object; // @ Lord: определить где оно именно удаляется в итоге
    xr_string m_need_job;
};

struct JobDataSmartTerrain
{
    std::uint32_t m_priority;
    CALifeSmartTerrainTask* m_alife_task = nullptr;
    // @ First - section | Second - job_type (that taking from gulag_general as JobData::SubData::m_job_id respectively)
    std::pair<xr_string, xr_string> m_job_id;
};

class Script_SE_SmartTerrain : public CSE_ALifeSmartZone
{
    using inherited = CSE_ALifeSmartZone;

public:
    Script_SE_SmartTerrain(LPCSTR section);
    virtual ~Script_SE_SmartTerrain(void);

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
        data.m_job_id = -1;
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
    inline void setDefenceRestrictor(const xr_string& string) noexcept { this->m_defence_restictor = string; }
    inline void setAttackRestrictor(const xr_string& string) noexcept { this->m_attack_restrictor = string; }
    inline void setSafeRestrictor(const xr_string& string) noexcept { this->m_safe_restirctor = string; }

    void read_params(void);

private:
    bool m_is_initialized;
    bool m_is_registered;
    std::uint32_t m_population;
    int a = sizeof(xrTime);
    xrTime m_smart_alarm_time;
    Script_SmartTerrainControl* m_base_on_actor_control;
    xr_map<std::uint32_t, xrTime> m_dead_time;
    xr_map<xr_string, std::uint32_t> m_npc_by_job_section;
    xr_map<std::uint32_t, CSE_ALifeDynamicObject*> m_arriving_npc;
    xr_vector<CSE_ALifeDynamicObject*> m_npc_to_register;
    xr_string m_smart_level;
    xr_string m_defence_restictor;
    xr_string m_attack_restrictor;
    xr_string m_safe_restirctor;
};
} // namespace Scripts
} // namespace Cordis
