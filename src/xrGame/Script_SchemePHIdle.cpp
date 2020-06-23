#include "stdafx.h"
#include "Script_SchemePHIdle.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemePHIdle::Script_SchemePHIdle(CScriptGameObject* const p_client_object, DataBase::Script_ComponentScheme_PHIdle* storage)
    : inherited_scheme(p_client_object, storage), m_p_storage(storage)
{
    this->m_scheme_name = "ph_idle";
}

Script_SchemePHIdle::~Script_SchemePHIdle(void) {}

void Script_SchemePHIdle::reset_scheme(const bool value, CScriptGameObject* const p_client_object)
{
    Msg("[Scripts/Script_SchemePHIdle/reset_scheme(is_loading, p_client_object)] %s", this->m_npc->Name());

    this->m_npc->SetNonscriptUsable(this->m_p_storage->IsNonScriptUsable());
}

void Script_SchemePHIdle::update(const float delta)
{
    if (XR_LOGIC::try_switch_to_another_section(
            this->m_npc, this->m_p_storage, DataBase::Storage::getInstance().getActor()))
        return;
}

void Script_SchemePHIdle::hit_callback(CScriptGameObject* const p_client_object, const float amount,
    const Fvector& local_direction, CScriptGameObject* const p_client_who, const std::int16_t bone_index)
{
    MESSAGE("%s %d %s %d",
        p_client_object->Name(), amount, p_client_who ? p_client_who->Name() : "Unknown", bone_index);

    if (!this->m_p_storage->getHitOnBone().at(bone_index).empty())
    {
        XR_LOGIC::switch_to_section(p_client_object, this->m_p_storage->getIni(),
            XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_npc,
                this->m_p_storage->getHitOnBone().at(bone_index)));
    }
}

void Script_SchemePHIdle::deactivate(CScriptGameObject* const p_client_object) { this->m_npc->SetTipText(""); }

void Script_SchemePHIdle::use_callback(CScriptGameObject* const p_client_object, CScriptGameObject* const p_client_who)
{
    if (!this->m_p_storage->getOnUseCondlist().empty())
    {
        if (XR_LOGIC::switch_to_section(this->m_npc, this->m_p_storage->getIni(),
                XR_LOGIC::pick_section_from_condlist(
                    DataBase::Storage::getInstance().getActor(), this->m_npc, this->m_p_storage->getOnUseCondlist())))
        {
            return;
        }
    }
}

void Script_SchemePHIdle::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    DataBase::Script_ComponentScheme_PHIdle* p_storage =
        XR_LOGIC::assign_storage_and_bind<DataBase::Script_ComponentScheme_PHIdle>(p_client_object, p_ini, scheme_name, section_name, gulag_name);

    if (!p_storage)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    p_storage->setLogic(XR_LOGIC::cfg_get_switch_conditions(p_ini, section_name, p_client_object));
    p_storage->setNonScriptUsable(Globals::Utils::cfg_get_bool(p_ini, section_name, "nonscript_usable"));
    p_storage->setTipName(Globals::Utils::cfg_get_string(p_ini, section_name, "tips"));
    p_storage->setOnUseCondlist(XR_LOGIC::cfg_get_condlist(p_ini, section_name, "on_use", p_client_object).getCondlist());
    p_storage->setHitOnBone(Globals::Utils::parse_data_1v(
        p_client_object, Globals::Utils::cfg_get_string(p_ini, section_name, "hit_on_bone")));
    p_client_object->SetTipText(p_storage->getTipName().c_str());
}

} // namespace Scripts
} // namespace Cordis
