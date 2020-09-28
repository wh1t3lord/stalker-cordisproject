#pragma once

namespace Cordis
{
	namespace Scripts
	{
		class Script_EvaluatorCombatEnemy : public CScriptPropertyEvaluator
		{
		public: 
			Script_EvaluatorCombatEnemy(const xr_string& name, DataBase::Script_ComponentScheme_PostCombat* p_storage) : CScriptPropertyEvaluator(nullptr, name.c_str()), m_p_storage(p_storage)
			{
				this->m_p_storage->setTimer(Globals::get_time_global());
			}

			~Script_EvaluatorCombatEnemy(void) {}

			_value_type evaluate(void) override;

		private:
			DataBase::Script_ComponentScheme_PostCombat* m_p_storage;
		};

		class Script_ActionPostCombatWait : CScriptActionBase 
		{
		public: 
			Script_ActionPostCombatWait(const xr_string& name, DataBase::Script_ComponentScheme_PostCombat* p_storage) : CScriptActionBase(nullptr, name.c_str()), m_p_storage(p_storage), m_p_state_manager(nullptr), m_is_anim_started(false)
			{

			}
			~Script_ActionPostCombatWait(void);

			static void add_post_combat_idle(CScriptGameObject* const p_client_object);

			void initialize(void) override;
			void finalize(void) override;
			void execute(void) override;

		private:
			bool m_is_anim_started;
			DataBase::Script_ComponentScheme_PostCombat* m_p_storage;
			Script_StateManager* m_p_state_manager;
		};
	}
}