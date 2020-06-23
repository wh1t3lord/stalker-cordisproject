#include "stdafx.h"
#include "Script_SchemeSRPsyAntenna.h"

constexpr std::uint32_t _kPsyStateOutSide = 0;
constexpr std::uint32_t _kPsyStateInside = 1;
constexpr std::uint32_t _kPsyStateNotDefined = 2;

namespace Cordis
{
namespace Scripts
{
Script_SchemeSRPsyAntenna::Script_SchemeSRPsyAntenna(
    CScriptGameObject* const p_client_object, DataBase::Script_ComponentScheme_SRPsyAntenna* storage)
    : inherited_scheme(p_client_object, storage), m_state(_kPsyStateNotDefined),
      m_manager_psy_antenna(&Script_PsyAntennaManager::getInstance()), m_p_storage(storage)
{
    this->m_scheme_name = "sr_psy_antenna";
}

Script_SchemeSRPsyAntenna::~Script_SchemeSRPsyAntenna(void) {}

void Script_SchemeSRPsyAntenna::reset_scheme(const bool is_loading, CScriptGameObject* const p_client_object)
{
    if (is_loading)
    {
        this->m_state = XR_LOGIC::pstor_retrieve_number(this->m_npc, "inside");
    }

    if (this->m_state == _kPsyStateInside)
    {
        this->zone_leave();
    }

    this->m_state = _kPsyStateNotDefined;
    this->switch_state(DataBase::Storage::getInstance().getActor());
}

void Script_SchemeSRPsyAntenna::deactivate(CScriptGameObject* const p_client_object)
{
    if (this->m_state == _kPsyStateInside)
    {
        this->zone_leave();
    }
}

void Script_SchemeSRPsyAntenna::update(const float delta)
{
    CScriptGameObject* p_client_object = DataBase::Storage::getInstance().getActor();

    if (XR_LOGIC::try_switch_to_another_section(this->m_npc, this->m_p_storage, p_client_object))
    {
        return;
    }

    this->switch_state(p_client_object);
}

void Script_SchemeSRPsyAntenna::save(void)
{
    // Lord: желательно переделать
    XR_LOGIC::pstor_store(this->m_npc, "inside", static_cast<std::uint32_t>(this->m_state));
}

void Script_SchemeSRPsyAntenna::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    DataBase::Script_ComponentScheme_SRPsyAntenna* p_storage = XR_LOGIC::assign_storage_and_bind<DataBase::Script_ComponentScheme_SRPsyAntenna>(p_client_object, p_ini, scheme_name, section_name, gulag_name);

    if (!p_storage)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    p_storage->setLogic(XR_LOGIC::cfg_get_switch_conditions(p_ini, section_name, p_client_object));

    float intensity = Globals::Utils::cfg_get_number(p_ini, section_name, "eff_intensity");

    if (fis_zero(intensity))
        intensity = 0.01f;

    p_storage->setIntensity(intensity);

    xr_string postprocess_name = Globals::Utils::cfg_get_string(p_ini, section_name, "postprocess");

    if (postprocess_name.empty())
        postprocess_name = "psy_antenna.ppe";

    p_storage->setPostProcessName(postprocess_name);

    float hit_intensity = Globals::Utils::cfg_get_number(p_ini, section_name, "hit_intensity");

    if (fis_zero(hit_intensity))
        hit_intensity = 0.01f;

    p_storage->setHitIntensity(hit_intensity);

    p_storage->setPhantomProbability(
        Globals::Utils::cfg_get_number(p_ini, section_name, "phantom_prob") * 0.01f);

    p_storage->setMuteSoundThreshold(
        Globals::Utils::cfg_get_number(p_ini, section_name, "mute_sound_threshold"));
    p_storage->setNoStatic(Globals::Utils::cfg_get_bool(p_ini, section_name, "no_static"));
    p_storage->setNoMumble(Globals::Utils::cfg_get_bool(p_ini, section_name, "no_mumble"));

    xr_string wound_name = Globals::Utils::cfg_get_string(p_ini, section_name, "hit_type");

    if (wound_name.empty())
        wound_name = "wound";

    p_storage->setHitTypeName(wound_name);

    float hit_frequency = Globals::Utils::cfg_get_number(p_ini, section_name, "hit_freq");

    if (fis_zero(hit_frequency))
        hit_frequency = 5000.0f;

    p_storage->setHitFrequency(hit_frequency);
}

void Script_SchemeSRPsyAntenna::zone_enter(void)
{
    this->m_state = _kPsyStateInside;

    CurrentGameUI()->enable_fake_indicators(true);

    float sound_intensity_base =
        this->m_manager_psy_antenna->getSoundIntensityBase() + this->m_p_storage->getIntensity();
    this->m_manager_psy_antenna->setSoundIntensityBase(sound_intensity_base);

    float mute_sound_threshold =
        this->m_manager_psy_antenna->getMuteSoundThreshold() + this->m_p_storage->getMuteSoundThreshold();
    this->m_manager_psy_antenna->setMuteSoundThreshold(mute_sound_threshold);

    float hit_intensity =
        this->m_manager_psy_antenna->getHitIntensity() + this->m_p_storage->getIntensity();
    this->m_manager_psy_antenna->setHitIntensity(hit_intensity);

    float phantom_spawn_probability = this->m_manager_psy_antenna->getPhantomSpawnProbability() +
        this->m_p_storage->getPhantomProbability();
    this->m_manager_psy_antenna->setPhantomSpawnProbability(phantom_spawn_probability);

    this->m_manager_psy_antenna->setNoStatic(this->m_p_storage->IsSRPsyAntennaNoStatic());
    this->m_manager_psy_antenna->setNoMumble(this->m_p_storage->IsSRPsyAntennaNoMumble());
    this->m_manager_psy_antenna->setHitTypeName(this->m_p_storage->getHitTypeName());
    this->m_manager_psy_antenna->setHitFrequency(this->m_p_storage->getHitFrequency());

    if (this->m_p_storage->getPostProcessName().empty() ||
        this->m_p_storage->getPostProcessName() == "nil") // LorD: проверить будет ли дропать nil, если будет то найти и исправить когда это будет, чтобы все "nil" просто проверялись всегда как .empty()
    {
        Msg("[Scripts/Script_SchemeSRPsyAntenna/update(delta)] WARNING: postprocess is 'nil' return!");
        return;
    }

    if (this->m_manager_psy_antenna->getPostProcess().at(this->m_p_storage->getPostProcessName()) ==
        std::tuple<float, float, std::uint32_t>(0.0f, 0.0f, 0))
    {
        this->m_manager_psy_antenna->AddPostProcessCount();
        std::uint32_t id = 1500 + this->m_manager_psy_antenna->getPostProcessCount();
        this->m_manager_psy_antenna->setPostProcess(
            this->m_p_storage->getPostProcessName(), std::make_tuple(0.0f, 0.0f, id));

        Globals::Game::level::add_pp_effector(this->m_p_storage->getPostProcessName().c_str(), id, true);
        Globals::Game::level::set_pp_effector_factor(id, 0.01f);
    }

    float intensity_base = std::get<_POSTPROCESS_GET_INTENSITY_BASE>(this->m_manager_psy_antenna->getPostProcess().at(
                               this->m_p_storage->getPostProcessName())) +
        this->m_p_storage->getIntensity();

    this->m_manager_psy_antenna->setPostProcessIntensityBase(
        this->m_p_storage->getPostProcessName(), intensity_base);
}

void Script_SchemeSRPsyAntenna::zone_leave(void)
{
    this->m_state = _kPsyStateOutSide;
    CurrentGameUI()->enable_fake_indicators(false);

    float sound_intensity_base =
        this->m_manager_psy_antenna->getSoundIntensityBase() - this->m_p_storage->getIntensity();
    this->m_manager_psy_antenna->setSoundIntensityBase(sound_intensity_base);

    float mute_sound_threshold =
        this->m_manager_psy_antenna->getMuteSoundThreshold() - this->m_p_storage->getMuteSoundThreshold();
    this->m_manager_psy_antenna->setMuteSoundThreshold(mute_sound_threshold);

    float hit_intensity =
        this->m_manager_psy_antenna->getHitIntensity() - this->m_p_storage->getHitIntensity();
    this->m_manager_psy_antenna->setHitIntensity(hit_intensity);

    float phantom_probability = this->m_manager_psy_antenna->getPhantomSpawnProbability() -
        this->m_p_storage->getPhantomProbability();
    this->m_manager_psy_antenna->setPhantomSpawnProbability(phantom_probability);

    if (this->m_p_storage->getPostProcessName() == "nil" || // LorD: проверить будет ли дропать nil, если будет то найти и исправить когда это будет, чтобы все "nil" просто проверялись всегда как .empty()
        this->m_p_storage->getPostProcessName().empty())
    {
        MESSAGEWR("postprocess_name.empty() == true! You set an "
            "empty string");
        return;
    }

    if (this->m_manager_psy_antenna->getPostProcess().at(this->m_p_storage->getPostProcessName()) !=
        std::tuple<float, float, std::uint32_t>(0.0f, 0.0f, 0))
    {
        float intensity_base =
            std::get<_POSTPROCESS_GET_INTENSITY_BASE>(
                this->m_manager_psy_antenna->getPostProcess().at(this->m_p_storage->getPostProcessName())) -
            this->m_p_storage->getIntensity();
        this->m_manager_psy_antenna->setPostProcessIntensityBase(
            this->m_p_storage->getPostProcessName(), intensity_base);
    }
}

void Script_SchemeSRPsyAntenna::switch_state(CScriptGameObject* const p_client_actor)
{
    if (!p_client_actor)
    {
        R_ASSERT2(false, "can't be!");
    }

    if (this->m_state != _kPsyStateInside)
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
