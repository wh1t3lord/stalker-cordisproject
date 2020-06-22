#pragma once
namespace Cordis
{
namespace Scripts
{
class Script_EvaluatorCorpse : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorCorpse(const xr_string& evaluator_name, DataBase::Script_ComponentScheme_XRCorpseDetection* storage)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_storage(storage)
    {
    }

    ~Script_EvaluatorCorpse(void) {}

    virtual _value_type evaluate(void);

private:
    DataBase::Script_ComponentScheme_XRCorpseDetection* m_p_storage;
};

class Script_SchemeXRCorpseDetection : public Script_ISchemeStalker
{
    using inherited_scheme = Script_ISchemeStalker;

public:
    Script_SchemeXRCorpseDetection(void) = delete;
    Script_SchemeXRCorpseDetection(const xr_string& action_name, DataBase::Script_ComponentScheme_XRCorpseDetection* storage)
        : inherited_scheme(nullptr, action_name, storage)
    {
        this->m_scheme_name = "corpse_detection";
    }
    ~Script_SchemeXRCorpseDetection(void);

    virtual void initialize(void);
    virtual void execute(void);
    virtual void finalize(void);

    // Lord: доделать add_to_binder и прочий стаф
};

} // namespace Scripts
} // namespace Cordis
