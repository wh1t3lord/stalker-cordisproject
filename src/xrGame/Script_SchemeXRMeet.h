#pragma once

namespace Cordis
{
	namespace Scripts
	{
		class Script_EvaluatorContact : public CScriptPropertyEvaluator
		{
		public:
			Script_EvaluatorContact(const xr_string& name, DataBase::Script_ComponentScheme_XRMeet* storage) : CScriptPropertyEvaluator(nullptr, name.c_str()), m_p_storage(storage) {}

			virtual _value_type evaluate(void);
		private:
			DataBase::Script_ComponentScheme_XRMeet* m_p_storage;
		};

		class Script_SchemeXRMeet : public CScriptActionBase
		{
		public:
			Script_SchemeXRMeet(void) = delete;
			Script_SchemeXRMeet(const xr_string& name, DataBase::Script_ComponentScheme_XRMeet* storage) : CScriptActionBase(nullptr, name.c_str()), m_p_storage(storage) {}
			~Script_SchemeXRMeet(void);

			virtual void initialize(void);
			virtual void execute(void);
			virtual void finalize(void);

			static void add_to_binder(CScriptGameObject* const p_object, CScriptIniFile* const p_ini, const xr_string& scheme_name, const xr_string& section_name, DataBase::Script_IComponentScheme* storage);
			static void set_meet(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini, const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);
			static void	reset_meet(CScriptGameObject* const p_client_object, const xr_string& scheme_name, const DataBase::Storage_Data& storage, const xr_string& section_name);
			static void init_meet(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini, const xr_string& section_name, const xr_string& scheme_name, DataBase::Script_IComponentScheme* storage);
			static bool is_meet(CScriptGameObject* const p_client_object);
			static void process_npc_usability(CScriptGameObject* const p_client_object);
		private:
			DataBase::Script_ComponentScheme_XRMeet* m_p_storage;
		};

		class Script_XRMeetManager
		{
		public:
			Script_XRMeetManager(void) = delete;
			Script_XRMeetManager(CScriptGameObject* const p_client_object, DataBase::Script_ComponentScheme_XRMeet* storage);
			~Script_XRMeetManager(void);

			void update_state(void);
			void set_start_distance(void);
			void update(void);

			inline void setUseName(const xr_string& name) noexcept
			{
				if (name.empty())
				{
					MESSAGEW("you are trying to set an empty string!");
				}

				this->m_use_name = name;
			}

			inline const xr_string& getCurrentDistanceName(void) const noexcept { return this->m_current_distance_name; }
		private:
			bool m_is_allow_break;
			bool m_is_npc_camp_director;
			bool m_is_hello_passed;
			bool m_is_bye_passed;
			CScriptGameObject* m_p_npc;
			DataBase::Script_ComponentScheme_XRMeet* m_p_storage;
			xr_string m_allow_break_name;
			xr_string m_trade_enable_name;
			xr_string m_use_name;
			xr_string m_abuse_mode_name;
			xr_string m_start_dialog;
			xr_string m_current_distance_name;
		};
	}
}


