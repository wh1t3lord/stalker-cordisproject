#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SchemePHOnHit : public Script_ISchemeEntity
{
    using inherited_scheme = Script_ISchemeEntity;

public:
    Script_SchemePHOnHit(void) = delete;
    Script_SchemePHOnHit(CScriptGameObject* const p_client_object, DataBase::Script_ComponentScheme_PHOnHit* storage);
    ~Script_SchemePHOnHit(void);

    virtual void hit_callback(CScriptGameObject* const p_client_object, const float amount,
        const Fvector& local_direction, CScriptGameObject* const p_client_who, const std::int16_t bone_index);

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

        Script_ISchemeEntity* p_scheme = new Script_SchemePHOnHit(p_client_object, reinterpret_cast<DataBase::Script_ComponentScheme_PHOnHit*>(storage));
        DataBase::Storage::getInstance().setStorageSchemesActions(p_client_object->ID(), scheme_name, p_scheme);
        DataBase::Storage::getInstance().setStorageSchemesActionSchemeIDForUnSubscribing(
            p_client_object->ID(), scheme_name, p_scheme->getSchemeID());
    }

    // @ PRIVATE, uses in XR_LOGIC
    static void set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);

    static void disable_scheme(CScriptGameObject* const p_client_object, const xr_string& scheme_name)
    {
        if (!p_client_object)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        if (scheme_name.empty())
        {
            MESSAGEWR("scheme_name.empty() == true!");
            return;
        }

        DataBase::Storage::getInstance().UnsubscribeAction(p_client_object->ID(), scheme_name);
    }

private:
    DataBase::Script_ComponentScheme_PHOnHit* m_p_storage;
};

} // namespace Scripts
} // namespace Cordis
