#include "stdafx.h"
#include "Script_SchemeSRPsyAntenna.h"

constexpr std::uint32_t _kStateOutSide = 0;
constexpr std::uint32_t _kStateInside = 1;
constexpr std::uint32_t _kStateNotDefined = 2;

namespace Cordis
{
namespace Scripts
{
Script_SchemeSRPsyAntenna::Script_SchemeSRPsyAntenna(
    CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage), m_state(_kStateNotDefined),
      m_manager_psy_antenna(&Script_PsyAntennaManager::getInstance()){this->m_scheme_name = "sr_psy_antenna";}

      Script_SchemeSRPsyAntenna::~Script_SchemeSRPsyAntenna(void)
{
}

void Script_SchemeSRPsyAntenna::reset_scheme(const bool is_loading, CScriptGameObject* const p_client_object)
{
    if (is_loading)
    {
        this->m_state = XR_LOGIC::pstor_retrieve_number(this->m_npc, "inside");
    }

    if (this->m_state == _kStateInside)
    {
        this->zone_leave();
    }

    this->m_state = _kStateNotDefined;
    this->switch_state(DataBase::Storage::getInstance().getActor());
}

void Script_SchemeSRPsyAntenna::deactivate(CScriptGameObject* const p_client_object)
{
    if (this->m_state == _kStateInside)
    {
        this->zone_leave();
    }
}

void Script_SchemeSRPsyAntenna::update(const float delta)
{
    CScriptGameObject* p_client_object = DataBase::Storage::getInstance().getActor();

    if (XR_LOGIC::try_switch_to_another_section(this->m_npc, *this->m_p_storage, p_client_object))
    {
        return;
    }

    this->switch_state(p_client_object);
}

void Script_SchemeSRPsyAntenna::save(void)
{
    XR_LOGIC::pstor_store(this->m_npc, "inside", static_cast<std::uint8_t>(this->m_state));
}

void Script_SchemeSRPsyAntenna::zone_enter(void)
{
    this->m_state = _kStateInside;

    CurrentGameUI()->enable_fake_indicators(true);

    float sound_intensity_base =
        this->m_manager_psy_antenna->getSoundIntensityBase() + this->m_p_storage->getSRPsyAntennaIntensity();
    this->m_manager_psy_antenna->setSoundIntensityBase(sound_intensity_base);

    float mute_sound_threshold =
        this->m_manager_psy_antenna->getMuteSoundThreshold() + this->m_p_storage->getSRPsyAntennaMuteSoundThreshold();
    this->m_manager_psy_antenna->setMuteSoundThreshold(mute_sound_threshold);

    float hit_intensity = this->m_manager_psy_antenna->getHitIntensity() + this->m_p_storage->getSRPsyAntennaIntensity();
    this->m_manager_psy_antenna->setHitIntensity(hit_intensity);

    float phantom_spawn_probability = this->m_manager_psy_antenna->getPhantomSpawnProbability() +
        this->m_p_storage->getSRPsyAntennaPhantomProbability();
    this->m_manager_psy_antenna->setPhantomSpawnProbability(phantom_spawn_probability);

    this->m_manager_psy_antenna->setNoStatic(this->m_p_storage->IsSRPsyAntennaNoStatic());
    this->m_manager_psy_antenna->setNoMumble(this->m_p_storage->IsSRPsyAntennaNoMumble());
    this->m_manager_psy_antenna->setHitTypeName(this->m_p_storage->getSRPsyAntennaHitTypeName());
    this->m_manager_psy_antenna->setHitFrequency(this->m_p_storage->getSRPsyAntennaHitFrequency());

    if (this->m_p_storage->getSRPsyAntennaPostProcessName().empty() ||
        this->m_p_storage->getSRPsyAntennaPostProcessName() == "nil")
    {
        Msg("[Scripts/Script_SchemeSRPsyAntenna/update(delta)] WARNING: postprocess is 'nil' return!");
        return;
    }

    if (this->m_manager_psy_antenna->getPostProcess().at(this->m_p_storage->getSRPsyAntennaPostProcessName()) ==
        std::tuple<float, float, std::uint32_t>(0.0f, 0.0f, 0))
    {
        this->m_manager_psy_antenna->AddPostProcessCount();
        std::uint32_t id = 1500 + this->m_manager_psy_antenna->getPostProcessCount();
        this->m_manager_psy_antenna->setPostProcess(
            this->m_p_storage->getSRPsyAntennaPostProcessName(), std::make_tuple(0.0f, 0.0f, id));

        Globals::Game::level::add_pp_effector(this->m_p_storage->getSRPsyAntennaPostProcessName().c_str(), id, true);
        Globals::Game::level::set_pp_effector_factor(id, 0.01f);
    }

    float intensity_base = std::get<_POSTPROCESS_GET_INTENSITY_BASE>(this->m_manager_psy_antenna->getPostProcess().at(
                               this->m_p_storage->getSRPsyAntennaPostProcessName())) +
        this->m_p_storage->getSRPsyAntennaIntensity();

    this->m_manager_psy_antenna->setPostProcessIntensityBase(
        this->m_p_storage->getSRPsyAntennaPostProcessName(), intensity_base);
}

void Script_SchemeSRPsyAntenna::zone_leave(void)
{
    this->m_state = _kStateOutSide;
    CurrentGameUI()->enable_fake_indicators(false);

    float sound_intensity_base =
        this->m_manager_psy_antenna->getSoundIntensityBase() - this->m_p_storage->getSRPsyAntennaIntensity();
    this->m_manager_psy_antenna->setSoundIntensityBase(sound_intensity_base);

    float mute_sound_threshold =
        this->m_manager_psy_antenna->getMuteSoundThreshold() - this->m_p_storage->getSRPsyAntennaMuteSoundThreshold();
    this->m_manager_psy_antenna->setMuteSoundThreshold(mute_sound_threshold);

    float hit_intensity =
        this->m_manager_psy_antenna->getHitIntensity() - this->m_p_storage->getSRPsyAntennaHitIntensity();
    this->m_manager_psy_antenna->setHitIntensity(hit_intensity);

    float phantom_probability = this->m_manager_psy_antenna->getPhantomSpawnProbability() -
        this->m_p_storage->getSRPsyAntennaPhantomProbability();
    this->m_manager_psy_antenna->setPhantomSpawnProbability(phantom_probability);

    if (this->m_p_storage->getSRPsyAntennaPostProcessName() == "nil" ||
        this->m_p_storage->getSRPsyAntennaPostProcessName().empty())
    {
        Msg("[Scripts/Script_SchemeSRPsyAntenna/zone_leave()] WARNING: postprocess_name.empty() == true! You set an "
            "empty string");
        return;
    }

    if (this->m_manager_psy_antenna->getPostProcess().at(this->m_p_storage->getSRPsyAntennaPostProcessName()) !=
        std::tuple<float, float, std::uint32_t>(0.0f, 0.0f, 0))
    {
        float intensity_base =
            std::get<_POSTPROCESS_GET_INTENSITY_BASE>(
                this->m_manager_psy_antenna->getPostProcess().at(this->m_p_storage->getSRPsyAntennaPostProcessName())) -
            this->m_p_storage->getSRPsyAntennaIntensity();
        this->m_manager_psy_antenna->setPostProcessIntensityBase(
            this->m_p_storage->getSRPsyAntennaPostProcessName(), intensity_base);
    }
}

void Script_SchemeSRPsyAntenna::switch_state(CScriptGameObject* const p_client_actor)
{
    if (!p_client_actor)
    {
        R_ASSERT2(false, "can't be!");
    }

    if (this->m_state != _kStateInside)
    {
        if (this->m_npc->inside(p_client_actor->Position()))
        {
            this->zone_enter();
            return;
        }
    }
    else
    {
        if (!this->m_npc->inside(p_client_actor->Position()))
        {
            this->zone_leave();
            return;
        }
    }
}

} // namespace Scripts
} // namespace Cordis
