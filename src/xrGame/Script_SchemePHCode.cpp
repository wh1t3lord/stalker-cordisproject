#include "stdafx.h"
#include "Script_SchemePHCode.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemePHCode::Script_SchemePHCode(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage)
{
}

Script_SchemePHCode::~Script_SchemePHCode(void) {}

void Script_SchemePHCode::deactivate(CScriptGameObject* const p_client_object) { this->m_npc->SetTipText(""); }

void Script_SchemePHCode::reset_scheme(const bool value, CScriptGameObject* const p_client_object)
{
    this->m_npc->SetNonscriptUsable(false);
}

void Script_SchemePHCode::use_callback(CScriptGameObject* const p_client_object, CScriptGameObject* const p_client_who)
{
    // Lord: доделать когда будет ui_numpad
}

void Script_SchemePHCode::OnNumberReceive(const xr_string& text)
{
    if (this->m_p_storage->getPHCodeCode())
    {
        if (static_cast<std::uint32_t>(atoi(text.c_str())) == this->m_p_storage->getPHCodeCode())
        {
            if (!this->m_p_storage->getPHCodeOnCodeCondlist().empty())
            {
                Msg("[Scripts/Script_SchemePHCode/OnNumberReceive(text)] on_code %s", text.c_str());
                XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_npc,
                    this->m_p_storage->getPHCodeOnCodeCondlist());
            }
        }
    }
    else
    {
        const xr_map<std::uint32_t, CondlistData>& condlist = this->m_p_storage->getPHCodeOnCheckCode().at(text);
        if (!condlist.empty())
        {
            Msg("[Scripts/Script_SchemePHCode/OnNumberReceive(text)] on_check_code %s", text.c_str());
            XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_npc, condlist);
        }
    }
}

void Script_SchemePHCode::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "it can't be!");
        return;
    }

    DataBase::Storage_Scheme* p_storage =
        XR_LOGIC::assign_storage_and_bind(p_client_object, p_ini, scheme_name, section_name, gulag_name);

    p_storage->setLogic(XR_LOGIC::cfg_get_switch_conditions(p_ini, section_name, p_client_object));
    p_storage->setPHCodeTipName(Globals::Utils::cfg_get_string(p_ini, section_name, "tips").empty() ?
            "st_codelock" :
            Globals::Utils::cfg_get_string(p_ini, section_name, "tips"));
    p_client_object->SetTipText(p_storage->getPHCodeTipName().c_str());
    p_storage->setPHCodeCode(static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(p_ini, section_name, "code")));

    if (!p_storage->getPHCodeCode())
    {
        R_ASSERT2(false, "you must intiialize you key-code");
    }
}

} // namespace Scripts
} // namespace Cordis
