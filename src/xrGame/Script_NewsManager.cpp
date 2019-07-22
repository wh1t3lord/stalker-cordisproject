#include "stdafx.h"
#include "Script_NewsManager.h"

namespace Cordis
{
namespace Scripts
{
Script_NewsManager::~Script_NewsManager(void) {}

void Script_NewsManager::SendSound(CScriptGameObject* object, const xr_string& faction, const xr_string& point,
    const xr_string& str, const xr_string& str1, const float& delay)
{
    if (!object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    if (faction.size())
    {
        // Lord: доделать!
    }
}
} // namespace Scripts
} // namespace Cordis
