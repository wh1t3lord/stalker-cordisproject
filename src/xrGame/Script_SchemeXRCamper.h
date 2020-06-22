#pragma once

namespace Cordis
{
	namespace Scripts
	{
		class Script_EvaluatorCamperEnd : public CScriptPropertyEvaluator
		{
		public:
			Script_EvaluatorCamperEnd(const xr_string& name, DataBase::Script_ComponentScheme_XRCamper* storage) : CScriptPropertyEvaluator(nullptr, name.c_str()), m_p_storage(storage)
			{

			}

			virtual _value_type evaluate(void)
			{
				return !XR_LOGIC::is_active(this->m_object, this->m_p_storage);
			}

		private:
			DataBase::Script_ComponentScheme_XRCamper* m_p_storage;
		};

		class Script_EvaluatorCloseCombat : public CScriptPropertyEvaluator
		{
		public:
			Script_EvaluatorCloseCombat(const xr_string& name, DataBase::Script_ComponentScheme_XRCamper* storage) : CScriptPropertyEvaluator(nullptr, name.c_str()), m_p_storage(storage), m_is_close_combat(false)
			{}

			virtual _value_type evaluate(void);
		private:
			bool m_is_close_combat;
			DataBase::Script_ComponentScheme_XRCamper* m_p_storage;
		};

		class Script_SchemeXRCamper : public Script_ISchemeStalker
		{
			using inherited_scheme = Script_ISchemeStalker;

		public:
			Script_SchemeXRCamper(void) = delete;
			Script_SchemeXRCamper(const xr_string& name, void* storage);
			~Script_SchemeXRCamper(void);

			virtual void initialize(void);
			virtual void execute(void);
			virtual void finalize(void);

			virtual void activate_scheme(const bool is_loading, CScriptGameObject* const p_client_object);
			virtual void reset_scheme(const bool value, CScriptGameObject* const p_client_object);

			bool process_point(std::uint32_t mode, std::uint32_t number)
			{
				return false;
			}

			static void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
				const xr_string& scheme_name, const xr_string& section_name, void* storage);

			// @ PRIVATE, uses in XR_LOGIC
			static void set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
				const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);

		private:
			bool process_danger(void);
			bool on_place(void);
			bool can_shoot(void);
			std::pair<std::uint32_t, Fvector> get_next_point(const std::uint32_t flag);
			void scan(const std::uint32_t flag);
		private:
			bool m_is_danger;
			std::uint8_t m_flag;
			std::uint32_t m_scantime;
			Script_MoveManager* m_p_move_manager;
			CScriptGameObject* m_p_enemy;
			Fvector m_enemy_position;
			Fvector m_look_position;
			Fvector m_dest_position;
			Fvector m_look_point;
			std::pair<std::uint32_t, Fvector> m_next_point;
			xr_map<std::uint32_t, std::pair<std::uint32_t, Fvector>> m_scan_table;	

		};
	}
}


