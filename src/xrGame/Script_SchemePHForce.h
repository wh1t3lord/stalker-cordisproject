#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SchemePHForce : public Script_ISchemeEntity
{
    using inherited_scheme = Script_ISchemeEntity;

public:
    Script_SchemePHForce(void) = delete;
    Script_SchemePHForce(CScriptGameObject* const p_client_object, DataBase::Script_ComponentScheme_PHForce* storage);
    ~Script_SchemePHForce(void);

    virtual void reset_scheme(const bool value, CScriptGameObject* const p_client_object);
    virtual void update(const float delta);

    // @ PRIVATE uses, in XR_LOGIC
    static inline void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, DataBase::Script_ComponentScheme_PHForce* storage)
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

        Script_ISchemeEntity* p_scheme = new Script_SchemePHForce(p_client_object, reinterpret_cast<DataBase::Script_ComponentScheme_PHForce*>(storage));
        DataBase::Storage::getInstance().setStorageSchemesActions(p_client_object->ID(), section_name, p_scheme);
    }

    // @ PRIVATE, uses in XR_LOGIC
    static void set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);

private:
    bool m_is_process;
    std::uint32_t m_time;
    DataBase::Script_ComponentScheme_PHForce* m_p_storage;
};

} // namespace Scripts
} // namespace Cordis
