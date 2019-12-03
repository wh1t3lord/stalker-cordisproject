#include "stdafx.h"
#include "Script_SchemePHOnHit.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemePHOnHit::Script_SchemePHOnHit(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage)
{
}

Script_SchemePHOnHit::~Script_SchemePHOnHit(void) {}

void Script_SchemePHOnHit::hit_callback(CScriptGameObject* const p_client_object, const float amount,
    const Fvector& local_direction, CScriptGameObject* const p_client_who, const std::int16_t bone_index)
{
    Msg("[Scripts/Script_SchemePHOnHit/hit_callback(p_client_object, amount, local_direction, p_client_who, "
        "bone_index)] %s %d %s %d",
        p_client_object->Name(), amount, p_client_who ? p_client_who->Name() : "Unknown", bone_index);

    if (!DataBase::Storage::getInstance().getStorage().at(this->m_id).getActiveSchemeName().empty())
    {
        if (XR_LOGIC::try_switch_to_another_section(
                p_client_object, *this->m_storage, DataBase::Storage::getInstance().getActor()))
        {
            return;
        }
    }
}

} // namespace Scripts
} // namespace Cordis
