#pragma once

namespace Cordis
{
namespace Scripts
{
    // @ Можете доделать!
class Script_SchemeSRPostProcess : public Script_ISchemeEntity
{
    using inherited_scheme = Script_ISchemeEntity;

public:
    Script_SchemeSRPostProcess(void) = delete;
    Script_SchemeSRPostProcess(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage);
    ~Script_SchemeSRPostProcess(void);

    virtual void reset_scheme(const bool value, CScriptGameObject* const p_client_object);
    virtual void deactivate(CScriptGameObject* const p_client_object);
    virtual void update(const float delta);

    // Lord: доделать и set_scheme тоже!
    // @ PRIVATE uses, in XR_LOGIC
    static inline void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, DataBase::Storage_Scheme& storage)
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

        Msg("[Scripts/add_to_binder(p_client_object, p_ini, scheme_name, section_name, storage)] added "
            "Script_SchemeMobWalker scheme to binder, name=%s scheme=%s section=%s",
            p_client_object->Name(), scheme_name.c_str(), section_name.c_str());
    }

    // @ PRIVATE, uses in XR_LOGIC
    static inline void set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
    {
    }

private:
    void update_hit(const float delta);

private:
    bool m_is_actor_inside;
    std::uint32_t m_eff_time;
    std::uint32_t m_hit_time;
    float m_gray_amplitude;
    float m_intensity;
    float m_intensity_base;
    float m_hit_power;
    float m_intensity_inertion;
    SPPInfo::SNoise m_noise;
    SPPInfo::SColor m_gray_color;
    SPPInfo::SColor m_base_color;
    CScriptEffector m_postprocess;
};
}
} // namespace Cordis

