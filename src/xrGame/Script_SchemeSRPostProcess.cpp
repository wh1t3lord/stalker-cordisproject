#include "stdafx.h"
#include "Script_SchemeSRPostProcess.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemeSRPostProcess::Script_SchemeSRPostProcess(
    CScriptGameObject* const p_client_object, DataBase::Script_ComponentScheme_SRPostProcess* storage)
    : inherited_scheme(p_client_object, storage), m_is_actor_inside(false), m_intensity(0.0f), m_intensity_base(0.0f),
      m_hit_power(0.0f), m_gray_amplitude(0.0f), m_hit_time(0), m_eff_time(0), m_intensity_inertion(0.0f), m_p_storage(storage)
{
    this->m_scheme_name = "sr_postprocess";
}

Script_SchemeSRPostProcess::~Script_SchemeSRPostProcess(void) {}

void Script_SchemeSRPostProcess::reset_scheme(const bool value, CScriptGameObject* const p_client_object)
{
    this->m_is_actor_inside = false;
    this->m_gray_amplitude = 1.0f;
    this->m_gray_color = SPPInfo::SColor(0.5f, 0.5f, 0.5f);
    this->m_base_color = SPPInfo::SColor(0.5f, 0.5f, 0.5f);
    this->m_noise = SPPInfo::SNoise(0.9f, 0.5f, 30.0f);
    this->m_eff_time = 0;
    this->m_hit_time = 0;
    this->m_intensity = 0.0f;
    this->m_intensity_base = this->m_p_storage->getIntensity();
    this->m_hit_power = 0.0f;

    if (this->m_intensity_base < 0.0f)
    {
        this->m_intensity_inertion = -this->m_p_storage->getIntensitySpeed();
    }
    else
    {
        this->m_intensity_inertion = this->m_p_storage->getIntensitySpeed();
    }

    //     this->m_postprocess = CScriptEffector(this->m_id + 2000, 10000000);
    //     this->m_postprocess.Add();
}

void Script_SchemeSRPostProcess::deactivate(CScriptGameObject* const p_client_object) {}

void Script_SchemeSRPostProcess::update(const float delta) {}

void Script_SchemeSRPostProcess::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    DataBase::Script_ComponentScheme_SRPostProcess* p_storage =
        XR_LOGIC::assign_storage_and_bind<DataBase::Script_ComponentScheme_SRPostProcess>(p_client_object, p_ini, scheme_name, section_name, gulag_name);

    if (!p_storage)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    p_storage->setLogic(XR_LOGIC::cfg_get_switch_conditions(p_ini, section_name, p_client_object));

    float hit_intesity = Globals::Utils::cfg_get_number(p_ini, section_name, "hit_intensity");

    if (fis_zero(hit_intesity))
        hit_intesity = 1.0f;

    p_storage->setHitIntensity(hit_intesity);

    float intensity = Globals::Utils::cfg_get_number(p_ini, section_name, "intensity");

    if (fis_zero(intensity))
        intensity = 0.01f;

    p_storage->setIntensity(intensity);

    float intensity_speed = Globals::Utils::cfg_get_number(p_ini, section_name, "intensity_speed");

    if (fis_zero(intensity_speed))
        intensity_speed = 0.01f;

    p_storage->setIntensitySpeed(intensity_speed);
}

void Script_SchemeSRPostProcess::update_hit(const float delta) {}
} // namespace Scripts
} // namespace Cordis
