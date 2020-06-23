#include "stdafx.h"
#include "Script_SchemePHHit.h"
#include "script_hit.h"
#include "Script_XR_Logic.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemePHHit::Script_SchemePHHit(CScriptGameObject* const p_client_object, DataBase::Script_ComponentScheme_PHHit* storage)
    : inherited_scheme(p_client_object, storage), m_p_storage(storage)
{
    this->m_scheme_name = "ph_hit";
}

Script_SchemePHHit::~Script_SchemePHHit(void) {}

void Script_SchemePHHit::reset_scheme(const bool value, CScriptGameObject* const p_client_object)
{
    Msg("[Scripts/Script_SchemePHHit/reset_scheeme(is_loading, p_client_object)] %s", this->m_npc->Name());

    const Fvector& patrol_position =
        CPatrolPathParams(this->m_p_storage->getDirectionPathName().c_str()).point(std::uint32_t(0));
    const Fvector& entity_position = this->m_npc->Position();
    CScriptHit hit;

    hit.m_fPower = this->m_p_storage->getPower();
    hit.m_fImpulse = this->m_p_storage->getImpulse();
    hit.set_bone_name(this->m_p_storage->getBoneName().c_str());
    hit.m_tHitType = ALife::eHitTypeStrike;
    hit.m_tDirection = Fvector(patrol_position).sub(entity_position);
    hit.m_tpDraftsman = this->m_npc;
    this->m_npc->Hit(&hit);

    Msg("[Scripts/Script_SchemePHHit/reset_scheme(is_loading, p_client_object)] %f %f %f", hit.m_tDirection.x,
        hit.m_tDirection.y, hit.m_tDirection.z);
}

void Script_SchemePHHit::update(const float delta)
{
    if (!DataBase::Storage::getInstance().getActor())
        return;

    if (XR_LOGIC::try_switch_to_another_section(
            this->m_npc, this->m_p_storage, DataBase::Storage::getInstance().getActor()))
        return;
}

void Script_SchemePHHit::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    DataBase::Script_ComponentScheme_PHHit* p_storage =
        XR_LOGIC::assign_storage_and_bind<DataBase::Script_ComponentScheme_PHHit>(p_client_object, p_ini, scheme_name, section_name, gulag_name);

    if (!p_storage)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    p_storage->setLogic(XR_LOGIC::cfg_get_switch_conditions(p_ini, section_name, p_client_object));

    p_storage->setPower(Globals::Utils::cfg_get_number(p_ini, section_name, "power"));
    float impulse = Globals::Utils::cfg_get_number(p_ini, section_name, "impulse");

    if (fis_zero(impulse))
        impulse = 1000.0f;

    p_storage->setImpulse(impulse);
    p_storage->setBoneName(Globals::Utils::cfg_get_string(p_ini, section_name, "bone"));
    p_storage->setDirectionPathName(Globals::Utils::cfg_get_string(p_ini, section_name, "dir_path"));
}

} // namespace Scripts
} // namespace Cordis
