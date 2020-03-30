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

		Script_XRMeetManager::Script_XRMeetManager(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage) : m_is_bye_passed(false), m_is_hello_passed(false), m_is_npc_camp_director(false), m_p_storage(&storage), m_p_npc(p_client_object)
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
	/*			state_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_npc);*/
			}
		}

		void Script_XRMeetManager::set_start_distance(void)
		{
		}

		void Script_XRMeetManager::update(void)
		{
		}

	}
}