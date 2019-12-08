#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_EvaluatorWoundedExist : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorWoundedExist(const xr_string& evaluator_name, DataBase::Storage_Scheme& storage)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_storage(&storage)
    {
    }

    ~Script_EvaluatorWoundedExist(void) {}

    virtual _value_type evaluate(void);

private:
    DataBase::Storage_Scheme* m_p_storage;
};

class Script_SchemeXRHelpWounded : public Script_ISchemeStalker
{
    using inherited_scheme = Script_ISchemeStalker;

public:
    Script_SchemeXRHelpWounded(void) = delete;
    Script_SchemeXRHelpWounded(const xr_string& action_name, DataBase::Storage_Scheme& storage);
    ~Script_SchemeXRHelpWounded(void);

    virtual void initialize(void);
    virtual void execute(void);
    virtual void finalize(void);

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

    static inline void set_help_wounded(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name)
    {
    }

    static inline void reset_help_wounded(
        CScriptGameObject* const p_client_object, const xr_string& scheme_name, const xr_string& section_name)
    {
    }

    static inline bool is_under_help_wounded(CScriptGameObject* const p_client_object) 
    {

    }
};
} // namespace Scripts
} // namespace Cordis
