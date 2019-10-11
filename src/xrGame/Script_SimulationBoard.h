#pragma once
#include "xrCore/xrCore.h"
#include "xrServer_Objects_Alife.h"
#include "xrCore/xr_ini.h"
/*#include "Script_Globals.h"*/
#include "Script_SE_SimulationSquad.h"

namespace Cordis
{
namespace Scripts
{
// Lord: доделать этот скрипт!

// Lord: подумать над названием константы правильно ли?
constexpr int kSimulationBoardSimulationDistance = 150;
// @ Exceptional data structure without any get/set method
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

    xr_map<std::uint32_t, std::function<bool(CSE_ALifeOnlineOfflineGroup*, CSE_ALifeObject*)>> m_squad;
    xr_map<std::uint32_t, std::function<bool(CSE_ALifeOnlineOfflineGroup*, CSE_ALifeObject*)>> m_smart;
    std::function<bool(CSE_ALifeOnlineOfflineGroup*, CSE_ALifeObject*)> m_actor;
};

struct SmartDataSimulationBoard
{
    inline const xr_map<std::uint32_t, Script_SE_SimulationSquad*>& getSquads(void) const noexcept
    {
        return this->m_squads;
    }

    inline void setSquads(const xr_map<std::uint32_t, Script_SE_SimulationSquad*>& map)
    {
        if (!map.size())
        {
            Msg("[SmartDataSimulationBoard/setSquads(map)] WARNING: map.size() = 0! You are trying to set an empty "
                "map! No assignment!");
            return;
        }

        this->m_squads = map;
    }

    inline void setSquads(const std::pair<std::uint32_t, Script_SE_SimulationSquad*>& pair)
    {
        if (pair.first == Globals::kUnsignedInt32Undefined)
        {
            Msg("[SmartDataSimulationBoard/setSquads(pair)] WARNING: pair.first = std::uint32_t(-1)! You are trying to "
                "set an undefined value! No assignment!");
            return;
        }

        if (!pair.second)
        {
            Msg("[SmartDataSimulationBoard/setSquads(pair)] WARNING: pair.second = null! You are trying to set an "
                "empty pair! No assignment!");
            return;
        }
    }

    inline void setSquads(const std::uint32_t& id, Script_SE_SimulationSquad* p_server_object)
    {
        if (id == Globals::kUnsignedInt32Undefined)
        {
            Msg("[SmartDataSimulationBoard/setSquads(id, p_server_object)] WARNING: id = std::uint32_t(-1)! You are trying to set an "
                "undefined value! No assignment!");
            return;
        }

        if (!p_server_object)
        {
            Msg("[SmartDataSimulationBoard/setSquads(id, p_server_object)] p_server_object = null!");
      //      return;
        }

        this->m_squads[id] = p_server_object;
    }

    inline const std::uint32_t& getStayedSquadQuan(void) const noexcept { return this->m_stayed_squad_quan; }
    inline void setStayedSquadQuan(const std::uint32_t& value) noexcept { this->m_stayed_squad_quan = value; }
    inline Script_SE_SmartTerrain* getServerSmartTerrain(void) const { return this->m_p_server_smart; }

private:
    std::uint32_t m_stayed_squad_quan = 0;
    Script_SE_SmartTerrain* m_p_server_smart = nullptr;
    xr_map<std::uint32_t, Script_SE_SimulationSquad*> m_squads;
};

// @ WA - without arguments
class Script_SimulationBoard
{
private:
    Script_SimulationBoard(void) : m_setting_ini("misc\\simulation.ltx"), m_is_simulation_started(true)
    {
        this->m_squad_ltx = Globals::get_system_ini();
        this->m_simulation_activities[SimulationActivitiesType::stalker].m_smart[SimulationActivitiesType::base] =
            [](CSE_ALifeOnlineOfflineGroup* squad, CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return ((Globals::in_time_interval(18, 8) && (!XR_CONDITION::is_surge_started())) &&
                (Globals::check_squad_for_enemies(squad)) &&
                (target->name() == "zat_stalker_base_smart" || target->name() == "jup_a6" ||
                    target->name() == "pri_a16"));
        };

        // @ WA
        this->m_simulation_activities[SimulationActivitiesType::stalker].m_smart[SimulationActivitiesType::surge] =
            [](CSE_ALifeOnlineOfflineGroup* squad, CSE_ALifeObject* target) -> bool {
            return XR_CONDITION::is_surge_started();
        };

        // @ WA
        this->m_simulation_activities[SimulationActivitiesType::stalker].m_smart[SimulationActivitiesType::territory] =
            [](CSE_ALifeOnlineOfflineGroup* squad, CSE_ALifeObject* target) -> bool {
            return Globals::in_time_interval(8, 18) && (!XR_CONDITION::is_surge_started());
        };

        // @ WA
        this->m_simulation_activities[SimulationActivitiesType::stalker].m_smart[SimulationActivitiesType::resource] =
            [](CSE_ALifeOnlineOfflineGroup* squad, CSE_ALifeObject* target) -> bool {
            return Globals::in_time_interval(8, 18) && (!XR_CONDITION::is_surge_started());
        };

        this->m_simulation_activities[SimulationActivitiesType::stalker].m_actor = nullptr;

        this->m_simulation_activities[SimulationActivitiesType::bandit].m_squad[SimulationActivitiesType::stalker] =
            [](CSE_ALifeOnlineOfflineGroup* squad, CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "Object is null!");
                return false;
            }

            return Globals::in_time_interval(8, 21) &&
                (!XR_CONDITION::is_surge_started() &&
                    (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance));
        };

        this->m_simulation_activities[SimulationActivitiesType::bandit].m_smart[SimulationActivitiesType::base] =
            [](CSE_ALifeOnlineOfflineGroup* squad, CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "Object is null!");
                return false;
            }

            return Globals::in_time_interval(21, 8) &&
                (!XR_CONDITION::is_surge_started() && (!Globals::check_squad_for_enemies(squad)) &&
                    (target->name() == "zat_stalker_base_smart" || target->name() == "jup_a10_smart_terrain"));
        };

        // @ WA
        this->m_simulation_activities[SimulationActivitiesType::bandit].m_smart[SimulationActivitiesType::territory] =
            [](CSE_ALifeOnlineOfflineGroup* squad, CSE_ALifeObject* target) -> bool {
            return Globals::in_time_interval(8, 21) && (!XR_CONDITION::is_surge_started());
        };

        // @ WA
        this->m_simulation_activities[SimulationActivitiesType::bandit].m_smart[SimulationActivitiesType::surge] =
            [](CSE_ALifeOnlineOfflineGroup* squad, CSE_ALifeObject* target) -> bool {
            return XR_CONDITION::is_surge_started();
        };

        this->m_simulation_activities[SimulationActivitiesType::bandit].m_actor = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                                                      CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "Object is null!");
                return false;
            }

            return Globals::has_alife_info("sim_bandit_attack_harder") &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::dolg].m_squad[SimulationActivitiesType::freedom] =
            [](CSE_ALifeOnlineOfflineGroup* squad, CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "Object is null!");
                return false;
            }

            return Globals::in_time_interval(8, 19) && (!XR_CONDITION::is_surge_started()) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::dolg]
            .m_squad[SimulationActivitiesType::monster_predatory_day] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                                            CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(8, 19) && (!XR_CONDITION::is_surge_started()) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::dolg]
            .m_squad[SimulationActivitiesType::monster_predatory_night] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                                              CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "Object is null!");
                return false;
            }
            return Globals::in_time_interval(8, 19) && (!XR_CONDITION::is_surge_started()) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::dolg]
            .m_squad[SimulationActivitiesType::monster_vegetarian] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                                         CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "Object is null!");
                return false;
            }
            return Globals::in_time_interval(8, 19) && (!XR_CONDITION::is_surge_started()) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::dolg]
            .m_squad[SimulationActivitiesType::monster_zombied_day] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                                          CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "Object is null!");
                return false;
            }

            return Globals::in_time_interval(8, 19) && (!XR_CONDITION::is_surge_started()) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::dolg]
            .m_squad[SimulationActivitiesType::monster_special] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                                      CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "Object is null!");
                return false;
            }

            return Globals::in_time_interval(8, 19) && (!XR_CONDITION::is_surge_started()) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::dolg].m_smart[SimulationActivitiesType::base] =
            [](CSE_ALifeOnlineOfflineGroup* squad, CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "Object is null!");
                return false;
            }

            return Globals::in_time_interval(19, 8) && (!XR_CONDITION::is_surge_started()) &&
                (!Globals::check_squad_for_enemies(squad)) &&
                (target->name() == "zat_stalker_base_smart" || target->name() == "jup_a6" ||
                    target->name() == "pri_a16");
        };

        // @ WA
        this->m_simulation_activities[SimulationActivitiesType::dolg].m_smart[SimulationActivitiesType::territory] =
            [](CSE_ALifeOnlineOfflineGroup* squad, CSE_ALifeObject* target) -> bool {
            return Globals::in_time_interval(8, 19) && (!XR_CONDITION::is_surge_started());
        };

        // @ WA
        this->m_simulation_activities[SimulationActivitiesType::dolg].m_smart[SimulationActivitiesType::surge] =
            [](CSE_ALifeOnlineOfflineGroup* squad, CSE_ALifeObject* target) -> bool {
            return XR_CONDITION::is_surge_started();
        };

        this->m_simulation_activities[SimulationActivitiesType::dolg].m_actor = nullptr;

        this->m_simulation_activities[SimulationActivitiesType::freedom].m_squad[SimulationActivitiesType::dolg] =
            [](CSE_ALifeOnlineOfflineGroup* squad, CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(8, 19) && (!XR_CONDITION::is_surge_started()) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::freedom].m_smart[SimulationActivitiesType::base] =
            [](CSE_ALifeOnlineOfflineGroup* squad, CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(19, 8) && (!XR_CONDITION::is_surge_started()) &&
                (!Globals::check_squad_for_enemies(squad)) &&
                (target->name() == "zat_stalker_base_smart" || target->name() == "jup_a6" ||
                    target->name() == "pri_a16");
        };

        // @ WA
        this->m_simulation_activities[SimulationActivitiesType::freedom].m_smart[SimulationActivitiesType::territory] =
            [](CSE_ALifeOnlineOfflineGroup* squad, CSE_ALifeObject* target) -> bool {
            return Globals::in_time_interval(8, 19) && (!XR_CONDITION::is_surge_started());
        };

        // @ WA
        this->m_simulation_activities[SimulationActivitiesType::freedom].m_smart[SimulationActivitiesType::surge] =
            [](CSE_ALifeOnlineOfflineGroup* squad, CSE_ALifeObject* target) -> bool {
            return XR_CONDITION::is_surge_started();
        };

        this->m_simulation_activities[SimulationActivitiesType::freedom].m_actor = nullptr;

        // @ WA
        this->m_simulation_activities[SimulationActivitiesType::killer].m_smart[SimulationActivitiesType::territory] =
            [](CSE_ALifeOnlineOfflineGroup* squad, CSE_ALifeObject* target) -> bool {
            return (!XR_CONDITION::is_surge_started());
        };

        // @ WA
        this->m_simulation_activities[SimulationActivitiesType::killer].m_smart[SimulationActivitiesType::surge] =
            [](CSE_ALifeOnlineOfflineGroup* squad, CSE_ALifeObject* target) -> bool {
            return (XR_CONDITION::is_surge_started());
        };

        this->m_simulation_activities[SimulationActivitiesType::killer].m_actor = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                                                      CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance;
        };

        // @ WA
        this->m_simulation_activities[SimulationActivitiesType::zombied].m_smart[SimulationActivitiesType::territory] =
            [](CSE_ALifeOnlineOfflineGroup* squad, CSE_ALifeObject* target) -> bool { return true; };

        // @ WA
        this->m_simulation_activities[SimulationActivitiesType::zombied].m_smart[SimulationActivitiesType::lair] =
            [](CSE_ALifeOnlineOfflineGroup* squad, CSE_ALifeObject* target) -> bool { return true; };

        this->m_simulation_activities[SimulationActivitiesType::zombied].m_actor = nullptr;

        // @ WA
        this->m_simulation_activities[SimulationActivitiesType::monster_predatory_day]
            .m_squad[SimulationActivitiesType::monster_vegetarian] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                                         CSE_ALifeObject* target) -> bool {
            return Globals::in_time_interval(6, 19);
        };

        this->m_simulation_activities[SimulationActivitiesType::monster_predatory_day]
            .m_squad[SimulationActivitiesType::stalker] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                              CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(6, 19) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::monster_predatory_day]
            .m_squad[SimulationActivitiesType::bandit] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                             CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(6, 19) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::monster_predatory_day]
            .m_squad[SimulationActivitiesType::dolg] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                           CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(6, 19) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::monster_predatory_day]
            .m_squad[SimulationActivitiesType::freedom] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                              CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(6, 19) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::monster_predatory_day]
            .m_squad[SimulationActivitiesType::killer] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                             CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }
            return Globals::in_time_interval(6, 19) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        // @ WA
        this->m_simulation_activities[SimulationActivitiesType::monster_predatory_day]
            .m_smart[SimulationActivitiesType::territory] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                                CSE_ALifeObject* target) -> bool {
            return Globals::in_time_interval(6, 19);
        };

        // @ WA
        this->m_simulation_activities[SimulationActivitiesType::monster_predatory_day]
            .m_smart[SimulationActivitiesType::lair] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                           CSE_ALifeObject* target) -> bool {
            return Globals::in_time_interval(19, 6);
        };

        this->m_simulation_activities[SimulationActivitiesType::monster_predatory_day].m_actor =
            [](CSE_ALifeOnlineOfflineGroup* squad, CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(19, 6) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        // @ WA
        this->m_simulation_activities[SimulationActivitiesType::monster_predatory_night]
            .m_squad[SimulationActivitiesType::monster_vegetarian] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                                         CSE_ALifeObject* target) -> bool {
            return Globals::in_time_interval(21, 6);
        };

        this->m_simulation_activities[SimulationActivitiesType::monster_predatory_night]
            .m_squad[SimulationActivitiesType::stalker] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                              CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(19, 6) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::monster_predatory_night]
            .m_squad[SimulationActivitiesType::bandit] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                             CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(19, 6) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::monster_predatory_night]
            .m_squad[SimulationActivitiesType::dolg] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                           CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(19, 6) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::monster_predatory_night]
            .m_squad[SimulationActivitiesType::freedom] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                              CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(19, 6) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::monster_predatory_night]
            .m_squad[SimulationActivitiesType::killer] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                             CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(19, 6) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        // @ WA
        this->m_simulation_activities[SimulationActivitiesType::monster_predatory_night]
            .m_smart[SimulationActivitiesType::territory] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                                CSE_ALifeObject* target) -> bool {
            return Globals::in_time_interval(19, 6);
        };

        // @ WA
        this->m_simulation_activities[SimulationActivitiesType::monster_predatory_night]
            .m_smart[SimulationActivitiesType::lair] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                           CSE_ALifeObject* target) -> bool {
            return Globals::in_time_interval(6, 19);
        };

        this->m_simulation_activities[SimulationActivitiesType::monster_predatory_night].m_actor =
            [](CSE_ALifeOnlineOfflineGroup* squad, CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(19, 6) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        // @ WA
        this->m_simulation_activities[SimulationActivitiesType::monster_vegetarian]
            .m_smart[SimulationActivitiesType::lair] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                           CSE_ALifeObject* target) -> bool { return true; };

        this->m_simulation_activities[SimulationActivitiesType::monster_vegetarian].m_actor =
            [](CSE_ALifeOnlineOfflineGroup* squad, CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(19, 6) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::monster_zombied_day]
            .m_squad[SimulationActivitiesType::stalker] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                              CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(6, 19) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::monster_zombied_day]
            .m_squad[SimulationActivitiesType::bandit] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                             CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(6, 19) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::monster_zombied_day]
            .m_squad[SimulationActivitiesType::dolg] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                           CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(6, 19) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::monster_zombied_day]
            .m_squad[SimulationActivitiesType::freedom] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                              CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(6, 19) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::monster_zombied_day]
            .m_squad[SimulationActivitiesType::killer] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                             CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(6, 19) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        // @ WA
        this->m_simulation_activities[SimulationActivitiesType::monster_zombied_day]
            .m_smart[SimulationActivitiesType::territory] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                                CSE_ALifeObject* target) -> bool {
            return (!XR_CONDITION::is_surge_started());
        };

        // @ WA
        this->m_simulation_activities[SimulationActivitiesType::monster_zombied_day]
            .m_smart[SimulationActivitiesType::lair] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                           CSE_ALifeObject* target) -> bool {
            return Globals::in_time_interval(19, 6);
        };

        this->m_simulation_activities[SimulationActivitiesType::monster_zombied_day].m_actor =
            [](CSE_ALifeOnlineOfflineGroup* squad, CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(6, 19) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::monster_zombied_night]
            .m_squad[SimulationActivitiesType::stalker] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                              CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(19, 6) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::monster_zombied_night]
            .m_squad[SimulationActivitiesType::bandit] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                             CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(19, 6) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::monster_zombied_night]
            .m_squad[SimulationActivitiesType::dolg] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                           CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(19, 6) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::monster_zombied_night]
            .m_squad[SimulationActivitiesType::freedom] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                              CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(19, 6) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        this->m_simulation_activities[SimulationActivitiesType::monster_zombied_night]
            .m_squad[SimulationActivitiesType::killer] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                             CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(19, 6) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        // @ WA
        this->m_simulation_activities[SimulationActivitiesType::monster_zombied_night]
            .m_smart[SimulationActivitiesType::territory] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                                CSE_ALifeObject* target) -> bool {
            return Globals::in_time_interval(19, 6);
        };

        // @ WA
        this->m_simulation_activities[SimulationActivitiesType::monster_zombied_night]
            .m_smart[SimulationActivitiesType::lair] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                           CSE_ALifeObject* target) -> bool {
            return Globals::in_time_interval(6, 19);
        };

        this->m_simulation_activities[SimulationActivitiesType::monster_zombied_night].m_actor =
            [](CSE_ALifeOnlineOfflineGroup* squad, CSE_ALifeObject* target) -> bool {
            if (!squad || !target)
            {
                R_ASSERT2(false, "object is null!");
                return false;
            }

            return Globals::in_time_interval(19, 6) &&
                (Globals::sim_dist_to(squad, target) <= kSimulationBoardSimulationDistance);
        };

        // @ WA
        this->m_simulation_activities[SimulationActivitiesType::monster_special]
            .m_smart[SimulationActivitiesType::lair] = [](CSE_ALifeOnlineOfflineGroup* squad,
                                                           CSE_ALifeObject* target) -> bool { return true; };
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

#pragma region Getters
    inline const xr_map<xr_string, Script_SE_SmartTerrain*>& getSmartTerrainsByName(void) const
    {
        return this->m_smarts_by_name;
    }

    inline void setSmartTerrainsByName(const xr_map<xr_string, Script_SE_SmartTerrain*>& map)
    {
        if (!map.size())
        {
            Msg("[Script_SimulationBoard/setSmartTerrainsByName(map)] WARNING: map.size() = 0! You are trying to "
                "set an empty map! No assignment!");
            return;
        }

        this->m_smarts_by_name = map;
    }

    inline void setSmartTerrainsByName(const std::pair<xr_string, Script_SE_SmartTerrain*>& pair)
    {
        if (!pair.first.size())
        {
            Msg("[Script_SimulationBoard/setSmartTerrainsByName(pair)] WARNING: pair.first.size() = 0! You are "
                "trying "
                "to set an empty pair! No assignment!");
            return;
        }

        if (pair.second)
        {
            Msg("[Script_SimulationBoard/setSmartTerrainsByName(pair)] WARNING: pair.second = null! You are trying "
                "to "
                "set an empty pair! No assignment!");
            return;
        }

        this->m_smarts_by_name.insert(pair);
    }

    inline void setSmartTerrainsByName(const xr_string& smart_terrain_name, Script_SE_SmartTerrain* p_server_object)
    {
        if (!smart_terrain_name.size())
        {
            Msg("[Script_SimulationBoard/setSmartTerrainsByName(smart_terrain_name, p_server_object)] WARNING: "
                "smart_terrain_name.size() = 0! You are trying to set an empty string! No assignment!");
            return;
        }

        if (!p_server_object)
        {
            Msg("[Script_SimulationBoard/setSmartTerrainsByName(smart_terrain_name, p_server_object)] WARNING: "
                "p_server_object = null! You are trying to set an empty object! No assignment!");
            return;
        }

        this->m_smarts_by_name[smart_terrain_name] = p_server_object;
    }

    inline const xr_map<std::uint32_t, SmartDataSimulationBoard>& getSmarts(void) const noexcept
    {
        return this->m_smarts;
    }

    inline void setSmarts(const xr_map<std::uint32_t, SmartDataSimulationBoard>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_SimulationBoard/setSmarts(map)] WARNING: map.size() = 0! You are trying to set an empty "
                "map! "
                "No assignment!");
            return;
        }

        this->m_smarts = map;
    }

    inline void setSmarts(const std::pair<std::uint32_t, SmartDataSimulationBoard>& pair) noexcept
    {
        if (pair.first == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_SimulationBoard/setSmarts(pair)] WARNING: pair.first = std::uint32_t(-1)! You are trying "
                "to "
                "set an empty pair! No assignment!");
            return;
        }

        this->m_smarts.insert(pair);
    }

    inline void setSmarts(const std::uint32_t& id, const SmartDataSimulationBoard& data) noexcept
    {
        if (id == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_SimulationBoard/setSmarts(id, data)] WARNING: id = std::uint32_t(-1)! You are trying to "
                "set "
                "an undefined value! No assignment!");
            return;
        }

        this->m_smarts[id] = data;
    }

    inline const xr_map<std::uint32_t, SimulationActivitiesType>& getSimulationActivities(void) const noexcept
    {
        return this->m_simulation_activities;
    }

    inline void setSimulationActivities(const xr_map<std::uint32_t, SimulationActivitiesType>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_SimulationBoard] WARNING: map.size() = 0! You are trying to set an empty map! No "
                "assignment!");
            return;
        }

        this->m_simulation_activities = map;
    }

    inline void setSimulationActivities(const std::pair<std::uint32_t, SimulationActivitiesType>& pair) noexcept
    {
        if (pair.first == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_SimulationBoard/setSimulationActivities(pair)] WARNING: pair.first = std::uint32_t(-1)! "
                "You "
                "are trying to set an empty pair! No assignment!");
            return;
        }

        this->m_simulation_activities.insert(pair);
    }

    inline void setSimulationActivities(const std::uint32_t& id, const SimulationActivitiesType& data) noexcept
    {
        if (id == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_SimulationBoard/setSimulationActivities(id, data)] WARNING: id = std::uint32_t(-1)! You "
                "are "
                "trying to set an undefined value! No assignment!");
            return;
        }

        this->m_simulation_activities[id] = data;
    }
#pragma endregion

    inline void start_simulation(void) noexcept { this->m_is_simulation_started = true; }
    inline void stop_simulation(void) noexcept { this->m_is_simulation_started = false; }

    inline void set_actor_community(LPCSTR buffer)
    {
        if (!buffer)
        {
            R_ASSERT2(false, "string can't empty!");
            return;
        }

        DataBase::Storage::getInstance().getActor()->SetCharacterCommunity(buffer, 0, 0);
    }

    inline void assigned_squad_to_smart(Script_SE_SimulationSquad* squad, const std::uint32_t& smart_id = 0)
    {
        if (!squad)
        {
            R_ASSERT2(false, "object was null!");
            return;
        }

        if (!smart_id)
            Msg("[Scripts/Script_SimulationBoard/assing_squad_to_smart(squad, smart_id)] WARNING: smart_id was "
                "null!");

        if (smart_id)
        {
            if (!this->m_smarts[smart_id].getServerSmartTerrain())
            {
                this->m_temporary_assigned_squad[smart_id] = squad;
                return;
            }
        }

        std::uint16_t old_smart_id = Globals::kUnsignedInt16Undefined;

        if (squad->getSmartTerrainID())
            old_smart_id = squad->getSmartTerrainID();

        if (old_smart_id && this->m_smarts[old_smart_id].getServerSmartTerrain())
        {
            this->m_smarts[old_smart_id].setSquads(squad->ID, nullptr); 
         //   this->m_smarts[old_smart_id].m_squads[squad->ID] = nullptr;
            this->m_smarts[old_smart_id].getServerSmartTerrain()->refresh();
        }

        if (!smart_id)
        {
            squad->assign_smart();
        }

        squad->assign_smart(this->m_smarts[smart_id].getServerSmartTerrain());

       // this->m_smarts[smart_id].m_squads[squad->ID] = squad;
        this->m_smarts[smart_id].setSquads(squad->ID, squad);
        this->m_smarts[smart_id].getServerSmartTerrain()->refresh();
    }

    inline void enter_squad_to_smart(Script_SE_SimulationSquad* squad, const std::uint32_t& smart_id)
    {
        if (!this->m_smarts[smart_id].getServerSmartTerrain())
        {
            return;
        }
    }

    void register_smart(Script_SE_SmartTerrain* object);
    void init_smart(Script_SE_SmartTerrain* object);
    void setup_squad_and_group(CSE_ALifeDynamicObject* object);
    void remove_squad(Script_SE_SimulationSquad* server_squad);
    void exit_smart(Script_SE_SimulationSquad* server_squad, const std::uint32_t& smart_terrain_id);
    void fill_start_position(void);

private:
    Script_SE_SimulationSquad* create_squad(Script_SE_SmartTerrain* smart, const xr_string& squad_id);

private:
    /*
    enum group_id_by_levels
    {
        zaton = 1,
        pripyat,
        jupiter,
        labx8,
        jupiter_underground
    };*/

    bool m_is_simulation_started;
    bool m_is_start_position_filled;
    const CInifile* m_squad_ltx;
    xr_map<std::uint32_t, SimulationActivitiesType> m_simulation_activities;
    xr_map<std::uint32_t, SmartDataSimulationBoard> m_smarts;
    xr_map<xr_string, Script_SE_SmartTerrain*> m_smarts_by_name;
    xr_map<std::uint32_t, Script_SE_SimulationSquad*> m_temporary_entered_squad;
    xr_map<std::uint32_t, Script_SE_SimulationSquad*> m_temporary_assigned_squad;
    CScriptIniFile m_setting_ini;
};
} // namespace Scripts
} // namespace Cordis
