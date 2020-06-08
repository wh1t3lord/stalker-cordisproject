#include "stdafx.h"
#include "Script_SchemeXRCombatCamper.h"

constexpr std::uint32_t kBase = Cordis::Scripts::Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kCombatCamperBase;
constexpr std::uint32_t kPropEnable = kBase;
constexpr std::uint32_t kPropSee = kBase + 1;
constexpr std::uint32_t kActionShoot = Cordis::Scripts::Globals::XR_ACTIONS_ID::kCombatCamperBase;
constexpr std::uint32_t kActionLookAround = kActionShoot + 1;

namespace Cordis
{
	namespace Scripts
	{
		Script_SchemeXRCombatCamper::Script_SchemeXRCombatCamper(const xr_string& name, void* storage) : Script_ISchemeStalker(nullptr, name, storage)
		{
		}
		Script_SchemeXRCombatCamper::~Script_SchemeXRCombatCamper(void)
		{
		}

		void Script_SchemeXRCombatCamper::initialize(void)
		{
			CScriptActionBase::initialize();

			Globals::set_state(this->m_object, "hide_fire", StateManagerCallbackData(), 0, std::pair<Fvector, CScriptGameObject* const>(Fvector(), this->m_object->GetBestEnemy()), StateManagerExtraData());
			this->m_p_storage->setXRCombatCamperAction(true);
		}

		void Script_SchemeXRCombatCamper::execute(void)
		{
			CScriptActionBase::execute();
		}

		void Script_SchemeXRCombatCamper::finalize(void)
		{
			CScriptActionBase::finalize();
			this->m_p_storage->setXRCombatCamperAction(false);
		}

		void Script_SchemeXRCombatCamper::add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini, const xr_string& section_name, const xr_string& scheme_name, void* storage)
		{
			if (!p_client_object)
			{
				MESSAGEWR("p_client_object == nullptr!");
				return;
			}

			if (!p_ini)
			{
				MESSAGEWR("p_ini == nullptr!");
				return;
			}
			
			if (section_name.empty())
			{
				MESSAGEWR("invalid section!");
				return;
			}

			if (scheme_name.empty())
			{
				MESSAGEWR("invalid scheme!");
				return;
			}

			CScriptActionPlanner* const p_planner = Globals::get_script_action_planner(p_client_object);

			xr_map<xr_string, std::uint32_t> properties;

			properties["state_mgr_logic_active"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kStateManager + 4;

			p_planner->add_evaluator(kPropEnable, new Script_EvaluatorCombatCamper("combat_camper", storage));
			p_planner->add_evaluator(kPropSee, new Script_EvaluatorCombatCamperSee("combat_camper_see", storage));

			Script_SchemeXRCombatCamper* p_scheme = new Script_SchemeXRCombatCamper("combat_camper_shoot", storage);
			p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAlive, true));
			p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyEnemy, true));
			p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAnomaly, false));
			p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kScriptCombat, true));
			p_scheme->add_condition(CWorldProperty(kPropEnable, true));
			p_scheme->add_condition(CWorldProperty(kPropSee, true));
			p_scheme->add_effect(CWorldProperty(StalkerDecisionSpace::eWorldPropertyEnemy, false));
			p_scheme->add_effect(CWorldProperty(properties.at("state_mgr_logic_active"), false));
			p_planner->add_operator(kActionShoot, p_scheme);

			Script_SchemeXRCombatCamperLook* const p_scheme_look = new Script_SchemeXRCombatCamperLook("combat_camper_look_around", storage);
			p_scheme_look->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAnomaly, false));
			p_scheme_look->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kScriptCombat, true));
			p_scheme_look->add_condition(CWorldProperty(kPropEnable, true));
			p_scheme_look->add_condition(CWorldProperty(kPropSee, false));
			p_scheme_look->add_effect(CWorldProperty(kPropSee, true));
			p_scheme_look->add_effect(CWorldProperty(properties.at("state_mgr_logic_active"), false));
			p_planner->add_operator(kActionLookAround, p_scheme_look);

			DataBase::Storage::getInstance().setStorageSchemesActions(p_client_object->ID(), scheme_name, p_scheme_look);

			storage.setXRCombatCamperAction(false);
		}

		Script_SchemeXRCombatCamperLook::Script_SchemeXRCombatCamperLook(const xr_string& name, void* storage) : Script_ISchemeStalker(nullptr, name, storage), m_forget_time(0), m_change_dir_time(0)
		{
		}

		Script_SchemeXRCombatCamperLook::~Script_SchemeXRCombatCamperLook(void)
		{
		}

		void Script_SchemeXRCombatCamperLook::initialize(void)
		{
			CScriptActionBase::initialize();
			this->m_p_storage->setXRCombatCamperAction(true);

			this->reset();
		}

		void Script_SchemeXRCombatCamperLook::execute(void)
		{
			CScriptActionBase::execute();

			if (this->m_forget_time < Device.dwTimeGlobal)
			{
				MESSAGE("pl:Disabling_memory_object[8]");
				this->m_p_storage->setXRCombatCamperLastSeenPosition(Fvector());
				return;
			}

			if (this->m_change_dir_time < Device.dwTimeGlobal)
			{
				this->m_change_dir_time = Device.dwTimeGlobal + Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(2000, 4000);

				float angle = Globals::Script_RandomFloat::getInstance().Generate(0.0f, 120.0f) - 60.0f;

				if (Globals::is_vector_nil(this->m_p_storage->getXRCombatCamperLastSeenPosition()))
				{
					MESSAGEW("report this error to STALKER-829 bug [%s]", this->m_object->Name());
				}

				Fvector direction = Fvector().set(this->m_p_storage->getXRCombatCamperLastSeenPosition()).sub(this->m_object->Position());

				Globals::set_state(this->m_object, "hide", StateManagerCallbackData(), 0, std::pair<Fvector, CScriptGameObject* const>(this->m_object->Position().add(direction), nullptr), StateManagerExtraData());
			}
		}

		void Script_SchemeXRCombatCamperLook::finalize(void)
		{
			CScriptActionBase::finalize();

			this->m_p_storage->setXRCombatCamperLastSeenPosition(Fvector());
			this->m_p_storage->setXRCombatCamperAction(false);
		}

		void Script_SchemeXRCombatCamperLook::hit_callback(CScriptGameObject* const p_client_object, const float amount, const Fvector& local_direction, CScriptGameObject* const p_client_who, const std::int16_t bone_index)
		{
			if (p_client_who == nullptr || !this->m_p_storage->isXRCombatCamperAction())
				return;

			if (!this->m_object)
				return;
			

			CScriptGameObject* const p_best_enemy = this->m_object->GetBestEnemy();

			if (p_best_enemy && p_client_who->ID() == p_best_enemy->ID())
			{
				this->m_p_storage->setXRCombatCamperLastSeenPosition(p_best_enemy->Position());
				this->reset();
			}
		}

		void Script_SchemeXRCombatCamperLook::reset(void)
		{
			this->m_forget_time = Device.dwTimeGlobal + 30000;
			this->m_change_dir_time = Device.dwTimeGlobal + 15000;

			if (Globals::is_vector_nil(this->m_p_storage->getXRCombatCamperLastSeenPosition()) && this->m_object->GetBestEnemy())
			{
				this->m_p_storage->setXRCombatCamperLastSeenPosition(this->m_object->GetBestEnemy()->Position());
			}

			Globals::set_state(this->m_object, "hide", StateManagerCallbackData(), 0, std::pair<Fvector, CScriptGameObject* const>(this->m_p_storage->getXRCombatCamperLastSeenPosition(), nullptr), StateManagerExtraData());
		}
		Script_EvaluatorCombatCamperSee::~Script_EvaluatorCombatCamperSee(void)
		{
		}
}
}