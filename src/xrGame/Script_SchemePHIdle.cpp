#include "stdafx.h"
#include "Script_SchemePHIdle.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemePHIdle::Script_SchemePHIdle(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage)
{
    this->m_scheme_name = "ph_idle";
}

Script_SchemePHIdle::~Script_SchemePHIdle(void) {}

void Script_SchemePHIdle::reset_scheme(const bool value, CScriptGameObject* const p_client_object)
{
    Msg("[Scripts/Script_SchemePHIdle/reset_scheme(is_loading, p_client_object)] %s", this->m_npc->Name());

    this->m_npc->SetNonscriptUsable(this->m_storage->IsPHIdleNonScriptUsable());
}

void Script_SchemePHIdle::update(const float delta)
{
    if (XR_LOGIC::try_switch_to_another_section(
            this->m_npc, *this->m_storage, DataBase::Storage::getInstance().getActor()))
        return;
}

void Script_SchemePHIdle::hit_callback(CScriptGameObject* const p_client_object, const float amount,
    const Fvector& local_direction, CScriptGameObject* const p_client_who, const std::int16_t bone_index)
{
    Msg("[Scripts/Script_SchemePHIdle/hit_callback(p_client_object, amount, local_direction, p_client_who, "
        "bone_index)] %s %d %s %d",
        p_client_object->Name(), amount, p_client_who ? p_client_who->Name() : "Unknown", bone_index);

    if (!this->m_storage->getHitOnBone().at(bone_index).empty())
    {
        XR_LOGIC::switch_to_section(p_client_object, this->m_storage->getIni(),
            XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_npc,
                this->m_storage->getHitOnBone().at(bone_index)));
    }
}

void Script_SchemePHIdle::deactivate(CScriptGameObject* const p_client_object) { this->m_npc->SetTipText(""); }

void Script_SchemePHIdle::use_callback(CScriptGameObject* const p_client_object, CScriptGameObject* const p_client_who)
{
    if (!this->m_storage->getOnUseCondlist().empty())
    {
        if (XR_LOGIC::switch_to_section(this->m_npc, this->m_storage->getIni(), XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_npc, this->m_storage->getOnUseCondlist())))
        {
            return;
        }
    }
}

} // namespace Scripts
} // namespace Cordis
