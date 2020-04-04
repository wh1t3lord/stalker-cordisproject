#include "stdafx.h"
#include "Script_SchemeXRCombatZombied.h"

constexpr std::uint32_t kCombatZombieShoot = 1;
constexpr std::uint32_t kCombatZombieDanger = 2;

namespace Cordis
{
	namespace Scripts
	{
		Script_SchemeXRCombatZombied::Script_SchemeXRCombatZombied(const xr_string& action_name, DataBase::Storage_Scheme& storage) : Script_ISchemeStalker(nullptr, action_name, storage), m_is_was_hit(false), m_is_valid_path(false), m_enemy_last_seen_level_vertex_id(0), m_last_level_vertex_id(0), m_turn_time(0), m_enemy_last_accessible_level_vertex_id(0), m_hit_reaction_end_time(0)
		{
		}

		Script_SchemeXRCombatZombied::~Script_SchemeXRCombatZombied(void)
		{
		}

		void Script_SchemeXRCombatZombied::initialize(void)
		{
			CScriptActionBase::initialize();
			this->m_object->set_desired_direction();
			this->m_object->set_detail_path_type(DetailPathManager::eDetailPathTypeSmooth);
			this->m_last_state_name.clear();
			CScriptGameObject* const p_best_enemy = this->m_object->GetBestEnemy();
			this->m_enemy_last_seen_position = p_best_enemy->Position();
			this->m_enemy_last_seen_level_vertex_id = p_best_enemy->level_vertex_id();
			this->m_last_level_vertex_id = 0;
			this->m_is_valid_path = false;
			this->m_turn_time = 0;
			this->m_p_storage->setXRCombatZombiedCurrentAction(kCombatZombieShoot);
		}

		void Script_SchemeXRCombatZombied::execute(void)
		{
			CScriptActionBase::execute();

			CScriptGameObject* const p_best_enemy = this->m_object->GetBestEnemy();
			bool is_see = this->m_object->CheckObjectVisibility(p_best_enemy);

			if (is_see)
			{
				this->m_enemy_last_seen_level_vertex_id = p_best_enemy->level_vertex_id();
				this->m_enemy_last_seen_position = p_best_enemy->Position();
			}

			if (this->m_last_level_vertex_id != this->m_enemy_last_seen_level_vertex_id)
			{
				this->m_last_level_vertex_id = this->m_enemy_last_seen_level_vertex_id;
				this->m_is_valid_path = false;

				if (this->m_object->accessible_vertex_id(this->m_enemy_last_seen_level_vertex_id) == false)
				{
					this->m_enemy_last_accessible_level_vertex_id = this->m_object->accessible_nearest(Globals::Game::level::vertex_position(this->m_enemy_last_seen_level_vertex_id), this->m_enemy_last_accessible_position);
				}
				else
				{
					this->m_enemy_last_accessible_level_vertex_id = this->m_enemy_last_seen_level_vertex_id;
					this->m_enemy_last_accessible_position = this->m_enemy_last_seen_position;
				}
			}

			this->m_object->set_path_type(MovementManager::EPathType::ePathTypeLevelPath);

			if (this->m_object->Position().distance_to_sqr(this->m_enemy_last_accessible_position) > 9.0f)
			{
				if (this->m_is_valid_path == false)
				{
					this->m_is_valid_path = true;
					this->m_object->set_dest_level_vertex_id(this->m_enemy_last_accessible_level_vertex_id);
				}

				if (is_see)
				{
					this->set_state("raid_fire", p_best_enemy, Fvector());
				}
				else if (this->m_is_was_hit)
				{
					this->m_is_was_hit = false;
					this->m_hit_reaction_end_time = Globals::get_time_global() + 5000;
					this->set_state("raid_fire", nullptr, this->m_enemy_last_seen_position);
				}
				else if (this->m_hit_reaction_end_time > Globals::get_time_global())
				{

				}
				else
				{
					this->set_state("raid", nullptr, this->m_enemy_last_seen_position);
				}

				this->m_turn_time = 0;
			}
			else
			{
				if (is_see)
				{
					this->set_state("threat_fire", p_best_enemy, Fvector());
					this->m_turn_time = 0;
				}
				else
				{
					if (this->m_is_was_hit)
					{
						this->m_is_was_hit = false;
						this->m_turn_time = Globals::get_time_global() + Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(5000, 7000);
						this->set_state("threat_na", nullptr, this->m_enemy_last_seen_position);
					}
					else if (this->m_turn_time < Globals::get_time_global())
					{
						this->m_turn_time = Globals::get_time_global() + Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(3000, 5000);
						this->set_state("threat_na", nullptr, this->calc_random_direction());
					}
				}
			}
		}

		void Script_SchemeXRCombatZombied::finalize(void)
		{
			CScriptActionBase::finalize();
			this->m_p_storage->setXRCombatZombiedCurrentAction(0);
		}

		void Script_SchemeXRCombatZombied::hit_callback(CScriptGameObject* const p_client_object, const float amount, const Fvector& local_direction, CScriptGameObject* const p_client_who, const std::int16_t bone_index)
		{
			if (p_client_who == nullptr)
				return;

			if (this->m_p_storage->getXRCombatZombiedCurrentAction() == kCombatZombieShoot)
			{
				if (this->m_object == nullptr)
					return;

				CScriptGameObject* const p_best_enemy = this->m_object->GetBestEnemy();

				if (p_best_enemy && p_best_enemy->ID() == p_client_who->ID())
				{
					this->m_is_was_hit = true;
					this->m_enemy_last_seen_level_vertex_id = p_best_enemy->level_vertex_id();
					this->m_enemy_last_seen_position = p_best_enemy->Position();
				}
			}
		}

		void Script_SchemeXRCombatZombied::set_state(const xr_string& state_name, CScriptGameObject* const p_look_object, const Fvector& position)
		{
			std::pair<Fvector, CScriptGameObject* const> data(position, p_look_object);

			if (p_look_object)
				data.first = this->m_enemy_last_seen_position;

			Globals::set_state(this->m_object, state_name, StateManagerCallbackData(), 0, data, StateManagerExtraData());

			this->m_last_state_name = state_name;
		}

		Fvector Script_SchemeXRCombatZombied::calc_random_direction(void)
		{
			float angle = PI * 2.0f * Globals::Script_RandomFloat::getInstance().Generate();
			Fvector look_position = Fvector().set(this->m_object->Position());
			
			look_position.x = look_position.x + cos(angle);
			look_position.z = look_position.z + sin(angle);

			return look_position;
		}
	}
}