#include "stdafx.h"
#include "Script_SchemeXRHit.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemeXRHit::Script_SchemeXRHit(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage)
{
    this->m_scheme_name = "hit";
}

Script_SchemeXRHit::~Script_SchemeXRHit(void) {}

void Script_SchemeXRHit::hit_callback(CScriptGameObject* const p_client_object, const float amount,
    const Fvector& local_direction, CScriptGameObject* const p_client_who, const std::int16_t bone_index)
{
    DataBase::Storage::getInstance().setStorageHitBoneIndex(this->m_id, bone_index);

    if (fis_zero(amount) && !(p_client_object->invulnerable()))
    {
        return;
    }

    if (p_client_who)
    {
        Msg("[Scripts/Script_SchemeXRHit/hit_callback(p_client_object, amount, local_direction, p_client_who, "
            "bone_index)] %s hit by %s",
            p_client_object->Name(), p_client_who->Name());
        DataBase::Storage::getInstance().setStorageHitWhoID(p_client_object->ID(), p_client_who->ID());
    }
    else
    {
        Msg("[Scripts/Script_SchemeXRHit/hit_callback(p_client_object, amount, local_direction, p_client_who, "
            "bone_index)] %s hit by Unknown",
            p_client_object->Name());
        DataBase::Storage::getInstance().setStorageHitWhoID(p_client_object->ID(), Globals::kUnsignedInt16Undefined);
    }

    if (!DataBase::Storage::getInstance().getStorage().at(this->m_id).getActiveSchemeName().empty())
    {
        DataBase::Storage::getInstance().setStorageHitDeadlyHit(
            this->m_id, (amount >= this->m_npc->GetHealth() * 100.0f));

        if (XR_LOGIC::try_switch_to_another_section(
                p_client_object, *this->m_p_storage, DataBase::Storage::getInstance().getActor()))
        {
            DataBase::Storage::getInstance().setStorageHitDeadlyHit(this->m_id, false);
            return;
        }
        DataBase::Storage::getInstance().setStorageHitDeadlyHit(this->m_id, false);
    }
}

} // namespace Scripts
} // namespace Cordis
