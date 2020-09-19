#include "stdafx.h"
#include "Script_DataBase.h"

namespace Cordis
{
namespace Scripts
{
namespace DataBase
{
void Storage::setStorageStateManager(
    CScriptGameObject* const p_client_object, Script_StateManager* const p_state_manager)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "can't be!");
        return;
    }

    this->m_storage[p_client_object->ID()].setStateManager(p_state_manager);

    xr_map<xr_string, std::uint32_t> properties;
    properties["state_mgr_idle_combat"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kStateManager + 1;
    properties["state_mgr_idle_alife"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kStateManager + 2;
    properties["state_mgr_idle_smartcover"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kStateManager + 3;
    properties["state_mgr_logic_active"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kStateManager + 4;
    properties["state_mgr_idle_items"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kStateManager + 5;

    xr_map<xr_string, std::uint32_t> operators;
    operators["state_mgr_to_idle_combat"] = Globals::XR_ACTIONS_ID::kStateManager + 1;
    operators["state_mgr_to_idle_alife"] = Globals::XR_ACTIONS_ID::kStateManager + 2;
    operators["state_mgr_to_idle_items"] = Globals::XR_ACTIONS_ID::kStateManager + 3;

    CScriptActionPlanner* const p_planner = Globals::get_script_action_planner(p_client_object);
    if (!p_planner)
    {
        R_ASSERT2(false, "something is not right");
        return;
    }

    p_planner->add_evaluator(properties.at("state_mgr_idle_combat"),
        new Script_EvaluatorStateManagerIdle("state_mgr_idle_combat", p_state_manager));
    p_planner->add_evaluator(properties.at("state_mgr_idle_alife"),
        new Script_EvaluatorStateManagerIdleAlife("state_mgr_idle_alife", p_state_manager));
    p_planner->add_evaluator(properties.at("state_mgr_idle_items"),
        new Script_EvaluatorStateManagerIdleItems("state_mgr_idle_items", p_state_manager));
    p_planner->add_evaluator(properties.at("state_mgr_logic_active"),
        new Script_EvaluatorStateManagerLogicActive("state_mgr_logic_active", p_state_manager));
    Script_ActionStateManagerToIdle* p_action_to_idle =
        new Script_ActionStateManagerToIdle("state_mgr_to_idle_combat", p_state_manager);
    p_action_to_idle->add_condition(CWorldProperty(properties.at("state_mgr_idle_combat"), false));
    p_action_to_idle->add_effect(CWorldProperty(properties.at("state_mgr_idle_combat"), true));

    p_planner->add_operator(operators.at("state_mgr_to_idle_combat"), p_action_to_idle);

    Script_ActionStateManagerToIdle* p_action_to_idle_items =
        new Script_ActionStateManagerToIdle("state_mgr_to_idle_items", p_state_manager);
    p_action_to_idle_items->add_condition(CWorldProperty(properties.at("state_mgr_idle_items"), false));
    p_action_to_idle_items->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyItems, true));
    p_action_to_idle_items->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyEnemy, false));
    p_action_to_idle_items->add_effect(CWorldProperty(properties.at("state_mgr_idle_items"), true));

    p_planner->add_operator(operators.at("state_mgr_to_idle_items"), p_action_to_idle_items);

    Script_ActionStateManagerToIdle* p_action_to_idle_alife =
        new Script_ActionStateManagerToIdle("state_mgr_to_idle_alife", p_state_manager);
    p_action_to_idle_alife->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyEnemy, false));
    p_action_to_idle_alife->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyDanger, false));
    p_action_to_idle_alife->add_condition(CWorldProperty(properties.at("state_mgr_logic_active"), false));
    p_action_to_idle_alife->add_condition(CWorldProperty(properties.at("state_mgr_idle_alife"), false));
    p_action_to_idle_alife->add_effect(CWorldProperty(properties.at("state_mgr_idle_alife"), true));

    p_planner->add_operator(operators.at("state_mgr_to_idle_alife"), p_action_to_idle_alife);

    CScriptActionPlanner::COperator& p_action_alife = p_planner->action(StalkerDecisionSpace::eWorldPropertyALife);
    p_action_alife.add_condition(CWorldProperty(properties.at("state_mgr_idle_alife"), true));

    CScriptActionPlanner::COperator& p_action_gather_items =
        p_planner->action(StalkerDecisionSpace::eWorldOperatorGatherItems);
    p_action_gather_items.add_condition(CWorldProperty(properties.at("state_mgr_idle_items"), true));

    CScriptActionPlanner::COperator& p_action_combat_planner =
        p_planner->action(StalkerDecisionSpace::eWorldOperatorCombatPlanner);
    p_action_combat_planner.add_condition(CWorldProperty(properties.at("state_mgr_idle_combat"), true));

    CScriptActionPlanner::COperator& p_action_anomaly_planner =
        p_planner->action(StalkerDecisionSpace::eWorldOperatorAnomalyPlanner);
    p_action_anomaly_planner.add_condition(CWorldProperty(properties.at("state_mgr_idle_combat"), true));

    CScriptActionPlanner::COperator& p_action_danger_planner =
        p_planner->action(StalkerDecisionSpace::eWorldOperatorDangerPlanner);
    p_action_danger_planner.add_condition(CWorldProperty(properties.at("state_mgr_idle_combat"), true));
}

void Storage::setStorageMoveManager(CScriptGameObject* const p_client_object, Script_MoveManager* const p_move_manager)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    this->m_storage[p_client_object->ID()].setMoveManager(p_move_manager);
}
} // namespace DataBase
} // namespace Scripts
} // namespace Cordis
