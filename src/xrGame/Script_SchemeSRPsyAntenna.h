#pragma once

#include "script_hit.h"

namespace Cordis
{
namespace Scripts
{
const std::uint32_t _POSTPROCESS_GET_INTENSITY_BASE = 0;
const std::uint32_t _POSTPROCESS_GET_INTENSITY = 1;
const std::uint32_t _POSTPROCESS_GET_INDEX = 2;

class Script_PsyAntennaManager
{
private:
    Script_PsyAntennaManager(void) {}

public:
    static inline Script_PsyAntennaManager& getInstance(void) noexcept
    {
        static Script_PsyAntennaManager instance;
        return instance;
    }

    Script_PsyAntennaManager(const Script_PsyAntennaManager&) = delete;
    Script_PsyAntennaManager& operator=(const Script_PsyAntennaManager&) = delete;
    Script_PsyAntennaManager(Script_PsyAntennaManager&&) = delete;
    Script_PsyAntennaManager& operator=(Script_PsyAntennaManager&&) = delete;
    ~Script_PsyAntennaManager(void)
    {
        if (this->m_p_sound_object_left)
        {
            Msg("[Scripts/Script_PsyAntennaManager/~dtor()] deleting sound object left");
            xr_delete(this->m_p_sound_object_left);
        }

        if (this->m_p_sound_object_right)
        {
            Msg("[Scripts/Script_PsyAntennaManager/~dtor()] deleting sound object right");
            xr_delete(this->m_p_sound_object_right);
        }
    }

    inline void destroy(void)
    {
        if (this->m_p_sound_object_left)
            this->m_p_sound_object_left->Stop();

        if (this->m_p_sound_object_right)
            this->m_p_sound_object_right->Stop();

        Globals::Game::level::set_snd_volume(this->m_sound_volume);
        CurrentGameUI()->enable_fake_indicators(false);
    }

    inline void update_psy_hit(const float delta)
    {
        CUIGameCustom* p_hud = CurrentGameUI();
        if (!p_hud)
        {
            R_ASSERT2(false, "can't be!");
            return;
        }

        StaticDrawableWrapper* p_static = p_hud->GetCustomStatic("cs_psy_danger");
        if (this->m_hit_intensity > 0.0001f)
        {
            if (!p_static || !this->m_is_no_static)
            {
                p_hud->AddCustomStatic("cs_psy_danger", true);
                p_hud->GetCustomStatic("cs_psy_danger")->wnd()->TextItemControl()->SetTextST("st_psy_danger");
            }
        }
        else
        {
            if (p_static)
            {
                p_hud->RemoveCustomStatic("cs_psy_danger");
            }
        }

        if (static_cast<float>(Globals::get_time_global() - this->m_hit_time) > this->m_hit_frequency)
        {
            this->m_hit_time = Globals::get_time_global();
            float power = this->m_hit_amplitude * this->m_hit_intensity;

            if (power > 0.0001f)
            {
                CScriptHit psy_hit;
                psy_hit.m_fPower = power;
                psy_hit.m_tDirection = Fvector();
                psy_hit.m_fImpulse = 0.0f;
                psy_hit.m_tpDraftsman = DataBase::Storage::getInstance().getActor();

                if (this->m_hit_type_name == "chemical")
                {
                    p_hud->update_fake_indicators(2, true);
                    psy_hit.m_tHitType = ALife::eHitTypeChemicalBurn;
                }
                else
                {
                    p_hud->update_fake_indicators(3, true);
                    psy_hit.m_tHitType = ALife::eHitTypeTelepatic;
                }

                DataBase::Storage::getInstance().getActor()->Hit(&psy_hit);

                if (DataBase::Storage::getInstance().getActor()->GetHealth() < 0.0001f &&
                    DataBase::Storage::getInstance().getActor()->Alive())
                {
                    DataBase::Storage::getInstance().getActor()->Kill(DataBase::Storage::getInstance().getActor());
                }
            }
        }
    }

    inline void generate_phantoms(void)
    {
        if (!this->m_phantom_idle)
        {
            this->m_phantom_idle =
                Globals::Script_RandomInt::getInstance().Generate(std::uint32_t(2000), std::uint32_t(5000));
        }

        if (Globals::get_time_global() - this->m_phantom_time > this->m_phantom_idle)
        {
            this->m_phantom_time = Globals::get_time_global();
            this->m_phantom_idle =
                Globals::Script_RandomInt::getInstance().Generate(std::uint32_t(5000), std::uint32_t(10000));

            if (Globals::Script_RandomFloat::getInstance().Generate() < this->m_phantom_spawn_probability)
            {
                if (Script_PhantomManager::getInstance().getPhantomCount() < this->m_phantom_max_count)
                {
                    float radius = this->m_phantom_spawn_radius *
                        (((Globals::Script_RandomFloat::getInstance().Generate() / 2.0f) + 0.5f));
                    float angle =
                        this->m_phantom_field_of_view * (Globals::Script_RandomFloat::getInstance().Generate()) -
                        this->m_phantom_field_of_view * 0.5f;
                    Fvector direction =
                        Globals::vector_rotate_y(DataBase::Storage::getInstance().getActor()->Direction(), angle);

                    Script_PhantomManager::getInstance().spawn_phantom(
                        Fvector(DataBase::Storage::getInstance().getActor()->Position()).add(direction.mul(radius)));
                }
            }
        }
    }

    inline void update_sound(void)
    {
        if (!this->m_is_sound_initialized)
        {
            if (this->m_p_sound_object_left)
                this->m_p_sound_object_left->PlayAtPos(DataBase::Storage::getInstance().getActor(),
                    Fvector().set(-1.0f, 0.0f, 1.0f), 0, sm_2D | sm_Looped);
            if (this->m_p_sound_object_right)
                this->m_p_sound_object_right->PlayAtPos(
                    DataBase::Storage::getInstance().getActor(), Fvector().set(1.0f, 0.0f, 1.0f), 0, sm_2D | sm_Looped);

            this->m_is_sound_initialized = true;
        }

        float volume = 1.0f - ((this->m_sound_intensity * this->m_sound_intensity * this->m_sound_intensity) * 0.9f);

        if (volume < this->m_mute_sound_threshold)
            Globals::Game::level::set_snd_volume(this->m_mute_sound_threshold);
        else
            Globals::Game::level::set_snd_volume(volume);

        if (this->m_p_sound_object_left)
            this->m_p_sound_object_left->SetVolume(1.0f / (volume - 1.0f));

        if (this->m_p_sound_object_right)
            this->m_p_sound_object_right->SetVolume(1.0f / (volume - 1.0f));
    }

    inline bool update_postprocess(const std::tuple<float, float, std::uint32_t>& postprocess)
    {
        if (fis_zero(std::get<_POSTPROCESS_GET_INTENSITY>(postprocess)))
        {
            --this->m_postprocess_count;
            Globals::Game::level::remove_pp_effector(std::get<_POSTPROCESS_GET_INDEX>(postprocess));
            return false;
        }

        Globals::Game::level::set_pp_effector_factor(
            std::get<_POSTPROCESS_GET_INDEX>(postprocess), std::get<_POSTPROCESS_GET_INTENSITY>(postprocess), 0.3f);
        return true;
    }

    inline void update(const float delta)
    {
        this->generate_phantoms();
        if (!this->m_is_no_mumble)
        {
            this->m_sound_intensity =
                this->update_intensity(this->m_sound_intensity_base, this->m_sound_intensity, delta);
            this->update_sound();
        }

        for (std::pair<const xr_string, std::tuple<float, float, std::uint32_t>>& it : this->m_postprocess)
        {
            std::get<_POSTPROCESS_GET_INTENSITY>(it.second) =
                this->update_intensity(std::get<_POSTPROCESS_GET_INTENSITY_BASE>(it.second),
                    std::get<_POSTPROCESS_GET_INTENSITY>(it.second), delta);

            bool is_exist = this->update_postprocess(it.second);
            if (!is_exist)
            {
                this->m_postprocess[it.first] = std::make_tuple<float, float, std::uint32_t>(0.0f, 0.0f, 0);
            }
        }

        this->update_psy_hit(delta);
    }

    inline void save(NET_Packet& packet)
    {
        Msg("[Scripts/Script_PsyAntennaManager/save(packet)] called!");
        Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, false, "Script_PsyAntennaManager");
        packet.w_float(this->m_hit_intensity);
        packet.w_float(this->m_sound_intensity);
        packet.w_float(this->m_sound_intensity_base);
        packet.w_float(this->m_mute_sound_threshold);
        packet.w_u8(static_cast<std::uint8_t>(this->m_is_no_static));
        packet.w_u8(static_cast<std::uint8_t>(this->m_is_no_mumble));
        packet.w_stringZ(this->m_hit_type_name.c_str());
        packet.w_u32(static_cast<std::uint32_t>(this->m_hit_frequency));
        packet.w_u8(this->m_postprocess_count);

        for (const std::pair<xr_string, std::tuple<float, float, std::uint32_t>>& it : this->m_postprocess)
        {
            packet.w_stringZ(it.first.c_str());
            packet.w_float(std::get<_POSTPROCESS_GET_INTENSITY>(it.second));
            packet.w_float(std::get<_POSTPROCESS_GET_INTENSITY_BASE>(it.second));
            packet.w_u16(std::get<_POSTPROCESS_GET_INDEX>(it.second));
        }

        Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, true, "Script_PsyAntennaManager");
    }

    inline void load(NET_Packet& packet)
    {
        Msg("[Scripts/Script_PsyAntennaManager/load(packet)] called!");
        Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Load, false, "Script_PsyAntennaManager");

        this->m_hit_intensity = packet.r_float();
        this->m_sound_intensity = packet.r_float();
        this->m_sound_intensity_base = packet.r_float();
        this->m_mute_sound_threshold = packet.r_float();
        this->m_is_no_static = packet.r_u8();
        this->m_is_no_mumble = packet.r_u8();
        packet.r_stringZ(this->m_hit_type_name);
        this->m_hit_frequency = static_cast<float>(packet.r_u32());
        this->m_postprocess_count = packet.r_u8();

        for (std::uint32_t i = 0; i < this->m_postprocess_count; ++i)
        {
            xr_string postprocess_id_name;
            packet.r_stringZ(postprocess_id_name);
            float ii = packet.r_float();
            float ib = packet.r_float();
            std::uint32_t index = packet.r_u16();
            this->m_postprocess[postprocess_id_name] = std::make_tuple(ii, ib, index);
            Globals::Game::level::add_pp_effector(postprocess_id_name.c_str(), index, true);
            Globals::Game::level::set_pp_effector_factor(index, ii); 
        }

        Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Load, true, "Script_PsyAntennaManager");
    }

    inline float getSoundIntensityBase(void) const noexcept { return this->m_sound_intensity_base; }
    inline void setSoundIntensityBase(const float value) noexcept { this->m_sound_intensity_base = value; }

    inline float getMuteSoundThreshold(void) const noexcept { return this->m_mute_sound_threshold; }
    inline void setMuteSoundThreshold(const float value) noexcept { this->m_mute_sound_threshold = value; }

    inline float getHitIntensity(void) const noexcept { return this->m_hit_intensity; }
    inline void setHitIntensity(const float value) noexcept { this->m_hit_intensity = value; }

    inline float getPhantomSpawnProbability(void) const noexcept { return this->m_phantom_spawn_probability; }
    inline void setPhantomSpawnProbability(const float value) noexcept { this->m_phantom_spawn_probability = value; }

    inline bool IsNoStatic(void) const noexcept { return this->m_is_no_static; }
    inline void setNoStatic(const bool value) noexcept { this->m_is_no_static = value; }

    inline bool IsNoMumble(void) const noexcept { return this->m_is_no_mumble; }
    inline void setNoMumble(const bool value) noexcept { this->m_is_no_mumble = value; }

    inline const xr_string& getHitTypeName(void) const noexcept { return this->m_hit_type_name; }
    inline void setHitTypeName(const xr_string& type_name) noexcept
    {
        if (type_name.empty())
        {
            Msg("[Scripts/Script_PsyAntennaManager/setHitTypeName(type_name)] WARNING: type_name.empty() == true! You "
                "set an empty string!");
        }

        this->m_hit_type_name = type_name;
    }

    inline float getHitFrequency(void) const noexcept { return this->m_hit_frequency; }
    inline void setHitFrequency(const float value) noexcept { this->m_hit_frequency = value; }

    inline const xr_map<xr_string, std::tuple<float, float, std::uint32_t>>& getPostProcess(void) const noexcept
    {
        return this->m_postprocess;
    }

    inline void setPostProcessIndex(const xr_string& postprocess_name, const std::uint32_t value) noexcept
    {
        if (postprocess_name.empty())
        {
            Msg("[Scripts/Script_PsyAntennaManager/setPostProcessIndex(postprocess_name, value)] WARNING: "
                "postprocess_name.empty() == true! You are trying to set an empty string return ...");
            return;
        }

        std::get<_POSTPROCESS_GET_INDEX>(this->m_postprocess[postprocess_name]) = value;
    }

    inline void setPostProcessIntensityBase(const xr_string& postprocess_name, const float value) noexcept
    {
        if (postprocess_name.empty())
        {
            Msg("[Scripts/Script_PsyAntennaManager/setPostProcessIntensityBase(postprocess_name, value)] WARNING: "
                "postprocess_name.empty() == true! You are trying to set an empty string return ...");
            return;
        }

        std::get<_POSTPROCESS_GET_INTENSITY>(this->m_postprocess[postprocess_name]) = value;
    }

    inline void setPostProcessIntensity(const xr_string& postprocess_name, const float value) noexcept
    {
        if (postprocess_name.empty())
        {
            Msg("[Scripts/Script_PsyAntennaManager/setPostProcessIntensity(postprocess_name, value)] WARNING: "
                "postprocess_name.empty() == true! You set an empty string return ...");
            return;
        }

        std::get<_POSTPROCESS_GET_INTENSITY_BASE>(this->m_postprocess[postprocess_name]) = value;
    }

    inline void setPostProcess(const xr_string& postprocess_name, const std::tuple<float, float, int>& data) noexcept
    {
        if (postprocess_name.empty())
        {
            Msg("[Scripts/Script_PsyAntennaManager/setPostProcess(process_name, data)] WARNING: "
                "postprocess_name.empty() == true! You set an empty string return ...");
            return;
        }

        this->m_postprocess[postprocess_name] = data;
    }

    inline void AddPostProcessCount(void) noexcept { ++this->m_postprocess_count; }
    inline void RemovePostProcessCount(void) noexcept { --this->m_postprocess_count; }
    inline std::uint32_t getPostProcessCount(void) const noexcept { return this->m_postprocess_count; }

private:
    inline float update_intensity(const float intensity_base, const float intensity, const float delta)
    {
        float di = this->m_intensity_inertion * delta * 0.01f;
        float li = intensity_base;
        if (abs(intensity_base - intensity) >= di)
        {
            li = intensity - di;
        }
        else
        {
            li = intensity + di;
        }

        if (li < 0.0f)
            li = 0.0f;
        else if (li > 1.0f)
            li = 1.0f;

        return li;
    }

private:
    bool m_is_sound_initialized;
    bool m_is_no_static;
    bool m_is_no_mumble;
    std::uint32_t m_phantom_max_count;
    std::uint32_t m_hit_time;
    std::uint32_t m_phantom_time;
    std::uint32_t m_postprocess_count;
    std::uint32_t m_phantom_idle;
    float m_phantom_spawn_radius;
    float m_phantom_spawn_height;
    float m_phantom_field_of_view;
    float m_phantom_spawn_probability;
    float m_hit_amplitude;
    float m_intensity_inertion;
    float m_hit_intensity;
    float m_sound_intensity;
    float m_sound_intensity_base;
    float m_mute_sound_threshold;
    float m_max_mumble_volume;
    float m_hit_frequency;
    float m_sound_volume;
    xr_map<xr_string, std::tuple<float, float, std::uint32_t>>
        m_postprocess; // 0 - intensity_base, 1 - intensity, 2 - index
    xr_string m_hit_type_name;
    CScriptSound* m_p_sound_object_right;
    CScriptSound* m_p_sound_object_left;
};

class Script_SchemeSRPsyAntenna : public Script_ISchemeEntity
{
    using inherited_scheme = Script_ISchemeEntity;

public:
    Script_SchemeSRPsyAntenna(void) = delete;
    Script_SchemeSRPsyAntenna(CScriptGameObject* const p_client_object, DataBase::Script_ComponentScheme_SRPsyAntenna* storage);
    ~Script_SchemeSRPsyAntenna(void);

    virtual void reset_scheme(const bool value, CScriptGameObject* const p_client_object);
    virtual void deactivate(CScriptGameObject* const p_client_object);
    virtual void update(const float delta);
    virtual void save(void);

    // @ PRIVATE uses, in XR_LOGIC
    static inline void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, DataBase::Script_IComponentScheme* storage)
    {
        if (!p_client_object)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        if (!p_ini)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        MESSAGEI("added scheme to binder, name=%s scheme=%s section=%s",
            p_client_object->Name(), scheme_name.c_str(), section_name.c_str());

        Script_ISchemeEntity* p_scheme = new Script_SchemeSRPsyAntenna(p_client_object, static_cast<DataBase::Script_ComponentScheme_SRPsyAntenna*>(storage));
        DataBase::Storage::getInstance().setStorageSchemesActions(p_client_object->ID(), scheme_name, p_scheme);
    }

    // @ PRIVATE, uses in XR_LOGIC
    static void set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);

private:
    void zone_enter(void);
    void zone_leave(void);
    void switch_state(CScriptGameObject* const p_client_actor);

private:
    std::uint32_t m_state;
    DataBase::Script_ComponentScheme_SRPsyAntenna* m_p_storage;
    Script_PsyAntennaManager* m_manager_psy_antenna;
};
} // namespace Scripts
} // namespace Cordis
