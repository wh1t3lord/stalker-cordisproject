#include "stdafx.h"
#include "Script_Animpoint.h"

#include "Script_GlobalHelper.h"

namespace Cordis
{
namespace Scripts
{
Script_Animpoint::Script_Animpoint(CScriptGameObject* npc, DataBase::StorageAnimpoint_Data& storage) 
{
    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
    }

}

Script_Animpoint::~Script_Animpoint(void) {}

void Script_Animpoint::calculate_position(void) 
{
    Script_SE_SmartCover* server_smartcover =
        Script_GlobalHelper::getInstance().getGameRegisteredServerSmartCovers()[this->m_cover_name];
}

} // namespace Scripts
} // namespace Cordis
