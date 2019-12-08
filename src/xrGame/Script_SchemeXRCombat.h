#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_EvaluatorCheckCombat : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorCheckCombat(const xr_string& evaluator_name, DataBase::Storage_Scheme& storage)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_storage(&storage)
    {
    }

    virtual _value_type evaluate(void);

private:
    DataBase::Storage_Scheme* m_p_storage;
};

class Script_EvaluatorEnemy : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorEnemy(const xr_string& evaluator_name, DataBase::Storage_Scheme& storage)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_storage(&storage)
    {
    }

    virtual _value_type evaluate(void);

private:
    DataBase::Storage_Scheme* m_p_storage;
};

namespace XR_COMBAT
{
// Lord: доделать и set_scheme тоже!
// @ PRIVATE uses, in XR_LOGIC
inline void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
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

inline void set_combat_checker(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name)
{
}

inline void disable_scheme(CScriptGameObject* const p_client_object, const xr_string& scheme_name) 
{
}

} // namespace XR_COMBAT

} // namespace Scripts
} // namespace Cordis
