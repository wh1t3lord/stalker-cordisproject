#include "stdafx.h"
#include "Script_SchemeXRReachTask.h"

constexpr std::uint32_t _kGetSoldierID = 0;
constexpr std::uint32_t _kGetDirection = 1;
constexpr std::uint32_t _kGetDist = 2;

namespace Cordis
{
	namespace Scripts
	{
		Script_ReachTaskManager::Script_ReachTaskManager(const xr_string& target_id_name) : m_target_name(target_id_name), m_current_state_name("patrol"), m_commander_id(Globals::kUnsignedInt16Undefined), m_formation_name("back"), m_commander_level_vertex_id(Globals::kUnsignedInt32Undefined), m_commander_direction(Fvector().set(0.0f, 0.0f, 1.0f)), m_npc_count(0) 
		{
		}

		Script_ReachTaskManager::~Script_ReachTaskManager(void)
		{
		}

		void Script_ReachTaskManager::add_npc(CScriptGameObject* const p_npc)
		{
			if (p_npc == nullptr)
			{
				MESSAGEWR("Invalid npc");
				return;
			}

			if (p_npc->Alive() == false || this->m_npc_list.find(p_npc->ID()) != this->m_npc_list.end())
				return;
			
			this->m_npc_list[p_npc->ID()] = {p_npc->ID(), Fvector().set(1.0f, 0.0f, 0.0f), 0.0f};
			++this->m_npc_count;

			if (p_npc->ID() == Globals::get_object_squad(p_npc->ID())->commander_id())
			{
				this->m_commander_id = p_npc->ID();
				MESSAGE("assigned npc %s to patrol manager", p_npc->Name());
			}

			this->reset_positions();
		}

		void Script_ReachTaskManager::remove_npc(CScriptGameObject* const p_npc)
		{
			if (p_npc == nullptr)
			{
				MESSAGEWR("Invalid npc");
				return;
			}

			if (this->m_npc_list.find(p_npc->ID()) == this->m_npc_list.end())
			{
				MESSAGEWR("Can't delete npc because he is not registered in this list!");
				return;
			}

			--this->m_npc_count;
			if (p_npc->ID() == this->m_commander_id)
			{
				this->m_commander_id = Globals::kUnsignedInt16Undefined;
				this->reset_positions();
			}
		}

		void Script_ReachTaskManager::reset_positions(void)
		{
			const xr_vector<std::pair<Fvector, float>>& picked_formation = Script_GlobalHelper::getInstance().getXRReachTaskFormations().at(this->m_formation_name);
			std::uint32_t index = 0;

			for (std::pair<const std::uint16_t, std::tuple<std::uint16_t, Fvector, float>>& it : this->m_npc_list)
			{
				CSE_Abstract* const p_server_instance = ai().alife().objects().object(std::get<_kGetSoldierID>(it.second));
				if (p_server_instance == nullptr)
				{
					MESSAGEWR("can't find server instance by ID %d", std::get<_kGetSoldierID>(it.second));
					return;
				}

				Script_SE_SimulationSquad* const p_server_squad = Globals::get_object_squad(p_server_instance->ID);

				if (p_server_squad == nullptr)
				{
					MESSAGEWR("can't find server squad by ID %d", (p_server_instance->ID));
					return;
				}

				if (this->m_commander_id == Globals::kUnsignedInt16Undefined)
				{
					this->m_commander_id = p_server_squad->commander_id();
				}

				if (this->m_commander_id != std::get<_kGetSoldierID>(it.second))
				{
					std::get<_kGetDirection>(it.second) = picked_formation.at(index).first;
					std::get<_kGetDist>(it.second) = picked_formation.at(index).second;
					++index;
				}

			}

		}

		void Script_ReachTaskManager::set_formation(const xr_string& formation_name)
		{
			if (this->m_formation_name.empty())
			{
				MESSAGEWR("Invalid formation");
				return;
			}

			if (this->m_formation_name != "back")
			{
				MESSAGEW("passed formation as argument doesn't exist %s", formation_name.c_str());
				this->m_formation_name = "back";
				return;
			}

			this->m_formation_name = formation_name;
			this->reset_positions();
		}

		CScriptGameObject* Script_ReachTaskManager::get_commander(CScriptGameObject* const p_npc)
		{
			if (p_npc == nullptr)
			{
				MESSAGEWR("invalid npc");
				return nullptr;
			}

			const std::uint16_t npc_id = p_npc->ID();

			if (this->m_npc_list.find(npc_id) == this->m_npc_list.end())
			{
				MESSAGEWR("npc doesn't registered in this manager! %s", p_npc->Name());
				return nullptr;
			}

			if (p_npc->ID() == this->m_commander_id)
			{
				MESSAGEWR("Commander is called this function! Can't be");
				return nullptr; 
			}

			std::uint16_t commander_id = std::get<_kGetSoldierID>(this->m_npc_list.at(this->m_commander_id));
			if (commander_id == 0 || commander_id == Globals::kUnsignedInt16Undefined)
			{
				MESSAGEWR("not present in manager!");
				return nullptr;
			}

			return Globals::Game::level::get_object_by_id(commander_id);
			
		}

		void Script_ReachTaskManager::get_npc_command(CScriptGameObject* const p_npc, std::uint32_t& vertex, Fvector& direction, xr_string& state_name)
		{
			if (p_npc == nullptr)
			{
				MESSAGEWR("Invalid NPC");
				return;
			}

			const std::uint16_t npc_id = p_npc->ID();

			if (this->m_npc_list.find(p_npc->ID()) == this->m_npc_list.end())
			{
				MESSAGEWR("this npc %d doesn't exist in manager!", npc_id);
				return;
			}

			if (p_npc->ID() == this->m_commander_id)
			{
				MESSAGEWR("commander is called this manager!");
				return;
			}

			CScriptGameObject* const p_client_commander = Globals::Game::level::get_object_by_id(std::get<_kGetSoldierID>(this->m_npc_list.at(this->m_commander_id)));

			if (p_client_commander == nullptr)
			{
				MESSAGEWR("commander is nullptr!");
				return;
			}

			Fvector _direction = p_client_commander->Direction();
			Fvector position;

			std::uint32_t vertex_id = p_client_commander->location_on_path(5.0f, &position);

			if (Globals::Game::level::vertex_position(vertex_id).distance_to(Globals::Game::level::get_object_by_id(std::get<_kGetSoldierID>(this->m_npc_list.at(npc_id)))->Position()) > 5.0f)
			{
				vertex_id = p_client_commander->level_vertex_id();
			}

			_direction.y = 0.0f;
			_direction.normalize();

			Fvector dir_s = std::get<_kGetDirection>(this->m_npc_list.at(npc_id));
			float dist_s = std::get<_kGetDist>(this->m_npc_list.at(npc_id));

			float angle = Globals::yaw_degree(dir_s, Fvector().set(0.0f, 0.0f, 1.0f));
			Fvector vvv = Globals::vector_cross(dir_s, Fvector().set(0.0f, 0.0f, 1.0f));

			if (vvv.y < 0.0f)
			{
				angle = -angle;
			}

			dir_s = Globals::vector_rotate_y(_direction, angle);

			float d__d = 2.0f;

			std::uint32_t result_vertex = Globals::vertex_in_direction(Globals::vertex_in_direction(vertex_id, dir_s, dist_s), _direction, d__d);

			float distance = p_client_commander->Position().distance_to(Globals::Game::level::get_object_by_id(std::get<_kGetSoldierID>(this->m_npc_list.at(npc_id)))->Position());

			if (distance > dist_s + 2.0f)
			{
				xr_string new_state_name = Script_GlobalHelper::getInstance().getXRPatrolAccelByCurrentType().at(this->m_current_state_name);
				if (new_state_name.empty() == false)
				{
					vertex = result_vertex;
					direction = _direction;
					state_name = new_state_name;
					return;
				}
			}

			vertex = result_vertex;
			direction = _direction;
			state_name = this->m_current_state_name;

			return;
		}

		void Script_ReachTaskManager::set_command(CScriptGameObject* const p_npc, const xr_string& command_name, const xr_string& formation_name)
		{
			if (p_npc == nullptr)
			{
				MESSAGEWR("Invalid npc");
				return;
			}

			if (p_npc->Alive() == false)
			{
				MESSAGEWR("commander is dead");
				return;
			}

			if (p_npc->ID() != this->m_commander_id)
			{
				MESSAGEWR("npc[%d] != commander_id[%d]", p_npc->ID(), this->m_commander_id);
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

		bool Script_ReachTaskManager::is_commander(const std::uint16_t npc_id)
		{
			return (npc_id == this->m_commander_id);	
		}

		bool Script_ReachTaskManager::is_commander_in_meet(void)
		{
			if (this->m_commander_id == Globals::kUnsignedInt16Undefined)
				return false;

			CScriptGameObject* const p_npc = Globals::Game::level::get_object_by_id(std::get<_kGetSoldierID>(this->m_npc_list.at(this->m_commander_id)));

			if (p_npc && p_npc->Alive())
				return Script_SchemeXRMeet::is_meet(p_npc);

			return false;
		}

		Script_SchemeXRReachTask::Script_SchemeXRReachTask(const xr_string& name, DataBase::Script_ComponentScheme_XRReachTask* storage) : Script_ISchemeStalker(nullptr, name, storage), m_p_storage(storage)
		{
		}

		Script_SchemeXRReachTask::~Script_SchemeXRReachTask(void)
		{
		}

		void Script_SchemeXRReachTask::initialize(void)
		{
			CScriptActionBase::initialize();
			Script_SE_SimulationSquad* const p_squad = Globals::get_object_squad(this->m_object->ID());

			if (!p_squad)
			{
				MESSAGEWR("Can't get_object_squad by ID %d", this->m_object->ID()); 
				return;
			}

			this->m_target_id = p_squad->getAssignedTargetID();
			this->m_squad_id = p_squad->ID;
			this->m_current_state_name = "patrol";
			this->m_formation_name = "back";
			this->m_level_vertex_id = Globals::kUnsignedInt16Undefined;
			this->m_dist = 0.0f;
			this->m_direction = Fvector().set(0.0f, 0.0f, 1.0f);
			this->m_is_on_point = false;
			this->m_is_was_reset = false;
			this->m_time_to_update = Globals::get_time_global() + 1000;

			this->m_object->set_desired_position();
			this->m_object->set_movement_selection_type(ESelectionType::eSelectionTypeMask);
			this->m_object->set_item(EObjectAction::eObjectActionIdle, this->m_object->best_weapon());
			this->m_object->set_body_state(EBodyState::eBodyStateStand);
			this->m_object->set_detail_path_type(DetailPathManager::eDetailPathTypeSmooth);
			this->m_object->set_mental_state(MonsterSpace::eMentalStateFree);
			this->m_object->set_movement_type(EMovementType::eMovementTypeWalk);

            CSE_Abstract* const p_target = ai().alife().objects().object(this->m_target_id);
			this->m_object->set_dest_game_vertex_id(p_target->cast_alife_dynamic_object()->m_tGraphID);

			this->m_object->set_path_type(MovementManager::ePathTypeGamePath);
			this->m_object->inactualize_patrol_path();
			this->m_object->set_sight(SightManager::eSightTypePathDirection, static_cast<Fvector*>(nullptr), static_cast<std::uint32_t>(0));

			xr_string generated_id_name(std::move(std::to_string(this->m_target_id)));
			generated_id_name.append("_to_");
			generated_id_name.append(std::to_string(this->m_squad_id));

			Script_ReachTaskManager* p_manager = nullptr;
			if (DataBase::Storage::getInstance().getPatrolsXRReachTask().find(generated_id_name) == DataBase::Storage::getInstance().getPatrolsXRReachTask().end())
			{
				p_manager = new Script_ReachTaskManager(std::to_string(this->m_target_id).c_str());
				DataBase::Storage::getInstance().setPatrolsXRReachTask(generated_id_name, p_manager);
			}
			else
			{
				p_manager = DataBase::Storage::getInstance().getPatrolsXRReachTask().at(generated_id_name);
			}

			if (p_manager == nullptr)
			{
				R_ASSERT2(false, "something is wrong!");
				return;
			}
 
			p_manager->add_npc(this->m_object);
		}

		void Script_SchemeXRReachTask::execute(void)
		{
			if (this->m_object->ID() == Globals::get_object_squad(this->m_object->ID())->commander_id())
			{
				this->commander_execute();
			}
			else
			{
				this->soldier_execute();
			}

			CScriptActionBase::execute();
		}

		void Script_SchemeXRReachTask::finalize(void)
		{
			this->m_object->set_movement_selection_type(ESelectionType::eSelectionTypeRandomBranching);
			CScriptActionBase::finalize();
		}

		void Script_SchemeXRReachTask::death_callback(CScriptGameObject* const p_client_victim, CScriptGameObject* const p_client_who)
		{
			if (p_client_who == nullptr || p_client_victim == nullptr)
			{
				MESSAGEWR("can't be, both arguemnts are empty!");
				return;
			}

			if (this->m_target_id)
			{
				xr_string generated_id_name = std::to_string(this->m_target_id).c_str();
				generated_id_name.append("_to_");
				generated_id_name.append(std::to_string(this->m_squad_id).c_str());

				const auto& storage_reach_tasks = DataBase::Storage::getInstance().getPatrolsXRReachTask();
				storage_reach_tasks.at(generated_id_name)->remove_npc(p_client_victim);
			}
		}

		void Script_SchemeXRReachTask::net_destroy(CScriptGameObject* const p_client_object)
		{
			if (this->m_target_id)
			{
				xr_string generated_id_name(std::move(std::to_string(this->m_target_id)));
				generated_id_name.append("_to_");
				generated_id_name.append(std::to_string(this->m_squad_id).c_str());

				const auto& storage_reach_tasks = DataBase::Storage::getInstance().getPatrolsXRReachTask();
				storage_reach_tasks.at(generated_id_name)->remove_npc(p_client_object);
			}
		}

		void Script_SchemeXRReachTask::set_reach_task(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini, const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
		{
			DataBase::Script_ComponentScheme_XRReachTask* const p_storage = XR_LOGIC::assign_storage_and_bind<DataBase::Script_ComponentScheme_XRReachTask>(p_client_object, p_ini, scheme_name, section_name, gulag_name);
		}

		void Script_SchemeXRReachTask::add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini, const xr_string& scheme_name, const xr_string& section_name, DataBase::Script_IComponentScheme* storage)
		{
			if (p_client_object == nullptr)
			{
				MESSAGEWR("Invalid npc!");
				return;
			}

			CScriptActionPlanner* const p_planner = Globals::get_script_action_planner(p_client_object);
			CScriptActionPlanner* const p_casted = Globals::cast_planner(&p_planner->action(StalkerDecisionSpace::eWorldOperatorALifePlanner));
			if (p_casted == nullptr)
			{
				MESSAGEWR("can't cast planner and subscribe action!");
				return;
			}

			Script_ISchemeStalker* const p_action = dynamic_cast<Script_ISchemeStalker*>(&p_casted->action(StalkerDecisionSpace::eWorldOperatorSmartTerrainTask));
			if (p_action == nullptr)
			{
				MESSAGEER("can't be bad casting!");
				return;
			}

			DataBase::Storage::getInstance().setStorageSchemesActions(p_client_object->ID(), scheme_name, p_action);
		}

		void Script_SchemeXRReachTask::add_reach_task_action(CScriptGameObject* const p_client_object)
		{
			if (p_client_object == nullptr)
			{
				MESSAGEER("Invalid npc can't register action!");
				return;
			}

			CScriptActionPlanner* const p_planner = Globals::get_script_action_planner(p_client_object);
			CScriptActionPlanner* const p_casted = Globals::cast_planner(&p_planner->action(StalkerDecisionSpace::eWorldOperatorALifePlanner));

			if (p_casted == nullptr)
			{
				MESSAGEER("bad cast!");
				return;
			}

			p_casted->remove_evaluator(StalkerDecisionSpace::eWorldPropertySmartTerrainTask);
			p_casted->add_evaluator(StalkerDecisionSpace::eWorldPropertySmartTerrainTask, new Script_EvaluatorReachedTaskLocation("reached_task_location"));
			p_casted->remove_operator(StalkerDecisionSpace::eWorldOperatorSmartTerrainTask);

			/*const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(p_client_object->ID());*/
			Script_SchemeXRReachTask* const p_action = new Script_SchemeXRReachTask("reach_task_location", nullptr);


			p_action->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyALife, true));
			p_action->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertySmartTerrainTask, true));
			p_action->add_effect(CWorldProperty(StalkerDecisionSpace::eWorldPropertySmartTerrainTask, false));

			p_casted->add_operator(StalkerDecisionSpace::eWorldOperatorSmartTerrainTask, p_action);
		}

		void Script_SchemeXRReachTask::commander_execute(void)
		{
			Script_SE_SimulationSquad* const p_server_squad = Globals::get_object_squad(this->m_object->ID());

			CSE_ALifeDynamicObject* p_server_squad_target = Script_SimulationObjects::getInstance().getObjects().at(p_server_squad->getAssignedTargetID());

			if (p_server_squad_target == nullptr && p_server_squad->getScriptTarget())
			{
				p_server_squad_target = ai().alife().objects().object(p_server_squad->getAssignedTargetID());
			}

			auto update_movement = [](CSE_ALifeDynamicObject* const p_target, CScriptGameObject* const p_object) -> void {
				if (p_object == nullptr)
				{
					MESSAGEWR("can't be p_object == nullptr");
					return;
				}

				if (p_target && !p_object->IsTalking())
				{
					if (XR_CONDITION::is_surge_started_server(nullptr, nullptr, {}))
					{
						p_object->set_movement_type(EMovementType::eMovementTypeRun);
						p_object->set_mental_state(EMentalState::eMentalStateFree);
						return;
					}
					MESSAGE("JUST DELETE THIS AFTER SUCCESSFUL TESTING CLSID p_target -> %d ||| CLSID ONLINE_OFFLINE_GROUP -> %d", p_target->script_clsid(), CLSID_SE_ONLINE_OFFLINE_GROUP);
					if (p_target->script_clsid() == Globals::get_script_clsid(CLSID_SE_ONLINE_OFFLINE_GROUP))
					{
						p_object->set_movement_type(EMovementType::eMovementTypeRun);
						if (p_target->o_Position.distance_to_sqr(p_object->Position()) <= 10000.0f)
						{
							p_object->set_mental_state(EMentalState::eMentalStateDanger);
						}
						else
						{
							p_object->set_mental_state(EMentalState::eMentalStateFree);
						}
					}
					else
					{
						p_object->set_movement_type(EMovementType::eMovementTypeWalk);
						p_object->set_mental_state(EMentalState::eMentalStateFree);
					}
				}
				else
				{
					p_object->set_movement_type(EMovementType::eMovementTypeStand);
				}
			};

			if (p_server_squad_target && !this->m_object->IsTalking())
			{
				Fvector position = p_server_squad_target->position();
				std::uint32_t level_vertex_id = p_server_squad_target->m_tNodeID;
				std::uint16_t game_vertex_id = p_server_squad_target->m_tGraphID;

				if (this->m_object->game_vertex_id() != game_vertex_id)
				{
					this->m_object->set_path_type(MovementManager::ePathTypeGamePath);
					this->m_object->set_dest_game_vertex_id(game_vertex_id);
					this->m_object->set_sight(SightManager::eSightTypePathDirection, static_cast<Fvector*>(nullptr), static_cast<std::uint32_t>(0));
					update_movement(p_server_squad_target, this->m_object);
					DataBase::Storage::getInstance().getPatrolsXRReachTask().at(xr_string(std::to_string(this->m_target_id).c_str()).append("_to_").append(std::to_string(this->m_squad_id).c_str()))->set_command(this->m_object, this->m_current_state_name, this->m_formation_name);
					return;
				}

				this->m_object->set_path_type(MovementManager::EPathType::ePathTypeLevelPath);

				if (this->m_object->accessible_position(position) == false)
				{
					Fvector ttp;
					std::uint32_t _level_vert_id = this->m_object->accessible_nearest(position, ttp);
					Globals::Game::level::vertex_position(_level_vert_id);
				}

				this->m_object->set_sight(SightManager::eSightTypePathDirection, static_cast<Fvector*>(nullptr), static_cast<std::uint32_t>(0));
				this->m_object->set_dest_level_vertex_id(level_vertex_id);
				this->m_object->set_dest_game_vertex_id(game_vertex_id);
			}

			update_movement(p_server_squad_target, this->m_object);
			DataBase::Storage::getInstance().getPatrolsXRReachTask().at(xr_string(std::to_string(this->m_target_id).c_str()).append("_to_").append(std::to_string(this->m_squad_id).c_str()))->set_command(this->m_object, this->m_current_state_name, this->m_formation_name);
		}

		void Script_SchemeXRReachTask::soldier_execute(void)
		{
			if (this->m_time_to_update - Globals::get_time_global())
			{
				return;
			}

			Script_SE_SimulationSquad* const p_squad = Globals::get_object_squad(this->m_object->ID());
			CSE_ALifeDynamicObject* p_server_object = Script_SimulationObjects::getInstance().getObjects().at(p_squad->getAssignedTargetID());

			if (p_server_object == nullptr && p_squad->getScriptTarget())
			{
				p_server_object = ai().alife().objects().object(p_squad->getAssignedTargetID());
			}

			this->m_time_to_update = Globals::get_time_global() + 1000;
			xr_string picked_id_name = std::to_string(this->m_target_id).c_str();
			picked_id_name.append("_to_").append(std::to_string(this->m_squad_id).c_str());

			DataBase::Storage::getInstance().getPatrolsXRReachTask().at(picked_id_name)->get_npc_command(this->m_object, this->m_level_vertex_id, this->m_direction, this->m_current_state_name);
			this->m_level_vertex_id = Globals::Utils::send_to_nearest_accessible_vertex(this->m_object, this->m_level_vertex_id);
			Fvector desired_direction = this->m_direction;
			
			if (Globals::is_vector_nil(desired_direction) == false)
			{
				desired_direction.normalize();
				this->m_object->set_desired_direction(&desired_direction);
			}

			this->m_object->set_path_type(MovementManager::ePathTypeLevelPath);

			if (p_server_object == nullptr || XR_CONDITION::is_surge_started_server(nullptr, nullptr, {}))
			{
				this->m_object->set_movement_type(Globals::Game::level::get_object_by_id(p_squad->commander_id())->movement_type());
				this->m_object->set_mental_state(Globals::Game::level::get_object_by_id(p_squad->commander_id())->mental_state());
				return;
			}
			else
			{
				if (p_server_object->m_script_clsid == Globals::get_script_clsid(CLSID_SE_ONLINE_OFFLINE_GROUP))
				{
					this->m_object->set_movement_type(Globals::Game::level::get_object_by_id(p_squad->commander_id())->movement_type());
					this->m_object->set_mental_state(Globals::Game::level::get_object_by_id(p_squad->commander_id())->mental_state());
					return;
				}
			}

			if (Globals::Game::level::get_object_by_id(Globals::get_object_squad(this->m_object->ID())->commander_id())->movement_type() == MonsterSpace::eMovementTypeStand)
			{
				this->m_object->set_movement_type(MonsterSpace::eMovementTypeStand);
				return;
			}

			if (Globals::Game::level::vertex_position(this->m_level_vertex_id).distance_to(this->m_object->Position()) > 5.0f)
			{
				this->m_object->set_movement_type(EMovementType::eMovementTypeRun);
			}
			else
			{
				this->m_object->set_movement_type(EMovementType::eMovementTypeWalk);
			}
		}
		Script_EvaluatorReachedTaskLocation::_value_type Script_EvaluatorReachedTaskLocation::evaluate(void)
		{
			Script_SE_SimulationSquad* const p_squad = Globals::get_object_squad(this->m_object->ID());

			if (p_squad && p_squad->getCurrentAction().getName() == Globals::kSimulationSquadCurrentActionIDReachTarget)
			{
				CSE_Abstract* const p_server_object = ai().alife().objects().object(p_squad->getAssignedTargetID());
				if (p_server_object == nullptr)
					return false;

				return !p_server_object->cast_script_se_smartterrain()->am_i_reached(p_squad); // Lord: проверить какой гарантированно тип прилетает для каста!
			}

			return false;
		}
}
}
