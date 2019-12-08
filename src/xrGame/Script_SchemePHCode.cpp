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

} // namespace Scripts
} // namespace Cordis
