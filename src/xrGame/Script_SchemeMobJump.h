#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SchemeMobJump : public Script_ISchemeMonster
{
    using inherited_scheme = Script_ISchemeMonster;

public:
    Script_SchemeMobJump(void) = delete;
    Script_SchemeMobJump(CScriptGameObject* const p_client_object, DataBase::Script_ComponentScheme_MobJump* storage);
    ~Script_SchemeMobJump(void);

    virtual void reset_scheme(const bool, CScriptGameObject* const p_client_object);
    virtual void update(const float delta);

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

        MESSAGEI("added scheme to binder, name=%s scheme=%s section=%s",
            p_client_object->Name(), scheme_name.c_str(), section_name.c_str());

        Script_ISchemeEntity* action = new Script_SchemeMobJump(p_client_object, reinterpret_cast<DataBase::Script_ComponentScheme_MobJump*>(storage));
        DataBase::Storage::getInstance().setStorageSchemesActions(
            p_client_object->ID(), action->getSchemeName(), action);
    }

    // @ PRIVATE, uses in XR_LOGIC
    static void set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);

    enum : std::uint32_t
    {
        STATE_START_LOOK = 1,
        STATE_WAIT_LOOK_END,
        STATE_JUMP
    };

private:
    std::uint32_t m_current_state = STATE_START_LOOK;
    DataBase::Script_ComponentScheme_MobJump* m_p_storage;
    Fvector m_point;
};

} // namespace Scripts
} // namespace Cordis
