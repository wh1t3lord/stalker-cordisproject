#pragma once

namespace Cordis
{
	namespace Scripts
	{
		class Script_SchemeXRCombatIgnore : public Script_ISchemeEntity
		{
		public:
			Script_SchemeXRCombatIgnore(void) = delete;
			Script_SchemeXRCombatIgnore(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage);
			~Script_SchemeXRCombatIgnore(void);

			
			virtual void hit_callback(CScriptGameObject* const p_client_object, const float amount, const Fvector& local_direction, CScriptGameObject* const p_client_who, const std::int16_t bone_index);
			static inline bool is_enemy_xr_combat_ignore(CScriptGameObject* const p_client_object, CScriptGameObject* const p_enemy, DataBase::Storage_Scheme* p_storage);
		private:

		};
	}
}


