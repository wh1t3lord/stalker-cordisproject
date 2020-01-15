#include "stdafx.h"
#include "Script_SchemeSRNoWeapon.h"

constexpr std::uint32_t _kStateNoWhere = 0;
constexpr std::uint32_t _kStateInside = 1;
constexpr std::uint32_t _kStateOutSide = 2;

namespace Cordis
{
namespace Scripts
{
Script_SchemeSRNoWeapon::Script_SchemeSRNoWeapon(
    CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage), m_state(_kStateNoWhere)
{
    this->m_scheme_name = "sr_no_weapon";
}

Script_SchemeSRNoWeapon::~Script_SchemeSRNoWeapon(void) {}

void Script_SchemeSRNoWeapon::reset_scheme(const bool value, CScriptGameObject* const p_client_object)
{
    this->m_state = _kStateNoWhere;
    this->switch_state(DataBase::Storage::getInstance().getActor());
    DataBase::Storage::getInstance().setNoWeaponZones(this->m_npc->Name(), false);
}

void Script_SchemeSRNoWeapon::update(const float delta)
{
    CScriptGameObject* const p_client_actor = DataBase::Storage::getInstance().getActor();

    if (XR_LOGIC::try_switch_to_another_section(this->m_npc, *this->m_p_storage, p_client_actor))
    {
        if (this->m_state == _kStateInside)
            this->zone_leave();

        return;
    }

    this->switch_state(p_client_actor);

    StaticDrawableWrapper* const p_static = CurrentGameUI()->GetCustomStatic("can_use_weapon_now");
    if (p_static && Globals::Game::get_game_time().diffSec(this->m_inited_time) > 30.0f)
    {
        CurrentGameUI()->RemoveCustomStatic("can_use_weapon_now");
    }
}

void Script_SchemeSRNoWeapon::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    DataBase::Storage_Scheme* p_storage =
        XR_LOGIC::assign_storage_and_bind(p_client_object, p_ini, scheme_name, section_name, gulag_name);

    if (!p_storage)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    p_storage->setLogic(XR_LOGIC::cfg_get_switch_conditions(p_ini, section_name, p_client_object));
}

void Script_SchemeSRNoWeapon::zone_enter(void)
{
    this->m_state = _kStateInside;
    Script_Binder_Actor* const p_binder_actor =
        dynamic_cast<Script_Binder_Actor*>(DataBase::Storage::getInstance().getActor()->binded_object());
    if (!p_binder_actor)
    {
        R_ASSERT2(false, "it can't be! Check your binder for actor!");
        return;
    }

    p_binder_actor->setHideWeapon(true);
    StaticDrawableWrapper* const p_static = CurrentGameUI()->GetCustomStatic("can_use_weapon_now");
    if (p_static)
    {
        CurrentGameUI()->RemoveCustomStatic("can_use_weapon_now");
    }

    Msg("[Scripts/Script_SchemeSRNoWeapon/zone_enter()] entering no weapon zone [%s]", this->m_npc->Name());
}

void Script_SchemeSRNoWeapon::zone_leave(void)
{
    this->m_state = _kStateOutSide;
    Script_Binder_Actor* const p_binder_actor =
        dynamic_cast<Script_Binder_Actor*>(DataBase::Storage::getInstance().getActor()->binded_object());
    if (!p_binder_actor)
    {
        R_ASSERT2(false, "it can't be! Check your registerd binder for actor!");
        return;
    }

    p_binder_actor->setHideWeapon(false);
    StaticDrawableWrapper* const p_static = CurrentGameUI()->GetCustomStatic("can_use_weapon_now");
    if (p_static)
    {
        CurrentGameUI()->RemoveCustomStatic("can_use_weapon_now");
    }

    Msg("[Scripts/Script_SchemeSRNoWeapon/zone_leave()] leaving no weapon zone [%s]", this->m_npc->Name());
}

void Script_SchemeSRNoWeapon::switch_state(CScriptGameObject* const p_client_actor)
{
    if (this->m_state == _kStateNoWhere || this->m_state == _kStateOutSide)
    {
        if (this->m_npc->inside(p_client_actor->Center()))
        {
            this->zone_enter();
            return;
        }
    }

    if (this->m_state == _kStateInside || this->m_state == _kStateNoWhere)
    {
        if (!this->m_npc->inside(p_client_actor->Center()))
        {
            this->zone_leave();
            return;
        }

        CScriptGameObject* const p_active_item = p_client_actor->GetActiveItem();
        if (p_active_item && Globals::IsWeapon(p_active_item))
        {
            Msg("[Scripts/Script_SchemeSRNoWeapon/switch_state(p_client_actor)] WARNING: actor is inside, but with "
                "active weapon %s",
                p_active_item->Name());
        }
    }
}

} // namespace Scripts
} // namespace Cordis
