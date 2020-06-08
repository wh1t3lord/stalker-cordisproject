#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_EvaluatorNeedWalker : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorNeedWalker(const xr_string& evaluator_name, void* storage)
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
    Script_SchemeXRWalker(const xr_string& action_name, void* storage);
    ~Script_SchemeXRWalker(void);

    virtual void initialize(void);
    virtual void execute(void);
    virtual void finalize(void);

    virtual void activate_scheme(const bool is_loading, CScriptGameObject* const p_client_object);
    virtual void update(const float delta);

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

        MESSAGE("added scheme to binder, name=%s scheme=%s section=%s",
            p_client_object->Name(), scheme_name.c_str(), section_name.c_str());

        CScriptActionPlanner* const p_planner = Globals::get_script_action_planner(p_client_object);

        if (!p_planner)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        xr_map<xr_string, std::uint32_t> properties;
        xr_map<xr_string, std::uint32_t> operators;

        properties["event"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kReaction;
        properties["need_walker"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kZmeyWalkerBase;
        properties["state_mgr_logic_active"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kStateManager + 4;
        operators["action_walker"] = Globals::XR_ACTIONS_ID::kZmeyWalkerBase + 1;

        p_planner->add_evaluator(properties.at("need_walker"), new Script_EvaluatorNeedWalker("walker_need", storage));

        Script_SchemeXRWalker* const p_scheme = new Script_SchemeXRWalker("action_walker_activity", storage);
        p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAlive, true));
        p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyDanger, false));
        p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyEnemy, false));
        p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAnomaly, false));
        p_scheme->add_condition(CWorldProperty(properties.at("need_walker"), true));

        p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kStoheMeetBase + 1, false));
        p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kSidorWoundedBase, false));
        p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kAbuseBase, false));
        p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kWoundedExist, false));
        p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kCorpseExist, false));
        p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyItems, false));

        p_scheme->add_effect(CWorldProperty(properties.at("need_walker"), false));
        p_scheme->add_effect(CWorldProperty(properties.at("state_mgr_logic_active"), false));
        p_planner->add_operator(operators.at("action_walker"), p_scheme);

        DataBase::Storage::getInstance().setStorageSchemesActions(p_client_object->ID(), scheme_name, p_scheme);

        p_planner->action(Globals::XR_ACTIONS_ID::kAlife).add_condition(CWorldProperty(properties.at("need_walker"), false));
    }

    // @ PRIVATE, uses in XR_LOGIC
    static void set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);

private:
    bool m_is_in_camp;
    xr_vector<std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>> m_avail_actions;
    Script_CampData* m_p_camp;
};
} // namespace Scripts
} // namespace Cordis
