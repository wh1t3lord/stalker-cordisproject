#include "stdafx.h"
#include "Script_SchemePHOscillate.h"
#include "physics_shell_scripted.h"
#include "physics_joint_scripted.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemePHOscillate::Script_SchemePHOscillate(
    CScriptGameObject* const p_client_object, DataBase::Script_ComponentScheme_PHOscillate* storage)
    : inherited_scheme(p_client_object, storage), m_koeff(0.0f), m_time(0), m_p_joint(nullptr), m_is_pause(false), m_p_storage(storage)
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

    this->m_koeff = this->m_p_storage->getForce() / static_cast<float>(this->m_p_storage->getPeriod());
    this->m_p_joint = this->m_npc->get_physics_shell()->get_Joint(this->m_p_storage->getJointName().c_str());
    this->m_time = Globals::get_time_global();
    this->m_is_pause = false;
}

void Script_SchemePHOscillate::update(const float delta)
{
    std::uint32_t current_time = Globals::get_time_global();

    if (this->m_is_pause)
    {
        if (current_time - this->m_time < this->m_p_storage->getPeriod() / 2)
        {
            return;
        }

        this->m_time = current_time;
        this->m_is_pause = false;
    }

    if (current_time - this->m_time >= this->m_p_storage->getPeriod())
    {
        this->m_direction.x = -this->m_direction.x;
        this->m_direction.z = -this->m_direction.z;
        float angle = this->m_p_storage->getAngle();
        this->m_direction =
            Globals::vector_rotate_y(Fvector().set(-this->m_direction.x, 0.0f, -this->m_direction.z), angle);
        this->m_time = current_time;
        this->m_is_pause = true;
        return;
    }

    float force = static_cast<float>((current_time - this->m_time)) * this->m_koeff;
    this->m_npc->set_const_force(this->m_direction, force, 2);
}

void Script_SchemePHOscillate::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    if (!p_ini)
    {
        MESSAGEW("p_ini == nullptr!");
    }

    DataBase::Script_ComponentScheme_PHOscillate* p_storage =
        XR_LOGIC::assign_storage_and_bind<DataBase::Script_ComponentScheme_PHOscillate>(p_client_object, p_ini, scheme_name, section_name, gulag_name);

    if (!p_storage)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    p_storage->setLogic(XR_LOGIC::cfg_get_switch_conditions(p_ini, section_name, p_client_object));
    p_storage->setJointName(Globals::Utils::cfg_get_string(p_ini, section_name, "joint"));
    if (p_storage->getJointName().empty())
    {
        R_ASSERT2(false, "joint name can't an empty string");
        return;
    }

    p_storage->setPeriod(
        static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(p_ini, section_name, "period")));
    p_storage->setForce(Globals::Utils::cfg_get_number(p_ini, section_name, "force"));

    if (fis_zero(p_storage->getForce()) || !p_storage->getPeriod())
    {
        R_ASSERT2(false, "they are not defined! Please check your configuration file and set the values!");
        return;
    }

    p_storage->setAngle(Globals::Utils::cfg_get_number(p_ini, section_name, "correct_angle"));
}

} // namespace Scripts
} // namespace Cordis
