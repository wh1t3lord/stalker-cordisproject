#pragma once

namespace Cordis
{
	namespace Scripts
	{
		class Script_ReachTaskManager
		{
		public:
			Script_ReachTaskManager(void) = delete;
			Script_ReachTaskManager(const xr_string& target_id_name);
			~Script_ReachTaskManager(void);

			void add_npc(CScriptGameObject* const p_npc);
			void remove_npc(CScriptGameObject* const p_npc);
			void reset_positions(void);
			void set_formation(const xr_string& formation_name);
			CScriptGameObject* get_commander(CScriptGameObject* const p_npc);
			void get_npc_command(CScriptGameObject* const p_npc, std::uint32_t& vertex, Fvector& direction, xr_string& state_name);
			void set_command(CScriptGameObject* const p_npc, const xr_string& command_name, const xr_string& formation_name);
			bool is_commander(const std::uint16_t npc_id);
			bool is_commander_in_meet(void);

		private:
			std::uint16_t m_commander_id;
			std::uint32_t m_commander_level_vertex_id;
			std::uint32_t m_npc_count;
			Fvector m_commander_direction;
			xr_map<std::uint16_t, std::tuple<std::uint16_t, Fvector, float>> m_npc_list;
			xr_string m_current_state_name;
			xr_string m_formation_name;
			xr_string m_target_name;
		};

		class Script_EvaluatorReachedTaskLocation : public CScriptPropertyEvaluator
		{
		public:
			Script_EvaluatorReachedTaskLocation(const xr_string& name, DataBase::Storage_Scheme& storage) : CScriptPropertyEvaluator(nullptr, name.c_str()) {}
			~Script_EvaluatorReachedTaskLocation(void) {}

			virtual _value_type evaluate(void);
		};

		class Script_SchemeXRReachTask : public Script_ISchemeStalker
		{
		public:
			Script_SchemeXRReachTask(void) = delete;
			Script_SchemeXRReachTask(const xr_string& name);
			~Script_SchemeXRReachTask(void);

			virtual void initialize(void);
			virtual void execute(void);
			virtual void finalize(void);

			virtual void death_callback(CScriptGameObject* const p_client_victim, CScriptGameObject* const p_client_who);
			virtual void net_destroy(CScriptGameObject* const p_client_object);

			static void set_reach_task(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
				const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);

			static void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
				const xr_string& scheme_name, const xr_string& section_name, DataBase::Storage_Scheme& storage);

			static void add_reach_task_action(CScriptGameObject* const p_client_object);
		private:
			void commander_execute(void);
			void soldier_execute(void);

		private:
			bool m_is_on_point;
			bool m_is_was_reset;
			std::uint16_t m_target_id;
			std::uint16_t m_squad_id;
			std::uint32_t m_level_vertex_id;
			int m_time_to_update;
			float m_dist;
			Fvector m_direction;
			xr_string m_current_state_name;
			xr_string m_formation_name;
			

		};
	}
}


