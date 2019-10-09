#include "stdafx.h"
#include "Script_Animpoint.h"

#include "Script_GlobalHelper.h"

namespace Cordis
{
namespace Scripts
{
Script_Animpoint::Script_Animpoint(CScriptGameObject* npc, DataBase::StorageAnimpoint_Data& storage)
    : m_npc_id(npc->ID()), m_storage(&storage)
{
}

Script_Animpoint::~Script_Animpoint(void)
{
    Msg("[Scripts/Script_Animpoint/~dtor] Deleting: [%s]", this->m_cover_name.c_str());
}

bool Script_Animpoint::is_position_riched(void) { return false; }

void Script_Animpoint::start(void) {}

void Script_Animpoint::stop(void) {}

void Script_Animpoint::update(void) {}

void Script_Animpoint::activate_scheme(void)
{
    this->calculate_position();

    if (this->m_is_started)
    {
        if (!this->m_storage->IsUseCamp() && this->m_cover_name == this->m_storage->getCoverName())
        {
            this->fill_approved_actions();
        }
    }
}

void Script_Animpoint::fill_approved_actions(void)
{
    bool is_in_camp = this->m_is_camp;

    if (this->m_storage->getAvailAnimations().size())
    {
        for (const xr_string& it : this->m_storage->getAvailAnimations())
        {
        //    this->m_storage->setApprovedAction(it);
        }
    }
    else
    {
    }
}

void Script_Animpoint::calculate_position(void)
{
    Script_SE_SmartCover* server_smartcover = Script_GlobalHelper::getInstance().getGameRegisteredServerSmartCovers().at(this->m_cover_name);
}

} // namespace Scripts
} // namespace Cordis
