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
                p_client_victim, *this->m_storage, DataBase::Storage::getInstance().getActor()))
            return;
    }
}

} // namespace Scripts
} // namespace Cordis
