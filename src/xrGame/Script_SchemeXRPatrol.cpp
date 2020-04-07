#include "stdafx.h"
#include "Script_SchemeXRPatrol.h"

constexpr std::uint32_t kGetSoldier = 0;
constexpr std::uint32_t kGetDirection = 1;
constexpr std::uint32_t kGetDist = 2;

namespace Cordis
{
	namespace Scripts
	{
		Script_EvaluatorPatrolEnd::_value_type Script_EvaluatorPatrolEnd::evaluate(void)
		{
			return XR_LOGIC::is_active(this->m_object, *this->m_p_storage);
		}

		Script_EvaluatorPatrolComm::~Script_EvaluatorPatrolComm(void)
		{
		}

		Script_EvaluatorPatrolComm::_value_type Script_EvaluatorPatrolComm::evaluate(void)
		{
			// Lord: доделать
			return false;
		}

		Script_SchemeXRPatrol::Script_SchemeXRPatrol(const xr_string& name, DataBase::Storage_Scheme& storage) : Script_ISchemeStalker(nullptr, name, storage)
		{
		}

		Script_SchemeXRPatrol::~Script_SchemeXRPatrol(void)
		{
		}


		Script_XRPatrolManager::Script_XRPatrolManager(const xr_string& path_name) : m_path_name(path_name), m_current_state_name("patrol"), m_commander_id(Globals::kUnsignedInt16Undefined), m_commander_level_vertex_id(Globals::kUnsignedInt32Undefined), m_npc_count(0), m_formation_name("back")
		{
		}

		Script_XRPatrolManager::~Script_XRPatrolManager(void)
		{
		}

		void Script_XRPatrolManager::add_npc(CScriptGameObject* const p_npc, const bool is_leader)
		{
		}

		void Script_XRPatrolManager::remove_npc(CScriptGameObject* const p_npc)
		{
		}

		void Script_XRPatrolManager::reset_positions(void)
		{
		}

		void Script_XRPatrolManager::set_formation(const xr_string& formation_name)
		{
		}

		CScriptGameObject* const Script_XRPatrolManager::get_commander(CScriptGameObject* const p_npc)
		{
			return nullptr;
		}

		void Script_XRPatrolManager::get_npc_command(CScriptGameObject* const p_npc, std::uint32_t& vertex, Fvector& direction, xr_string& state_name)
		{
		}

		void Script_XRPatrolManager::set_command(CScriptGameObject* const p_npc, const xr_string& command_name, const xr_string& formation_name)
		{
		}

		bool Script_XRPatrolManager::is_commander(const std::uint16_t npc_id)
		{
			return false;
		}

		bool Script_XRPatrolManager::is_commander_in_meet(void)
		{
			return false;
		}

	}
}