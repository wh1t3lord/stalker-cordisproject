#pragma once

namespace Cordis
{
	namespace Scripts
	{
		class Script_EvaluatorPatrolEnd : public CScriptPropertyEvaluator
		{
		public:
			Script_EvaluatorPatrolEnd(const xr_string& name, DataBase::Storage_Scheme& storage) : CScriptPropertyEvaluator(nullptr, name.c_str()), m_p_storage(&storage) {}
			~Script_EvaluatorPatrolEnd(void);
			virtual _value_type evaluate(void);

		private:
			DataBase::Storage_Scheme* m_p_storage;
		};

		class Script_EvaluatorPatrolComm : public CScriptPropertyEvaluator 
		{
		public:
			Script_EvaluatorPatrolComm(const xr_string& name, DataBase::Storage_Scheme& storage) : CScriptPropertyEvaluator(nullptr, name.c_str()), m_p_storage(&storage) {}
			~Script_EvaluatorPatrolComm(void);

			virtual _value_type evaluate(void);

		private:
			DataBase::Storage_Scheme* m_p_storage;
		};

		class Script_ActionCommander : public Script_ISchemeStalker
		{
		public:
			Script_ActionCommander(CScriptGameObject* const p_client_object, const xr_string& name, DataBase::Storage_Scheme& storage);
			~Script_ActionCommander(void);

			virtual void initialize(void);
			virtual void execute(void);
			virtual void finalize(void);

			virtual void death_callback(CScriptGameObject* const p_client_victim, CScriptGameObject* const p_client_who);
			virtual void net_destroy(CScriptGameObject* const p_client_object);
			virtual void deactivate(CScriptGameObject* const p_client_object);
			virtual void activate_scheme(const bool is_loading, CScriptGameObject* const p_client_object);

			bool formation_callback(std::uint32_t number, std::uint32_t index);
		private:
			bool m_is_was_reset;
			Script_MoveManager* m_p_move_manager;
			xr_string m_current_state_name;
			xr_string m_old_state_name;
		};
		 
		class Script_SchemeXRPatrol : public Script_ISchemeStalker
		{
		public:
			Script_SchemeXRPatrol(void) = delete;
			Script_SchemeXRPatrol(const xr_string& name, DataBase::Storage_Scheme& storage);
			~Script_SchemeXRPatrol(void);

		private:
		};

		class Script_XRPatrolManager
		{
		public:
			Script_XRPatrolManager(void) = delete;
			Script_XRPatrolManager(const xr_string& path_name);

			~Script_XRPatrolManager(void);

			void add_npc(CScriptGameObject* const p_npc, const bool is_leader);
			void remove_npc(CScriptGameObject* const p_npc);
			void reset_positions(void);
			void set_formation(const xr_string& formation_name);
			CScriptGameObject* const get_commander(CScriptGameObject* const p_npc);
			void get_npc_command(CScriptGameObject* const p_npc, std::uint32_t& vertex, Fvector& direction, xr_string& state_name);
			void set_command(CScriptGameObject* const p_npc, const xr_string& command_name, const xr_string& formation_name);
			bool is_commander(const std::uint16_t npc_id);
			bool is_commander_in_meet(void);

		private:
			std::uint16_t m_commander_id;
			std::uint32_t m_commander_level_vertex_id;
			std::uint32_t m_npc_count;
			Fvector m_commander_direction;
			xr_map<std::uint16_t, std::tuple<CScriptGameObject*, Fvector, std::uint32_t>> m_npc_list;
			xr_string m_path_name;
			xr_string m_current_state_name;
			xr_string m_formation_name;
		};
	}
}


