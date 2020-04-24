#pragma once

namespace Cordis
{
	namespace Scripts
	{
		class Script_EvaluatorUseSmartCoverInCombat : public CScriptPropertyEvaluator
		{
		public:
			Script_EvaluatorUseSmartCoverInCombat(const xr_string& name, DataBase::Storage_Scheme& storage) : CScriptPropertyEvaluator(nullptr, name.c_str()), m_p_storage(&storage) {}
			~Script_EvaluatorUseSmartCoverInCombat(void) {}

			virtual _value_type evaluate(void);
		private:
			DataBase::Storage_Scheme* m_p_storage;
		};

		class Script_EvaluatorNeedSmartCover : public CScriptPropertyEvaluator
		{
		public:
			Script_EvaluatorNeedSmartCover(const xr_string& name, DataBase::Storage_Scheme& storage) : CScriptPropertyEvaluator(nullptr, name.c_str()), m_p_storage(&storage) {}
			~Script_EvaluatorNeedSmartCover(void) {}

			virtual _value_type evaluate(void);

		private:
			DataBase::Storage_Scheme* m_p_storage;
		};

		class Script_SchemeXRSmartCover : public Script_ISchemeStalker
		{
		public:
			Script_SchemeXRSmartCover(void) = delete;
			Script_SchemeXRSmartCover(const xr_string& name, DataBase::Storage_Scheme& storage);
			~Script_SchemeXRSmartCover(void);

		private:
			std::uint16_t m_target_enemy_id;
			Fvector m_fire_position;
			xr_map<CondlistData, std::uint32_t> m_condlist_cover;
			xr_map<CondlistData, std::uint32_t> m_condlist_target_path;
			xr_string m_cover_state_name;
			xr_string m_cover_name;
		};
	}
}


