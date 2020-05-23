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

class Script_ActionXRHelpWounded : public CScriptActionBase
{
    using inherited_scheme = CScriptActionBase;

public:
    Script_ActionXRHelpWounded(void) = delete;
    Script_ActionXRHelpWounded(const xr_string& action_name, DataBase::Storage_Scheme& storage);
    ~Script_ActionXRHelpWounded(void);

    virtual void initialize(void);
    virtual void execute(void);
    virtual void finalize(void);

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

        MESSAGE("registering scheme name=%s scheme=%s section=%s", p_client_object->Name(), scheme_name.c_str(), section_name.c_str());

        CScriptActionPlanner* p_planner = Globals::get_script_action_planner(p_client_object);

        if (!p_planner)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        p_planner->add_evaluator(
            Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kWoundedExist, new Script_EvaluatorWoundedExist("wounded_exist", storage));

        Script_ActionXRHelpWounded* p_action = new Script_ActionXRHelpWounded("action_help_wounded", storage);
        p_action->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAlive, true));
        p_action->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyEnemy, false));
        p_action->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyDanger, false));
        p_action->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAnomaly, false));
        p_action->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kWoundedExist, true));
        p_action->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kSidorWoundedBase, false));
        p_action->add_effect(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kWoundedExist, false));
        p_planner->add_operator(Globals::XR_ACTIONS_ID::kWoundedExist, p_action);

        p_planner->action(Globals::XR_ACTIONS_ID::kAlife)
            .add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kWoundedExist, false));

        p_planner->action((Globals::XR_ACTIONS_ID::kStateManager + 2))
            .add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kWoundedExist, false));
    }

    static void set_help_wounded(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);

    static inline void reset_help_wounded(
        CScriptGameObject* const p_client_object, const xr_string& scheme_name, const xr_string& section_name)
    {
        if (!p_client_object)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(p_client_object->ID());

        if (!storage.getIni())
        {
            R_ASSERT2(false, "can't be you might be not right initialize it in assign_storage_and_bind function");
            return;
        }

        bool result = true;
        if (storage.getIni()->line_exist(section_name.c_str(), "help_wounded_enabled"))
            result = Globals::Utils::cfg_get_bool(storage.getIni(), section_name, "help_wounded_enabled");

        storage.getSchemes().at("help_wounded")->setXRHelpWoundedHelpWoundedEnabled(result);
    }

    static inline bool is_under_help_wounded(CScriptGameObject* const p_client_object)
    {
        CScriptActionPlanner* p_planner = Globals::get_script_action_planner(p_client_object);

        if (!p_planner)
        {
            R_ASSERT2(false, "object is null!");
            return false;
        }

        if (!p_planner->initialized())
            return false;

        std::uint32_t current_id = p_planner->current_action_id();
        return (current_id == Globals::XR_ACTIONS_ID::kWoundedExist);
    }

private:
    DataBase::Storage_Scheme* m_p_storage;
    xr_string m_scheme_name;
};
} // namespace Scripts
} // namespace Cordis
