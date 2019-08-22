#include "stdafx.h"
#include "Script_SimulationBoard.h"
#include "Script_SE_SmartTerrain.h"
#include "Script_SE_SimulationSquad.h"

namespace Cordis
{
namespace Scripts
{
Script_SimulationBoard::~Script_SimulationBoard(void) {}
void Script_SimulationBoard::register_smart(Script_SE_SmartTerrain* object)
{
    if (!object)
    {
        R_ASSERT2(false, "object was null");
        return;
    }

    Msg("[Scripts/Script_SimulationBoard/register_smart(object)] register smart -> %s", object->name());

    if (this->m_smarts[object->ID].m_smart)
    {
        R_ASSERT2(false, "You are trying to register smart, which it is already registered!");
        return;
    }

    SmartDataSimulationBoard data;
    data.m_smart = object;

    this->m_smarts[object->ID] = data;
    this->m_smarts_by_name[object->name()] = object;
}

void Script_SimulationBoard::init_smart(Script_SE_SmartTerrain* object)
{
    if (!object)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    if (this->m_temporary_assigned_squad[object->ID])
    {
        for (std::pair<const std::uint32_t, Script_SE_SimulationSquad*>& it : this->m_temporary_assigned_squad)
        {
            if (it.second)
            {
            }
        }
    }
}

void Script_SimulationBoard::setup_squad_and_group(CSE_ALifeDynamicObject* object)
{
    xr_string level_name = Globals::Game::level::get_name();
    CSE_ALifeDynamicObject* server_object = ai().alife().objects().object(object->ID);

    std::uint32_t group_id = Script_GlobalHelper::getInstance().getSimulationBoardGroupIDLevelsByName()[level_name] ?
        Script_GlobalHelper::getInstance().getSimulationBoardGroupIDLevelsByName()[level_name] :
        0;

    CSE_ALifeMonsterAbstract* server_monster = object->cast_monster_abstract();
    if (!server_monster)
    {
        R_ASSERT2(false, "Bad casting! Check your class");
        return;
    }

    Globals::change_team_squad_group(object, server_monster->s_team, group_id, server_monster->s_squad);

    Script_SE_SimulationSquad* server_squad =
        ai().alife().objects().object(server_monster->m_group_id)->cast_script_se_simulationsquad();

    if (!server_object)
    {
        Msg("[Scripts/Script_SimulationBoard/setup_squad_and_group(object)] WARNING: bad casting!");

        Globals::change_team_squad_group(object, server_monster->s_team, server_monster->s_group, 0);
        return;
    }

    if (server_squad->getCurrentAction().m_name.size() && server_squad->getCurrentAction().m_name == "reach_target")
    {
    
    }
}

} // namespace Scripts
} // namespace Cordis
