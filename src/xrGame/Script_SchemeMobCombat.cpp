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

    if (this->m_p_storage->IsEnabled() && this->m_npc->GetEnemy())
    {
        if (!DataBase::Storage::getInstance().getStorage().at(this->m_npc->ID()).getActiveSchemeName().empty())
        {
            if (XR_LOGIC::try_switch_to_another_section(
                    this->m_npc, *this->m_p_storage, DataBase::Storage::getInstance().getActor()))
                return;
        }
    }
}

void Script_SchemeMobCombat::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    if (scheme_name.empty())
    {
        R_ASSERT2(false, "can't be an empty string!");
        return;
    }

    DataBase::Storage_Scheme* p_storage =
        XR_LOGIC::assign_storage_and_bind(p_client_object, p_ini, scheme_name, section_name, gulag_name);

    if (!p_storage)
    {
        R_ASSERT2(false, "it can't be ");
        return;
    }
    
    p_storage->setLogic(XR_LOGIC::cfg_get_switch_conditions(p_ini, section_name, p_client_object));
    p_storage->setEnabled(true);
}

void Script_SchemeMobCombat::disable_scheme(CScriptGameObject* const p_client_object, const xr_string& scheme_name)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    if (scheme_name.empty())
    {
        R_ASSERT2(false, "cam't be an empty string!");
        return;
    }

    DataBase::Storage::getInstance().setStorageSchemesEnabled(p_client_object->ID(), scheme_name, false);
}

} // namespace Scripts
} // namespace Cordis
