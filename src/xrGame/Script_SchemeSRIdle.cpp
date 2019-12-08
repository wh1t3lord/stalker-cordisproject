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
    if (XR_LOGIC::try_switch_to_another_section(this->m_npc, *this->m_p_storage, DataBase::Storage::getInstance().getActor()))
    {
        return;
    }
}

} // namespace Scripts
} // namespace Cordis
