#include "stdafx.h"
#include "Script_SchemeXRPatrol.h"

constexpr std::uint32_t kGetSoldier = 0;
constexpr std::uint32_t kGetDirection = 1;
constexpr std::uint32_t kGetDist = 2;

namespace Cordis
{
	namespace Scripts
	{
		Script_EvaluatorPatrolEnd::~Script_EvaluatorPatrolEnd(void)
		{
		}

		Script_EvaluatorPatrolEnd::_value_type Script_EvaluatorPatrolEnd::evaluate(void)
		{
			return XR_LOGIC::is_active(this->m_object, *this->m_p_storage);
		}

		Script_EvaluatorPatrolComm::~Script_EvaluatorPatrolComm(void)
		{
		}

		Script_EvaluatorPatrolComm::_value_type Script_EvaluatorPatrolComm::evaluate(void)
		{
			return DataBase::Storage::getInstance().getPatrolsXRPatrol().at(this->m_p_storage->getXRPatrolPatrolKeyName())->is_commander(this->m_object->ID());
		}

		Script_SchemeXRPatrol::Script_SchemeXRPatrol(const xr_string& name, DataBase::Storage_Scheme& storage) : Script_ISchemeStalker(nullptr, name, storage), m_level_vertex_id(Globals::kUnsignedInt32Undefined), m_dist(0), m_direction(Fvector().set(0.0f, 0.0f, 1.0f)), m_is_on_point(false), m_is_was_reset(false), m_time_to_update(Globals::get_time_global() + 1000), m_current_state_name("patrol")
		{
		}

		Script_SchemeXRPatrol::~Script_SchemeXRPatrol(void)
		{
		}

		void Script_SchemeXRPatrol::activate_scheme(const bool is_loading, CScriptGameObject* const p_client_object)
		{
			this->m_p_storage->ClearSignals();
			MESSAGE("set soldier patrol path %s", this->m_p_storage->getPathWalkName());

			this->m_p_storage->setPathWalkInfo(Globals::Utils::path_parse_waypoints(this->m_p_storage->getPathWalkName()));
			this->m_p_storage->setPathLookInfo(Globals::Utils::path_parse_waypoints(this->m_p_storage->getPathLookName()));

			std::function<bool(std::uint32_t, std::uint32_t)> callback = std::bind(&Script_SchemeXRPatrol::formation_callback, this, std::placeholders::_1, std::placeholders::_2);
			this->m_p_move_manager->reset(this->m_p_storage->getPathWalkName(), this->m_p_storage->getPathWalkInfo(), this->m_p_storage->getPathLookName(), this->m_p_storage->getPathLookInfo(), "", this->m_p_storage->getXRPatrolSuggestedStates(), callback, false);
		}

		void Script_SchemeXRPatrol::initialize(void)
		{
			CScriptActionBase::initialize();

			this->m_object->set_desired_position();
			this->m_object->set_desired_direction();
			this->m_is_on_point = false;
		}

		void Script_SchemeXRPatrol::execute(void)
		{
			if (this->m_time_to_update - Globals::get_time_global() > 0)
				return;

			this->m_time_to_update = Globals::get_time_global() + 1000;

			DataBase::Storage::getInstance().getPatrolsXRPatrol().at(this->m_p_storage->getXRPatrolPatrolKeyName())->get_npc_command(this->m_object, this->m_level_vertex_id, this->m_direction, this->m_current_state_name);

			this->m_level_vertex_id = Globals::Utils::send_to_nearest_accessible_vertex(this->m_object, this->m_level_vertex_id);
			Fvector desired_direction = this->m_direction;
			MESSAGE("desired_direction = {%f %f %f}", this->m_direction.x, this->m_direction.y, this->m_direction.z);

			if (Globals::is_vector_nil(desired_direction) == false)
			{
				desired_direction.normalize();
				this->m_object->set_desired_direction(&desired_direction);
			}

			this->m_object->set_path_type(MovementManager::ePathTypeLevelPath);
		}

		void Script_SchemeXRPatrol::finalize(void)
		{
			if (this->m_object->Alive())
				this->m_p_move_manager->finalize();

			CScriptActionBase::finalize();
		}

		void Script_SchemeXRPatrol::death_callback(CScriptGameObject* const p_client_victim, CScriptGameObject* const p_client_who)
		{
			if (!p_client_who && !p_client_victim)
			{
				MESSAGEWR("can't be check code ...");
				return;
			}

			DataBase::Storage::getInstance().getPatrolsXRPatrol().at(this->m_p_storage->getXRPatrolPatrolKeyName())->remove_npc(p_client_victim);
		}

		void Script_SchemeXRPatrol::deactivate(CScriptGameObject* const p_client_object)
		{
			DataBase::Storage::getInstance().getPatrolsXRPatrol().at(this->m_p_storage->getXRPatrolPatrolKeyName())->remove_npc(p_client_object);
		}

		void Script_SchemeXRPatrol::net_destroy(CScriptGameObject* const p_client_object)
		{
			this->deactivate(p_client_object);
		}

		bool Script_SchemeXRPatrol::formation_callback(std::uint32_t, std::uint32_t)
		{
			return false;
		}

		void Script_SchemeXRPatrol::add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini, const xr_string& scheme_name, const xr_string& section_name, DataBase::Storage_Scheme& storage)
		{
			if (p_client_object == nullptr)
			{
				MESSAGEWR("Can't be!");
				return;
			}

			if (p_ini == nullptr)
			{
				MESSAGEWR("Can't be");
				return;
			}

			xr_map<xr_string, std::uint32_t> properties;
			xr_map<xr_string, std::uint32_t> operators;

			properties["event"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kReaction;
			properties["patrol_end"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kSidorPatrolBase;
			properties["patrol_comm"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kSidorPatrolBase + 1;
			properties["state_mgr_logic_active"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kStateManager + 4;

			operators["action_patrol"] = Globals::XR_ACTIONS_ID::kSidorActPatrol;
			operators["action_commander"] = Globals::XR_ACTIONS_ID::kSidorActPatrol + 1;

			CScriptActionPlanner* const p_planner = Globals::get_script_action_planner(p_client_object);

			p_planner->add_evaluator(properties.at("patrol_end"), new Script_EvaluatorPatrolEnd("patrol_end", storage));
			p_planner->add_evaluator(properties.at("patrol_comm"), new Script_EvaluatorPatrolComm("patrol_comm", storage));

			Script_ActionCommander* const p_scheme_commander = new Script_ActionCommander(p_client_object, "action_commander", storage);
			p_scheme_commander->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAlive, true));
			p_scheme_commander->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyDanger, false));
			p_scheme_commander->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyEnemy, false));
			p_scheme_commander->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAnomaly, false));
			
			p_scheme_commander->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kStoheMeetBase + 1, false));
			p_scheme_commander->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kSidorWoundedBase, false));
			p_scheme_commander->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kAbuseBase, false));
			p_scheme_commander->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kWoundedExist, false));
			p_scheme_commander->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kCorpseExist, false));
			p_scheme_commander->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyItems, false));

			p_scheme_commander->add_condition(CWorldProperty(properties.at("patrol_end"), false));
			p_scheme_commander->add_condition(CWorldProperty(properties.at("patrol_comm"), true));
			p_scheme_commander->add_effect(CWorldProperty(properties.at("patrol_end"), true));
			p_scheme_commander->add_effect(CWorldProperty(properties.at("state_mgr_logic_active"), false));
			p_planner->add_operator(operators.at("action_commander"), p_scheme_commander);
			DataBase::Storage::getInstance().setStorageSchemesActions(p_client_object->ID(), scheme_name, p_scheme_commander);

			Script_SchemeXRPatrol* const p_scheme = new Script_SchemeXRPatrol("action_patrol", storage);
			p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAlive, true));
			p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyDanger, false));
			p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyEnemy, false));
			p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAnomaly, false));

			p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kStoheMeetBase + 1, false));
			p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kSidorWoundedBase, false));
			p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kAbuseBase, false));
			p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kWoundedExist, false));
			p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kCorpseExist, false));
			p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyItems, false));

			p_scheme->add_condition(CWorldProperty(properties.at("patrol_end"), false));
			p_scheme->add_condition(CWorldProperty(properties.at("patrol_comm"), false));
			p_scheme->add_effect(CWorldProperty(properties.at("patrol_end"), true));
			p_scheme->add_effect(CWorldProperty(properties.at("state_mgr_logic_active"), false));
			p_planner->add_operator(operators.at("action_patrol"), p_scheme);

			p_planner->action(Globals::XR_ACTIONS_ID::kAlife).add_condition(CWorldProperty(properties.at("patrol_end"), true));

		}

		void Script_SchemeXRPatrol::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini, const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
		{
		}


		Script_XRPatrolManager::Script_XRPatrolManager(const xr_string& path_name) : m_path_name(path_name), m_current_state_name("patrol"), m_commander_id(Globals::kUnsignedInt16Undefined), m_commander_level_vertex_id(Globals::kUnsignedInt32Undefined), m_npc_count(0), m_formation_name("back")
		{
		}

		Script_XRPatrolManager::~Script_XRPatrolManager(void)
		{
		}

		void Script_XRPatrolManager::add_npc(CScriptGameObject* const p_npc, const bool is_leader)
		{
			if (p_npc == nullptr)
			{
				MESSAGEWR("p_npc == nullptr!");
				return;
			}

			if (!p_npc->Alive() || this->m_npc_list.find(p_npc->ID()) == this->m_npc_list.end())
				return;

			if (this->m_npc_count == 7)
			{
				MESSAGEWR("xr_patrol attempt to add more than 7 npc %s", p_npc->Name());
				return;
			}

			this->m_npc_list[p_npc->ID()] = {p_npc, Fvector().set(1.0f, 0.0f, 0.0f), 0};

			++this->m_npc_count;

			if (this->m_npc_count == 1 || is_leader)
			{
				this->m_commander_id = p_npc->ID();
				MESSAGE("assigned npc %s as a patrol commander", p_npc->Name());
			}

			MESSAGE("p_npc %s added to patrol manager %s", p_npc->Name(), this->m_path_name.c_str());
			this->reset_positions();
		}

		void Script_XRPatrolManager::remove_npc(CScriptGameObject* const p_npc)
		{
			if (p_npc == nullptr)
			{
				MESSAGEWR("p_npc == nullptr");
				return;
			}

			if (this->m_npc_list.find(p_npc->ID()) == this->m_npc_list.end())
				return;

			MESSAGE("p_npc %s removed from patrol manager %s", p_npc->Name(), this->m_path_name.c_str());
			this->m_npc_list.erase(p_npc->ID());
			--this->m_npc_count;

			if (p_npc->ID() == this->m_commander_id)
			{
				this->m_commander_id = Globals::kUnsignedInt16Undefined;
				this->reset_positions();
			}
		}

		void Script_XRPatrolManager::reset_positions(void)
		{
			std::uint32_t index = 0;
			const xr_vector<std::pair<Fvector, std::uint32_t>>& picked_formation = Script_GlobalHelper::getInstance().getXRPatrolFormations().at(this->m_formation_name);
			for (std::pair<const std::uint16_t, std::tuple<CScriptGameObject*, Fvector, std::uint32_t>>& it : this->m_npc_list)
			{
				if (this->m_commander_id == Globals::kUnsignedInt16Undefined && index == 0)
				{
					this->m_commander_id = std::get<kGetSoldier>(it.second)->ID();
				}

				if (this->m_commander_id != std::get<kGetSoldier>(it.second)->ID())
				{
					if (index >= picked_formation.size())
					{
						MESSAGEWR("Can't be! something is very wrong! Check your data and code");
						return;
					}

					std::get<kGetDirection>(it.second) = picked_formation.at(index).first;
					std::get<kGetDist>(it.second) = picked_formation.at(index).second;
					++index;
				}
			}
		}

		void Script_XRPatrolManager::set_formation(const xr_string& formation_name)
		{
			if (formation_name.empty())
			{
				MESSAGEWR("Can't be an empty string!");
				return;
			}

			if (formation_name != "around" && formation_name != "back" && formation_name != "line")
			{
				MESSAGEWR("you are using non existed formation [%s], check your code", formation_name.c_str());
				return;
			}

			this->m_formation_name = formation_name;
			this->reset_positions();
		}

		CScriptGameObject* const Script_XRPatrolManager::get_commander(CScriptGameObject* const p_npc)
		{
			if (p_npc == nullptr)
			{
				MESSAGEWR("Invalid NPC!");
				return nullptr;
			}

			if (this->m_npc_list.find(p_npc->ID()) == this->m_npc_list.end())
			{
				MESSAGEWR("NPC %s is not registered!", p_npc->Name());
				return nullptr;
			}

			if (p_npc->ID() == this->m_commander_id)
			{
				MESSAGEWR("commander called this function!");
				return nullptr;
			}

			CScriptGameObject* const p_commander = std::get<kGetSoldier>(this->m_npc_list.at(this->m_commander_id));
			if (p_commander == nullptr)
			{
				MESSAGEWR("commander is not present in this manager!");
				return nullptr;
			}

			return p_commander;
		}

		void Script_XRPatrolManager::get_npc_command(CScriptGameObject* const p_npc, std::uint32_t& vertex, Fvector& direction, xr_string& state_name)
		{
			if (p_npc == nullptr)
			{
				MESSAGEWR("Invalid NPC!");
				return;
			}

			if (this->m_npc_list.find(p_npc->ID()) == this->m_npc_list.end())
			{
				MESSAGEWR("NPC %s is not registered!", p_npc->Name());
				return;
			}

			if (p_npc->ID() == this->m_commander_id)
			{
				MESSAGEWR("commander called this function!");
				return;
			}

			CScriptGameObject* const p_commander = std::get<kGetSoldier>(this->m_npc_list.at(this->m_commander_id));
			
			if (p_commander == nullptr)
			{
				MESSAGEWR("p_commander == nullptr! Can't be");
				return;
			}

			Fvector _direction = p_commander->Direction();
			Fvector position;

			std::uint32_t vertex_id = p_commander->location_on_path(5.0f, &position);

			if (Globals::Game::level::vertex_position(vertex_id).distance_to(std::get<kGetSoldier>(this->m_npc_list.at(p_npc->ID()))->Position()) > 5.0f)
			{
				vertex_id = p_commander->level_vertex_id();
			}

			_direction.y = 0.0f;
			_direction.normalize();

			Fvector dir_s = std::get<kGetDirection>(this->m_npc_list.at(p_npc->ID()));
			std::uint32_t dist_s = std::get<kGetDist>(this->m_npc_list.at(p_npc->ID()));

			float angle = Globals::yaw_degree(dir_s, Fvector().set(0.0f, 0.0f, 1.0f));
			Fvector vvv = Globals::vector_cross(dir_s, Fvector().set(0.0f, 0.0f, 1.0f));

			if (vvv.y < 0.0f)
				angle = -angle;

			dir_s = Globals::vector_rotate_y(_direction, angle);
			float d = 2.0f;

			vertex = Globals::vertex_in_direction(Globals::vertex_in_direction(vertex_id, dir_s, dist_s), _direction, d);
			float distance = p_commander->Position().distance_to(std::get<kGetSoldier>(this->m_npc_list.at(p_npc->ID()))->Position());

			if (distance > dist_s + 2.0f)
			{
				xr_string new_state_name = Script_GlobalHelper::getInstance().getXRPatrolAccelByCurrentType().at(this->m_current_state_name);
				if (new_state_name.empty() == false)
				{
					direction = _direction;
					state_name = new_state_name;
					return;
				}
			}

			direction = _direction;
			state_name = this->m_current_state_name;
		}

		void Script_XRPatrolManager::set_command(CScriptGameObject* const p_npc, const xr_string& command_name, const xr_string& formation_name)
		{
			if (p_npc == nullptr)
			{
				MESSAGEWR("Invalid NPC!");
				return;
			}

			if (p_npc->Alive() == false)
			{
				this->remove_npc(p_npc);
				return;
			}

			this->m_current_state_name = command_name;
			if (this->m_formation_name != formation_name)
			{
				this->m_formation_name = formation_name;
				this->set_formation(formation_name);
			}

			this->m_commander_level_vertex_id = p_npc->level_vertex_id();
			this->m_commander_direction = p_npc->Direction();
		}

		bool Script_XRPatrolManager::is_commander(const std::uint16_t npc_id)
		{
			return (npc_id == this->m_commander_id);
		}

		bool Script_XRPatrolManager::is_commander_in_meet(void)
		{
			if (this->m_commander_id == Globals::kUnsignedInt16Undefined)
				return false;

			CScriptGameObject* const p_npc = std::get<kGetSoldier>(this->m_npc_list.at(this->m_commander_id));
			if (p_npc && p_npc->Alive())
				return Script_SchemeXRMeet::is_meet(p_npc);

			return false;
		}

		Script_ActionCommander::Script_ActionCommander(CScriptGameObject* const p_client_object, const xr_string& name, DataBase::Storage_Scheme& storage) : Script_ISchemeStalker(nullptr, name, storage), m_is_was_reset(false), m_current_state_name("patrol"), m_p_move_manager(DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getMoveManager())
		{
		}

		Script_ActionCommander::~Script_ActionCommander(void)
		{
		}

		void Script_ActionCommander::initialize(void)
		{
			CScriptActionBase::initialize();
			this->m_object->set_desired_position();
			this->m_object->set_desired_direction();

			this->activate_scheme(false, nullptr);
		}

		void Script_ActionCommander::execute(void)
		{
			CScriptActionBase::execute();
			this->m_p_move_manager->update();

			xr_string new_state_name = DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getStateManager()->getTargetStateName();

			if (this->m_old_state_name != new_state_name)
			{
				if (this->m_p_storage->isXRPatrolSilent() == false)
				{
					if (new_state_name == "sneak")
					{
						XR_SOUND::set_sound_play(this->m_object->ID(), "patrol_sneak", xr_string(), 0);
					}
					else if (new_state_name == "sneak_run" || new_state_name == "assault" || new_state_name == "run" || new_state_name == "rush")
					{
						XR_SOUND::set_sound_play(this->m_object->ID(), "patrol_run", xr_string(), 0);
					}
					else
					{
						if (this->m_old_state_name == "sneak" || this->m_old_state_name == "sneak_run" || this->m_old_state_name == "run" || this->m_old_state_name == "assault" || this->m_old_state_name == "rush")
							XR_SOUND::set_sound_play(this->m_object->ID(), "patrol_walk", xr_string(), 0);
					}

					this->m_old_state_name = new_state_name;
				}
			}

			DataBase::Storage::getInstance().getPatrolsXRPatrol().at(this->m_p_storage->getXRPatrolPatrolKeyName())->set_command(this->m_object, new_state_name, this->m_p_storage->getXRPatrolFormationName());
		}

		void Script_ActionCommander::finalize(void)
		{
			if (this->m_object->Alive())
			{
				DataBase::Storage::getInstance().getPatrolsXRPatrol().at(this->m_p_storage->getXRPatrolPatrolKeyName())->set_command(this->m_object, "guard", this->m_p_storage->getXRPatrolFormationName());
				this->m_p_move_manager->finalize(nullptr);
			}

			CScriptActionBase::finalize();
		}

		void Script_ActionCommander::death_callback(CScriptGameObject* const p_client_victim, CScriptGameObject* const p_client_who)
		{
			if (p_client_who == nullptr && p_client_victim == nullptr)
			{
				MESSAGEWR("can't be!");
				return;
			}

			DataBase::Storage::getInstance().getPatrolsXRPatrol().at(this->m_p_storage->getXRPatrolPatrolKeyName())->remove_npc(p_client_victim);
		}

		void Script_ActionCommander::net_destroy(CScriptGameObject* const p_client_object)
		{
			this->deactivate(p_client_object);
		}

		void Script_ActionCommander::deactivate(CScriptGameObject* const p_client_object)
		{
			DataBase::Storage::getInstance().getPatrolsXRPatrol().at(this->m_p_storage->getXRPatrolPatrolKeyName())->remove_npc(p_client_object);
		}

		void Script_ActionCommander::activate_scheme(const bool is_loading, CScriptGameObject* const p_client_object)
		{
			this->m_p_storage->ClearSignals();
			MESSAGE("set patrol path %s", this->m_p_storage->getPathWalkName());

			this->m_p_storage->setPathWalkInfo(Globals::Utils::path_parse_waypoints(this->m_p_storage->getPathWalkName()));
			this->m_p_storage->setPathLookInfo(Globals::Utils::path_parse_waypoints(this->m_p_storage->getPathLookName()));

			std::function<bool(std::uint32_t, std::uint32_t)> callback = std::bind(&Script_ActionCommander::formation_callback, this, std::placeholders::_1, std::placeholders::_2);
			this->m_p_move_manager->reset(this->m_p_storage->getPathWalkName(), this->m_p_storage->getPathWalkInfo(), this->m_p_storage->getPathLookName(), this->m_p_storage->getPathLookInfo(), "", this->m_p_storage->getXRPatrolSuggestedStates(), callback, false);
		}

		bool Script_ActionCommander::formation_callback(std::uint32_t number, std::uint32_t index)
		{
			if (number == 0)
			{
				this->m_p_storage->setXRPatrolFormationName("line");
			}
			else if (number == 1)
			{
				this->m_p_storage->setXRPatrolFormationName("around");
			}
			else if (number == 2)
			{
				this->m_p_storage->setXRPatrolFormationName("back");
			}

			MESSAGE("formation is set to %s", this->m_p_storage->getXRPatrolFormationName().c_str());

			return false;
		}

}
}