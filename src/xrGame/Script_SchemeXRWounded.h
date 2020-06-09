#pragma once

namespace Cordis
{
	namespace Scripts
	{
		class Script_EvaluatorWound : public CScriptPropertyEvaluator
		{
		public:
			Script_EvaluatorWound(void) = delete;
			Script_EvaluatorWound(const xr_string& name, DataBase::Script_ComponentScheme_XRWounded* storage) : CScriptPropertyEvaluator(nullptr, name.c_str()), m_p_storage(storage) {}
			~Script_EvaluatorWound(void);

			virtual _value_type evaluate(void);

		private:
			DataBase::Script_ComponentScheme_XRWounded* m_p_storage;
		};

		class Script_EvaluatorCanFight : public CScriptPropertyEvaluator
		{
		public:
			Script_EvaluatorCanFight(void) = delete;
			Script_EvaluatorCanFight(const xr_string& name, DataBase::Script_ComponentScheme_XRWounded* storage) : CScriptPropertyEvaluator(nullptr, name.c_str()), m_p_storage(storage) {}
			~Script_EvaluatorCanFight(void);

			virtual _value_type evaluate(void);

		private:
			DataBase::Script_ComponentScheme_XRWounded* m_p_storage;
		};


		class Script_SchemeXRWounded : public CScriptActionBase
		{
		public: 
			Script_SchemeXRWounded(void) = delete;
			Script_SchemeXRWounded(const xr_string& name, DataBase::Script_ComponentScheme_XRWounded* storage) : CScriptActionBase(nullptr, name.c_str()), m_p_storage(storage) {}
			~Script_SchemeXRWounded(void);


			virtual void initialize(void);
			virtual void execute(void);
			virtual void finalize(void);
			static void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
				const xr_string& scheme_name, const xr_string& section_name, void* storage);
			static void set_wounded(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini, const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);
			static void reset_wounded(CScriptGameObject* const p_client_object, const xr_string& scheme_name, const DataBase::Storage_Data& storage, const xr_string& section_name);
			static void init_wounded(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini, const xr_string& section_name, DataBase::Storage_Scheme* p_storage, const xr_string& scheme_name);

		private:
			DataBase::Script_ComponentScheme_XRWounded* m_p_storage;
		};

		class Script_WoundedManager
		{
		public:
			Script_WoundedManager(void) = delete;
			Script_WoundedManager(CScriptGameObject* const p_client_object, void* storage);
			~Script_WoundedManager(void);

			void update(void);
			void eat_medkit(void);
			void process_fight(const std::uint32_t health, xr_string& result_name);
			void process_victim(const std::uint32_t health, xr_string& result_name);
			void process_hp_wound(const std::uint32_t health, xr_string& state_name, xr_string& sound_name);
			void process_psy_wound(const std::uint32_t health, xr_string& state_name, xr_string& sound_name);
			void hit_callback(void);
			std::uint32_t get_key_from_distance(const xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& data, const std::uint32_t health);
			inline void unlock_medkit(void) { this->m_is_can_use_medkit = true; }
			inline bool isCanUseMedkit(void) const noexcept { return this->m_is_can_use_medkit; }
            inline const xr_string& getStateName(void) const noexcept { return this->m_state_name; }
		private:
			bool m_is_can_use_medkit;
			CScriptGameObject* m_p_npc;
			DataBase::Storage_Scheme* m_p_storage;
			xr_string m_state_name;
			xr_string m_sound_name;
			xr_string m_fight_name;
			xr_string m_victim_name;
		};
	}
}


