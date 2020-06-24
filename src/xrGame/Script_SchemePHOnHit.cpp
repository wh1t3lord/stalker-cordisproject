#include "stdafx.h"
#include "Script_SchemePHOnHit.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemePHOnHit::Script_SchemePHOnHit(CScriptGameObject* const p_client_object, DataBase::Script_ComponentScheme_PHOnHit* storage)
    : inherited_scheme(p_client_object, storage), m_p_storage(storage)
{
}

Script_SchemePHOnHit::~Script_SchemePHOnHit(void) {}

void Script_SchemePHOnHit::hit_callback(CScriptGameObject* const p_client_object, const float amount,
    const Fvector& local_direction, CScriptGameObject* const p_client_who, const std::int16_t bone_index)
{
    MESSAGE("%s %d %s %d",
        p_client_object->Name(), amount, p_client_who ? p_client_who->Name() : "Unknown", bone_index);

    if (!DataBase::Storage::getInstance().getStorage().at(this->m_id).getActiveSchemeName().empty())
    {
        if (XR_LOGIC::try_switch_to_another_section(
                p_client_object, this->m_p_storage, DataBase::Storage::getInstance().getActor()))
        {
            return;
        }
    }
}

void Script_SchemePHOnHit::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    if (!p_ini)
    {
        MESSAGEW("p_ini == nullptr!");
    }

    DataBase::Script_ComponentScheme_PHOnHit* p_storage = XR_LOGIC::assign_storage_and_bind<DataBase::Script_ComponentScheme_PHOnHit>(p_client_object, p_ini, scheme_name, section_name, gulag_name);

    if (!p_storage)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    p_storage->setLogic(XR_LOGIC::cfg_get_switch_conditions(p_ini, section_name, p_client_object));
}

} // namespace Scripts
} // namespace Cordis
