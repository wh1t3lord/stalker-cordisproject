#pragma once

namespace Cordis
{
namespace Scripts
{
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

    inline void check_light(CScriptGameObject* const p_client_object)
    {
        if (!p_client_object)
        {
            Msg("[Scripts/Script_SRLightManager/check_light(p_client_object)] WARNING: p_client_object was nullptr "
                "return ...");
            return;
        }

        CScriptGameObject* const p_client_torch = p_client_object->GetObjectByName("device_torch");
        if (!p_client_torch)
        {
            Msg("[Scripts/Script_SRLightManager/check_light(p_client_object)] WARNING: p_client_torch was nullptr "
                "return ...");
        }

        bool is_light = false;
        bool is_forced = false;

        if (!p_client_object->Alive())
        {
            is_light = false;
            is_forced = true;
        }

        if (!is_forced)
        {
            for (std::pair<const std::uint16_t, Script_SchemeSRLight*>& it : this->m_light_zones)
            {
                if (it.second->check_stalker(p_client_object, is_light))
                {
                    break;
                }
            }
        }

        if (!is_forced)
        {
            std::uint32_t hours = Globals::get_time_hours();

            if (hours <= 4 || hours >= 22)
                is_light = true;

            if (!is_light)
                if (Script_GlobalHelper::getInstance().getIndoorLevels().at(Globals::Game::level::get_name()))
                    is_light = true;
        }

        if (!is_forced && is_light)
        {
            const xr_string& active_scheme_name =
                DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getActiveSchemeName();
            if (active_scheme_name == "kamp" || active_scheme_name == "camper" || active_scheme_name == "sleeper")
            {
                is_light = false;
                is_forced = true;
            }
        }

        if (!is_forced && is_light)
        {
            if (p_client_object->GetBestEnemy() &&
                Script_GlobalHelper::getInstance().getIndoorLevels().find(Globals::Game::level::get_name()) !=
                    Script_GlobalHelper::getInstance().getIndoorLevels().end())
            {
                is_light = false;
            }
        }
        // @ Lord: I guess it right test it 
        if (is_light)
            p_client_torch->enable_attachable_item(is_light);
    }

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
    Script_SchemeSRLight(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage);
    ~Script_SchemeSRLight(void);

    virtual void reset_scheme(const bool value, CScriptGameObject* const p_client_object);
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
    bool check_stalker(CScriptGameObject* const p_client_object, bool& is_returned_light_value);

private:
    bool m_is_active;
};
} // namespace Scripts
} // namespace Cordis
