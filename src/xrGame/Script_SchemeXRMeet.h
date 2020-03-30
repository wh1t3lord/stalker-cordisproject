#pragma once

namespace Cordis
{
	namespace Scripts
	{
		class Script_EvaluatorContact : public CScriptPropertyEvaluator
		{
		public:
			Script_EvaluatorContact(const xr_string& name, DataBase::Storage_Scheme& storage) : CScriptPropertyEvaluator(nullptr, name.c_str()) {}

			virtual _value_type evaluate(void);
		private:
			DataBase::Storage_Scheme* m_p_storage;
		};

		class Script_SchemeXRMeet
		{
		};

		class Script_XRMeetManager
		{
		public:
			Script_XRMeetManager(void) = delete;
			Script_XRMeetManager(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage);
			~Script_XRMeetManager(void);

			void update_state(void);
			void set_start_distance(void);
			void update(void);

		private:
			bool m_is_npc_camp_director;
			bool m_is_hello_passed;
			bool m_is_bye_passed;
			CScriptGameObject* m_p_npc;
			DataBase::Storage_Scheme* m_p_storage;
			xr_string m_allow_break_name;
			xr_string m_trade_enable_name;
			xr_string m_use_name;
			xr_string m_abuse_mode_name;
			xr_string m_start_dialog;
			xr_string m_current_distance_name;
		};
	}
}


