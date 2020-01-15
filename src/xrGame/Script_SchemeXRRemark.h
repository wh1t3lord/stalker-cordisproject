#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_EvaluatorNeedRemark : public CScriptPropertyEvaluator
{
public:
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

    ~Script_SchemeXRRemark(void);

    virtual void initialize(void);
    virtual void execute(void);
    virtual void finalize(void);

    virtual void activate_scheme(const bool is_loading, CScriptGameObject* const p_client_object);
    virtual void update(const float delta);

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
            "Script_SchemeXRRemark scheme to binder, name=%s scheme=%s section=%s",
            p_client_object->Name(), scheme_name.c_str(), section_name.c_str());

        CScriptActionPlanner* p_planner = Globals::get_script_action_planner(p_client_object);

        if (!p_planner)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        p_planner->add_evaluator(
            Globals::XR_ACTIONS_ID::kZmeyRemarkBase + 1, new Script_EvaluatorNeedRemark("remark_need_remark", storage));

        Script_ISchemeStalker* p_scheme = new Script_SchemeXRRemark("action_remark_activity", storage);
        p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAlive, true));
        p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyDanger, false));
        p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyEnemy, false));
        p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAnomaly, false));
        p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kZmeyRemarkBase + 1, true));
        p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kStoheMeetBase + 1, false));
        p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kSidorWoundedBase, false));
        p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kAbuseBase, false));
        p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kWoundedExist, false));
        p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kCorpseExist, false));
        p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyItems, false));
        p_scheme->add_effect(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kZmeyRemarkBase + 1, false));
        p_scheme->add_effect(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kStateManager + 4, false));

        p_planner->add_operator(Globals::XR_ACTIONS_ID::kZmeyRemarkBase + 1, p_scheme);

        DataBase::Storage::getInstance().setStorageSchemesActions(p_client_object->ID(), scheme_name, p_scheme);
        p_planner->action(Globals::XR_ACTIONS_ID::kAlife)
            .add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::kZmeyRemarkBase + 1, false));
    }

    // @ PRIVATE, uses in XR_LOGIC
    static void set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);

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
