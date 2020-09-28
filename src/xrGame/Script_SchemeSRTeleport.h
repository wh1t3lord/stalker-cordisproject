#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SchemeSRTeleport : public Script_ISchemeEntity
{
    using inherited_scheme = Script_ISchemeEntity;

public:
    Script_SchemeSRTeleport(void) = delete;
    Script_SchemeSRTeleport(CScriptGameObject* const p_client_object, DataBase::Script_ComponentScheme_SRTeleport* storage);
    ~Script_SchemeSRTeleport(void);

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

        Script_ISchemeEntity* p_scheme = new Script_SchemeSRTeleport(p_client_object, static_cast<DataBase::Script_ComponentScheme_SRTeleport*>(storage));
        DataBase::Storage::getInstance().setStorageSchemesActions(p_client_object->ID(), scheme_name, p_scheme);
    }

    // @ PRIVATE, uses in XR_LOGIC
    static void set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);

private:
    bool m_state;
    std::uint32_t m_timer;
    DataBase::Script_ComponentScheme_SRTeleport* m_p_storage;
};
} // namespace Scripts
} // namespace Cordis
