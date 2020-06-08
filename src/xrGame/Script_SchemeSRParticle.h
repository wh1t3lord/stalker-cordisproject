#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SchemeSRParticle : public Script_ISchemeEntity
{
    using inherited_scheme = Script_ISchemeEntity;

public:
    Script_SchemeSRParticle(void) = delete;
    Script_SchemeSRParticle(CScriptGameObject* const p_client_object, void* storage);
    ~Script_SchemeSRParticle(void);

    virtual void reset_scheme(const bool value, CScriptGameObject* const p_client_object);
    virtual void update(const float delta);
    virtual void deactivate(CScriptGameObject* const p_client_object);

    // @ PRIVATE uses, in XR_LOGIC
    static inline void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, void* storage)
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

        Script_ISchemeEntity* p_scheme = new Script_SchemeSRParticle(p_client_object, storage);
        DataBase::Storage::getInstance().setStorageSchemesActions(p_client_object->ID(), scheme_name, p_scheme);
    }

    // @ PRIVATE, uses in XR_LOGIC
    static void set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);

private:
    bool IsEnd(void);
    void update_mode_1(void);
    void update_mode_2(void);

private:
    bool m_is_started;
    bool m_is_first_played;
    std::uint32_t m_last_update;
    CPatrolPathParams* m_path;
    xr_vector<ParticleData> m_particles;
};
} // namespace Scripts
} // namespace Cordis
