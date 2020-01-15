#pragma once


class cphysics_shell_scripted;

namespace Cordis
{
namespace Scripts
{
    // Lord: реализовать try_to_open_door и try_to_close_door когда будет xr_motivator
class Script_SchemePHDoor : public Script_ISchemeEntity
{
    using inherited_scheme = Script_ISchemeEntity;

public:
    Script_SchemePHDoor(void) = delete;
    Script_SchemePHDoor(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage);
    ~Script_SchemePHDoor(void);

    virtual void reset_scheme(const bool value, CScriptGameObject* const p_client_object);
    virtual void update(const float delta);
    virtual void use_callback(CScriptGameObject* const p_client_object, CScriptGameObject* const p_client_who);
    virtual void hit_callback(CScriptGameObject* const p_client_object, const float amount,
        const Fvector& local_direction, CScriptGameObject* const p_client_who, const std::int16_t bone_index);
    virtual void deactivate(CScriptGameObject* const p_client_object);
    bool open_fastcall(void);
    bool fastcall(void);

    // @ PRIVATE uses, in XR_LOGIC
    static void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, DataBase::Storage_Scheme& storage);

    // @ PRIVATE, uses in XR_LOGIC
    static void set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);

private:
    void close_door(const bool is_disable_sound);
    void open_door(const bool is_disable_sound);
    void close_action(void);
    bool try_switch(void);

    bool is_closed(void) noexcept;


    bool is_open(void) noexcept;

private:
    bool m_is_initialized;
    bool m_is_block;
    bool m_is_soundless_block;
    bool m_is_show_tips;
    float m_low_limits;
    float m_high_limits;
    cphysics_joint_scripted* m_p_joint;
};
} // namespace Scripts
} // namespace Cordis
