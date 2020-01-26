#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_EvaluatorNeedWalker : public CScriptPropertyEvaluator
{
    Script_EvaluatorNeedWalker(const xr_string& evaluator_name, DataBase::Storage_Scheme& storage)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_storage(&storage)
    {
    }

    virtual _value_type evaluate(void) 
    { 
        return XR_LOGIC::is_active(this->m_object, *this->m_p_storage);
    }

private:
    DataBase::Storage_Scheme* m_p_storage;
};

class Script_SchemeXRWalker : public Script_ISchemeStalker
{
    using inherited_scheme = Script_ISchemeStalker;

public:
    Script_SchemeXRWalker(void) = delete;
    Script_SchemeXRWalker(const xr_string& action_name, DataBase::Storage_Scheme& storage);
    ~Script_SchemeXRWalker(void);

    virtual void initialize(void);
    virtual void execute(void);
    virtual void finalize(void);

    virtual void activate_scheme(const bool is_loading, CScriptGameObject* const p_client_object);
    virtual void update(const float delta);

private:
    bool m_is_in_camp;
    xr_vector<std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>> m_avail_actions;
};
} // namespace Scripts
} // namespace Cordis
