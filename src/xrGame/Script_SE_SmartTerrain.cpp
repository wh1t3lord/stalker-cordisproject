#include "stdafx.h"
#include "Script_SE_SmartTerrain.h"
#include "Script_GulagGenerator.h"

CInifile ini_file_locations = CInifile("misc\\smart_terrain_masks.ltx");

namespace Cordis
{
namespace Scripts
{
bool is_job_available_to_npc(
    const NpcInfo& npc_info, const JobData_SubData& job_info, const bool& is_monster, Script_SE_SmartTerrain* smart)
{
    if (!smart)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    // @ Lord: убедить что будет выполняться оригинальное условие что объект вообще был инициализирован после default
    // инициализации, то есть xrTime() -> xrTime().setSomeValue();
    if (smart->getDeadTime()[job_info.m_job_index] > 0)
        return false;

    if (is_monster != npc_info.m_is_monster)
        return false;

    if (job_info.m_function)
    {
        if (job_info.m_function(npc_info.m_server_object, smart, job_info.m_function_params, npc_info))
            return false;
    }

    return true;
}

inline void job_iterator(
    JobData& jobs, NpcInfo& npc_data, std::uint32_t selected_job_priority, Script_SE_SmartTerrain* smart)
{
    if (!smart)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }
}

} // namespace Scripts
} // namespace Cordis

namespace Cordis
{
namespace Scripts
{
Script_SE_SmartTerrain::Script_SE_SmartTerrain(LPCSTR section)
    : inherited(section), m_is_initialized(false), m_is_registered(false), m_population(0)
{
}

Script_SE_SmartTerrain::~Script_SE_SmartTerrain(void)
{
    if (this->m_base_on_actor_control)
        delete this->m_base_on_actor_control;
}

void Script_SE_SmartTerrain::on_before_register(void) { inherited::on_before_register(); }

void Script_SE_SmartTerrain::read_params(void) {}
void Script_SE_SmartTerrain::on_after_reach(Script_SE_SimulationSquad* squad)
{
    if (!squad)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    for (AssociativeVector<std::uint16_t, CSE_ALifeMonsterAbstract*>::const_iterator it =
             squad->squad_members().begin();
         it != squad->squad_members().end(); ++it)
    {
        if ((*it).second)
        {
            Script_SimulationBoard::getInstance().setup_squad_and_group((*it).second->cast_alife_dynamic_object());
        }
    }

    squad->setCurrentTargetID(this->ID);
}

void Script_SE_SmartTerrain::on_reach_target(Script_SE_SimulationSquad* squad)
{
    if (!squad)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    squad->set_location_types(this->name_replace());

    Script_SimulationBoard::getInstance().assigned_squad_to_smart(squad, this->ID);

    for (AssociativeVector<std::uint16_t, CSE_ALifeMonsterAbstract*>::const_iterator it =
             squad->squad_members().begin();
         it != squad->squad_members().end(); ++it)
    {
        if (DataBase::Storage::getInstance().getOfflineObjects()[(*it).first].second.size())
        {
            DataBase::Storage::getInstance().getOfflineObjects()[(*it).first].first = Globals::kUnsignedInt16Undefined;
            DataBase::Storage::getInstance().getOfflineObjects()[(*it).first].second = "";
        }
    }
}

void Script_SE_SmartTerrain::clear_dead(CSE_ALifeDynamicObject* server_object)
{
    if (!server_object)
    {
        R_ASSERT2(false, "object was nullptr!");
        return;
    }

    if (this->m_npc_info[server_object->ID].m_job_link.m_job_index != Globals::kUnsignedInt32Undefined)
    {
        this->m_dead_time[this->m_npc_info[server_object->ID].m_job_link.m_job_index] = Globals::Game::get_game_time();
        this->m_npc_info[server_object->ID].clear();
        CSE_ALifeMonsterAbstract* object = server_object->cast_monster_abstract();

        if (!object)
        {
            R_ASSERT2(false, "bad cast!");
            return;
        }

        object->m_smart_terrain_id = 0xffff;
        return;
    }
    else
    {
        R_ASSERT2(false, "something went wrong! Can't be here that this->m_npc_info = unInitialize");
        return;
    }

    if (this->m_arriving_npc[server_object->ID])
    {
        this->m_arriving_npc[server_object->ID] = nullptr;
        CSE_ALifeMonsterAbstract* object = server_object->cast_monster_abstract();

        if (!object)
        {
            R_ASSERT2(false, "bad cast!");
            return;
        }

        object->m_smart_terrain_id = 0xffff;
        //   return;
    }
}

} // namespace Scripts
} // namespace Cordis
