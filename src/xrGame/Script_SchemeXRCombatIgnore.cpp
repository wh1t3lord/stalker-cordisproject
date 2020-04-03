#include "stdafx.h"
#include "Script_SchemeXRCombatIgnore.h"

namespace Cordis
{
	namespace Scripts
	{
		Script_SchemeXRCombatIgnore::Script_SchemeXRCombatIgnore(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage) : Script_ISchemeEntity(p_client_object, storage)
		{
		}

		Script_SchemeXRCombatIgnore::~Script_SchemeXRCombatIgnore(void)
		{
		}

		void Script_SchemeXRCombatIgnore::hit_callback(CScriptGameObject* const p_client_object, const float amount, const Fvector& local_direction, CScriptGameObject* const p_client_who, const std::int16_t bone_index)
		{
			
		}

		inline bool Script_SchemeXRCombatIgnore::is_enemy_xr_combat_ignore(CScriptGameObject* const p_client_object, CScriptGameObject* const p_enemy, DataBase::Storage_Scheme* p_storage)
		{
			return false;
		}
	}
}