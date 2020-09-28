#pragma once

#include "Script_Globals.h"
 
namespace Cordis
{
namespace Scripts
{
namespace XR_MEET
{
inline bool is_meet(CScriptGameObject* npc)
{
    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    CScriptActionPlanner* manager = npc->action_planner<CScriptActionPlanner>();
    if (!manager)
    {
        R_ASSERT2(false, "can't cast and maybe object was damaged");
        return false;
    }

    if (manager->initialized())
    {
        std::uint32_t id = manager->current_action_id();

        if (id == Globals::XR_ACTIONS_ID::kStoheMeetBase + 1)
            return true;
    }

    return false;
}
} // namespace XR_MEET
} // namespace Scripts
} // namespace Cordis
