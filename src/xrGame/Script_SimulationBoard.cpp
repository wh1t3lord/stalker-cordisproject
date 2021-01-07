#include "stdafx.h"
#include "Script_SimulationBoard.h"
#include "Script_SE_SmartTerrain.h"
#include "Script_SE_SimulationSquad.h"
#include "xrAICore/Navigation/game_graph_space.h"
#include "Script_XR_Condition.h"

CScriptIniFile settings_ini = CScriptIniFile("misc\\simulation.ltx");

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

    Msg("[Scripts/Script_SimulationBoard/register_smart(object)] register smart -> %s", object->name_replace());

    if (this->m_smarts[object->ID].getServerSmartTerrain())
    {
        R_ASSERT2(false, "You are trying to register smart, which it is already registered!");
        return;
    }

    SmartDataSimulationBoard data;
/*    data.getServerSmartTerrain() = object;*/
    data.setServerSmartTerrain(object);
    this->m_smarts[object->ID] = data;
    this->m_smarts_by_name[object->name_replace()] = object;
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
                this->assigned_squad_to_smart(it.second, object->ID);
        }

        this->m_temporary_assigned_squad[object->ID] = nullptr;
    }

    if (this->m_temporary_entered_squad[object->ID])
    {
        for (std::pair<const std::uint32_t, Script_SE_SimulationSquad*>& it : this->m_temporary_entered_squad)
        {
            if (it.second)
                this->enter_squad_to_smart(it.second, object->ID);
        }

        this->m_temporary_entered_squad[object->ID] = nullptr;
    }
}

void Script_SimulationBoard::setup_squad_and_group(CSE_ALifeDynamicObject* object)
{
    xr_string level_name = (!Globals::Game::level::get_name().empty()) ? Globals::Game::level::get_name() : "";
    CSE_ALifeDynamicObject* server_object = ai().alife().objects().object(object->ID);

    std::uint32_t group_id = level_name.size() ?
        (Script_GlobalHelper::getInstance().getSimulationBoardGroupIDLevelsByName().at(level_name) ?
                Script_GlobalHelper::getInstance().getSimulationBoardGroupIDLevelsByName().at(level_name) :
                0) :
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
    if (server_squad->getCurrentAction().getName().size() &&
        server_squad->getCurrentAction().getName() == Globals::kSimulationSquadCurrentActionIDReachTarget)
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

    if (smart->m_script_clsid == Globals::get_script_clsid(CLSID_SE_SMART_TERRAIN))
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

    if (!server_squad->getCurrentAction().getName().size())
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

    if (!smart.getServerSmartTerrain())
    {
        R_ASSERT2(false, "Smart nullptr while smart_terrain_id is a valid value! So it can't be!!");
        return;
    }
    std::uint32_t quan_value = smart.getServerSmartTerrain()->getStaydSquadQuan();
    Msg("[Scripts/Script_SimulationBoard/exit_smart(server_squad, smart_terrain_id)] Squad %d exit smart %s. Quan = %d",
        server_squad->ID, smart.getServerSmartTerrain()->name_replace(), quan_value);
    --quan_value;
    smart.getServerSmartTerrain()->setStaydSquadQuan(quan_value);
    smart.setSquads(server_squad->ID, nullptr);
}

void Script_SimulationBoard::fill_start_position(void)
{
    if (this->m_is_start_position_filled)
        return;

    this->m_is_start_position_filled = true;

    const GameGraph::LEVEL_MAP& levels = Globals::Game::get_game_graph()->header().levels();
    for (GameGraph::LEVEL_MAP::const_iterator it = levels.begin(); it != levels.end(); ++it)
    {
        xr_string section_name = "start_position_";
        section_name += ai().game_graph().header().level(it->first).name().c_str();

        if (!settings_ini.section_exist(section_name.c_str()))
            return;

        std::uint32_t count = settings_ini.line_count(section_name.c_str());

        for (std::uint32_t i = 0; i < count; ++i)
        {
            const char* ID;
            const char* Value;
            if (settings_ini.r_line(section_name.c_str(), i, &ID, &Value))
            {
                xr_vector<xr_string> buffer_names = Globals::Utils::parse_names(Value);

                for (xr_string& it : buffer_names)
                {
                    Script_SE_SmartTerrain* smart = this->m_smarts_by_name[it];

                    if (!smart)
                    {
                        R_ASSERT2(false, "Wrong smart name in start position!");
                        return;
                    }

                    Script_SE_SimulationSquad* squad = this->create_squad(smart, ID);

                    this->enter_squad_to_smart(squad, smart->ID);
                }
            }
        }
    }
}

Script_SE_SimulationSquad* Script_SimulationBoard::create_squad(
    Script_SE_SmartTerrain* smart, const xr_string& squad_id)
{
    Script_SE_SimulationSquad* server_object =
        Globals::Game::alife_create(squad_id, smart->Position(), smart->m_tNodeID, smart->m_tGraphID)
            ->cast_script_se_simulationsquad();

    if (!server_object)
    {
        R_ASSERT2(false, "Bad casting. It can't be!!!!");
        return nullptr;
    }

    Msg("[Scripts/Script_SimulationBoard/create_squad(smart, squad_id_name)] Creating squad[%s] in smart[%s]",
        squad_id.c_str(), smart->name_replace());

    server_object->create_npc(smart);
    server_object->set_squad_relation();

    this->assigned_squad_to_smart(server_object, smart->ID);

    for (AssociativeVector<std::uint16_t, CSE_ALifeMonsterAbstract*>::const_iterator it =
             server_object->squad_members().begin();
         it != server_object->squad_members().end(); ++it)
    {
        this->setup_squad_and_group(it->second);
    }

    return server_object;
}

CSE_ALifeDynamicObject* Script_SimulationBoard::get_squad_target(Script_SE_SimulationSquad* const p_squad)
{
    if (p_squad == nullptr)
    {
        MESSAGEWR("passed invalid squad!");
        return nullptr;
    }

    const auto& data = Script_SimulationObjects::getInstance().getObjects();

    xr_map<float, CSE_ALifeDynamicObject*> buffer;

    CSE_ALifeDynamicObject* p_most_priority_task = nullptr;

    for (const std::pair<std::uint16_t, CSE_ALifeDynamicObject*>& it : data)
    {
        float current_prior = 0.0f;
		if (it.second)
		{
            if (it.second->ID != p_squad->ID)
            {
                auto* p_try_1 = it.second->cast_script_se_actor();
                auto* p_try_2 = it.second->cast_script_se_simulationsquad();
                auto* p_try_3 = it.second->cast_script_se_smartterrain();

                if (p_try_1)
                {
                    current_prior = p_try_1->evaluate_priority(p_squad);
                }
                else if (p_try_2)
                {
                    current_prior = p_try_2->evaluate_priority(p_squad);
                }
                else if (p_try_3)
                {
                    current_prior = p_try_3->evaluate_prior(p_squad);
                }
                else 
                {
                    R_ASSERT2(false, "can't cast and can't be!");
                }
            }

			if (current_prior > 0.0f)
			{
                buffer[current_prior] = it.second;
			}
        }
    }

    if (buffer.empty() == false)
    {
        int index = static_cast<int>(floorf(0.3 * static_cast<float>(buffer.size())));

        if (index == 0) 
            index = 1;

        int _local_iterator = 0;

        for (const auto& it : buffer)
        {
            ++_local_iterator;

            if (_local_iterator == index)
            {
                p_most_priority_task = it.second;
            }
        }
    }

    if (p_most_priority_task == nullptr)
    {
        if (p_squad->getSmartTerrainID() != Globals::kUnsignedInt16Undefined)
        {
            p_most_priority_task = ai().alife().objects().object(p_squad->getSmartTerrainID());
        }
        else
        {
            p_most_priority_task = p_squad;
        }
    }

	return p_most_priority_task;
}

} // namespace Scripts
} // namespace Cordis
