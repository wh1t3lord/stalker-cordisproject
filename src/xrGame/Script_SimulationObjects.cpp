#include "stdafx.h"
#include "Script_SimulationObjects.h"

namespace Cordis
{
namespace Scripts
{
Script_SimulationObjects::~Script_SimulationObjects(void) {}

float Script_SimulationObjects::evaluate_priority(CSE_ALifeObject* target, CSE_ALifeObject* squad)
{
    if (!target || !squad)
    {
        R_ASSERT2(false, "Object was null!");
        return 0.0f;
    }

    float priority = 0.0f;

    // Lord: доделать!
}

void Script_SimulationObjects::get_properties(CSE_ALifeObject* object)
{
    if (!object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    xr_string properties_section = object->name();
 
}

} // namespace Scripts
} // namespace Cordis
