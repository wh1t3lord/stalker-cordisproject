#include "stdafx.h"
#include "Script_SimulationBoard.h"
#include "Script_SE_SmartTerrain.h"

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

}

} // namespace Scripts
} // namespace Cordis
