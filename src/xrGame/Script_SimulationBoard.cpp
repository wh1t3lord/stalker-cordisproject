#include "stdafx.h"
#include "Script_SimulationBoard.h"
#include "Script_SE_SmartTerrain.h"
#include "Script_SE_SimulationSquad.h"

namespace Cordis
{
namespace Scripts
{
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
}

} // namespace Scripts
} // namespace Cordis
