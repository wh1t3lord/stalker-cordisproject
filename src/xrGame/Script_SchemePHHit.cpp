#include "stdafx.h"
#include "Script_SchemePHHit.h"
#include "script_hit.h"
#include "Script_XR_Logic.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemePHHit::Script_SchemePHHit(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage)
{
    this->m_scheme_name = "ph_hit";
}

Script_SchemePHHit::~Script_SchemePHHit(void) {}

void Script_SchemePHHit::reset_scheme(const bool value, CScriptGameObject* const p_client_object)
{
    Msg("[Scripts/Script_SchemePHHit/reset_scheeme(is_loading, p_client_object)] %s", this->m_npc->Name());

    
    const Fvector& patrol_position = CPatrolPathParams(this->m_storage->getPHHitDirectionPathName().c_str()).point(std::uint32_t(0));
    const Fvector& entity_position = this->m_npc->Position();
    CScriptHit hit;

    hit.m_fPower = this->m_storage->getPHHitPower();
    hit.m_fImpulse = this->m_storage->getPHHitImpulse();
    hit.set_bone_name(this->m_storage->getPHHitBoneName().c_str());
    hit.m_tHitType = ALife::eHitTypeStrike;
    hit.m_tDirection = Fvector(patrol_position).sub(entity_position);
    hit.m_tpDraftsman = this->m_npc;
    this->m_npc->Hit(&hit);
    
    Msg("[Scripts/Script_SchemePHHit/reset_scheme(is_loading, p_client_object)] %f %f %f", hit.m_tDirection.x, hit.m_tDirection.y, hit.m_tDirection.z);
}

void Script_SchemePHHit::update(const float delta) 
{
    if (!DataBase::Storage::getInstance().getActor())
        return;
    
    if (XR_LOGIC::try_switch_to_another_section(
            this->m_npc, *this->m_storage, DataBase::Storage::getInstance().getActor()))
        return;
}

} // namespace Scripts
} // namespace Cordis
