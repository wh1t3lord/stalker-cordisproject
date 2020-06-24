#include "stdafx.h"
#include "Script_PostCombat.h"

namespace Cordis
{
	namespace Scripts
	{
		Script_EvaluatorCombatEnemy::_value_type Script_EvaluatorCombatEnemy::evaluate(void)
		{
			CScriptGameObject* const p_best_enemy = this->m_object->GetBestEnemy();

			if (p_best_enemy)
			{
				if (DataBase::Storage::getInstance().getStorage().find(this->m_object->ID()) != DataBase::Storage::getInstance().getStorage().end())
				{
					const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(this->m_object->ID());
					if (storage.getSchemes().find("combat_ignore") != storage.getSchemes().end())
					{
						DataBase::Script_ComponentScheme_XRCombatIgnore* const p_storage = static_cast<DataBase::Script_ComponentScheme_XRCombatIgnore*>(storage.getSchemes().at("combat_ignore"));
						if (p_storage)
						{
							if (!Script_SchemeXRCombatIgnore::is_enemy_xr_combat_ignore(this->m_object, p_best_enemy, p_storage) == false)
							{
								return false;
							}
						}
					}
				}
			}

			if (p_best_enemy && this->m_p_storage->getTimer())
			{
				this->m_p_storage->setPostCombatLastBestEnemyID(p_best_enemy->ID());
				this->m_p_storage->setPostCombatLastBestEnemyName(p_best_enemy->Name());
				this->m_p_storage->setPostCombatTimer(0);
				return true;
			}

			if (p_best_enemy == nullptr && this->m_p_storage->getTimer() == 0)
			{
				const DataBase::Data_Overrides& overrides = DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getOverrides();
				std::uint32_t min_time = overrides.getMinPostCombatTime() * 1000;
				std::uint32_t max_time = overrides.getMaxPostCombatTime() * 1000;

				if (min_time == 0)
					min_time = 10000;

				if (max_time == 0)
					max_time = 15000;

				if (this->m_p_storage->getLastBestEnemyID() == DataBase::Storage::getInstance().getActor()->ID())
				{
					this->m_p_storage->setPostCombatTimer(Globals::get_time_global());
				}
				else
				{
					this->m_p_storage->setPostCombatTimer(Globals::get_time_global() + Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(min_time, max_time));
				}
			}

			if (this->m_p_storage->getTimer() == 0)
				return (p_best_enemy != nullptr);

			if (Globals::get_time_global() < this->m_p_storage->getTimer())
				return true;

			if (this->m_p_storage->getAnimation() == nullptr)
				return false;

			this->m_p_storage->getAnimation()->set_state("", false);

			return (this->m_p_storage->getAnimation()->getStates().getAnimationMarker() != 0);
		}

		Script_ActionPostCombatWait::~Script_ActionPostCombatWait(void)
		{
			if (this->m_p_state_manager)
			{
				MESSAGEI("deleting state manager");
				xr_delete(this->m_p_state_manager);
			}
		}

		void Script_ActionPostCombatWait::add_post_combat_idle(CScriptGameObject* const p_client_object)
		{
			if (p_client_object == nullptr)
			{
				MESSAGEWR("invalid object");
				return;
			}

			CScriptActionPlanner* const p_planner = Globals::get_script_action_planner(p_client_object);
			CScriptActionPlanner* const p_combat_planner = Globals::cast_planner(&p_planner->action(StalkerDecisionSpace::eWorldOperatorCombatPlanner));

			if (p_combat_planner == nullptr)
			{
				R_ASSERT2(false, "bad cast!");
				return;
			}

			DataBase::Script_ComponentScheme_XRCombatIgnore* const p_storage = new DataBase::Script_ComponentScheme_XRCombatIgnore();
			DataBase::Storage::getInstance().setStorageScheme(p_client_object->ID(), "post_combat_wait", p_storage);

			p_planner->remove_evaluator(StalkerDecisionSpace::eWorldPropertyEnemy);
			p_planner->add_evaluator(StalkerDecisionSpace::eWorldPropertyEnemy, new Script_EvaluatorCombatEnemy("evaluator_combat_enemy", p_storage));

			p_combat_planner->remove_evaluator(StalkerDecisionSpace::eWorldPropertyEnemy);
			p_combat_planner->add_evaluator(StalkerDecisionSpace::eWorldPropertyEnemy, new Script_EvaluatorCombatEnemy("evaluator_combat_enemy", p_storage));

			p_combat_planner->remove_operator(StalkerDecisionSpace::eWorldOperatorPostCombatWait);

			Script_ActionPostCombatWait* const p_action = new Script_ActionPostCombatWait("action_post_combat_wait", p_storage);
			p_action->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyEnemy, false));
			p_action->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyPureEnemy, false));
			p_action->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyCriticallyWounded, false));
			p_action->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyDangerGrenade, false));
			p_action->add_effect(CWorldProperty(StalkerDecisionSpace::eWorldPropertyEnemy, false));


			p_combat_planner->add_operator(StalkerDecisionSpace::eWorldOperatorPostCombatWait, p_action);
		}

		void Script_ActionPostCombatWait::initialize(void)
		{
			CScriptActionBase::initialize();

			this->m_object->set_item(MonsterSpace::EObjectAction::eObjectActionIdle, this->m_object->best_weapon());
			this->m_object->set_mental_state(MonsterSpace::EMentalState::eMentalStateDanger);
			this->m_object->set_body_state(MonsterSpace::EBodyState::eBodyStateCrouch);
			this->m_object->set_movement_type(MonsterSpace::EMovementType::eMovementTypeStand);
			this->m_object->set_sight(SightManager::ESightType::eSightTypeCover, nullptr, 0);
			this->m_p_state_manager = new Script_StateManager(this->m_object);
			this->m_p_state_manager->setAnimState(new Script_StateAnimation(this->m_object, *this->m_p_state_manager, false));
			this->m_p_storage->setPostCombatAnimation(new Script_StateAnimation(this->m_object, *this->m_p_state_manager, true));
			this->m_is_anim_started = false;
		}

		void Script_ActionPostCombatWait::finalize(void)
		{
			XR_SOUND::set_sound_play(this->m_object->ID(), "post_combat_relax", xr_string(), 0);

			if (this->m_is_anim_started)
				this->m_p_storage->getAnimation()->set_state();

			Script_StateAnimation* p_object = this->m_p_storage->getAnimation();
			
			MESSAGEI("deleting animation from post combat storage!");
			xr_delete(p_object);
			this->m_p_storage->setPostCombatAnimation(nullptr);

			CScriptActionBase::finalize();
		}

		void Script_ActionPostCombatWait::execute(void)
		{
			CScriptActionBase::execute();
			
			auto check_weapon_locked = [&]() -> bool {
				bool is_weapon_strapped = this->m_object->weapon_strapped();
				bool is_weapon_unstrapped = this->m_object->weapon_unstrapped();

				if (!(is_weapon_unstrapped || is_weapon_unstrapped))
					return true;

				CScriptGameObject* const p_best_weapon = this->m_object->best_weapon();

				if (p_best_weapon == nullptr)
					return false;

				if (this->m_object->GetActiveItem() == nullptr)
					return false;

				bool is_weapon_going_to_be_strapped = this->m_object->is_weapon_going_to_be_strapped(p_best_weapon);

				if (is_weapon_going_to_be_strapped && !is_weapon_strapped)
					return true;

				if (!is_weapon_unstrapped && !is_weapon_going_to_be_strapped)
					return true;

				return false;
			};

			if (!this->m_object->in_smart_cover() && (!check_weapon_locked()))
			{
				this->m_is_anim_started = true;
				this->m_p_storage->getAnimation()->set_state("hide", false);
				this->m_p_storage->getAnimation()->set_control();
			}

			XR_SOUND::set_sound_play(this->m_object->ID(), "post_combat_wait", xr_string(), 0);
		}
	}
}


