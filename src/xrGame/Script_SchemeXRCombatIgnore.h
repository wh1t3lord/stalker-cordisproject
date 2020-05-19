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

			bool enemy_callback(CScriptGameObject* const p_object, CScriptGameObject* const p_enemy);
			virtual void hit_callback(CScriptGameObject* const p_client_object, const float amount, const Fvector& local_direction, CScriptGameObject* const p_client_who, const std::int16_t bone_index);
			static bool is_enemy_xr_combat_ignore(CScriptGameObject* const p_client_object, CScriptGameObject* const p_enemy, DataBase::Storage_Scheme* p_storage);

			static void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
				const xr_string& scheme_name, const xr_string& section_name, DataBase::Storage_Scheme& storage);
			static void set_combat_ignore_checker(CScriptGameObject* const p_npc, CScriptIniFile* const p_ini, const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);
			static void reset_combat_ignore_checker(CScriptGameObject* const p_npc, const xr_string& scheme_name, const DataBase::Storage_Data& p_storage, const xr_string& section_name);
			static void disable_scheme(CScriptGameObject* const p_client_object, const xr_string& scheme_name);
		private:

		};
	}
}


