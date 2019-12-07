#include "stdafx.h"
#include "Script_SchemeSRDeimos.h"

constexpr std::uint32_t _kPostProcessEffectorID = 5;
constexpr std::uint32_t _kCameraEffectorID = 6;
constexpr std::uint32_t _kPostProcessEffector2ID = 7;

namespace Cordis
{
namespace Scripts
{
Script_SchemeSRDeimos::Script_SchemeSRDeimos(
    CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage)
{
    this->m_scheme_name = "sr_deimos";
    this->m_storage->setSRDeimosIntensity(0.0f);
    this->m_camera_effector_time = 0;
    this->m_phase = 0;
}

Script_SchemeSRDeimos::~Script_SchemeSRDeimos(void) {}

void Script_SchemeSRDeimos::update(const float delta)
{
    if (!DataBase::Storage::getInstance().getActor() || Device.dwPrecacheFrame > 1)
    {
        return;
    }
    CScriptGameObject* const p_client_actor = DataBase::Storage::getInstance().getActor();

    Script_Binder_Actor* const p_binder_actor = dynamic_cast<Script_Binder_Actor*>(p_client_actor->binded_object());
    if (!p_binder_actor)
    {
        R_ASSERT2(false, "can't be check your binded object for Actor");
        return;
    }

    if (p_binder_actor->getDeimosIntensity())
    {
        this->m_storage->setSRDeimosIntensity(p_binder_actor->getDeimosIntensity());
        p_binder_actor->setDeimosIntensity(0.0f);

        if (this->m_storage->getSRDeimosIntensity() > this->m_storage->getSRDeimosDisableBound())
        {
            // Lord: протестировать
            Globals::Game::level::add_pp_effector(
                xr_string(this->m_storage->getSRDeimosPostProcessEffectorName()).append(".ppe").c_str(),
                _kPostProcessEffectorID, true);
            XR_SOUND::play_sound_looped(p_client_actor->ID(), this->m_storage->getSRDeimosNoiseSoundName());
            this->m_phase = 1;
        }

        if (this->m_storage->getSRDeimosIntensity() > this->m_storage->getSRDeimosSwitchLowerBound())
        {
            XR_SOUND::play_sound_looped(p_client_actor->ID(), this->m_storage->getSRDeimosHeartBeetSoundName());
            this->m_phase = 2;
        }
    }

    const Fvector& movement_speed = p_client_actor->GetMovementSpeed();
    float current_speed = movement_speed.magnitude();
    float intensity_delta = (this->m_storage->getSRDeimosMovementSpeed() - current_speed) * 0.005f;

    if (intensity_delta)
        intensity_delta = this->m_storage->getSRDeimosGrowingKoef() * intensity_delta;
    else
        intensity_delta = this->m_storage->getSRDeimosLoweringKoef() * intensity_delta;

    this->m_storage->setSRDeimosIntensity((this->m_storage->getSRDeimosIntensity() + intensity_delta));

    if (this->m_storage->getSRDeimosIntensity() > 1.0f)
        this->m_storage->setSRDeimosIntensity(1.0f);
    else if (this->m_storage->getSRDeimosIntensity() < 0.0f)
        this->m_storage->setSRDeimosIntensity(0.0f);

    float postprocess_intensity = this->m_storage->getSRDeimosIntensity();
    float noise_intensity = this->m_storage->getSRDeimosIntensity();
    float heartbeet_intensity = this->m_storage->getSRDeimosIntensity();

    if (this->m_phase)
    {
        Globals::Game::level::set_pp_effector_factor(_kPostProcessEffectorID, postprocess_intensity);
        XR_SOUND::set_volume_sound_looped(
            p_client_actor->ID(), this->m_storage->getSRDeimosNoiseSoundName(), noise_intensity);

        if (this->m_phase > 1)
            XR_SOUND::set_volume_sound_looped(
                p_client_actor->ID(), this->m_storage->getSRDeimosHeartBeetSoundName(), heartbeet_intensity);
    }

    if (intensity_delta)
    {
        if (this->m_storage->getSRDeimosIntensity() > this->m_storage->getSRDeimosSwitchUpperBound())
        {
            std::uint32_t current_time = Globals::get_time_global();
            if (current_time - this->m_camera_effector_time > this->m_storage->getSRDeimosCameraEffectorRepeatingTime())
            {
                this->m_camera_effector_time = Globals::get_time_global();
                Globals::Game::level::add_cam_effector(xr_string("camera_effects\\")
                                                           .append(this->m_storage->getSRDeimosCameraEffectorName())
                                                           .append(".anm")
                                                           .c_str(),
                    _kCameraEffectorID, false, "");
                Globals::Game::level::add_pp_effector(
                    xr_string(this->m_storage->getSRDeimosPostProcessEffector2Name()).append(".ppe").c_str(),
                    _kPostProcessEffector2ID, false);
                p_client_actor->SetHealth(-this->m_storage->getSRDeimosHealthLost());
            }
        }
        else if (this->m_storage->getSRDeimosIntensity() > this->m_storage->getSRDeimosSwitchLowerBound())
        {
            if (this->m_phase < 2)
            {
                XR_SOUND::play_sound_looped(p_client_actor->ID(), this->m_storage->getSRDeimosHeartBeetSoundName());
                XR_SOUND::set_volume_sound_looped(
                    p_client_actor->ID(), this->m_storage->getSRDeimosHeartBeetSoundName(), heartbeet_intensity);
                this->m_phase = 2;
            }
        }
        else if (this->m_storage->getSRDeimosIntensity() > this->m_storage->getSRDeimosDisableBound())
        {
            if (this->m_phase < 1)
            {
                Globals::Game::level::add_pp_effector(
                    xr_string(this->m_storage->getSRDeimosPostProcessEffectorName()).append(".ppe").c_str(),
                    _kPostProcessEffectorID, true);
                Globals::Game::level::set_pp_effector_factor(_kPostProcessEffectorID, postprocess_intensity);
                XR_SOUND::play_sound_looped(p_client_actor->ID(), this->m_storage->getSRDeimosNoiseSoundName());
                XR_SOUND::set_volume_sound_looped(
                    p_client_actor->ID(), this->m_storage->getSRDeimosNoiseSoundName(), noise_intensity);
                this->m_phase = 1;
            }
        }
    }
    else
    {
        if (this->m_storage->getSRDeimosIntensity() < this->m_storage->getSRDeimosDisableBound())
        {
            if (this->m_phase)
            {
                XR_SOUND::stop_sound_looped(p_client_actor->ID(), this->m_storage->getSRDeimosNoiseSoundName());
                Globals::Game::level::remove_pp_effector(_kPostProcessEffectorID);
                this->m_phase = 0;
            }
        }
        else if (this->m_storage->getSRDeimosIntensity() < this->m_storage->getSRDeimosSwitchLowerBound())
        {
            if (this->m_phase > 1)
            {
                XR_SOUND::stop_sound_looped(p_client_actor->ID(), this->m_storage->getSRDeimosHeartBeetSoundName());
                this->m_phase = 1;
            }
        }
        else if (this->m_storage->getSRDeimosIntensity() < this->m_storage->getSRDeimosSwitchUpperBound())
        {
            Globals::Game::level::remove_cam_effector(_kCameraEffectorID);
            Globals::Game::level::remove_pp_effector(_kPostProcessEffector2ID);
        }
    }

    if (XR_LOGIC::try_switch_to_another_section(this->m_npc, *this->m_storage, p_client_actor))
    {
        if (this->m_phase)
        {
            XR_SOUND::stop_sound_looped(p_client_actor->ID(), this->m_storage->getSRDeimosNoiseSoundName());
            Globals::Game::level::remove_pp_effector(_kPostProcessEffectorID);
            if (this->m_phase > 1)
            {
                XR_SOUND::stop_sound_looped(p_client_actor->ID(), this->m_storage->getSRDeimosHeartBeetSoundName());
                Globals::Game::level::remove_cam_effector(_kCameraEffectorID);
                Globals::Game::level::remove_pp_effector(_kPostProcessEffector2ID);
            }
        }

        return;
    }
}

} // namespace Scripts
} // namespace Cordis
