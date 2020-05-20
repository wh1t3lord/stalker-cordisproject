#include "stdafx.h"
#include "Script_SchemeXRWounded.h"

// @ For a tuple
constexpr std::uint32_t _kGetDistance = 0;
constexpr std::uint32_t _kGetStateCondlist = 1;
constexpr std::uint32_t _kGetSoundCondlist = 2;

namespace Cordis
{
	namespace Scripts
	{
		Script_SchemeXRWounded::~Script_SchemeXRWounded(void)
		{
		}

		void Script_SchemeXRWounded::initialize(void)
		{
			CScriptActionBase::initialize();
			this->m_object->set_desired_position();
			this->m_object->set_desired_direction();

			if (this->m_p_storage->getXRWoundedHelpStartDialogName().empty() == false)
				this->m_object->SetStartDialog(this->m_p_storage->getXRWoundedHelpStartDialogName().c_str());

			this->m_object->enable_movement(false);
			this->m_object->DisableTrade();
			this->m_object->wounded(true);
		}

		void Script_SchemeXRWounded::execute(void)
		{
			CScriptActionBase::execute();

			xr_string wounded_victim_name = XR_LOGIC::pstor_retrieve_string(this->m_object, "wounded_victim");

			CScriptGameObject* p_victim = nullptr;
			if (wounded_victim_name.empty())
			{
				p_victim = nullptr;
			}
			else
			{
				p_victim = Globals::get_story_object(wounded_victim_name);
			}

			if (this->m_p_storage->isXRWoundedAutoHeal())
			{
				if (this->m_p_storage->getWoundedManager()->isCanUseMedkit() == false)
				{
					std::uint32_t begin_wounded = XR_LOGIC::pstor_retrieve_number(this->m_object, "begin_wounded");
					std::uint32_t current_time = Globals::get_time_global();

					if (begin_wounded == 0)
					{
						XR_LOGIC::pstor_store(this->m_object, "begin_wounded", current_time);
					}
					else if (current_time - begin_wounded > 60000)
					{
						Globals::Game::alife_create("medkit_script", this->m_object->Position(), this->m_object->level_vertex_id(), this->m_object->game_vertex_id(), this->m_object->ID());
						this->m_p_storage->getWoundedManager()->unlock_medkit();
					}
				}
			}

			xr_string wounded_state_name = XR_LOGIC::pstor_retrieve_string(this->m_object, "wounded_state");
			xr_string wounded_sound_name = XR_LOGIC::pstor_retrieve_string(this->m_object, "wounded_sound");

			if (wounded_state_name == Globals::kStringTrue)
			{
				CScriptHit hit;
				hit.m_fPower = 0.0f;
				hit.m_tDirection = this->m_object->Direction();
				hit.m_caBoneName = "bip01_spine";
				hit.m_tpDraftsman = DataBase::Storage::getInstance().getActor();
				hit.m_fImpulse = 0.0f;
				hit.m_tHitType = ALife::eHitTypeWound;
				this->m_object->Hit(&hit);
			}
			else
			{
				if (this->m_p_storage->isXRWoundedUseMedkit())
					this->m_p_storage->getWoundedManager()->eat_medkit();

				if (wounded_state_name.empty() == false)
				{
					R_ASSERT2(false, "wrong wounded animation check your wounded manager and data (hp_state and hp_state_see)");
					return;
				}

				Globals::set_state(this->m_object, wounded_state_name, StateManagerCallbackData(), 0, std::pair<Fvector, CScriptGameObject* const>(Fvector(), p_victim), StateManagerExtraData());
			}

			if (wounded_sound_name.empty())
			{
				XR_SOUND::set_sound_play(this->m_object->ID(), "", xr_string(), 0);
			}
			else
			{
				XR_SOUND::set_sound_play(this->m_object->ID(), wounded_sound_name, xr_string(), 0);
			}


		}

		void Script_SchemeXRWounded::finalize(void)
		{
			CScriptActionBase::finalize();
			this->m_object->EnableTrade();
			this->m_object->DisableTalk();
			this->m_object->wounded(false);
			this->m_object->enable_movement(true);
		}

		void Script_SchemeXRWounded::add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini, const xr_string& scheme_name, const xr_string& section_name, DataBase::Storage_Scheme& storage)
		{
			if (p_client_object == nullptr)
			{
				MESSAGEWR("p_client_object == nullptr!");
				return;
			}

			xr_map<xr_string, std::uint32_t> operators;
			xr_map<xr_string, std::uint32_t> properties;

			properties["wounded"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kSidorWoundedBase;
			properties["can_fight"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kSidorWoundedBase + 1;
			operators["wounded"] = Globals::XR_ACTIONS_ID::kSidorActWoundedBase;

			CScriptActionPlanner* const p_planner = Globals::get_script_action_planner(p_client_object);
			p_planner->add_evaluator(properties.at("wounded"), new Script_EvaluatorWound("wounded", storage));
			p_planner->add_evaluator(properties.at("can_fight"), new Script_EvaluatorCanFight("can_fight", storage));

			Script_SchemeXRWounded* p_scheme = new Script_SchemeXRWounded("wounded_action", storage);
			p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAlive, true));
			p_scheme->add_condition(CWorldProperty(properties.at("wounded"), true));
			p_scheme->add_effect(CWorldProperty(properties.at("wounded"), false));
			p_scheme->add_effect(CWorldProperty(StalkerDecisionSpace::eWorldPropertyEnemy, false));
			p_scheme->add_effect(CWorldProperty(properties.at("can_fight"), true));
			
			p_planner->add_operator(operators.at("wounded"), p_scheme);
			p_planner->action(Globals::XR_ACTIONS_ID::kAlife).add_condition(CWorldProperty(properties.at("wounded"), false));
			p_planner->action(StalkerDecisionSpace::eWorldOperatorGatherItems).add_condition(CWorldProperty(properties.at("wounded"), false));
			p_planner->action(StalkerDecisionSpace::eWorldOperatorCombatPlanner).add_condition(CWorldProperty(properties.at("can_fight"), true));
			p_planner->action(StalkerDecisionSpace::eWorldOperatorDangerPlanner).add_condition(CWorldProperty(properties.at("can_fight"), true));
			p_planner->action(StalkerDecisionSpace::eWorldOperatorAnomalyPlanner).add_condition(CWorldProperty(properties.at("can_fight"), true));
		}

		void Script_SchemeXRWounded::set_wounded(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini, const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
		{
			DataBase::Storage_Scheme* p_storage = XR_LOGIC::assign_storage_and_bind(p_client_object, p_ini, scheme_name, section_name, "");
			p_storage->setWoundedManager(new Script_WoundedManager(p_client_object, *p_storage));
		}

		void Script_SchemeXRWounded::reset_wounded(CScriptGameObject* const p_client_object, const xr_string& scheme_name, const DataBase::Storage_Data& storage, const xr_string& section_name)
		{
			xr_string wounded_section_name;
			if (scheme_name.empty())
				wounded_section_name = Globals::Utils::cfg_get_string(storage.getIni(), storage.getSectionLogicName(), "wounded");
			else
				wounded_section_name = Globals::Utils::cfg_get_string(storage.getIni(), section_name, "wounded");

			init_wounded(p_client_object, storage.getIni(), wounded_section_name, storage.getSchemes().at("wounded"), scheme_name);
			storage.getSchemes().at("wounded")->getWoundedManager()->hit_callback();
		}

		void Script_SchemeXRWounded::init_wounded(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini, const xr_string& section_name, DataBase::Storage_Scheme* p_storage, const xr_string& scheme_name)
		{
			if (section_name == p_storage->getXRWoundedWoundedSectionName() && (section_name.empty() == false))
				return;

			if (!p_client_object)
			{
				R_ASSERT2(false, "can'be");
				return;
			}

			p_storage->setXRWoundedWoundedSectionName(section_name);
			xr_map<xr_string, xr_string> defaults;
			bool is_default_use_medkit = false;
			bool is_default_enable_talk = true;
			bool is_default_not_for_help = true;

			std::uint32_t picked_index = p_client_object->ID() % 3;
			if (picked_index > Script_GlobalHelper::getInstance().getXRWoundedStates().size() - 1)
			{
				R_ASSERT2(false, "bad index, it can't be check your data");
				return;
			}

			xr_string community_name = Globals::character_community(p_client_object);

			if (community_name == "monolith")
			{
				xr_string state_name = Script_GlobalHelper::getInstance().getXRWoundedStates().at(picked_index);
				defaults["hp_state"].append("20|");
				defaults["hp_state"].append(state_name);
				defaults["hp_state"].append("@nil");
				defaults["hp_state_see"].append("20|");
				defaults["hp_state_see"].append(state_name);
				defaults["hp_state_see"].append("@nil");
				defaults["psy_state"].clear();
				defaults["hp_victim"].append("20|nil");
				defaults["hp_cover"].append("20|false");
				defaults["hp_fight"].append("20|false");
				defaults["help_dialog"].clear();
				defaults["help_start_dialog"].clear();
				is_default_enable_talk = true;
				is_default_not_for_help = true;
				is_default_use_medkit = false;
			}
			else if (community_name == "zombied")
			{
				defaults["hp_state"].append("40|wounded_zombie@nil");
				defaults["hp_state_see"].append("40|wounded_zombie@nil");
				defaults["psy_state"].clear();
				defaults["hp_victim"].append("40|nil");
				defaults["hp_cover"].append("40|false");
				defaults["hp_fight"].append("40|false");
                defaults["help_dialog"].clear();
                defaults["help_start_dialog"].clear();
                is_default_use_medkit = false;
                is_default_enable_talk = true;
                is_default_not_for_help = true;
			}
            else
            {
                xr_string state_name = Script_GlobalHelper::getInstance().getXRWoundedStates().at(picked_index);
                defaults["hp_state"].append("20|");
                defaults["hp_state"].append(state_name);
                defaults["hp_state"].append("@help_heavy");
                defaults["hp_state_see"].append("20|");
                defaults["hp_state_see"].append(state_name);
                defaults["hp_state_see"].append("@help_heavy");
                defaults["psy_state"].append("20|{=best_pistol}psy_armed,psy_pain@wounded_psy|20|{=best_pistol}psy_shoot,psy_pain@{=best_pistol}wounded_psy_shoot,wounded_psy");
                defaults["hp_victim"].append("20|nil");
                defaults["hp_fight"].append("20|false");
                defaults["help_dialog"].append("dm_help_wounded_medkit_dialog");
                defaults["help_start_dialog"].clear();
                defaults["hp_cover"].clear();
                is_default_use_medkit = true;
                is_default_enable_talk = true;
                is_default_not_for_help = false;
            }

            if (section_name.empty())
            {
                p_storage->setXRWoundedHealthState(Globals::Utils::parse_data(defaults.at("hp_state")));
                p_storage->setXRWoundedHealthStateSee(Globals::Utils::parse_data(defaults.at("hp_state_see")));
                p_storage->setXRWoundedPsyState(Globals::Utils::parse_data(defaults.at("psy_state")));
                p_storage->setXRWoundedHealthVictim(Globals::Utils::parse_data(defaults.at("hp_victim")));
                p_storage->setXRWoundedHealthCover(Globals::Utils::parse_data(defaults.at("hp_cover")));
                p_storage->setXRWoundedHealthFight(Globals::Utils::parse_data(defaults.at("hp_fight")));
                p_storage->setXRWoundedHelpDialogName(defaults.at("help_dialog"));
                p_storage->setXRWoundedHelpStartDialogName("");
                p_storage->setXRWoundedUseMedkit(is_default_use_medkit);
                p_storage->setXRWoundedAutoHeal(true);
                p_storage->setXRWoundedEnableTalk(true);
                p_storage->setXRWoundedNotForHelp(is_default_not_for_help);
            }
            else
            {
                xr_string hp_state_name = Globals::Utils::cfg_get_string(p_ini, section_name, "hp_state");
                if (hp_state_name.empty())
                    hp_state_name = defaults.at("hp_state");
                
                p_storage->setXRWoundedHealthState(Globals::Utils::parse_data(hp_state_name));

                xr_string hp_state_see_name = Globals::Utils::cfg_get_string(p_ini, section_name, "hp_state_see");
                if (hp_state_see_name.empty())
                    hp_state_see_name = defaults.at("hp_state_see");

                p_storage->setXRWoundedHealthStateSee(Globals::Utils::parse_data(hp_state_see_name));

                xr_string psy_state_name = Globals::Utils::cfg_get_string(p_ini, section_name, "psy_state");
                if (psy_state_name.empty())
                    psy_state_name = defaults.at("psy_state");

                p_storage->setXRWoundedPsyState(Globals::Utils::parse_data(psy_state_name));

                xr_string hp_victim_name = Globals::Utils::cfg_get_string(p_ini, section_name, "hp_victim");
                if (hp_victim_name.empty())
                    hp_victim_name = defaults.at("hp_victim");

                p_storage->setXRWoundedHealthFight(Globals::Utils::parse_data(hp_victim_name));

                xr_string hp_cover_name = Globals::Utils::cfg_get_string(p_ini, section_name, "hp_cover");
                if (hp_cover_name.empty())
                    hp_cover_name = defaults.at("hp_cover");

                p_storage->setXRWoundedHealthCover(Globals::Utils::parse_data(hp_cover_name));

                xr_string hp_fight_name = Globals::Utils::cfg_get_string(p_ini, section_name, "hp_fight");
                if (hp_fight_name.empty())
                    hp_fight_name = defaults.at("hp_fight");

                p_storage->setXRWoundedHealthFight(Globals::Utils::parse_data(hp_fight_name));

                xr_string help_dialog_name = Globals::Utils::cfg_get_string(p_ini, section_name, "help_dialog");
                if (help_dialog_name.empty())
                    help_dialog_name = defaults.at("help_dialog");

                p_storage->setXRWoundedHelpDialogName(help_dialog_name);

                p_storage->setXRWoundedHelpStartDialogName(Globals::Utils::cfg_get_string(p_ini, section_name, "help_start_dialog"));

                p_storage->setXRWoundedUseMedkit(Globals::Utils::cfg_get_bool(p_ini, section_name, "use_medkit"));
                p_storage->setXRWoundedAutoHeal(Globals::Utils::cfg_get_bool(p_ini, section_name, "autoheal"));
                p_storage->setXRWoundedEnableTalk(Globals::Utils::cfg_get_bool(p_ini, section_name, "enable_talk"));
                p_storage->setXRWoundedNotForHelp(Globals::Utils::cfg_get_bool(p_ini, section_name, "not_for_help"));
            }

            p_storage->setXRWoundedSet(true);
		}

		Script_WoundedManager::Script_WoundedManager(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage) : m_is_can_use_medkit(false), m_p_npc(p_client_object), m_p_storage(&storage)
		{
		}

		Script_WoundedManager::~Script_WoundedManager(void)
		{
		}

		void Script_WoundedManager::update(void)
		{
			std::uint32_t health = static_cast<std::uint32_t>(100.0f * this->m_p_npc->GetHealth());
			std::uint32_t psy_health = static_cast<std::uint32_t>(100.0f * this->m_p_npc->GetPsyHealth());

			this->process_psy_wound(psy_health, this->m_state_name, this->m_sound_name);

			if (this->m_state_name.empty() && this->m_sound_name.empty())
			{
				this->process_hp_wound(health, this->m_state_name, this->m_sound_name);
				this->process_fight(health, this->m_fight_name);
				this->process_victim(health, this->m_victim_name);
			}
			else
			{
				this->m_fight_name = Globals::kStringFalse;
				this->m_victim_name.clear();
			}

			XR_LOGIC::pstor_store(this->m_p_npc, "wounded_state", this->m_state_name);
			XR_LOGIC::pstor_store(this->m_p_npc, "wounded_sound", this->m_sound_name);
			XR_LOGIC::pstor_store(this->m_p_npc, "wounded_fight", this->m_fight_name);
			XR_LOGIC::pstor_store(this->m_p_npc, "wounded_victim", this->m_victim_name);
		}

		void Script_WoundedManager::eat_medkit(void)
		{
			if (this->m_is_can_use_medkit)
			{
				CScriptGameObject* const p_script_item = this->m_p_npc->GetObjectByName("medkit_script");
				if (p_script_item)
					this->m_p_npc->eat(p_script_item);

				CScriptGameObject* const p_medkit = this->m_p_npc->GetObjectByName("medkit");
				CScriptGameObject* const p_medkit_army = this->m_p_npc->GetObjectByName("medkit_army");
				CScriptGameObject* const p_medkit_scientic = this->m_p_npc->GetObjectByName("medkit_scientic");

				if (p_medkit)
					Globals::Game::alife_release(ai().alife().objects().object(p_medkit->ID()), true);
				else if (p_medkit_army)
					Globals::Game::alife_release(ai().alife().objects().object(p_medkit_army->ID()), true);
				else if (p_medkit_scientic)
					Globals::Game::alife_release(ai().alife().objects().object(p_medkit_scientic->ID()), true);

				std::uint32_t begin_wounded = XR_LOGIC::pstor_retrieve_number(this->m_p_npc, "begin_wounded");
				std::uint32_t current_time = Globals::get_time_global();

				xr_string temp;
				if (begin_wounded && current_time - begin_wounded <= 60000)
					XR_SOUND::set_sound_play(this->m_p_npc->ID(), "help_thanks", temp, 0);

				XR_LOGIC::pstor_store(this->m_p_npc, "begin_wounded", static_cast<std::uint32_t>(0));
			}

			this->m_is_can_use_medkit = false;
			this->update();
		}

		void Script_WoundedManager::process_fight(const std::uint32_t health, xr_string& result_name)
		{
			std::uint32_t key = this->get_key_from_distance(this->m_p_storage->getXRWoundedHealthFight(), health);

			if (key == 0)
			{
				MESSAGEW("maybe it is not right, check your data in debug mode!");
			}

			if (key == Globals::kUnsignedInt32Undefined)
			{
				MESSAGEWR("Something is not right check your data!");
				return;
			}

			if (this->m_p_storage->getXRWoundedHealthFight().empty())
			{
				result_name = Globals::kStringTrue;
				return;
			}

			const xr_map<std::uint32_t, CondlistData>& condlist_state = std::get<_kGetStateCondlist>(this->m_p_storage->getXRWoundedHealthFight().at(key));

			if (condlist_state.empty() == false)
				result_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, condlist_state);
			else
				result_name = Globals::kStringTrue;
		}

		void Script_WoundedManager::process_victim(const std::uint32_t health, xr_string& result_name)
		{
			std::uint32_t key = this->get_key_from_distance(this->m_p_storage->getXRWoundedHealthVictim(), health);

			if (key == 0)
			{
				MESSAGEW("maybe it is not right, check your data in debug mode!");
			}

			if (key == Globals::kUnsignedInt32Undefined)
			{
				MESSAGEWR("Something is not right check your data!");
				return;
			}

			const xr_map<std::uint32_t, CondlistData>& condlist_state = std::get<_kGetStateCondlist>(this->m_p_storage->getXRWoundedHealthVictim().at(key));

			if (condlist_state.empty() == false)
				result_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, condlist_state);
			else
				result_name.clear();
		}

		void Script_WoundedManager::process_hp_wound(const std::uint32_t health, xr_string& state_name, xr_string& sound_name)
		{
			std::uint32_t key = this->get_key_from_distance(this->m_p_storage->getXRWoundedHealthState(), health);

			if (key == 0)
			{
				MESSAGEW("maybe it is not right! Check data in debug mode!");
			}

			if (key == Globals::kUnsignedInt32Undefined)
			{
				MESSAGEWR("Something is not right check your data!");
				return;
			}

			if (this->m_p_npc->CheckObjectVisibility(DataBase::Storage::getInstance().getActor()))
			{
				const xr_map<std::uint32_t, CondlistData>& condlist_state = std::get<_kGetStateCondlist>(this->m_p_storage->getXRWoundedHealthStateSee().at(key));
				const xr_map<std::uint32_t, CondlistData>& condlist_sound = std::get<_kGetSoundCondlist>(this->m_p_storage->getXRWoundedHealthStateSee().at(key));

				if (condlist_state.empty() == false)
					state_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, condlist_state);
				else
					state_name.clear();

				if (condlist_sound.empty() == false)
					sound_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, condlist_sound);
				else
					sound_name.clear();
			}
			else
			{
				const xr_map<std::uint32_t, CondlistData>& condlist_state = std::get<_kGetStateCondlist>(this->m_p_storage->getXRWoundedHealthState().at(key));
				const xr_map<std::uint32_t, CondlistData>& condlist_sound = std::get<_kGetSoundCondlist>(this->m_p_storage->getXRWoundedHealthState().at(key));

				if (condlist_state.empty() == false)
					state_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, condlist_state);
				else
					state_name.clear();

				if (condlist_sound.empty() == false)
					sound_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, condlist_sound);
				else
					sound_name.clear();
			}
		}

		void Script_WoundedManager::process_psy_wound(const std::uint32_t health, xr_string& state_name, xr_string& sound_name)
		{
			std::uint32_t key = this->get_key_from_distance(this->m_p_storage->getXRWoundedPsyState(), health);
			if (key == 0)
			{
				MESSAGEW("maybe it is not right! Check data in debug mode!");
			}

			if (key == Globals::kUnsignedInt32Undefined)
			{
				MESSAGEWR("Something is not right check your data!");
				return;
			}

			const xr_map<std::uint32_t, CondlistData>& state_condlist = std::get<_kGetStateCondlist>(this->m_p_storage->getXRWoundedPsyState().at(key));
			const xr_map<std::uint32_t, CondlistData>& sound_condlist = std::get<_kGetSoundCondlist>(this->m_p_storage->getXRWoundedPsyState().at(key));

			if (state_condlist.empty() == false)
				state_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, state_condlist);
			else
				state_name.clear();

			if (sound_condlist.empty() == false)
				sound_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, sound_condlist);
			else
				sound_name.clear();
		}

		void Script_WoundedManager::hit_callback(void)
		{
			if (this->m_p_npc->Alive() == false)
				return;

			if (this->m_p_npc->critically_wounded())
				return;

			this->update();
		}

		std::uint32_t Script_WoundedManager::get_key_from_distance(const xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& data, const std::uint32_t health)
		{
			std::uint32_t key = 0;

			for (const std::pair<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& it : data)
			{
				if (std::get<_kGetDistance>(it.second) >= health)
				{
					key = it.first;
				}
				else
				{
					return key;
				}
			}

			return key;
		}

		Script_EvaluatorWound::~Script_EvaluatorWound(void)
		{
		}

		Script_EvaluatorWound::_value_type Script_EvaluatorWound::evaluate(void)
		{
			if (this->m_object->in_smart_cover())
				return false;

			if (this->m_p_storage->isXRWoundedSet() == false)
				return false;

			this->m_p_storage->getWoundedManager()->update();
			
			CScriptActionPlanner* const p_planner = Globals::get_script_action_planner(this->m_object);

			if (this->m_object->critically_wounded())
				return false;

			if (p_planner->evaluator(StalkerDecisionSpace::eWorldPropertyEnemy).evaluate() && (XR_LOGIC::pstor_retrieve_string(this->m_object, "wounded_fight") == Globals::kStringTrue))
				return false;

			// Lord: проверить ибо не хочется использовать "nil" как в оригинале
			return (XR_LOGIC::pstor_retrieve_string(this->m_object, "wounded_state").empty() == false);
		}

		Script_EvaluatorCanFight::~Script_EvaluatorCanFight(void)
		{
		}

		Script_EvaluatorCanFight::_value_type Script_EvaluatorCanFight::evaluate(void)
		{
			if (this->m_object->critically_wounded())
				return true;

			return (XR_LOGIC::pstor_retrieve_string(this->m_object, "wounded_fight") != Globals::kStringFalse);
		}
}
}
