#include "stdafx.h"
#include "Script_SchemeSRLight.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemeSRLight::Script_SchemeSRLight(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage), m_is_active(false)
{
    this->m_scheme_name = "sr_light";
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
            this->m_npc, *this->m_p_storage, DataBase::Storage::getInstance().getActor()))
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
        is_returned_light_value = this->m_p_storage->IsSRLightLight();
        return true; 
    }

    is_returned_light_value = false;
    return false;
}

void Script_SRLightManager::check_light(CScriptGameObject* const p_client_object)
{
    if (!p_client_object)
    {
        Msg("[Scripts/Script_SRLightManager/check_light(p_client_object)] WARNING: p_client_object was nullptr "
            "return ...");
        return;
    }

    CScriptGameObject* const p_client_torch = p_client_object->GetObjectByName("device_torch");
    if (!p_client_torch)
    {
        Msg("[Scripts/Script_SRLightManager/check_light(p_client_object)] WARNING: p_client_torch was nullptr "
            "return ...");
    }

    bool is_light = false;
    bool is_forced = false;

    if (!p_client_object->Alive())
    {
        is_light = false;
        is_forced = true;
    }

    if (!is_forced)
    {
        for (std::pair<const std::uint16_t, Script_SchemeSRLight*>& it : this->m_light_zones)
        {
            if (it.second->check_stalker(p_client_object, is_light))
            {
                break;
            }
        }
    }

    if (!is_forced)
    {
        std::uint32_t hours = Globals::get_time_hours();

        if (hours <= 4 || hours >= 22)
            is_light = true;

        if (!is_light)
            if (Script_GlobalHelper::getInstance().getIndoorLevels().at(Globals::Game::level::get_name()))
                is_light = true;
    }

    if (!is_forced && is_light)
    {
        const xr_string& active_scheme_name =
            DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getActiveSchemeName();
        if (active_scheme_name == "kamp" || active_scheme_name == "camper" || active_scheme_name == "sleeper")
        {
            is_light = false;
            is_forced = true;
        }
    }

    if (!is_forced && is_light)
    {
        if (p_client_object->GetBestEnemy() &&
            Script_GlobalHelper::getInstance().getIndoorLevels().find(Globals::Game::level::get_name()) !=
                Script_GlobalHelper::getInstance().getIndoorLevels().end())
        {
            is_light = false;
        }
    }
    // @ Lord: I guess it right test it
    if (is_light)
        p_client_torch->enable_attachable_item(is_light);
}

} // namespace Scripts
} // namespace Cordis
