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

    
}

} // namespace Scripts
} // namespace Cordis
