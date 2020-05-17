#include "stdafx.h"
#include "Script_Binder_Stalker.h"

namespace Cordis
{
namespace Scripts
{
Script_Binder_Stalker::Script_Binder_Stalker(CScriptGameObject* object) : inherited(object), m_is_loaded(false), m_is_first_update(false), m_last_update(0) {}

Script_Binder_Stalker::~Script_Binder_Stalker(void) {}

void Script_Binder_Stalker::reinit(void) 
{
    CScriptBinderObject::reinit();
    CScriptActionPlanner* const p_planner = Globals::get_script_action_planner(this->m_object);
    if (p_planner == nullptr)
    {
        R_ASSERT2(false, "can't get planner!");
        return;
    }
    
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

    Script_StateManager* const p_state_manager = new Script_StateManager(this->m_object);

    p_planner->add_evaluator(properties.at("state_mgr_idle_combat"), new Script_EvaluatorStateManagerIdle("state_mgr_idle_combat", p_state_manager));
    p_planner->add_evaluator(properties.at("state_mgr_idle_alife"), new Script_EvaluatorStateManagerIdleAlife("state_mgr_idle_alife", p_state_manager));
    p_planner->add_evaluator(properties.at("state_mgr_idle_items"), new Script_EvaluatorStateManagerIdleItems("state_mgr_idle_items", p_state_manager));
    p_planner->add_evaluator(properties.at("state_mgr_logic_active"), new Script_EvaluatorStateManagerLogicActive("state_mgr_logic_active", p_state_manager));

    Script_ActionStateManagerToIdle* const p_action = new Script_ActionStateManagerToIdle("state_mgr_to_idle_combat", p_state_manager);
    p_action->add_condition(CWorldProperty(properties.at("state_mgr_idle_combat"), false));
    p_action->add_effect(CWorldProperty(properties.at("state_mgr_idle_combat"), true));
    p_planner->add_operator(operators.at("state_mgr_to_idle_combat"), p_action);

    Script_ActionStateManagerToIdle* const p_action_items = new Script_ActionStateManagerToIdle("state_mgr_to_idle_items", p_state_manager);
    p_action_items->add_condition(CWorldProperty(properties.at("state_mgr_idle_items"), false));
    p_action_items->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyItems, false));
    p_action_items->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyEnemy, false));
    p_action_items->add_effect(CWorldProperty(properties.at("state_mgr_idle_items"), true));
   
    p_planner->add_operator(operators.at("state_mgr_to_idle_items"), p_action_items);

    Script_ActionStateManagerToIdle* const p_action_alife = new Script_ActionStateManagerToIdle("state_mgr_to_idle_alife", p_state_manager);
    p_action_alife->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyEnemy, false));
    p_action_alife->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyDanger, false));

    p_action_alife->add_condition(CWorldProperty(properties.at("state_mgr_logic_active"), false));
    p_action_alife->add_condition(CWorldProperty(properties.at("state_mgr_idle_alife"), false));
    p_action_alife->add_effect(CWorldProperty(properties.at("state_mgr_idle_alife"), true));

    p_planner->add_operator(operators.at("state_mgr_to_idle_alife"), p_action_alife);

    p_planner->action(Globals::XR_ACTIONS_ID::kAlife).add_condition(CWorldProperty(properties.at("state_mgr_idle_alife"), true));
    p_planner->action(StalkerDecisionSpace::eWorldOperatorGatherItems).add_condition(CWorldProperty(properties.at("state_mgr_idle_items"), true));
    p_planner->action(StalkerDecisionSpace::eWorldOperatorCombatPlanner).add_condition(CWorldProperty(properties.at("state_mgr_idle_combat"), true));
    p_planner->action(StalkerDecisionSpace::eWorldOperatorAnomalyPlanner).add_condition(CWorldProperty(properties.at("state_mgr_idle_combat"), true));
    p_planner->action(StalkerDecisionSpace::eWorldOperatorDangerPlanner).add_condition(CWorldProperty(properties.at("state_mgr_idle_combat"), true));

    DataBase::Storage::getInstance().setStorageStateManager(this->m_object, p_state_manager);
    Script_MoveManager* const p_move_manager = new Script_MoveManager(this->m_object);
    
    DataBase::Storage::getInstance().setStorageMoveManager(this->m_object, p_move_manager);
}

void Script_Binder_Stalker::reload(LPCSTR section_name) {}

bool Script_Binder_Stalker::net_Spawn(SpawnType DC) 
{
    MESSAGE("add to database %s", this->m_object->Name());
    DataBase::Storage::getInstance().addObject(this->m_object);
    return true; 
}

void Script_Binder_Stalker::net_Destroy(void)
{
    MESSAGE("delete from database %s", this->m_object->Name());
    DataBase::Storage::getInstance().deleteObject(this->m_object);
}

void Script_Binder_Stalker::net_Import(NET_Packet* packet) {}

void Script_Binder_Stalker::net_Export(NET_Packet* packet) {}

void Script_Binder_Stalker::shedule_Update(std::uint32_t time_delta) {}

void Script_Binder_Stalker::save(NET_Packet* output_packet)
{
    this->m_is_loaded = true;
    Globals::set_save_marker(*output_packet, Globals::kSaveMarkerMode_Save, false, "Script_Binder_Stalker");
    inherited::save(output_packet);
    
    Globals::set_save_marker(*output_packet, Globals::kSaveMarkerMode_Save, true, "Script_Binder_Stalker");
}

void Script_Binder_Stalker::load(IReader* input_packet) {}

bool Script_Binder_Stalker::net_SaveRelevant(void) { return false; }

void Script_Binder_Stalker::net_Relcase(CScriptGameObject* object) {}

void Script_Binder_Stalker::waypoint_callback(CScriptGameObject* p_client_object, const std::uint32_t action_type_movement, const std::uint32_t point_index)
{
}

void Script_Binder_Stalker::hit_callback(CScriptGameObject* p_client_object, const float amount, const Fvector& local_direction, CScriptGameObject* p_client_who, const std::int16_t bone_index)
{
}

void Script_Binder_Stalker::death_callback(CScriptGameObject* p_client_victim, CScriptGameObject* p_client_who)
{
}

void Script_Binder_Stalker::hear_callback(CScriptGameObject* p_client_object, const std::uint16_t who_id, const int sound_type, const Fvector& sound_position, const float sound_power)
{
}

void Script_Binder_Stalker::use_callback(CScriptGameObject* p_client_object, CScriptGameObject* p_client_who)
{
}
} // namespace Scripts
} // namespace Cordis
