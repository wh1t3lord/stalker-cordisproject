#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SchemeSRLight;

class Script_SRLightManager
{
    friend class Script_SchemeSRLight;

private:
    Script_SRLightManager(void) = default;

public:
    static inline Script_SRLightManager& getInstance(void) noexcept
    {
        static Script_SRLightManager instance;
        return instance;
    }

    void check_light(CScriptGameObject* const p_client_object);

    // @ BE CAREFULL, PRIVATE SEE system_deallocation()
    inline void Deallocate(void)
    {
        for (std::pair<const std::uint16_t, Script_SchemeSRLight*>& it : this->m_light_zones)
        {
            if (it.second)
                it.second = nullptr;
        }
        this->m_light_zones.clear();
        Msg("[Scripts/Script_SRLightManager/Deallocate()] clearing zones!");
    }

private:
private:
    xr_map<std::uint16_t, Script_SchemeSRLight*> m_light_zones;
};

class Script_SchemeSRLight : public Script_ISchemeEntity
{
    friend class Script_SRLightManager;
    using inherited_scheme = Script_ISchemeEntity;

public:
    Script_SchemeSRLight(void) = delete;
    Script_SchemeSRLight(CScriptGameObject* const p_client_object, void* storage);
    ~Script_SchemeSRLight(void);

    virtual void reset_scheme(const bool value, CScriptGameObject* const p_client_object);
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

        Msg("[Scripts/add_to_binder(p_client_object, p_ini, scheme_name, section_name, storage)] added "
            "Script_SchemeMobWalker scheme to binder, name=%s scheme=%s section=%s",
            p_client_object->Name(), scheme_name.c_str(), section_name.c_str());

        Script_ISchemeEntity* p_scheme = new Script_SchemeSRLight(p_client_object, storage);
        DataBase::Storage::getInstance().setStorageSchemesActions(p_client_object->ID(), scheme_name, p_scheme);
    }

    // @ PRIVATE, uses in XR_LOGIC
    static void set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);


private:
    bool check_stalker(CScriptGameObject* const p_client_object, bool& is_returned_light_value);

private:
    bool m_is_active;
};
} // namespace Scripts
} // namespace Cordis
