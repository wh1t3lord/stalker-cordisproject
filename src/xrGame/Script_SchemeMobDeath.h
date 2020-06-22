#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SchemeMobDeath : public Script_ISchemeMonster
{
    using inherited_scheme = Script_ISchemeMonster;

public:
    Script_SchemeMobDeath(void) = delete;
    Script_SchemeMobDeath(CScriptGameObject* const p_client_object, DataBase::Script_ComponentScheme_MobDeath* storage);
    ~Script_SchemeMobDeath(void);

    virtual void death_callback(CScriptGameObject* const p_client_victim, CScriptGameObject* const p_client_who);

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

        Script_ISchemeEntity* action = new Script_SchemeMobDeath(p_client_object, reinterpret_cast<DataBase::Script_ComponentScheme_MobDeath*>(storage));
        DataBase::Storage::getInstance().setStorageSchemesActions(
            p_client_object->ID(), action->getSchemeName(), action);
    }

    // @ PRIVATE, uses in XR_LOGIC
    static void set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);
};
} // namespace Scripts
} // namespace Scripts
