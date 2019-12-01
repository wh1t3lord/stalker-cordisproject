#include "stdafx.h"
#include "Script_SchemePHButton.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemePHButton::Script_SchemePHButton(
    CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage), m_last_hit_time(0)
{
    this->m_scheme_name = "ph_button";
}

Script_SchemePHButton::~Script_SchemePHButton(void) {}

void Script_SchemePHButton::reset_scheme(const bool value, CScriptGameObject* const p_client_object)
{
    this->m_npc->play_cycle(this->m_storage->getPHButtonAnimationName().c_str(), this->m_storage->IsPHButtonBlending());
    this->m_last_hit_time = Globals::get_time_global();
}

void Script_SchemePHButton::update(const float delta)
{
    if (XR_LOGIC::try_switch_to_another_section(
            this->m_npc, *this->m_storage, DataBase::Storage::getInstance().getActor()))
        return;
}

void Script_SchemePHButton::use_callback(
    CScriptGameObject* const p_client_object, CScriptGameObject* const p_client_who)
{
    Msg("[Scripts/Script_SchemePHButton/use_callbakc(p_client_object, p_client_who)] used for %s %s", p_client_object->Name(), p_client_who->Name());

    if (this->try_switch())
        return;
}

void Script_SchemePHButton::hit_callback(CScriptGameObject* p_client_object, const float amount,
    const Fvector& local_direction, CScriptGameObject* p_client_who, const std::int16_t bone_index)
{
    // @ empty code
}
bool Script_SchemePHButton::try_switch(void)
{
    const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(this->m_id);
    if (!storage.getActiveSchemeName().empty())
    {
        if (storage.getActiveSchemeName() == "ph_button" && (!this->m_storage->getPHButtonOnPressCondlist().empty()))
        {
            if (XR_LOGIC::switch_to_section(this->m_npc, this->m_storage->getIni(),
                    XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_npc,
                        this->m_storage->getPHButtonOnPressCondlist())))
            {
                return true;
            }
        }
    }

    return false;
}
} // namespace Scripts
} // namespace Cordis
