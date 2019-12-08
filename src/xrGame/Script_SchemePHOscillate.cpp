#include "stdafx.h"
#include "Script_SchemePHOscillate.h"
#include "physics_shell_scripted.h"
#include "physics_joint_scripted.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemePHOscillate::Script_SchemePHOscillate(
    CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage), m_koeff(0.0f), m_time(0), m_p_joint(nullptr), m_is_pause(false)
{
}

Script_SchemePHOscillate::~Script_SchemePHOscillate(void) {}

void Script_SchemePHOscillate::reset_scheme(const bool value, CScriptGameObject* const p_client_object)
{
    this->m_time = Globals::get_time_global();
    this->m_direction = Fvector()
                            .set(Globals::Script_RandomFloat::getInstance().Generate(), 0.0f,
                                Globals::Script_RandomFloat::getInstance().Generate())
                            .normalize();
    this->m_koeff = this->m_p_storage->getForce() / static_cast<float>(this->m_p_storage->getPHOscillatePeriod());
    this->m_p_joint = this->m_npc->get_physics_shell()->get_Joint(this->m_p_storage->getPHOscillateJointName().c_str());
    this->m_time = Globals::get_time_global();
    this->m_is_pause = false;
}

void Script_SchemePHOscillate::update(const float delta)
{
    std::uint32_t current_time = Globals::get_time_global();

    if (this->m_is_pause)
    {
        if (current_time - this->m_time < this->m_p_storage->getPHOscillatePeriod() / 2)
        {
            return;
        }

        this->m_time = current_time;
        this->m_is_pause = false;
    }

    if (current_time - this->m_time >= this->m_p_storage->getPHOscillatePeriod())
    {
        this->m_direction.x = -this->m_direction.x;
        this->m_direction.z = -this->m_direction.z;
        float angle = this->m_p_storage->getAngle();
        this->m_direction = Globals::vector_rotate_y(Fvector().set(-this->m_direction.x, 0.0f, -this->m_direction.z), angle);
        this->m_time = current_time;
        this->m_is_pause = true;
        return;
    }

    float force = static_cast<float>((current_time - this->m_time)) * this->m_koeff;
    this->m_npc->set_const_force(this->m_direction, force, 2);
}

} // namespace Scripts
} // namespace Cordis
