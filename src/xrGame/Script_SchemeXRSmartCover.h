#pragma once

namespace Cordis
{
	namespace Scripts
	{
		class Script_EvaluatorUseSmartCoverInCombat : public CScriptPropertyEvaluator
		{
		public:
			Script_EvaluatorUseSmartCoverInCombat(const xr_string& name, void* storage) : CScriptPropertyEvaluator(nullptr, name.c_str()), m_p_storage(&storage) {}
			~Script_EvaluatorUseSmartCoverInCombat(void) {}

			virtual _value_type evaluate(void);
		private:
			DataBase::Storage_Scheme* m_p_storage;
		};

		class Script_EvaluatorNeedSmartCover : public CScriptPropertyEvaluator
		{
		public:
			Script_EvaluatorNeedSmartCover(const xr_string& name, void* storage) : CScriptPropertyEvaluator(nullptr, name.c_str()), m_p_storage(&storage) {}
			~Script_EvaluatorNeedSmartCover(void) {}

			virtual _value_type evaluate(void);

		private:
			DataBase::Storage_Scheme* m_p_storage;
		};

		class Script_SchemeXRSmartCover : public Script_ISchemeStalker
		{
		public:
			Script_SchemeXRSmartCover(void) = delete;
			Script_SchemeXRSmartCover(const xr_string& name, void* storage);
			~Script_SchemeXRSmartCover(void);

			virtual void finalize(void);
			virtual void initialize(void);
			virtual void execute(void);
			virtual void activate_scheme(const bool is_loading, CScriptGameObject* const p_client_object);
			virtual void deactivate(CScriptGameObject* const p_client_object);

			void target_selector(CScriptGameObject* const p_client_object);
			void check_target_selector(void);
			bool check_target(void);
			bool position_riched(void);

			static void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
				const xr_string& scheme_name, const xr_string& section_name, void* storage);

			static void set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
				const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);
		private:
			bool m_is_initialized;
			std::uint16_t m_target_enemy_id;
			Fvector m_fire_position;
			xr_map<std::uint32_t, CondlistData> m_condlist_cover;
			xr_map<std::uint32_t, CondlistData> m_condlist_target_path;
			xr_string m_cover_state_name;
			xr_string m_cover_name;
			xr_string m_target_path_name;
		};
	}
}


