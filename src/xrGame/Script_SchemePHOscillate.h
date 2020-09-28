#pragma once

class cphysics_joint_scripted;

namespace Cordis
{
namespace Scripts
{
class Script_SchemePHOscillate : public Script_ISchemeEntity
{
    using inherited_scheme = Script_ISchemeEntity;

public:
    Script_SchemePHOscillate(void) = delete;
    Script_SchemePHOscillate(CScriptGameObject* const p_client_object, DataBase::Script_ComponentScheme_PHOscillate* storage);
    ~Script_SchemePHOscillate(void);

    virtual void reset_scheme(const bool value, CScriptGameObject* const p_client_object);
    virtual void update(const float delta);

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

        Script_ISchemeEntity* p_scheme = new Script_SchemePHOscillate(p_client_object, static_cast<DataBase::Script_ComponentScheme_PHOscillate*>(storage));
        DataBase::Storage::getInstance().setStorageSchemesActions(p_client_object->ID(), scheme_name, p_scheme);
    }

    // @ PRIVATE, uses in XR_LOGIC
    static void set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);
    

private:
    bool m_is_pause;
    std::uint32_t m_time;
    float m_koeff;
    Fvector m_direction;
    cphysics_joint_scripted* m_p_joint;
    DataBase::Script_ComponentScheme_PHOscillate* m_p_storage;
};
}
} // namespace Cordis

