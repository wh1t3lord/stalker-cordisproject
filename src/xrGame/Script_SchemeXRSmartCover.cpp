#include "stdafx.h"
#include "Script_SchemeXRSmartCover.h"

namespace Cordis
{
	namespace Scripts
	{
		Script_EvaluatorUseSmartCoverInCombat::_value_type Script_EvaluatorUseSmartCoverInCombat::evaluate(void)
		{
			if (XR_LOGIC::is_active(this->m_object, *this->m_p_storage))
				return this->m_p_storage->isXRSmartCoverUseInCombat();

			return false;
		}

		Script_EvaluatorNeedSmartCover::_value_type Script_EvaluatorNeedSmartCover::evaluate(void)
		{
			return XR_LOGIC::is_active(this->m_object, *this->m_p_storage);
		}


		Script_SchemeXRSmartCover::Script_SchemeXRSmartCover(const xr_string& name, DataBase::Storage_Scheme& storage) : Script_ISchemeStalker(nullptr, name, storage), m_target_enemy_id(0), m_is_initialized(false)
		{
		}

		Script_SchemeXRSmartCover::~Script_SchemeXRSmartCover(void)
		{
		}

		void Script_SchemeXRSmartCover::finalize(void)
		{
			this->m_is_initialized = false;
			CScriptActionBase::finalize();
		}

		void Script_SchemeXRSmartCover::initialize(void)
		{
			CScriptActionBase::initialize();
			this->m_is_initialized = true;

			this->activate_scheme(false, nullptr);
		}

		void Script_SchemeXRSmartCover::execute(void)
		{
			CScriptActionBase::execute();
			
			if (DataBase::Storage::getInstance().getStorage().find(this->m_object->ID()) == DataBase::Storage::getInstance().getStorage().end())
			{
				MESSAGEWR("can't find storage for npc %d", this->m_object->ID());
				return;
			}

			const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(this->m_object->ID());

			xr_string need_cover_state_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_object, this->m_condlist_cover);

			if (need_cover_state_name == "default_behaviour" || Script_GlobalHelper::getInstance().getXRSmartCoverCoverSubState().at(this->m_cover_state_name) != Script_GlobalHelper::getInstance().getXRSmartCoverCoverSubState().at(need_cover_state_name))
			{
				this->m_cover_state_name = need_cover_state_name;
			}

			this->check_target_selector();

			if (this->m_target_enemy_id && this->m_object->in_smart_cover())
			{
				if (Globals::Game::level::get_object_by_id(this->m_target_enemy_id) && this->m_object->in_current_loophole_fov(Globals::Game::level::get_object_by_id(this->m_target_enemy_id)->Position()))
				{
					this->m_p_storage->setSignals("enemy_in_fov", true);
					this->m_p_storage->setSignals("enemy_not_in_fov", false);
				}
				else
				{
					this->m_p_storage->setSignals("enemy_in_fov", false);
					this->m_p_storage->setSignals("enemy_not_in_fov", true);
				}
			}


			if (this->m_p_storage->getXRSmartCoverSoundIdleName().empty() == false)
			{
				XR_SOUND::set_sound_play(this->m_object->ID(), this->m_p_storage->getXRSmartCoverSoundIdleName(), xr_string(), 0);
			}
		}

		void Script_SchemeXRSmartCover::activate_scheme(const bool is_loading, CScriptGameObject* const p_client_object)
		{
			this->m_p_storage->ClearSignals();

			if (this->m_is_initialized == false)
				return;

			bool is_used = false;
			this->m_object->set_smart_cover_target();
			this->m_target_enemy_id = 0;
 
			if (this->m_p_storage->getXRSmartCoverCoverName().find(Globals::kTemplateSmartCoverAndAnimpointName) != xr_string::npos)
			{
				xr_string copy_cover_name = this->m_p_storage->getXRSmartCoverCoverName();
				if (DataBase::Storage::getInstance().getScriptIDS().find(this->m_object->ID()) == DataBase::Storage::getInstance().getScriptIDS().end())
				{
					MESSAGEW("Can't find script_id by npc_id %d", this->m_object->ID());
					this->m_cover_name = this->m_p_storage->getXRSmartCoverCoverName();
				}
				else
				{
					const xr_string& script_id_name = DataBase::Storage::getInstance().getScriptIDS().at(this->m_object->ID());
					copy_cover_name.replace(copy_cover_name.find(Globals::kTemplateSmartCoverAndAnimpointName), strlen(Globals::kTemplateSmartCoverAndAnimpointName), script_id_name);
					this->m_cover_name = copy_cover_name;
					is_used = true;
				}

			}

			if (this->m_cover_name != this->m_p_storage->getXRSmartCoverCoverName() || is_used == false)
			{
				if ((DataBase::Storage::getInstance().getGameRegisteredServerSmartCovers().find(this->m_cover_name) == DataBase::Storage::getInstance().getGameRegisteredServerSmartCovers().end()) || (DataBase::Storage::getInstance().getGameRegisteredServerSmartCovers().at(this->m_cover_name) == nullptr))
				{
					MESSAGEWR("There is no smart_cover with name %s");
					return;
				}

				Globals::set_state(this->m_object, "smartcover", StateManagerCallbackData(), 0, std::pair<Fvector, CScriptGameObject* const>(Fvector(), nullptr), StateManagerExtraData());
				this->m_condlist_target_path = XR_LOGIC::parse_condlist_by_script_object(DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getActiveSectionName(), "cover_state", this->m_p_storage->getXRSmartCoverCoverStateName());
				this->m_cover_state_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_object, this->m_condlist_cover);
				this->target_selector(this->m_object);
				this->check_target_selector();

				this->m_object->idle_min_time(this->m_p_storage->getXRSmartCoverIdleMinTime());
				this->m_object->idle_max_time(this->m_p_storage->getXRSmartCoverIdleMaxTime());
				this->m_object->lookout_max_time(this->m_p_storage->getXRSmartCoverLookOutMaxTime());
				this->m_object->lookout_min_time(this->m_p_storage->getXRSmartCoverLookOutMinTime());
			}
		}

		void Script_SchemeXRSmartCover::deactivate(CScriptGameObject* const p_client_object)
		{
			this->m_p_storage->setXRSmartCoverCoverName(xr_string());
			this->m_p_storage->setXRSmartCoverLoopholeName(xr_string());
		}

		void Script_SchemeXRSmartCover::target_selector(CScriptGameObject* const p_client_object)
		{
			if (p_client_object == nullptr)
			{
				MESSAGEWR("invalid object!");
				return;
			}

			if (!p_client_object->Alive())
				return;

			if (this->m_cover_state_name == "idle_target")
			{
				p_client_object->set_smart_cover_target_idle();
			}
			else if (this->m_cover_state_name == "lookout_target")
			{
				this->check_target();
				p_client_object->set_smart_cover_target_lookout();
			}
			else if (this->m_cover_state_name == "fire_target")
			{
				p_client_object->set_smart_cover_target_fire();
			}
			else if (this->m_cover_state_name == "fire_no_lookout_target")
			{
				this->check_target();
				p_client_object->set_smart_cover_target_fire_no_lookout();
			}
			else
			{
				this->check_target();
				p_client_object->set_smart_cover_target_default(true);
			}
		}

		void Script_SchemeXRSmartCover::check_target_selector(void)
		{
			// Lord: проверка на nil нужно свести что пустая строка!ы
			if (this->m_cover_state_name.empty())
			{
				this->m_object->set_smart_cover_target_selector();
			}
			else
			{
				std::function<void(CScriptGameObject* const)> p_callback = std::bind(&Script_SchemeXRSmartCover::target_selector, this, std::placeholders::_1);
				this->m_object->set_smart_cover_target_selector(p_callback);
			}
		}

		bool Script_SchemeXRSmartCover::check_target(void)
		{
			xr_string target_path_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_object, this->m_condlist_target_path);
 

			// Lord: опять же проверка уже на строчный 'nil' 
			if (target_path_name.empty() == false)
			{
				if (target_path_name.find(Globals::kTemplateSmartCoverAndAnimpointName) != xr_string::npos)
				{
					if (DataBase::Storage::getInstance().getScriptIDS().find(this->m_object->ID()) != DataBase::Storage::getInstance().getScriptIDS().end())
					{
						target_path_name.replace(target_path_name.find(Globals::kTemplateSmartCoverAndAnimpointName), strlen(Globals::kTemplateSmartCoverAndAnimpointName), DataBase::Storage::getInstance().getScriptIDS().at(this->m_object->ID()));
						this->m_target_path_name = target_path_name;
					}
				}
				else
				{
					this->m_target_path_name = target_path_name;
				}

				if (Globals::patrol_path_exists(this->m_target_path_name.c_str()))
				{
					CPatrolPathParams params(this->m_target_path_name.c_str());
					this->m_object->set_smart_cover_target(params.point(static_cast<std::uint32_t>(0)));
					this->m_fire_position = params.point(static_cast<std::uint32_t>(0));
					return true;
				}
				else
				{
					MESSAGEW("There is no patrol path %s", this->m_target_path_name.c_str());
				}
			}
			else if (this->m_p_storage->getXRSmartCoverTargetEnemyName().empty() == false)
			{
				CScriptGameObject* const p_client_object = Globals::get_story_object(this->m_p_storage->getXRSmartCoverTargetEnemyName());
				if (p_client_object == nullptr)
				{
					MESSAGEWR("can't find client object by story_id %s", this->m_p_storage->getXRSmartCoverTargetEnemyName().c_str());
					return false;
				}

				this->m_target_enemy_id = p_client_object->ID();
				CScriptGameObject* const p_object = Globals::Game::level::get_object_by_id(this->m_target_enemy_id);
				if (this->m_target_enemy_id && p_object->Alive())
				{
					this->m_object->set_smart_cover_target(p_object);
					this->m_fire_position = Globals::Game::level::get_object_by_id(this->m_target_enemy_id)->Position();
					return true;
				}
			}

			return false;
		}

		bool Script_SchemeXRSmartCover::position_riched(void)
		{
			return this->m_object->in_smart_cover();
		}

		void Script_SchemeXRSmartCover::add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini, const xr_string& scheme_name, const xr_string& section_name, DataBase::Storage_Scheme& storage)
		{
			if (p_client_object == nullptr)
			{
				MESSAGEWR("Invalid object!");
				return;
			}

			if (p_ini == nullptr)
			{
				MESSAGEWR("Invalid ini!");
				return;
			}

			if (scheme_name.empty())
			{
				MESSAGEWR("Invalid scheme name!");
				return;
			}


			MESSAGEI("added scheme %s to binder npc[%d|%s]", scheme_name.c_str(), p_client_object->ID(), p_client_object->Name());

			xr_map<xr_string, std::uint32_t> operators;
			xr_map<xr_string, std::uint32_t> properties; 
			
			CScriptActionPlanner* const p_planner = Globals::get_script_action_planner(p_client_object);

			if (p_planner == nullptr)
			{
				MESSAGEWR("can't obtain cscriptactionplanner from npc %d %s", p_client_object->ID(), p_client_object->Name());
				return;
			}

			properties["need_smartcover"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kSmartCoverAction + 1;
			properties["use_smartcover_in_combat"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kSmartCoverAction + 2;
			properties["state_mgr_logic_active"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kStateManager + 4;

			operators["action_smartcover"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kSmartCoverAction;
			operators["action_combat_smartcover"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kSmartCoverAction + 2;

			p_planner->add_evaluator(properties.at("need_smartcover"), new Script_EvaluatorNeedSmartCover("smartcover_need", storage));
			p_planner->add_evaluator(properties.at("use_smartcover_in_combat"), new Script_EvaluatorUseSmartCoverInCombat("use_smartcover_in_combat", storage));

			Script_SchemeXRSmartCover* const p_scheme = new Script_SchemeXRSmartCover("action_smartcover_activity", storage);
			p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAlive, true));
			p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAnomaly, false));
			p_scheme->add_condition(CWorldProperty(properties.at("need_smartcover"), true));
			p_scheme->add_condition(CWorldProperty(properties.at("use_smartcover_in_combat"), false));
			p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyEnemy, false));

			p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kStoheMeetBase + 1, false));
			p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kSidorWoundedBase, false));
			p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kAbuseBase, false));

			p_scheme->add_effect(CWorldProperty(properties.at("need_smartcover"), false));
			p_scheme->add_effect(CWorldProperty(properties.at("state_mgr_logic_active"), false));

			p_planner->add_operator(operators.at("action_smartcover"), p_scheme);
			DataBase::Storage::getInstance().setStorageSchemesActions(p_client_object->ID(), scheme_name, p_scheme);

			Script_SchemeXRSmartCover* const p_scheme_combat = new Script_SchemeXRSmartCover("action_combat_smartcover", storage);
			p_scheme_combat->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAlive, true));
			p_scheme_combat->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAnomaly, false));
			p_scheme_combat->add_condition(CWorldProperty(properties.at("need_smartcover"), true));
			p_scheme_combat->add_condition(CWorldProperty(properties.at("use_smartcover_in_combat"), true));

			p_scheme_combat->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kStoheMeetBase + 1, false));
			p_scheme_combat->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kSidorWoundedBase, false));
			p_scheme_combat->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kAbuseBase, false));

			p_scheme_combat->add_effect(CWorldProperty(properties.at("need_smartcover"), false));
			p_scheme_combat->add_effect(CWorldProperty(StalkerDecisionSpace::eWorldPropertyEnemy, false));
			p_scheme_combat->add_effect(CWorldProperty(StalkerDecisionSpace::eWorldPropertyDanger, false));
			p_scheme_combat->add_effect(CWorldProperty(properties.at("state_mgr_logic_active"), false));

			p_planner->add_operator(operators.at("action_combat_smartcover"), p_scheme_combat);

			DataBase::Storage::getInstance().setStorageSchemesActions(p_client_object->ID(), scheme_name, p_scheme_combat);
			p_planner->action(Globals::XR_ACTIONS_ID::kAlife).add_condition(CWorldProperty(properties.at("need_smartcover"), false));
			p_planner->action(StalkerDecisionSpace::eWorldOperatorCombatPlanner).add_condition(CWorldProperty(properties.at("use_smartcover_in_combat"), false));
		}

		void Script_SchemeXRSmartCover::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini, const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
		{
			MESSAGEI("allocate storage for scheme %s", scheme_name.c_str());

			DataBase::Storage_Scheme* const p_storage = XR_LOGIC::assign_storage_and_bind(p_client_object, p_ini, scheme_name, section_name, gulag_name);
			p_storage->setLogic(XR_LOGIC::cfg_get_switch_conditions(p_ini, section_name, p_client_object));

			xr_string cover_name = Globals::Utils::cfg_get_string(p_ini, section_name, "cover_name");

			if (cover_name.empty())
				cover_name = "$script_id$_cover";

			p_storage->setXRSmartCoverCoverName(cover_name);
			p_storage->setXRSmartCoverLoopholeName(Globals::Utils::cfg_get_string(p_ini, section_name, "loophole_name"));

			xr_string cover_state_name = Globals::Utils::cfg_get_string(p_ini, section_name, "cover_state");

			if (cover_state_name.empty())
				cover_state_name = "default_behaviour";

			p_storage->setXRSmartCoverCoverStateName(cover_state_name);
			p_storage->setXRSmartCoverTargetEnemyName(Globals::Utils::cfg_get_string(p_ini, section_name, "target_enemy"));
			p_storage->setXRSmartCoverTargetPathName(Globals::Utils::cfg_get_string(p_ini, section_name, "target_path"));

			std::uint32_t idle_min_time = static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(p_ini, section_name, "idle_min_time"));
			if (!idle_min_time)
				idle_min_time = 6;
			p_storage->setXRSmartCoverIdleMinTime(idle_min_time);

			std::uint32_t idle_max_time = static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(p_ini, section_name, "idle_max_time"));
			if (!idle_max_time)
				idle_max_time = 10;

			p_storage->setXRSmartCoverIdleMaxTime(idle_max_time);

			float lookout_min_time = Globals::Utils::cfg_get_number(p_ini, section_name, "lookout_min_time");
			if (fis_zero(lookout_min_time))
				lookout_min_time = 6.0f;
			p_storage->setXRSmartCoverIdleMinTime(lookout_min_time);

			float lookout_max_time = Globals::Utils::cfg_get_number(p_ini, section_name, "lookout_max_time");
			if (fis_zero(lookout_max_time))
				lookout_max_time = 10.0f;

			p_storage->setXRSmartCoverIdleMaxTime(lookout_max_time);

			xr_string exit_body_state_name = Globals::Utils::cfg_get_string(p_ini, section_name, "exit_body_state");
			if (exit_body_state_name.empty())
				exit_body_state_name = "stand";
			p_storage->setXRSmartCoverExitBodyStateName(exit_body_state_name);

			p_storage->setXRSmartCoverUsePreCalcCover(Globals::Utils::cfg_get_bool(p_ini, section_name, "use_precalc_cover"));
			p_storage->setXRSmartCoverUseInCombat(Globals::Utils::cfg_get_bool(p_ini, section_name, "use_in_combat"));
			p_storage->setXRSmartCoverWeaponTypeName(Globals::Utils::cfg_get_string(p_ini, section_name, "weapon_type"));

			xr_string moving_name = Globals::Utils::cfg_get_string(p_ini, section_name, "def_state_moving");
			if (moving_name.empty())
				moving_name = "sneak";
			p_storage->setXRSmartCoverMovingName(moving_name);

			p_storage->setXRSmartCoverSoundIdleName(Globals::Utils::cfg_get_string(p_ini, section_name, "sound_idle"));
		}

	}
}