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
    // @ может передаваться и nullptr, так уж не особой роли играет это
    if (!object)
        Msg("[Script_NewsManager] -> SendSound() object is nullptr!");

    if (faction.size())
    {
        xr_string point_name = "";
        if (point.size())
        {
            // Lord: доделать
        }
    }
}
} // namespace Scripts
} // namespace Cordis
