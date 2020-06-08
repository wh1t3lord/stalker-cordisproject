#pragma once

namespace Cordis
{
	namespace Scripts
	{
		class Script_EvaluatorCombatCamper : public CScriptPropertyEvaluator
		{
		public:
			Script_EvaluatorCombatCamper(const xr_string& name, void* storage) : CScriptPropertyEvaluator(nullptr, name.c_str()) {}

			~Script_EvaluatorCombatCamper(void) {}

			virtual _value_type evaluate(void) 
			{ // @ Протестить вообще
				return (DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getScriptCombatTypeName() == "camper");
			}
		};

		class Script_EvaluatorCombatCamperSee : public CScriptPropertyEvaluator
		{
		public:
			Script_EvaluatorCombatCamperSee(const xr_string& name, void* storage) : CScriptPropertyEvaluator(nullptr, name.c_str()), m_p_storage(&storage) {}
			~Script_EvaluatorCombatCamperSee(void);

			virtual _value_type evaluate(void) 
			{
				CScriptGameObject* const p_best_enemy = this->m_object->GetBestEnemy();

				if (this->m_object->Alive() && p_best_enemy && this->m_object->CheckObjectVisibility(p_best_enemy))
				{
					this->m_p_storage->setXRCombatCamperLastSeenPosition(p_best_enemy->Position());
					return true;
				}

				return false;
			}
		private:
			DataBase::Storage_Scheme* m_p_storage;
		};

		class Script_SchemeXRCombatCamper : public Script_ISchemeStalker
		{
		public:
			Script_SchemeXRCombatCamper(void) = delete;
			Script_SchemeXRCombatCamper(const xr_string& name, void* storage);
			~Script_SchemeXRCombatCamper(void);
			
			virtual void initialize(void);
			virtual void execute(void);
			virtual void finalize(void);

			static void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini, const xr_string& section_name, const xr_string& scheme_name, void* storage);
		};


		class Script_SchemeXRCombatCamperLook : public Script_ISchemeStalker
		{
		public:
			Script_SchemeXRCombatCamperLook(void) = delete;
			Script_SchemeXRCombatCamperLook(const xr_string& name, void* storage);
			~Script_SchemeXRCombatCamperLook(void);

			virtual void initialize(void);
			virtual void execute(void);
			virtual void finalize(void);
			virtual void hit_callback(CScriptGameObject* const p_client_object, const float amount, const Fvector& local_direction, CScriptGameObject* const p_client_who, const std::int16_t bone_index);
		private:
			void reset(void);

		private:
			std::uint32_t m_forget_time;
			std::uint32_t m_change_dir_time;
		};
	}
}


