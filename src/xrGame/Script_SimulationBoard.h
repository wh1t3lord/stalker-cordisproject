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
    std::uint32_t m_stayed_squad_quan = 0;
    Script_SE_SmartTerrain* m_smart = nullptr;
    xr_map<std::uint32_t, Script_SE_SimulationSquad*> m_squads;
};

// @ WA - without arguments
class Script_SimulationBoard
{
private:
    Script_SimulationBoard(void) : m_setting_ini("misc\\simulation.ltx")
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

    void register_smart(Script_SE_SmartTerrain* object);
    void init_smart(Script_SE_SmartTerrain* object);
    inline void assigned_squad_to_smart(Script_SE_SimulationSquad* squad, const std::uint32_t& smart_id) {}

private:
    enum group_id_by_levels
    {
        zaton = 1,
        pripyat,
        jupiter,
        labx8,
        jupiter_underground
    };

    bool m_is_simulation_started;

    const CInifile* m_squad_ltx;
    xr_map<std::uint32_t, SimulationActivitiesType> m_simulation_activities;
    xr_map<std::uint32_t, SmartDataSimulationBoard> m_smarts;
    xr_map<xr_string, Script_SE_SmartTerrain*> m_smarts_by_name;
    xr_map<std::uint32_t, Script_SE_SimulationSquad*> m_temporary_entered_squad;
    xr_map<std::uint32_t, Script_SE_SimulationSquad*> m_temporary_assigned_squad;
    CInifile m_setting_ini;
};
} // namespace Scripts
} // namespace Cordis
