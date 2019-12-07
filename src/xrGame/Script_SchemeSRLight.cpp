#include "stdafx.h"
#include "Script_SchemeSRLight.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemeSRLight::Script_SchemeSRLight(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage), m_is_active(false)
{
}

Script_SchemeSRLight::~Script_SchemeSRLight(void) {}

void Script_SchemeSRLight::reset_scheme(const bool value, CScriptGameObject* const p_client_object)
{
    Msg("[Scripts/Script_SchemeSRLight/reset_scheme(is_loading, p_client_object)] %s", this->m_npc->Name());
    Script_SRLightManager::getInstance().m_light_zones[this->m_id] = this;
}

void Script_SchemeSRLight::update(const float delta)
{
    if (XR_LOGIC::try_switch_to_another_section(
            this->m_npc, *this->m_storage, DataBase::Storage::getInstance().getActor()))
    {
        this->m_is_active = false;
        Msg("[Scripts/Script_SchemeSRLight/update(delta)] Clear light zone");
        Script_SRLightManager::getInstance().m_light_zones[this->m_id] = nullptr;
        return;
    }

    this->m_is_active = true;
}

bool Script_SchemeSRLight::check_stalker(CScriptGameObject* const p_client_object, bool& is_returned_light_value)
{
    if (!this->m_is_active)
    {
        is_returned_light_value = false;
        return false;
    }

    if (this->m_npc->inside(p_client_object->Position()))
    {
        is_returned_light_value = this->m_storage->IsSRLightLight();
        return true; 
    }

    is_returned_light_value = false;
    return false;
}

} // namespace Scripts
} // namespace Cordis
