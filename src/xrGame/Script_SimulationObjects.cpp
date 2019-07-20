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

} // namespace Scripts
} // namespace Cordis
