#pragma once

namespace Cordis
{
	namespace Scripts
	{
		class Script_EvaluatorWound : public CScriptPropertyEvaluator
		{
		public:
			Script_EvaluatorWound(void) = delete;
			Script_EvaluatorWound(const xr_string& name, DataBase::Storage_Scheme& storage) : CScriptPropertyEvaluator(nullptr, name.c_str()), m_p_storage(&storage) {}
			~Script_EvaluatorWound(void);

			virtual _value_type evaluate(void);

		private:
			DataBase::Storage_Scheme* m_p_storage;
		};


		class Script_SchemeXRWounded : public CScriptActionBase
		{
		public: 
			Script_SchemeXRWounded(void) = delete;
			Script_SchemeXRWounded(const xr_string& name, DataBase::Storage_Scheme& storage) : CScriptActionBase(nullptr, name.c_str()), m_p_storage(&storage) {}
			~Script_SchemeXRWounded(void);


			virtual void initialize(void);
			virtual void execute(void);
			virtual void finalize(void);

		private:
			DataBase::Storage_Scheme* m_p_storage;
		};

		class Script_WoundedManager
		{
		public:
			Script_WoundedManager(void) = delete;
			Script_WoundedManager(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage);
			~Script_WoundedManager(void);

		private:
			bool m_is_can_use_medkit;
			CScriptGameObject* m_p_npc;
			DataBase::Storage_Scheme* m_p_storage;
		};
	}
}


