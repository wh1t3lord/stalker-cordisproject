#include "stdafx.h"
#include "Script_SchemeMobCombat.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemeMobCombat::Script_SchemeMobCombat(
    CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage)
{
}

Script_SchemeMobCombat::~Script_SchemeMobCombat(void) {}

void Script_SchemeMobCombat::combat_callback(void)
{
    Msg("[SCripts/Script_SchemeMobCombat/combat_callback()] %s", this->m_npc->Name());

    if (this->m_storage->IsEnabled() && this->m_npc->GetEnemy())
    {
        if (!DataBase::Storage::getInstance().getStorage().at(this->m_npc->ID()).getActiveSchemeName().empty())
        {
            // Lord: реализовать когда будет XR_LOGIC::try_switch_to_another_section
            return;
        }
    }
}

} // namespace Scripts
} // namespace Cordis
