#include "stdafx.h"
#include "Script_SchemeSRPostProcess.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemeSRPostProcess::Script_SchemeSRPostProcess(
    CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage), m_is_actor_inside(false), m_intensity(0.0f), m_intensity_base(0.0f),
      m_hit_power(0.0f), m_gray_amplitude(0.0f), m_hit_time(0), m_eff_time(0), m_intensity_inertion(0.0f)
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
    this->m_intensity_base = this->m_p_storage->getSRPostProcessIntensity();
    this->m_hit_power = 0.0f;

    if (this->m_intensity_base < 0.0f)
    {
        this->m_intensity_inertion = -this->m_p_storage->getSRPostProcessIntensitySpeed();
    }
    else
    {
        this->m_intensity_inertion = this->m_p_storage->getSRPostProcessIntensitySpeed();
    }

//     this->m_postprocess = CScriptEffector(this->m_id + 2000, 10000000);
//     this->m_postprocess.Add();
}

void Script_SchemeSRPostProcess::deactivate(CScriptGameObject* const p_client_object)
{

}

void Script_SchemeSRPostProcess::update(const float delta) {}

void Script_SchemeSRPostProcess::update_hit(const float delta) {}
} // namespace Scripts
} // namespace Cordis
