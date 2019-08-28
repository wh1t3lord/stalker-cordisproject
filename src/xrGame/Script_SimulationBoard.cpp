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

    Script_SE_SmartTerrain* smart = nullptr;
    if (server_squad->getCurrentAction().m_name.size() &&
        server_squad->getCurrentAction().m_name == Globals::kSimulationSquadCurrentActionIDReachTarget)
    {
        smart = ai().alife().objects().object(server_squad->getAssignedTargetID())->cast_script_se_smartterrain();
    }
    else if (server_squad->getSmartTerrainID())
    {
        smart = ai().alife().objects().object(server_squad->getSmartTerrainID())->cast_script_se_smartterrain();
    }

    if (!smart)
    {
        Msg("[Scripts/Script_SimulationBoard/setup_squad_and_group(object)] WARNING: bad casting!");
        Globals::change_team_squad_group(object, server_monster->s_team, server_monster->s_group, 0);
        return;
    }

    std::uint16_t object_squad_id = 0;

    if (smart->m_script_clsid == CLSID_SE_SMART_TERRAIN)
        object_squad_id = smart->getSquadID();

    Globals::change_team_squad_group(object, server_monster->s_team, server_monster->s_group, object_squad_id);
}

void Script_SimulationBoard::remove_squad(Script_SE_SimulationSquad* server_squad)
{
    if (!server_squad)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    Msg("[Scripts/Script_SimulationBoard/remove_squad(server_squad)] Removing squad %d", server_squad->ID);

    if (!server_squad->getCurrentAction().m_name.size())
    {
        this->exit_smart(server_squad, server_squad->getSmartTerrainID());
    }

    this->assigned_squad_to_smart(server_squad, Globals::kUnsignedInt32Undefined);

    server_squad->remove_squad();
}

void Script_SimulationBoard::exit_smart(Script_SE_SimulationSquad* server_squad, const std::uint32_t& smart_terrain_id)
{
    if (!server_squad)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    if (server_squad->getEnteredSmartID() != smart_terrain_id)
        return;

    server_squad->setEnteredSmartID(Globals::kUnsignedInt32Undefined);

    SmartDataSimulationBoard& smart = this->m_smarts[smart_terrain_id];

    if (!smart.m_smart)
    {
        R_ASSERT2(false, "Smart nullptr while smart_terrain_id is a valid value! So it can't be!!");
        return;
    }
    std::uint32_t quan_value = smart.m_smart->getStaydSquadQuan();
    Msg("[Scripts/Script_SimulationBoard/exit_smart(server_squad, smart_terrain_id)] Squad %d exit smart %s. Quan = %d", server_squad->ID, smart.m_smart->name_replace(), quan_value);
    --quan_value;
    smart.m_smart->setStaydSquadQuan(quan_value);
    smart.m_squads[server_squad->ID] = nullptr;
}

} // namespace Scripts
} // namespace Cordis
