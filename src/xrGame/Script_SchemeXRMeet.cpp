#include "stdafx.h"
#include "Script_SchemeXRMeet.h"

namespace Cordis
{
	namespace Scripts
	{
		Script_EvaluatorContact::_value_type Script_EvaluatorContact::evaluate(void)
		{
			return false;
		}

		Script_XRMeetManager::Script_XRMeetManager(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage) : m_is_bye_passed(false), m_is_hello_passed(false), m_is_npc_camp_director(false), m_p_storage(&storage), m_p_npc(p_client_object), m_is_allow_break(false) 
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
				state_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getXRMeetCloseAnim());
				victim_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getXRMeetCloseVictim());
			}
			else if (this->m_current_distance_name == "far")
			{
				state_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getXRMeetFarAnim());
				victim_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getXRMeetFarVictim());
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

			xr_string sound_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getXRMeetFarSound());
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
			
			float value1 = atof(XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getXRMeetFarDistance()).c_str());
			if (fis_zero(value1) || std::isnan(value1))
			{
				MESSAGEW("can't convert value from far_distance!");
				value1 = 0.0f;
			}

			float value2 = atof(XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getXRMeetCloseDistance()).c_str());
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
			else if (distance > this->m_p_storage->getXRMeetResetDistance())
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
				float converted = atof(XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getXRMeetCloseSoundDistance()).c_str());
				if (fis_zero(converted) || std::isnan(converted))
				{
					MESSAGEW("can't convert value from close_sound_distance!");
					converted = 0.0f;
				}

				float converted_far_sound_distance = atof(XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getXRMeetFarSoundDistance()).c_str());
				if (fis_zero(converted_far_sound_distance) || std::isnan(converted_far_sound_distance))
				{
					MESSAGEW("can't convert value from far_sound_distance!");
					converted_far_sound_distance = 0.0f;
				}

				if (distance <= converted)
				{
					if (!this->m_is_hello_passed)
					{
						xr_string sound_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getXRMeetCloseSoundHello());
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
							xr_string sound_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getXRMeetCloseSoundBye());
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

			float converted1 = atof(XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getXRMeetFarDistance()).c_str());
			float converted2 = atof(XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getXRMeetCloseDistance()).c_str());

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
			bool is_object_close = (is_actor_visible && distance <= converted2) || (this->m_p_npc->IsTalking() && !this->m_p_storage->getXRMeetMeetOnTalking().empty());

			if (is_object_close)
			{
				if (this->m_current_distance_name != "close")
					this->m_current_distance_name = "close";
			}
			else if (is_object_far)
			{
				this->m_current_distance_name = "far";
			}
			else if (distance > this->m_p_storage->getXRMeetResetDistance())
			{
				this->m_is_hello_passed = false;
				this->m_is_bye_passed = false;
				this->m_current_distance_name.clear();
			}
			else
			{
				this->m_current_distance_name.clear();
			}

			xr_string allow_break_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getXRMeetAllowBreak());
			bool is_true_value = (allow_break_name == "true");
			if (this->m_is_allow_break != is_true_value)
			{
				this->m_is_allow_break = is_true_value;
			}

			if (this->m_p_storage->getXRMeetMeetDialog().empty() == false)
			{
				xr_string start_dialog_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getXRMeetMeetDialog());

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

			xr_string use_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getXRMeetUse());

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

			xr_string use_text = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getXRMeetUseText());
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

			xr_string abuse_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getXRMeetAbuse());
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
				xr_string trade_enable_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc, this->m_p_storage->getXRMeetTradeEnable());

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

	}
}