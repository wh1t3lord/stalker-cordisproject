#include "stdafx.h"
#include "Script_SchemeXRMeet.h"

namespace Cordis
{
	namespace Scripts
	{
		Script_EvaluatorContact::_value_type Script_EvaluatorContact::evaluate(void)
		{
			if (this->m_p_storage->isSet() == false)
				return false;

			if (DataBase::Storage::getInstance().getActor())
			{
				if (!DataBase::Storage::getInstance().getActor()->Alive())
					return false;

				this->m_p_storage->getMeetManager()->update();

				if (CRD_Wounded::is_wounded(this->m_object))
					return false;

				if (this->m_object->GetBestEnemy())
					return false;

				CScriptActionPlanner* const p_planner = Globals::get_script_action_planner(this->m_object);

				if (p_planner->evaluator(StalkerDecisionSpace::eWorldPropertyEnemy).evaluate())
				{
					this->m_p_storage->getMeetManager()->setUseName("false");
					this->m_object->DisableTalk();
					return false;
				}

				return !(this->m_p_storage->getMeetManager()->getCurrentDistanceName().empty());
			}

			return false;
		}

		Script_XRMeetManager::Script_XRMeetManager(CScriptGameObject* const p_client_object, DataBase::Script_ComponentScheme_XRMeet* storage) : m_is_bye_passed(false), m_is_hello_passed(false), m_is_npc_camp_director(false), m_p_storage(storage), m_p_npc(p_client_object), m_is_allow_break(false)
		{
		}

		Script_XRMeetManager::~Script_XRMeetManager(void)
		{
		}

		void Script_XRMeetManager::update_state(void)
		{
			xr_string state_name;
			xr_string victim_name;
			
			if (this->m_current_distance_name == "close")
			{
				state_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getCloseAnim());
				victim_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getCloseVictim());
			}
			else if (this->m_current_distance_name == "far")
			{
				state_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getFarAnim());
				victim_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getFarVictim());
			}

			CScriptGameObject* p_look_object = nullptr;
			if (victim_name.empty() == false)
			{
				p_look_object = Globals::get_story_object(victim_name);
			}

			if (state_name.empty() == false)
			{
				if (p_look_object)
					Globals::set_state(this->m_p_npc, state_name, StateManagerCallbackData(), 0, std::pair<Fvector, CScriptGameObject* const>(Fvector(), nullptr), StateManagerExtraData());
				else
					Globals::set_state(this->m_p_npc, state_name, StateManagerCallbackData(), 0, std::pair<Fvector, CScriptGameObject* const>(Fvector(), p_look_object), StateManagerExtraData());
			}

			xr_string sound_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getFarSound());
			xr_string temp;
			if (sound_name.empty() == false)
				XR_SOUND::set_sound_play(this->m_p_npc->ID(), sound_name, temp, 0);
		}

		void Script_XRMeetManager::set_start_distance(void)
		{
			if (DataBase::Storage::getInstance().getActor() == nullptr)
			{
				this->m_is_hello_passed = false;
				this->m_is_bye_passed = false;
				this->m_current_distance_name.clear();
				return;
			}

			if (!this->m_p_npc->Alive())
			{
				this->m_is_hello_passed = false;
				this->m_is_bye_passed = false;
				this->m_current_distance_name.clear();
				return;
			}

			float distance = this->m_p_npc->Position().distance_to(DataBase::Storage::getInstance().getActor()->Position());
			bool is_actor_visible = this->m_p_npc->CheckObjectVisibility(DataBase::Storage::getInstance().getActor());
			
			float value1 = atof(XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getFarDistance()).c_str());
			if (fis_zero(value1) || std::isnan(value1))
			{
				MESSAGEW("can't convert value from far_distance!");
				value1 = 0.0f;
			}

			float value2 = atof(XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getCloseDistance()).c_str());
			if (fis_zero(value2) || std::isnan(value2))
			{
				MESSAGEW("can't convert value from close_distance!");
				value2 = 0.0f;
			}

			bool is_object_far = is_actor_visible && distance <= value1;
			bool is_object_close = (is_actor_visible && distance <= value2) || (this->m_p_npc->IsTalking());

			if (is_object_close)
			{
				this->m_is_hello_passed = true;
				this->m_current_distance_name = "close";
			}
			else if (is_object_far)
			{
				this->m_is_bye_passed = true;
				this->m_current_distance_name = "far";
			}
			else if (distance > this->m_p_storage->getResetDistance())
			{
				this->m_is_hello_passed = false;
				this->m_is_bye_passed = false;
				this->m_current_distance_name.clear();
			}
			else
			{
				this->m_current_distance_name.clear();
			}
		}

		void Script_XRMeetManager::update(void)
		{
			float distance = this->m_p_npc->Position().distance_to(DataBase::Storage::getInstance().getActor()->Position());

			bool is_actor_visible = this->m_p_npc->CheckObjectVisibility(DataBase::Storage::getInstance().getActor());

			if (is_actor_visible)
			{
				float converted = atof(XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getCloseSoundDistance()).c_str());
				if (fis_zero(converted) || std::isnan(converted))
				{
					MESSAGEW("can't convert value from close_sound_distance!");
					converted = 0.0f;
				}

				float converted_far_sound_distance = atof(XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getFarSoundDistance()).c_str());
				if (fis_zero(converted_far_sound_distance) || std::isnan(converted_far_sound_distance))
				{
					MESSAGEW("can't convert value from far_sound_distance!");
					converted_far_sound_distance = 0.0f;
				}

				if (distance <= converted)
				{
					if (!this->m_is_hello_passed)
					{
						xr_string sound_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getCloseSoundHello());
						if (sound_name.empty() == false && (Globals::is_npc_in_combat(this->m_p_npc)))
						{
							xr_string temp;
							XR_SOUND::set_sound_play(this->m_p_npc->ID(), sound_name, temp, 0);
						}

						this->m_is_hello_passed = true;
					}
				}
				else if (distance <= converted_far_sound_distance)
				{
					if (this->m_is_hello_passed)
					{
						if (!this->m_is_bye_passed)
						{
							xr_string sound_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getCloseSoundBye());
							if (sound_name.empty() == false && Globals::is_npc_in_combat(this->m_p_npc))
							{
								xr_string temp;
								XR_SOUND::set_sound_play(this->m_p_npc->ID(), sound_name, temp, 0);
							}

							this->m_is_bye_passed = true;
						}
					}
				}
			}

			float converted1 = atof(XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getFarDistance()).c_str());
			float converted2 = atof(XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getCloseDistance()).c_str());

			if (fis_zero(converted1) || std::isnan(converted1))
			{
				MESSAGEW("can't convert far_distance value!");
				converted1 = 0.0f;
			}

			if (fis_zero(converted2) || std::isnan(converted2))
			{
				MESSAGEW("can't convert close_distance value!");
				converted2 = 0.0f;
			}

			bool is_object_far = is_actor_visible && (distance <= converted1);
			bool is_object_close = (is_actor_visible && distance <= converted2) || (this->m_p_npc->IsTalking() && !this->m_p_storage->getMeetOnTalking().empty());

			if (is_object_close)
			{
				if (this->m_current_distance_name != "close")
					this->m_current_distance_name = "close";
			}
			else if (is_object_far)
			{
				this->m_current_distance_name = "far";
			}
			else if (distance > this->m_p_storage->getResetDistance())
			{
				this->m_is_hello_passed = false;
				this->m_is_bye_passed = false;
				this->m_current_distance_name.clear();
			}
			else
			{
				this->m_current_distance_name.clear();
			}

			xr_string allow_break_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getAllowBreak());
			bool is_true_value = (allow_break_name == "true");
			if (this->m_is_allow_break != is_true_value)
			{
				this->m_is_allow_break = is_true_value;
			}

			if (this->m_p_storage->getMeetDialog().empty() == false)
			{
				xr_string start_dialog_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getMeetDialog());

				if (this->m_start_dialog != start_dialog_name)
				{
					this->m_start_dialog = start_dialog_name;

					if (start_dialog_name.empty())
					{
						this->m_p_npc->RestoreDefaultStartDialog();
					}
					else
					{
						this->m_p_npc->SetStartDialog(start_dialog_name.c_str());

						if (this->m_p_npc->IsTalking())
							DataBase::Storage::getInstance().getActor()->RunTalkDialog(this->m_p_npc, !this->m_is_allow_break);
					}
				}
			}

			bool is_talking = this->m_p_npc->IsTalking();

			xr_string use_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getUse());

			if (this->m_is_npc_camp_director)
			{
				use_name = "false";
			}

			if (this->m_use_name != use_name)
			{
				if (use_name == "self")
				{
					if (!is_talking && Device.dwPrecacheFrame < 1)
					{
						this->m_p_npc->EnableTalk();
						this->m_p_npc->AllowBreakTalkDialog(this->m_is_allow_break);
						DataBase::Storage::getInstance().getActor()->RunTalkDialog(this->m_p_npc, !(this->m_is_allow_break));
					}

					if (Device.dwPrecacheFrame < 1)
						this->m_use_name = use_name;
				}
			}

			xr_string use_text = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getUseText());
			if (use_text.empty() == false)
			{
				this->m_p_npc->SetTipText(use_text.c_str());
			}
			else
			{
				if (this->m_p_npc->IsTalkEnabled())
				{
					this->m_p_npc->SetTipText("character_use");
				}
				else
				{
					this->m_p_npc->SetTipText("");
				}
			}

			this->m_p_npc->AllowBreakTalkDialog(this->m_is_allow_break);

			xr_string abuse_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getAbuse());
			if (this->m_abuse_mode_name != abuse_name)
			{
				if (abuse_name == "true")
				{
					Script_SchemeXRAbuse::enable_abuse(this->m_p_npc);
				}
				else
				{
					Script_SchemeXRAbuse::disable_abuse(this->m_p_npc);
				}
			}

			if (CRD_Wounded::is_wounded(this->m_p_npc))
			{
				this->m_trade_enable_name = "false";
			}
			else
			{
				xr_string trade_enable_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getTradeEnable());

				if (this->m_trade_enable_name != trade_enable_name)
				{
					if (trade_enable_name == "true")
					{
						this->m_p_npc->EnableTrade();
					}
					else
					{
						this->m_p_npc->DisableTrade();
					}

					this->m_trade_enable_name = trade_enable_name;
				}
			}
		}

		Script_SchemeXRMeet::~Script_SchemeXRMeet(void)
		{
		}

		void Script_SchemeXRMeet::initialize(void)
		{
			CScriptActionBase::initialize();
			this->m_object->set_desired_position();
			this->m_object->set_desired_direction();
		}

		void Script_SchemeXRMeet::execute(void)
		{
			CScriptActionBase::execute();
			this->m_p_storage->getMeetManager()->update_state();
		}

		void Script_SchemeXRMeet::finalize(void)
		{
			CScriptActionBase::finalize();
		}

		void Script_SchemeXRMeet::add_to_binder(CScriptGameObject* const p_object, CScriptIniFile* const p_ini, const xr_string& scheme_name, const xr_string& section_name, DataBase::Script_IComponentScheme* storage)
		{
			if (p_object == nullptr)
			{
				R_ASSERT2(false, "invalid object!");
				return;
			}

			if (p_ini == nullptr)
			{
				R_ASSERT2(false, "invalid object!");
				return;
			}

			if (scheme_name.empty())
			{
				R_ASSERT2(false, "invalid string!");
				return;
			}

			
			CScriptActionPlanner* const p_planner = Globals::get_script_action_planner(p_object);
			xr_map<xr_string, std::uint32_t> properties;
			xr_map<xr_string, std::uint32_t> operators;

			properties["contact"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kStoheMeetBase + 1;
			properties["wounded"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kSidorWoundedBase;
			properties["abuse"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kAbuseBase;
			properties["wounded_exist"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kWoundedExist;
			properties["corpse_exist"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kCorpseExist;

			operators["contact"] = Globals::XR_ACTIONS_ID::kStoheMeetBase + 1;
			operators["state_mgr_to_idle_alife"] = Globals::XR_ACTIONS_ID::kStateManager + 2;

			p_planner->add_evaluator(properties.at("contact"), new Script_EvaluatorContact("meet_contact", static_cast<DataBase::Script_ComponentScheme_XRMeet*>(storage)));

			Script_SchemeXRMeet* const p_action = new Script_SchemeXRMeet("action_process_meet", static_cast<DataBase::Script_ComponentScheme_XRMeet*>(storage));
			p_action->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAlive, true));
			p_action->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyEnemy, false));
			p_action->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyDanger, false));
			p_action->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAnomaly, false));
			p_action->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyItems, false));
			p_action->add_condition(CWorldProperty(properties.at("wounded_exist"), false));
		//	p_action->add_condition(CWorldProperty(properties.at("corpse_exist"), false)); Lord: расскоменнитровать ибо не реализовано!
			p_action->add_condition(CWorldProperty(properties.at("contact"), true));
			p_action->add_condition(CWorldProperty(properties.at("wounded"), false));
			p_action->add_condition(CWorldProperty(properties.at("abuse"), false));

			p_action->add_effect(CWorldProperty(properties.at("contact"), false));

			p_planner->add_operator(operators.at("contact"), p_action);

			p_planner->action(StalkerDecisionSpace::eWorldPropertyALife).add_condition(CWorldProperty(properties.at("contact"), false));
			p_planner->action(operators.at("state_mgr_to_idle_alife")).add_condition(CWorldProperty(properties.at("contact"), false));

			MESSAGEI("allocated meet manager for %d %s", p_object->ID(), p_object->Name());
			static_cast<DataBase::Script_ComponentScheme_XRMeet*>(storage)->setMeetManager(new Script_XRMeetManager(p_object, static_cast<DataBase::Script_ComponentScheme_XRMeet*>(storage)));

			// @ Lord: получить с subscribe_action_for_events ...
		}

		void Script_SchemeXRMeet::set_meet(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini, const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
		{
			DataBase::Script_ComponentScheme_XRMeet* const p_storage = XR_LOGIC::assign_storage_and_bind<DataBase::Script_ComponentScheme_XRMeet>(p_client_object, p_ini, scheme_name, section_name, gulag_name);
		}

		void Script_SchemeXRMeet::reset_meet(CScriptGameObject* const p_client_object, const xr_string& scheme_name, const DataBase::Storage_Data& storage, const xr_string& section_name)
		{
			xr_string meet_section_name;

			if (scheme_name.empty())
			{
				meet_section_name = Globals::Utils::cfg_get_string(storage.getIni(), storage.getSectionLogicName(), "meet");
			}
			else
			{
				meet_section_name = Globals::Utils::cfg_get_string(storage.getIni(), section_name, "meet");
			}

			DataBase::Script_ComponentScheme_XRMeet* p_storage_scheme = static_cast<DataBase::Script_ComponentScheme_XRMeet*>(storage.getSchemes().at("meet"));
			init_meet(p_client_object, storage.getIni(), meet_section_name, scheme_name, p_storage_scheme);
		}

		void Script_SchemeXRMeet::init_meet(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini, const xr_string& section_name, const xr_string& scheme_name, DataBase::Script_IComponentScheme* p_cast)
		{
			DataBase::Script_ComponentScheme_XRMeet* storage = static_cast<DataBase::Script_ComponentScheme_XRMeet*>(p_cast);
			if (section_name == storage->getMeetSectionName() || (section_name.empty()))
				return;

			storage->setMeetSectionName(section_name);

			xr_map<xr_string, xr_string> defaults;
			
			std::uint32_t relation_type = Globals::GameRelations::get_npcs_relation(p_client_object, DataBase::Storage::getInstance().getActor());

			if (relation_type == ALife::eRelationTypeEnemy)
			{
				defaults["close_distance"] = "0";
				defaults["close_anim"] = "";
				defaults["close_snd_distance"] = "0";
				defaults["close_snd_hello"] = "";
				defaults["close_snd_bye"] = "";
				defaults["close_victim"] = "";
				defaults["far_distance"] = "0";
				defaults["far_anim"] = "";
				defaults["far_snd_distance"] = "0";
				defaults["far_snd"] = "";
				defaults["far_victim"] = "";
				defaults["snd_on_use"] = "";
				defaults["use"] = "false";
				defaults["meet_dialog"] = "";
				defaults["abuse"] = "false";
				defaults["trade_enable"] = "true";
				defaults["allow_break"] = "true";
				defaults["meet_on_talking"] = "false";
				defaults["use_text"] = "";
			}
			else
			{
				defaults["close_distance"] = "{=is_wounded} 0, {!is_squad_commander} 0, 3";
				defaults["close_anim"] = "{=is_wounded} nil, {!is_squad_commander} nil, {=actor_has_weapon} threat_na, talk_default";
				defaults["close_snd_distance"] = "{=is_wounded} 0, {!is_squad_commander} 0, 3";
				defaults["close_snd_hello"] = "{=is_wounded} nil, {!is_squad_commander} nil, {=actor_enemy} nil, {=actor_has_weapon} meet_hide_weapon, meet_hello";
				defaults["close_snd_bye"] = "{=is_wounded} nil, {!is_squad_commander} nil, {=actor_enemy} nil, {=actor_has_weapon} nil, meet_hello";
				defaults["close_victim"] = "{=is_wounded} nil, {!is_squad_commander} nil, actor";
				defaults["far_distance"] = "{=is_wounded} 0, {!is_squad_commander} 0, 5";
				defaults["far_anim"] = "";
				defaults["far_snd_distance"] = "{=is_wounded} 0, {!is_squad_commander} 0, 5";
				defaults["far_snd"] = "";
				defaults["far_victim"] = "";
				defaults["snd_on_use"] = "{=is_wounded} nil, {!is_squad_commander} meet_use_no_talk_leader, {=actor_enemy} nil, {=has_enemy} meet_use_no_fight, {=actor_has_weapon} meet_use_no_weapon, {!dist_to_actor_le(3)} nil";
				defaults["use"] = "{=is_wounded} false, {!is_squad_commander} false, {=actor_enemy} false, {=has_enemy} false, {=actor_has_weapon} false, {=dist_to_actor_le(3)} true, false";
				defaults["meet_dialog"] = "";
				defaults["abuse"] = "{=has_enemy} false, true";
				defaults["trade_enable"] = "true";
				defaults["allow_break"] = "true";
				defaults["meet_on_talking"] = "true";
				defaults["use_text"] = "";
			}

			if (section_name == "no_meet")
			{
				storage->setCloseDistance(XR_LOGIC::parse_condlist_by_script_object(section_name, "close_distance", "0"));
				storage->setCloseAnim(XR_LOGIC::parse_condlist_by_script_object(section_name, "close_anim", "nil"));
				storage->setCloseSoundDistance(XR_LOGIC::parse_condlist_by_script_object(section_name, "close_distance", "0"));
				storage->setCloseSoundHello(XR_LOGIC::parse_condlist_by_script_object(section_name, "close_sound_hello", "nil"));
				storage->setCloseSoundBye(XR_LOGIC::parse_condlist_by_script_object(section_name, "close_sound_bye", "nil"));
				storage->setCloseVictim(XR_LOGIC::parse_condlist_by_script_object(section_name, "close_victim", "nil"));

				storage->setFarDistance(XR_LOGIC::parse_condlist_by_script_object(section_name, "far_distance", "0"));
				storage->setFarAnim(XR_LOGIC::parse_condlist_by_script_object(section_name, "far_anim", "nil"));
				storage->setFarSoundDistance(XR_LOGIC::parse_condlist_by_script_object(section_name, "far_distance", "0"));
				storage->setFarSound(XR_LOGIC::parse_condlist_by_script_object(section_name, "far_snd", "nil"));
				storage->setFarVictim(XR_LOGIC::parse_condlist_by_script_object(section_name, "far_victim", "nil"));

				storage->setSoundOnUse(XR_LOGIC::parse_condlist_by_script_object(section_name, "snd_on_use", "nil"));
				storage->setUse(XR_LOGIC::parse_condlist_by_script_object(section_name, "use", "false"));
				storage->setMeetDialog(XR_LOGIC::parse_condlist_by_script_object(section_name, "meet_dialog", "nil"));
				storage->setAbuse(XR_LOGIC::parse_condlist_by_script_object(section_name, "abuse", "false"));
				storage->setTradeEnable(XR_LOGIC::parse_condlist_by_script_object(section_name, "trade_enable", "true"));
				storage->setAllowBreak(XR_LOGIC::parse_condlist_by_script_object(section_name, "allow_break", "true"));
				storage->setMeetOnTalking(XR_LOGIC::parse_condlist_by_script_object(section_name, "meet_on_talking", "false"));
				storage->setUseText(XR_LOGIC::parse_condlist_by_script_object(section_name, "use_text", "nil"));
			}
			else
			{
				xr_string close_distance_name = Globals::Utils::cfg_get_string(p_ini, section_name, "close_distance");
				if (close_distance_name.empty())
					close_distance_name = defaults.at("close_distance");

				storage->setCloseDistance(XR_LOGIC::parse_condlist_by_script_object(section_name, "close_distance", close_distance_name));

				xr_string close_anim_name = Globals::Utils::cfg_get_string(p_ini, section_name, "close_anim");
				if (close_anim_name.empty())
					close_anim_name = defaults.at("close_anim");

				storage->setCloseAnim(XR_LOGIC::parse_condlist_by_script_object(section_name, "close_anim", close_anim_name));

				xr_string close_sound_distance_name = Globals::Utils::cfg_get_string(p_ini, section_name, "close_distance");
				if (close_distance_name.empty())
					close_distance_name = defaults.at("close_snd_distance");

				storage->setCloseSoundDistance(XR_LOGIC::parse_condlist_by_script_object(section_name, "close_distance", close_sound_distance_name));

				xr_string close_sound_hello_name = Globals::Utils::cfg_get_string(p_ini, section_name, "close_snd_hello");
				if (close_sound_hello_name.empty())
					close_sound_hello_name = defaults.at("close_snd_hello");

				storage->setCloseSoundHello(XR_LOGIC::parse_condlist_by_script_object(section_name, "close_sound_hello", close_sound_hello_name));

				xr_string close_sound_bye_name = Globals::Utils::cfg_get_string(p_ini, section_name, "close_snd_bye");
				if (close_sound_bye_name.empty())
					close_sound_bye_name = defaults.at("close_snd_bye");

				storage->setCloseSoundBye(XR_LOGIC::parse_condlist_by_script_object(section_name, "close_sound_bye", close_sound_bye_name));

				xr_string close_victim_name = Globals::Utils::cfg_get_string(p_ini, section_name, "close_victim");
				if (close_victim_name.empty())
					close_victim_name = defaults.at("close_victim");

				storage->setCloseVictim(XR_LOGIC::parse_condlist_by_script_object(section_name, "close_victim", close_victim_name));

				xr_string far_distance_name = Globals::Utils::cfg_get_string(p_ini, section_name, "far_distance");
				if (far_distance_name.empty())
					far_distance_name = defaults.at("far_distance");

				storage->setFarDistance(XR_LOGIC::parse_condlist_by_script_object(section_name, "far_distance", far_distance_name));

				xr_string far_anim_name = Globals::Utils::cfg_get_string(p_ini, section_name, "far_anim");
				if (far_anim_name.empty())
					far_anim_name = defaults.at("far_anim");

				storage->setFarAnim(XR_LOGIC::parse_condlist_by_script_object(section_name, "far_anim", far_anim_name));

				xr_string far_distance_sound_name = Globals::Utils::cfg_get_string(p_ini, section_name, "far_snd_distance");
				if (far_distance_sound_name.empty())
					far_distance_sound_name = defaults.at("far_snd_distance");

				storage->setFarSoundDistance(XR_LOGIC::parse_condlist_by_script_object(section_name, "far_distance", far_distance_sound_name));

				xr_string far_sound_name = Globals::Utils::cfg_get_string(p_ini, section_name, "far_snd");
				if (far_sound_name.empty())
					far_sound_name = defaults.at("far_snd");
				storage->setFarSound(XR_LOGIC::parse_condlist_by_script_object(section_name, "far_snd", far_sound_name));

				xr_string far_victim_name = Globals::Utils::cfg_get_string(p_ini, section_name, "far_victim");
				if (far_victim_name.empty())
					far_victim_name = defaults.at("far_victim");

				storage->setFarVictim(XR_LOGIC::parse_condlist_by_script_object(section_name, "far_victim", far_victim_name));

				xr_string sound_on_use_name = Globals::Utils::cfg_get_string(p_ini, section_name, "snd_on_use");
				if (sound_on_use_name.empty())
					sound_on_use_name = defaults.at("snd_on_use");
				storage->setSoundOnUse(XR_LOGIC::parse_condlist_by_script_object(section_name, "snd_on_use", sound_on_use_name));

				xr_string use_name = Globals::Utils::cfg_get_string(p_ini, section_name, "use");
				if (use_name.empty())
					use_name = defaults.at("use");
				storage->setUse(XR_LOGIC::parse_condlist_by_script_object(section_name, "use", use_name));

				xr_string meet_dialog_name = Globals::Utils::cfg_get_string(p_ini, section_name, "meet_dialog");
				if (meet_dialog_name.empty())
					meet_dialog_name = defaults.at("meet_dialog");

				storage->setMeetDialog(XR_LOGIC::parse_condlist_by_script_object(section_name, "meet_dialog", meet_dialog_name));

				xr_string abuse_name = Globals::Utils::cfg_get_string(p_ini, section_name, "abuse");
				if (abuse_name.empty())
					abuse_name = defaults.at("abuse");
				storage->setAbuse(XR_LOGIC::parse_condlist_by_script_object(section_name, "abuse", abuse_name));

				xr_string trade_enable_name = Globals::Utils::cfg_get_string(p_ini, section_name, "trade_enable");
				if (trade_enable_name.empty())
					trade_enable_name = defaults.at("trade_enable");

				storage->setTradeEnable(XR_LOGIC::parse_condlist_by_script_object(section_name, "trade_enable", trade_enable_name));

				xr_string allow_break_name = Globals::Utils::cfg_get_string(p_ini, section_name, "allow_break");
				if (allow_break_name.empty())
					allow_break_name = defaults.at("allow_break");
				storage->setAllowBreak(XR_LOGIC::parse_condlist_by_script_object(section_name, "allow_break", allow_break_name));

				xr_string meet_on_talking_name = Globals::Utils::cfg_get_string(p_ini, section_name, "meet_on_talking");
				if (meet_on_talking_name.empty())
					meet_on_talking_name = defaults.at("meet_on_talking");
				storage->setMeetOnTalking(XR_LOGIC::parse_condlist_by_script_object(section_name, "meet_on_talking", meet_on_talking_name));

				xr_string use_text_name = Globals::Utils::cfg_get_string(p_ini, section_name, "use_text");
				storage->setUseText(XR_LOGIC::parse_condlist_by_script_object(section_name, "use_text", use_text_name));
			}

			storage->getMeetManager()->set_start_distance();
			storage->setSet(true);
		}

		bool Script_SchemeXRMeet::is_meet(CScriptGameObject* const p_client_object)
		{
			CScriptActionPlanner* const p_planner = Globals::get_script_action_planner(p_client_object);
			if (p_planner && p_planner->initialized())
			{
				std::uint32_t id = p_planner->current_action_id();
				if (id == (Globals::XR_ACTIONS_ID::kStoheMeetBase + 1))
					return true;
			}

			return false;
		}

		void Script_SchemeXRMeet::process_npc_usability(CScriptGameObject* const p_client_object)
		{
			if (!p_client_object)
			{
				MESSAGEWR("p_client_object == nullptr!");
				return;
			}


			if (CRD_Wounded::is_wounded(p_client_object))
			{
				if (p_client_object->GetRelationType(DataBase::Storage::getInstance().getActor()) == ALife::eRelationTypeEnemy)
				{
					p_client_object->DisableTalk();
				}
				else
				{
					DataBase::Script_ComponentScheme_XRWounded* p_storage = static_cast<DataBase::Script_ComponentScheme_XRWounded*>(DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().at("wounded"));

					if (!p_storage)
					{
						MESSAGEWR("wounded scheme doesn't allocated at all!");
						return;
					}

					// Lord: äîäåëàòü!
				}
			}
		}

}
}