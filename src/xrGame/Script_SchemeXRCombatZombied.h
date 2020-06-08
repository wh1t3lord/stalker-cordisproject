#pragma once

namespace Cordis
{
	namespace Scripts
	{
		class Script_EvaluatorCombatZombied : public CScriptPropertyEvaluator
		{
		public:
			Script_EvaluatorCombatZombied(const xr_string& name, void* storage) : CScriptPropertyEvaluator(nullptr, name.c_str()), m_p_storage(&storage)
			{
				
			}

			virtual _value_type evaluate(void) 
			{
				return (Globals::character_community(this->m_object) == "zombied");
			}
		private:
			DataBase::Storage_Scheme* m_p_storage;
		};

		// @ Shoot
		class Script_SchemeXRCombatZombied : public Script_ISchemeStalker
		{
		public:
			Script_SchemeXRCombatZombied(void) = delete;
			Script_SchemeXRCombatZombied(const xr_string& action_name, void* storage);
			~Script_SchemeXRCombatZombied(void);

			virtual void initialize(void);
			virtual void execute(void);
			virtual void finalize(void);

			virtual void hit_callback(CScriptGameObject* const p_client_object, const float amount, const Fvector& local_direction, CScriptGameObject* const p_client_who, const std::int16_t bone_index);

			static void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini, const xr_string& section_name, const xr_string& scheme_name, void* p_storage);
		private:
			void set_state(const xr_string& state_name, CScriptGameObject* const p_look_object, const Fvector& position);
			Fvector calc_random_direction(void);
		private:
			bool m_is_was_hit;
			bool m_is_valid_path;
			std::uint32_t m_enemy_last_seen_level_vertex_id;
			std::uint32_t m_enemy_last_accessible_level_vertex_id;
			std::uint32_t m_last_level_vertex_id;
			std::uint32_t m_turn_time;
			std::uint32_t m_hit_reaction_end_time;
			Fvector m_enemy_last_seen_position;
			Fvector m_enemy_last_accessible_position;
			xr_string m_last_state_name;
		};

		class Script_SchemeXRCombatZombiedDanger : public Script_ISchemeStalker
		{
		public:
			Script_SchemeXRCombatZombiedDanger(void) = delete;
			Script_SchemeXRCombatZombiedDanger(const xr_string& name, void* storage);
			~Script_SchemeXRCombatZombiedDanger(void); 

			virtual void initialize(void);
			virtual void execute(void);
			virtual void finalize(void);
			virtual void hit_callback(CScriptGameObject* const p_client_object, const float amount, const Fvector& local_direction, CScriptGameObject* const p_client_who, const std::int16_t bone_index);
		private:
			void set_state(const xr_string& state_name, CScriptGameObject* const p_best_enemy, const Fvector& position);
		private:
			bool m_is_was_hit;
			std::uint16_t m_bestdanger_id;
			std::uint32_t m_bestdanger_level_vertex_id;
			std::uint32_t m_last_sent_level_vertex_id;
			std::uint32_t m_hit_reaction_end_time;
			Fvector m_enemy_last_seen_position;
			xr_string m_last_state_name;
		};
	}
}


