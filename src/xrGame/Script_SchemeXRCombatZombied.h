#pragma once

namespace Cordis
{
	namespace Scripts
	{
		// @ Shoot
		class Script_SchemeXRCombatZombied : public Script_ISchemeStalker
		{
		public:
			Script_SchemeXRCombatZombied(void) = delete;
			Script_SchemeXRCombatZombied(const xr_string& action_name, DataBase::Storage_Scheme& storage);
			~Script_SchemeXRCombatZombied(void);

			virtual void initialize(void);
			virtual void execute(void);
			virtual void finalize(void);

			virtual void hit_callback(CScriptGameObject* const p_client_object, const float amount, const Fvector& local_direction, CScriptGameObject* const p_client_who, const std::int16_t bone_index);
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
	}
}


