#pragma once
#include "Script_ISchemeStalker.h"
#include "Script_DataBase.h"

namespace Cordis
{
namespace Scripts
{
class Script_EvaluatorAbuse : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorAbuse(const xr_string& evaluator_name, DataBase::Storage_Scheme& storage)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_storage(&storage)
    {
    }

    ~Script_EvaluatorAbuse(void) {}

    virtual _value_type evaluate(void);

private:
    DataBase::Storage_Scheme* m_p_storage;
};


class Script_SchemeXRAbuse : public Script_ISchemeStalker
{
    using inherited_scheme = Script_ISchemeStalker;

public:
    Script_SchemeXRAbuse(const xr_string& action_name, DataBase::Storage_Scheme& storage)
        : inherited_scheme(nullptr, action_name, storage)
    {
    }

    ~Script_SchemeXRAbuse(void) {}

    virtual void execute(void);
    virtual void finalize(void);
    virtual void initialize(void);


    // @ PRIVATE uses, in XR_LOGIC
    static void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, DataBase::Storage_Scheme& storage);

    static void set_abuse(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name);
 

    static void add_abuse(CScriptGameObject* const p_client_object, const float value);


    static void clear_abuse(CScriptGameObject* const p_client_object);

    static void disable_abuse(CScriptGameObject* const p_client_object);

    static void enable_abuse(CScriptGameObject* const p_client_object);

};
} // namespace Scripts
} // namespace Cordis
