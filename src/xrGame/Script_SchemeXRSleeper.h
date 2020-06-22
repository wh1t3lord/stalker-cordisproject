#pragma once

namespace Cordis
{
    namespace Scripts
    {
    class Script_EvaluatorNeedSleeper : public CScriptPropertyEvaluator
    {
    public:
        Script_EvaluatorNeedSleeper(const xr_string& evaluator_name, DataBase::Script_ComponentScheme_XRSleeper* storage)
            : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_storage(storage)
        {
        }

        virtual _value_type evaluate(void) { return XR_LOGIC::is_active(this->m_object, this->m_p_storage); }

    private:
        DataBase::Script_ComponentScheme_XRSleeper* m_p_storage;
    };

    class Script_SchemeXRSleeper : public Script_ISchemeStalker
    {
        using inherited_scheme = Script_ISchemeStalker;
    public:
        Script_SchemeXRSleeper(const xr_string& action_name, DataBase::Script_ComponentScheme_XRSleeper* storage)
            : inherited_scheme(nullptr, action_name, storage), m_is_reset(false), m_p_move_manager(DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getMoveManager()), m_state(0)
        {

        }
        
        ~Script_SchemeXRSleeper(void);
        virtual void initialize(void);
        virtual void execute(void);
        virtual void finalize(void);
        virtual void activate_scheme(const bool is_loading, CScriptGameObject* const p_client_object);
        virtual void reset_scheme(const bool value, CScriptGameObject* const p_client_object);
        bool callback(std::uint32_t mode, std::uint32_t number);

        static inline void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
            const xr_string& scheme_name, const xr_string& section_name, DataBase::Script_IComponentScheme* storage)
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

            MESSAGEI("added scheme to binder, name=%s scheme=%s section=%s",
                p_client_object->Name(), scheme_name.c_str(), section_name.c_str());

            CScriptActionPlanner* const p_planner = Globals::get_script_action_planner(p_client_object);

            if (!p_planner)
            {
                R_ASSERT2(false, "object is null!");
                return;
            }

            xr_map<xr_string, std::uint32_t> properties;
            xr_map<xr_string, std::uint32_t> operators;

            properties["need_sleeper"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kZmeySleeperBase + 1;
            properties["state_mgr_logic_active"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kStateManager + 4;
            operators["action_sleeper"] = Globals::XR_ACTIONS_ID::kZmeySleeperBase + 1;

            p_planner->add_evaluator(
                properties.at("need_sleeper"), new Script_EvaluatorNeedSleeper("sleeper_need_sleep", static_cast<DataBase::Script_ComponentScheme_XRSleeper*>(storage)));
            
            Script_SchemeXRSleeper* const p_scheme = new Script_SchemeXRSleeper("action_sleeper_activity", static_cast<DataBase::Script_ComponentScheme_XRSleeper*>(storage));
            p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAlive, true));
            p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyDanger, false));
            p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyEnemy, false));
            p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAnomaly, false));
            p_scheme->add_condition(CWorldProperty(properties.at("need_sleeper"), true));

            p_scheme->add_condition(
                CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kStoheMeetBase + 1, false));
            p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kSidorWoundedBase, false));
            p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kAbuseBase, false));
            p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kWoundedExist, false));
            p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kCorpseExist, false));
            p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyItems, false));

            p_scheme->add_effect(CWorldProperty(properties.at("need_sleeper"), false));
            p_scheme->add_effect(CWorldProperty(properties.at("state_mgr_logic_active"), false));

            p_planner->add_operator(operators.at("action_sleeper"), p_scheme);
            DataBase::Storage::getInstance().setStorageSchemesActions(p_client_object->ID(), scheme_name, p_scheme);

            p_planner->action(Globals::XR_ACTIONS_ID::kAlife).add_condition(CWorldProperty(properties.at("need_sleeper"), false));
        }

        // @ PRIVATE, uses in XR_LOGIC
        static void set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
            const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);
    private:
        bool m_is_reset;
        std::uint32_t m_state;
        Script_MoveManager* m_p_move_manager;
    };
    }
}

