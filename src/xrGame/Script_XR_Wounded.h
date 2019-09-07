#pragma once

namespace Cordis
{
namespace Scripts
{
namespace XR_WOUNDED
{
inline bool is_wounded(CScriptGameObject* npc)
{
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
