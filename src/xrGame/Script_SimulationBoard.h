#pragma once

#include "xrCore/xrCore.h"
#include "xrServer_Objects_Alife.h"
#include "xrCore/xr_ini.h"

namespace Cordis
{
namespace Scripts
{
    // Lord: доделать этот скрипт!

struct SimulationActivitiesType
{
    // @ Это же относится для m_squad
    enum simulation_activities_id
    {
        stalker,
        bandit,
        dolg,
        freedom,
        killer,
        zombied,
        monster_predatory_day,
        monster_predatory_night,
        monster_vegetarian,
        monster_zombied_day,
        monster_zombied_night,
        monster_special
    };

    enum smart_id
    {
        base,
        territory,
        surge,
        resource,
        lair
    };

    xr_map<std::uint32_t, std::function<bool(CSE_ALifeObject*, CSE_ALifeObject*)>> m_squad;
    xr_map<std::uint32_t, std::function<bool(CSE_ALifeObject*, CSE_ALifeObject*)>> m_smart;
    std::function<bool(CSE_ALifeObject*, CSE_ALifeObject*)> m_actor;
};

class Script_SimulationBoard
{
private:
    Script_SimulationBoard(void) : m_setting_ini("misc\\simulation.ltx")
    {
        this->m_squad_ltx = Globals::get_system_ini();

        SimulationActivitiesType _stalker; // Lord: немного проебался там где используется аргумент squad заменить на Script_SimulationSquad*
        _stalker.m_smart[SimulationActivitiesType::base] = [](CSE_ALifeObject* squad, CSE_ALifeObject* target) -> bool {

        };

        this->m_simulation_activities[SimulationActivitiesType::stalker];




    }

public:
    inline static Script_SimulationBoard& getInstance(void) noexcept
    {
        static Script_SimulationBoard instance;
        return instance;
    }

    ~Script_SimulationBoard(void);
    Script_SimulationBoard(const Script_SimulationBoard&) = delete;
    Script_SimulationBoard& operator=(const Script_SimulationBoard&) = delete;
    Script_SimulationBoard(Script_SimulationBoard&&) = delete;
    Script_SimulationBoard& operator=(Script_SimulationBoard&&) = delete;

    void setActorCommunity();

private:
    enum group_id_by_levels
    {
        zaton = 1,
        pripyat,
        jupiter,
        labx8,
        jupiter_underground
    };
    const CInifile* m_squad_ltx;
    xr_map<std::uint32_t, SimulationActivitiesType> m_simulation_activities;
    CInifile m_setting_ini;
};
} // namespace Scripts
} // namespace Cordis
