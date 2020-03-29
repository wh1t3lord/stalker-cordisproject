#include "stdafx.h"
#include "Script_SchemeXRCamper.h"

namespace Cordis
{
	namespace Scripts
	{
		Script_EvaluatorCloseCombat::_value_type Script_EvaluatorCloseCombat::evaluate(void)
		{
			if (!XR_LOGIC::is_active(this->m_object, *this->m_p_storage))
				return true;

			CScriptActionPlanner* const p_planner = Globals::get_script_action_planner(this->m_object);

			if (!p_planner->evaluator(StalkerDecisionSpace::eWorldPropertyEnemy).evaluate())
				return false;

			if (!p_planner->evaluator(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kSidorWoundedBase + 1).evaluate())
				return false;
			
			if (p_planner->evaluator(StalkerDecisionSpace::eWorldPropertyDanger).evaluate())
				return true;

			if (this->m_object->GetBestEnemy() == nullptr)
				return this->m_is_close_combat;

			if (!this->m_is_close_combat)
				this->m_is_close_combat = this->m_object->Position().distance_to(this->m_object->memory_position(*this->m_object->GetBestEnemy())) < this->m_p_storage->getXRCamperRadius();

			if (this->m_is_close_combat)
			{
				std::uint32_t time = this->m_object->memory_time(*this->m_object->GetBestEnemy());

				if (time)
				{
					if (Globals::get_time_global() - time > 20000)
						this->m_is_close_combat = false;
				}
				else
				{
					this->m_is_close_combat = false;
				}
			}

			return this->m_is_close_combat;
		}

		Script_SchemeXRCamper::Script_SchemeXRCamper(const xr_string& name, DataBase::Storage_Scheme& storage) : inherited_scheme(nullptr, name, storage), m_is_danger(false), m_flag(0), m_p_enemy(nullptr), m_scantime(0)
		{
			this->m_p_move_manager = DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getMoveManager();
			
		}

		Script_SchemeXRCamper::~Script_SchemeXRCamper(void)
		{
		}

		void Script_SchemeXRCamper::initialize(void)
		{
			CScriptActionBase::initialize();

			this->m_object->set_desired_position();
			this->m_object->set_desired_direction();

			this->reset_scheme(false, nullptr);
			this->m_enemy_position = Fvector();
		}

		void Script_SchemeXRCamper::execute(void)
		{
			CScriptActionBase::execute();

			this->m_p_enemy = this->m_object->GetBestEnemy();

			if (this->m_p_enemy)
			{
				this->m_p_storage->setXRCamperMemoryEnemy(this->m_object->memory_time(*this->m_p_enemy));

				std::uint32_t mem_enemy = this->m_p_storage->getXRCamperMemoryEnemy();

				if (mem_enemy == 0 || (Globals::get_time_global() - mem_enemy > this->m_p_storage->getXRCamperIdle()))
				{
					this->m_p_enemy = nullptr;
					this->m_p_storage->setXRCamperMemoryEnemy(0);
					this->m_p_move_manager->Continue();
				}
			}
			else
			{
				if (this->m_p_storage->getXRCamperMemoryEnemy())
				{
					this->m_p_storage->setXRCamperMemoryEnemy(0);
					this->m_p_move_manager->Continue();
				}
			}

			if (this->m_p_enemy)
			{
				if (this->m_object->CheckObjectVisibility(this->m_p_enemy) && this->can_shoot())
				{
					if (this->m_p_storage->isXRCamperSniper())
					{
						if (this->m_p_storage->getXRCamperSuggestedStates().at("campering_fire").empty() == false)
						{
							Globals::set_state(this->m_object, this->m_p_storage->getXRCamperSuggestedStates().at("campering_fire"), StateManagerCallbackData(), 0, std::pair<Fvector, CScriptGameObject* const>(this->m_p_enemy->Position(), this->m_p_enemy), StateManagerExtraData());
						}
						else
						{
							Globals::set_state(this->m_object, "hide_sniper_fire", StateManagerCallbackData(), 0, std::pair<Fvector, CScriptGameObject* const>(this->m_p_enemy->Position(), this->m_p_enemy), StateManagerExtraData());
						}
					}
					else
					{
						if (this->m_p_storage->getXRCamperSuggestedStates().at("campering_fire").empty() == false)
						{
							Globals::set_state(this->m_object, this->m_p_storage->getXRCamperSuggestedStates().at("campering_fire"), StateManagerCallbackData(), 0, std::pair<Fvector, CScriptGameObject* const>(this->m_p_enemy->Position(), this->m_p_enemy), StateManagerExtraData());
						}
						else
						{
							Globals::set_state(this->m_object, "hide_fire", StateManagerCallbackData(), 0, std::pair<Fvector, CScriptGameObject* const>(this->m_p_enemy->Position(), this->m_p_enemy), StateManagerExtraData());
						}
					}

					xr_string temp_faction_name;
					XR_SOUND::set_sound_play(this->m_object->ID(), this->m_p_storage->getXRCamperAttackSoundName(), temp_faction_name, 0);
				}
				else
				{
					Fvector memory_position = this->m_object->memory_position(*this->m_p_enemy);

					if (Globals::is_vector_nil(this->m_enemy_position) || ((this->m_enemy_position.x != memory_position.x) && (this->m_enemy_position.y != memory_position.y) && (this->m_enemy_position.z != memory_position.z)))
					{
						this->m_enemy_position = memory_position;
						
						if (this->m_p_storage->isXRCamperSniper())
						{
							Fvector _position = this->m_object->Position();
							Fvector _direction = Fvector().set(this->m_enemy_position.x - _position.x, 0.0f, this->m_enemy_position.z - _position.z);

							_direction.normalize();

							float wide_sight = _position.distance_to(this->m_enemy_position) * tan(this->m_p_storage->getXRCamperEnemyDisp());

							Fvector point0 = Fvector().set(this->m_enemy_position.x + wide_sight * _direction.z, this->m_enemy_position.y, this->m_enemy_position.z - wide_sight * _direction.x);
							Fvector point1 = Fvector().set(this->m_enemy_position.x - wide_sight * _direction.z, this->m_enemy_position.y, this->m_enemy_position.z + wide_sight * _direction.x);
							
							this->m_p_storage->setXRCamperScanTable(Globals::kUnsignedInt32Undefined, 0, point0);
							this->m_p_storage->setXRCamperScanTable(Globals::kUnsignedInt32Undefined, 1, this->m_enemy_position);
							this->m_p_storage->setXRCamperScanTable(Globals::kUnsignedInt32Undefined, 2, point1);
						}
					}

					if (this->m_p_storage->isXRCamperSniper())
					{
						if (Globals::get_time_global() - this->m_p_storage->getXRCamperMemoryEnemy() < this->m_p_storage->getXRCamperPostEnemyWait())
						{
							std::pair<Fvector, CScriptGameObject* const> position(Fvector(), nullptr);
							if (Globals::is_vector_nil(this->m_enemy_position) == false)
							{
								position.first = this->m_enemy_position;
							}

							if (this->m_p_storage->getXRCamperSuggestedStates().at("campering").empty() == false)
							{
								Globals::set_state(this->m_object, this->m_p_storage->getXRCamperSuggestedStates().at("campering"), StateManagerCallbackData(), 0, position, StateManagerExtraData());
							}
							else
							{
								Globals::set_state(this->m_object, "hide_na", StateManagerCallbackData(), 0, position, StateManagerExtraData());
							}
						}
						else
						{
							this->scan(Globals::kUnsignedInt32Undefined);
						}
					}
					else
					{
						if (this->on_place())
						{
							std::pair<Fvector, CScriptGameObject* const> position(Fvector(), nullptr);

							if (Globals::is_vector_nil(this->m_enemy_position) == false)
							{
								position.first = this->m_enemy_position;
							}

							if (this->m_p_storage->getXRCamperSuggestedStates().at("campering").empty() == false)
							{
								Globals::set_state(this->m_object, this->m_p_storage->getXRCamperSuggestedStates().at("campering"), StateManagerCallbackData(), 0, position, StateManagerExtraData());
							}
							else
							{
								Globals::set_state(this->m_object, "hide", StateManagerCallbackData(), 0, position, StateManagerExtraData());
							}
						}
						else
						{
							this->m_p_move_manager->Continue();
							this->m_p_move_manager->update();
						}
					}
				}

				return;
			}


			bool is_danger = this->process_danger();

			if (is_danger)
			{
				this->m_is_danger = true;
				return;
			}

			if (this->m_is_danger)
			{
				this->m_is_danger = false;
				this->m_p_move_manager->Continue();
			}

			if (this->m_p_storage->isXRCamperSniper())
			{
				if (this->on_place())
				{
					if (this->m_scantime == 0)
						this->m_scantime = Globals::get_time_global();
					
					this->scan(this->m_p_storage->getXRCamperWpFlag());

					std::uint32_t temp;
					if (this->m_p_move_manager->standing_on_terminal_waypoint(temp))
						return;
					
					if (this->m_scantime && (Globals::get_time_global() - this->m_scantime >= this->m_p_storage->getXRCamperScanTimeFree()))
						this->m_p_move_manager->Continue();
				}
				else
				{
					this->m_scantime = 0;
					this->m_p_move_manager->update();
				}
			}
			else
			{
				this->m_p_move_manager->update();
			}
		}

		void Script_SchemeXRCamper::finalize(void)
		{
			this->m_p_move_manager->finalize(nullptr);
			CScriptActionBase::finalize();
		}

		void Script_SchemeXRCamper::activate_scheme(const bool is_loading, CScriptGameObject* const p_client_object)
		{
			this->reset_scheme(false, nullptr);
		}

		void Script_SchemeXRCamper::reset_scheme(const bool value, CScriptGameObject* const p_client_object)
		{
			Globals::set_state(this->m_object, "patrol", StateManagerCallbackData(), 0, std::pair<Fvector, CScriptGameObject* const>(Fvector(), nullptr), StateManagerExtraData());

			this->m_p_storage->ClearSignals();
			this->m_p_storage->ClearXRCamperScanTable();

			if (this->m_p_storage->isXRCamperSniper())
			{
				std::function<bool(std::uint32_t, std::uint32_t)> binded_function = std::bind(&Script_SchemeXRCamper::process_point, this, std::placeholders::_1, std::placeholders::_2);
				this->m_p_move_manager->reset(this->m_p_storage->getPathWalkName(), Globals::Utils::path_parse_waypoints(this->m_p_storage->getPathWalkName()), "", CondlistWaypoints(), "", this->m_p_storage->getXRCamperSuggestedStates(), binded_function, false);
				
				CPatrolPathParams path = CPatrolPathParams(this->m_p_storage->getPathLookName().c_str());

				if (path.m_path)
				{
					for (std::uint32_t k = 0; k < path.count(); ++k)
					{
						for (std::uint32_t i = 0; i <= 31; ++i)
						{
							if (path.flag(k, i))
							{
								this->m_p_storage->setXRCamperScanTable(i, k, path.point(k));
							}
						}
					}

					if (this->m_object->sniper_update_rate())
					{
						this->m_object->sniper_update_rate(true);
					}
				}
			}
			else
			{
				std::function<bool(std::uint32_t, std::uint32_t)> binded_function = std::bind(&Script_SchemeXRCamper::process_point, this, std::placeholders::_1, std::placeholders::_2);
				this->m_p_move_manager->reset(this->m_p_storage->getPathWalkName(), Globals::Utils::path_parse_waypoints(this->m_p_storage->getPathWalkName()), this->m_p_storage->getPathLookName(), Globals::Utils::path_parse_waypoints(this->m_p_storage->getPathLookName()), "", this->m_p_storage->getXRCamperSuggestedStates(), binded_function, false);

				if (this->m_object->sniper_update_rate())
					this->m_object->sniper_update_rate(false);
			}

			this->m_p_storage->setXRCamperLastLookPoint(0, Fvector());
			this->m_p_storage->setXRCamperCurrentLookPoint(0);
			this->m_p_storage->setXRCamperScanBegin(0);
		}

		bool Script_SchemeXRCamper::process_danger(void)
		{
			if (XR_DANGER::is_danger(this->m_object))
				return false;
			
			const CDangerObject*  p_best_danger = this->m_object->GetBestDanger();

			if (p_best_danger == nullptr)
				return false;

			const CEntityAlive* p_best_danger_object = p_best_danger->object();
			CDangerObject::EDangerType type = p_best_danger->type();
			std::uint32_t passed_time = Globals::get_time_global() - p_best_danger->time();
			type = p_best_danger->type();

			std::pair<Fvector, CScriptGameObject* const> posti(p_best_danger->position(), nullptr);

			if (!this->m_is_danger)
			{
				this->m_object->play_sound(StalkerSpace::eStalkerSoundAlarm, 1, 0, 1, 0);
			}


			bool is_urgent_danger = !!(p_best_danger_object) && (type == CDangerObject::EDangerType::eDangerTypeAttacked) && (Globals::get_time_global() - p_best_danger->time() < 5000);

			if (is_urgent_danger)
			{
				if (!p_best_danger_object)
					MESSAGEW("set empty Fvector for position, because p_best_danger_object == nullptr!");
				
				Fvector danger_poz = p_best_danger_object ? p_best_danger_object->Position() : Fvector();
				
				std::pair<Fvector, CScriptGameObject* const> danger_object_position(danger_poz, nullptr);

				if (this->m_p_storage->getXRCamperSuggestedStates().at("campering_fire").empty() == false)
				{
					Globals::set_state(this->m_object, this->m_p_storage->getXRCamperSuggestedStates().at("campering_fire"), StateManagerCallbackData(), 0, danger_object_position, StateManagerExtraData());
				}
				else
				{
					Globals::set_state(this->m_object, "hide_fire", StateManagerCallbackData(), 0, posti, StateManagerExtraData());
				}
			}
			else
			{
				if (this->m_p_storage->getXRCamperSuggestedStates().at("campering").empty() == false)
				{
					Globals::set_state(this->m_object, this->m_p_storage->getXRCamperSuggestedStates().at("campering"), StateManagerCallbackData(), 0, posti, StateManagerExtraData());
				}
				else
				{
					if (this->m_p_storage->isXRCamperSniper())
					{
						Globals::set_state(this->m_object, "hide_na", StateManagerCallbackData(), 0, posti, StateManagerExtraData());
					}
					else
					{
						Globals::set_state(this->m_object, "hide", StateManagerCallbackData(), 0, posti, StateManagerExtraData());
					}
				}
			}

			return true;
		}

		bool Script_SchemeXRCamper::on_place(void)
		{
			if (this->m_p_storage->isXRCamperNoRetreat())
				return false;

			CPatrolPathParams path = CPatrolPathParams(this->m_p_storage->getPathWalkName().c_str());

			if (path.m_path)
			{
				for (std::uint32_t k = 0; k < path.count(); ++k)
				{
					if (Globals::Utils::is_stalker_at_waypoint(this->m_object, CPatrolPathParams(this->m_p_storage->getPathWalkName().c_str()), k))
					{
						for (std::uint32_t i = 0; i <= 31; ++i)
						{
							if (path.flag(k, i))
							{
								this->m_p_storage->setXRCamperWpFlag(i);
								return true;
							}
						}

						this->m_p_storage->setXRCamperWpFlag(0);
						return false;
					}
				}
			}

			this->m_p_storage->setXRCamperWpFlag(0);
			return false;
		}

		bool Script_SchemeXRCamper::can_shoot(void)
		{
			if (this->m_p_storage->getXRCamperShootName() == "always")
				return true;
			
			if (this->m_p_storage->getXRCamperShootName() == "none")
				return true;

			if (this->m_p_storage->getXRCamperShootName() == "terminal")
			{
				std::uint32_t temp;
				if (this->m_p_move_manager->standing_on_terminal_waypoint(temp))
					return true;
				else
					return false;
			}

			MESSAGEW("unrecognized state check your passed aargument to this function %s", this->m_p_storage->getXRCamperShootName().c_str());
			return false;
		}

		std::pair<std::uint32_t, Fvector> Script_SchemeXRCamper::get_next_point(const std::uint32_t flag)
		{
			bool is_next = false;

			std::pair<std::uint32_t, Fvector> result;

			if (this->m_p_storage->getXRCamperScanTable().find(flag) == this->m_p_storage->getXRCamperScanTable().end())
			{
				MESSAGEWR("can't find data in scan table by index %d", flag);
				return result;
			}

			if (this->m_p_storage->getXRCamperLastLookPoint().first == 0 && Globals::is_vector_nil(this->m_p_storage->getXRCamperLastLookPoint().second))
			{
				auto sorting_function = [](const std::pair<std::uint32_t, Fvector>& one, const std::pair<std::uint32_t, Fvector>& two) -> bool {
					return (one.first < two.first);
				};

				xr_vector<std::pair<std::uint32_t, Fvector>> un_vector = const_cast<xr_vector<std::pair<std::uint32_t, Fvector>>&>(this->m_p_storage->getXRCamperScanTable().at(flag));
				std::sort(un_vector.begin(), un_vector.end(), sorting_function);
			}

			for (const std::pair<std::uint32_t, Fvector>& it : this->m_p_storage->getXRCamperScanTable().at(flag))
			{
				if (this->m_p_storage->getXRCamperLastLookPoint().first == 0 && Globals::is_vector_nil(this->m_p_storage->getXRCamperLastLookPoint().second))
				{
					return it;
				}

				if (is_next)
					return it;

				if (this->m_p_storage->getXRCamperLastLookPoint().first == it.first)
					is_next = true;
			}

			if (is_next)
			{
				if (this->m_p_storage->getXRCamperLastLookPoint().first == 0)
				{
					auto sorting_function = [](const std::pair<std::uint32_t, Fvector>& one, const std::pair<std::uint32_t, Fvector>& two) -> bool {
						return one.first < two.first;
					};

					xr_vector<std::pair<std::uint32_t, Fvector>> un_vector = const_cast<xr_vector<std::pair<std::uint32_t, Fvector>>&>(this->m_p_storage->getXRCamperScanTable().at(flag));
					std::sort(un_vector.begin(), un_vector.end(), sorting_function);
				}
				else
				{
					auto sorting_function = [](const std::pair<std::uint32_t, Fvector>& one, const std::pair<std::uint32_t, Fvector>& two) -> bool {
						return one.first > two.first;
					};

					xr_vector<std::pair<std::uint32_t, Fvector>> un_vector = const_cast<xr_vector<std::pair<std::uint32_t, Fvector>>&>(this->m_p_storage->getXRCamperScanTable().at(flag));
					std::sort(un_vector.begin(), un_vector.end(), sorting_function);
				}
			}

			return this->m_p_storage->getXRCamperLastLookPoint();
		}

		void Script_SchemeXRCamper::scan(const std::uint32_t flag)
		{
			if (this->m_p_storage->getXRCamperScanTable().find(flag) == this->m_p_storage->getXRCamperScanTable().end())
				return;

			if (this->m_flag)
			{
				this->m_flag = flag;
				this->m_p_storage->setXRCamperScanBegin(0);
				this->m_p_storage->setXRCamperCurrentLookPoint(0);
				this->m_p_storage->setXRCamperLastLookPoint(0, Fvector());
			}

			if (this->m_p_storage->getXRCamperScanBegin() == 0 || (Globals::get_time_global() - this->m_p_storage->getXRCamperScanBegin() > this->m_p_storage->getXRCamperTimeScanDelta()))
			{
				this->m_next_point = this->get_next_point(flag);

				if (!this->m_p_storage->getXRCamperCurrentLookPoint())
				{
					this->m_p_storage->setXRCamperCurrentLookPoint(1);
				}

				if (!this->m_p_storage->getXRCamperLastLookPoint().first && Globals::is_vector_nil(this->m_p_storage->getXRCamperLastLookPoint().second))
				{
					this->m_p_storage->setXRCamperLastLookPoint(this->m_next_point.first, this->m_next_point.second);
				}

				this->m_look_position = this->m_p_storage->getXRCamperLastLookPoint().second;
				this->m_dest_position = this->m_next_point.second;
				this->m_look_point.x = this->m_look_position.x + this->m_p_storage->getXRCamperCurrentLookPoint() * ((this->m_dest_position.x - this->m_look_position.x) / this->m_p_storage->getXRCamperScanDelta());
				this->m_look_point.y = this->m_look_position.y + this->m_p_storage->getXRCamperCurrentLookPoint() * ((this->m_dest_position.y - this->m_look_position.y) / this->m_p_storage->getXRCamperScanDelta());
				this->m_look_point.z = this->m_look_position.z + this->m_p_storage->getXRCamperCurrentLookPoint() * ((this->m_dest_position.z - this->m_look_position.z) / this->m_p_storage->getXRCamperScanDelta());

				if (this->m_p_storage->getXRCamperSuggestedStates().at("campering").empty() == false)
				{
					Globals::set_state(this->m_object, this->m_p_storage->getXRCamperSuggestedStates().at("campering"), StateManagerCallbackData(), 0, std::pair<Fvector, CScriptGameObject* const>(this->m_look_point, nullptr), StateManagerExtraData());
				}
				else
				{
					Globals::set_state(this->m_object, "hide_na", StateManagerCallbackData(), 0, std::pair<Fvector, CScriptGameObject* const>(this->m_look_point, nullptr), StateManagerExtraData());
				}

				if (this->m_p_storage->getXRCamperCurrentLookPoint() >= static_cast<std::uint32_t>(this->m_p_storage->getXRCamperScanDelta()))
				{
					this->m_p_storage->setXRCamperCurrentLookPoint(0);
					this->m_p_storage->setXRCamperLastLookPoint(0, Fvector());
				}
				else
				{
					if (this->m_p_storage->getXRCamperScanBegin())
					{
						this->m_p_storage->setXRCamperCurrentLookPoint(this->m_p_storage->getXRCamperCurrentLookPoint() + ((Globals::get_time_global() - this->m_p_storage->getXRCamperScanBegin()) / this->m_p_storage->getXRCamperScanDelta()));
					}
					else
					{
						this->m_p_storage->setXRCamperCurrentLookPoint(this->m_p_storage->getXRCamperCurrentLookPoint() + 1);
					}
				}

				this->m_p_storage->setXRCamperScanBegin(Globals::get_time_global());
			}
			
		}
	}
}