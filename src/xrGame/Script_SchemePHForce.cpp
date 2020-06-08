#include "stdafx.h"
#include "Script_SchemePHForce.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemePHForce::Script_SchemePHForce(CScriptGameObject* const p_client_object, void* storage)
    : inherited_scheme(p_client_object, storage), m_is_process(false), m_time(0)
{
}

Script_SchemePHForce::~Script_SchemePHForce(void) {}

void Script_SchemePHForce::reset_scheme(const bool value, CScriptGameObject* const p_client_object)
{
    if (!this->m_p_storage->getPHForceDelay())
    {
        this->m_time = Globals::get_time_global() + this->m_p_storage->getPHForceDelay();
    }
    this->m_is_process = false;
}

void Script_SchemePHForce::update(const float delta)
{
    if (XR_LOGIC::try_switch_to_another_section(
            this->m_npc, *this->m_p_storage, DataBase::Storage::getInstance().getActor()))
        return;

    if (this->m_is_process)
        return;

    if (this->m_p_storage->getPHForceDelay())
    {
        if (static_cast<int>(Globals::get_time_global()) - static_cast<int>(this->m_time) < 0)
        {
            return;
        }
    }

    Fvector direction = Fvector(this->m_p_storage->getPHForcePoint()).sub(this->m_npc->Position());
    direction.normalize();
    this->m_npc->set_const_force(direction, this->m_p_storage->getForce(), this->m_p_storage->getPHForceTime());
    this->m_is_process = true;
}

void Script_SchemePHForce::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    DataBase::Storage_Scheme* p_storage =
        XR_LOGIC::assign_storage_and_bind(p_client_object, p_ini, scheme_name, section_name, gulag_name);
    p_storage->setLogic(XR_LOGIC::cfg_get_switch_conditions(p_ini, section_name, p_client_object));

    p_storage->setForce(Globals::Utils::cfg_get_number(p_ini, section_name, "force"));
    p_storage->setPHForceTime(static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(p_ini, section_name, "time")));
    p_storage->setPHForceDelay(
        static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(p_ini, section_name, "delay")));

    if (fis_zero(p_storage->getForce()) || p_storage->getForce() <= 0.0f)
    {
        R_ASSERT2(false, "invalid force value");
    }

    if (p_storage->getPHForceTime() == 0 || p_storage->getPHForceTime() == Globals::kUnsignedInt32Undefined)
    {
        R_ASSERT2(false, "invalid time!");
    }

    xr_string path_name = Globals::Utils::cfg_get_string(p_ini, section_name, "point");
    std::uint32_t index =
        static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(p_ini, section_name, "point_index"));

    if (path_name.empty())
    {
        R_ASSERT2(false, "invalid waypoint_name!");
    }

    CPatrolPathParams patrol(path_name.c_str());

    if (index == Globals::kUnsignedInt32Undefined || patrol.count() <= index)
    {
        R_ASSERT2(false, "invalid waypoint_index");
    }

    p_storage->setPHForcePoint(patrol.point(index));
}

} // namespace Scripts
} // namespace Cordis
