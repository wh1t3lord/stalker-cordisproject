#pragma once

namespace Cordis
{
namespace Scripts
{
namespace XR_WOUNDED
{
inline bool is_wounded(CScriptGameObject* npc)
{
    Msg("[Scripts/XR_WOUNDED/is_wounded(npc)] NOT IMPLEMENTED!");

    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    return false;
}
} // namespace XR_WOUNDED
} // namespace Scripts
} // namespace Cordis
