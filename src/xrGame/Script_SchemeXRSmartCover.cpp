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
		}

		void Script_SchemeXRSmartCover::deactivate(CScriptGameObject* const p_client_object)
		{
		}

		void Script_SchemeXRSmartCover::target_selector(CScriptGameObject* const p_client_object)
		{
		}

		void Script_SchemeXRSmartCover::check_target_selector(void)
		{
		}

		bool Script_SchemeXRSmartCover::check_target(void)
		{
			return false;
		}

		bool Script_SchemeXRSmartCover::position_riched(void)
		{
			return false;
		}

	}
}