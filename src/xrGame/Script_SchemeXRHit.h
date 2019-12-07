#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SchemeXRHit : public Script_ISchemeEntity
{
    using inherited_scheme = Script_ISchemeEntity;

public:
    Script_SchemeXRHit(void) = delete;
    Script_SchemeXRHit(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage);
    ~Script_SchemeXRHit(void);

    virtual void hit_callback(CScriptGameObject* const p_client_object, const float amount,
        const Fvector& local_direction, CScriptGameObject* const p_client_who, const std::int16_t bone_index);

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

    // Lord: доделать
    static inline void set_hit_checker(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name)
    {
    
    }

    // Lord: доделать
    static inline void disable_scheme(CScriptGameObject* const p_client_object, const xr_string& scheme_name)
    {

    }
};
} // namespace Scripts
} // namespace Cordis
