#include "stdafx.h"
#include "Script_SchemePHForce.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemePHForce::Script_SchemePHForce(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage), m_is_process(false), m_time(0)
{
}

Script_SchemePHForce::~Script_SchemePHForce(void) {}

void Script_SchemePHForce::reset_scheme(const bool value, CScriptGameObject* const p_client_object)
{
    if (!this->m_storage->getPHForceDelay())
    {
        this->m_time = Globals::get_time_global() + this->m_storage->getPHForceDelay();
    }
    this->m_is_process = false;
}

void Script_SchemePHForce::update(const float delta)
{
    if (XR_LOGIC::try_switch_to_another_section(
            this->m_npc, *this->m_storage, DataBase::Storage::getInstance().getActor()))
        return;

    if (this->m_is_process)
        return;

    if (this->m_storage->getPHForceDelay())
    {
        if (static_cast<int>(Globals::get_time_global()) - static_cast<int>(this->m_time) < 0)
        {
            return;
        }
    }

    Fvector direction = Fvector(this->m_storage->getPHForcePoint()).sub(this->m_npc->Position());
    direction.normalize();
    this->m_npc->set_const_force(direction, this->m_storage->getForce(), this->m_storage->getPHForceTime());
    this->m_is_process = true;
}

} // namespace Scripts
} // namespace Cordis
