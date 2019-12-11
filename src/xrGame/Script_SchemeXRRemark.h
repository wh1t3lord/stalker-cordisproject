#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_EvaluatorNeedRemark : public CScriptPropertyEvaluator
{
    Script_EvaluatorNeedRemark(const xr_string& evaluator_name, DataBase::Storage_Scheme& storage)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_storage(&storage)
    {
    }

    virtual _value_type evaluate(void);

private:
    DataBase::Storage_Scheme* m_p_storage;
};

class Script_SchemeXRRemark : public Script_ISchemeStalker
{
    using inherited_scheme = Script_ISchemeStalker;

public:
    Script_SchemeXRRemark(void) = delete;
    Script_SchemeXRRemark(const xr_string& action_name, DataBase::Storage_Scheme& storage)
        : inherited_scheme(nullptr, action_name, storage), m_is_sound_end_signalled(false),
          m_is_action_end_signalled(false), m_is_animation_end_signalled(false), m_is_sound_scheduled(false),
          m_is_sound_started(false), m_state(0)
    {
        
    }

    virtual void initialize(void);
    virtual void execute(void);
    virtual void finalize(void);

    virtual void activate_scheme(const bool is_loading, CScriptGameObject* const p_client_object);
    virtual void update(const float delta);

private:
    // @ First -> p_client_look_object | Second -> look_position
    std::pair<CScriptGameObject*, Fvector> get_target(void);
    inline void time_callback(void)
    {
        this->m_state = XR_REMARK::kStateSound;
        this->update(0.0f);
    }

private:
    bool m_is_sound_end_signalled;
    bool m_is_action_end_signalled;
    bool m_is_animation_end_signalled;
    bool m_is_sound_scheduled;
    bool m_is_sound_started;
    std::uint32_t m_state;
};
} // namespace Scripts
} // namespace Cordis
