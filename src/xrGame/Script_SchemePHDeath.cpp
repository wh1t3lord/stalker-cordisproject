#include "stdafx.h"
#include "Script_SchemePHDeath.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemePHDeath::Script_SchemePHDeath(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage)
{
}
Script_SchemePHDeath::~Script_SchemePHDeath(void) {}

void Script_SchemePHDeath::death_callback(
    CScriptGameObject* const p_client_victim, CScriptGameObject* const p_client_who)
{
    if (!DataBase::Storage::getInstance().getStorage().at(this->m_id).getActiveSchemeName().empty())
    {
        if (XR_LOGIC::try_switch_to_another_section(
                p_client_victim, *this->m_p_storage, DataBase::Storage::getInstance().getActor()))
            return;
    }
}

void Script_SchemePHDeath::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    DataBase::Storage_Scheme* p_storage =
        XR_LOGIC::assign_storage_and_bind(p_client_object, p_ini, scheme_name, section_name, gulag_name);

    if (!p_storage)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    p_storage->setLogic(XR_LOGIC::cfg_get_switch_conditions(p_ini, section_name, p_client_object));
}

} // namespace Scripts
} // namespace Cordis
