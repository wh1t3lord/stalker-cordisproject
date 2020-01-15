#include "stdafx.h"
#include "Script_SchemeSRIdle.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemeSRIdle::Script_SchemeSRIdle(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage)
{
    this->m_scheme_name = "sr_idle";
}

Script_SchemeSRIdle::~Script_SchemeSRIdle(void) {}

void Script_SchemeSRIdle::reset_scheme(const bool value, CScriptGameObject* const p_client_object)
{
    this->m_p_storage->ClearSignals();
}

void Script_SchemeSRIdle::update(const float delta)
{
    if (XR_LOGIC::try_switch_to_another_section(
            this->m_npc, *this->m_p_storage, DataBase::Storage::getInstance().getActor()))
    {
        return;
    }
}

void Script_SchemeSRIdle::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
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
