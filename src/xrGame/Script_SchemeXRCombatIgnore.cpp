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

		bool Script_SchemeXRCombatIgnore::enemy_callback(CScriptGameObject* const p_object, CScriptGameObject* const p_enemy)
		{
			if (!p_object)
			{
				MESSAGEWR("p_object == nullptr!");
				return false;
			}

			if (!p_enemy)
			{
				MESSAGEWR("p_enemy == nullptr!");
				return false;
			}

			if (p_enemy->ID() == p_object->ID())
			{
				DataBase::Storage::getInstance().setXRCombatIgnoreFightingWithActorNpcs(p_object->ID(), true);
			}

			bool is_object_enemy = is_enemy_xr_combat_ignore(p_object, p_enemy, this->m_p_storage);
			if (is_object_enemy)
			{
				CSE_Abstract* const p_server_object = ai().alife().objects().object(p_object->ID());
				if (!p_server_object)
				{
					MESSAGEWR("can't find server instance by ID %d", p_object->ID());
					return false;
				}
				CSE_ALifeMonsterAbstract* const p_server_monster = p_server_object->cast_monster_abstract();
				if (p_server_monster && p_server_monster->m_smart_terrain_id != Globals::kUnsignedInt16Undefined)
				{
					Script_SE_SmartTerrain* const p_server_smart = ai().alife().objects().object(p_server_monster->m_smart_terrain_id)->cast_script_se_smartterrain();
					if (!p_server_smart)
					{
						MESSAGEWR("can't cast to Script_SE_SmartTerrain!");
						return false;
					}

					p_server_smart->set_alarm();

					if (p_enemy->ID() == DataBase::Storage::getInstance().getActor()->ID() && p_server_smart->getBaseOnActorControl())
					{
						p_server_smart->getBaseOnActorControl()->actor_attack();
					}
				}

				CSE_Abstract* const p_server_enemy = ai().alife().objects().object(p_enemy->ID());
				if (!p_server_enemy)
				{
					MESSAGEWR("Can't find server instance by ID %d", p_enemy->ID());
					return false;
				}

				CSE_ALifeMonsterAbstract* const p_server_enemy_monster = p_server_enemy->cast_monster_abstract();
				if (!p_server_enemy_monster)
				{
					MESSAGEWR("can't cast to CSE_ALifeMonsterAbstract!");
					return false;
				}
				bool is_exist_for_object = (Script_SimulationObjects::getInstance().getObjects().find(p_server_monster->m_group_id) != Script_SimulationObjects::getInstance().getObjects().end());
				bool is_not_exist_for_enemy = (Script_SimulationObjects::getInstance().getObjects().find(p_server_enemy_monster->m_group_id) == Script_SimulationObjects::getInstance().getObjects().end());
				if (p_server_enemy_monster->m_group_id != Globals::kUnsignedInt16Undefined && p_server_monster->m_group_id != Globals::kUnsignedInt16Undefined && is_exist_for_object && is_not_exist_for_enemy && (p_server_monster->o_Position.distance_to(p_server_enemy_monster->o_Position) > 900.0f))
					return false;
				
			}

			return is_object_enemy;
		}

		void Script_SchemeXRCombatIgnore::hit_callback(CScriptGameObject* const p_client_object, const float amount, const Fvector& local_direction, CScriptGameObject* const p_client_who, const std::int16_t bone_index)
		{
			if (p_client_who == nullptr)
				return;

			if (!amount)
				return;

			if (p_client_who->ID() == DataBase::Storage::getInstance().getActor()->ID())
			{
				const DataBase::Data_Overrides& overrides = DataBase::Storage::getInstance().getStorage().at(this->m_npc->ID()).getOverrides();
				if (overrides.getCombatIgnoreKeepWhenAttacked() == false)
					this->m_p_storage->setXRCombatIgnoreEnabled(false);
			}
		}

		bool Script_SchemeXRCombatIgnore::is_enemy_xr_combat_ignore(CScriptGameObject* const p_client_object, CScriptGameObject* const p_enemy, DataBase::Storage_Scheme* p_storage)
		{
			if (!p_client_object)
			{
				MESSAGEWR("p_client_object == nullptr!");
				return false;
			}

			if (!p_enemy)
			{
				MESSAGEWR("p_enemy == nullptr!");
				return false;
			}

			if (!p_storage)
			{
				MESSAGEWR("p_storage == nullptr!");
				return false;
			}

			if (!p_client_object->Alive())
			{
				return false;
			}

			if (p_client_object->critically_wounded())
				return true;

			if (p_storage->isXRCombatIgnoreEnabled() == false)
				return true;

			if (DataBase::Storage::getInstance().getStorage().find(p_client_object->ID()) == DataBase::Storage::getInstance().getStorage().end())
			{
				MESSAGEWR("early calling storage maybe not prepared!");
				return true;
			}

			const DataBase::Data_Overrides& overrides = DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getOverrides();
			std::uint16_t object_id = p_client_object->ID();
			const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(p_client_object->ID());

			DataBase::Storage::getInstance().setStorageEnemyID(object_id, p_enemy->ID());


			if (p_enemy->ID() != DataBase::Storage::getInstance().getActor()->ID())
			{
				for (const std::pair<xr_string, xr_string>& it : Script_GlobalHelper::getInstance().getXRCombatIgnoreSmartsByNoAssaultZones())
				{
					CScriptGameObject* p_zone = nullptr;
					if (DataBase::Storage::getInstance().getZoneByName().find(it.first) == DataBase::Storage::getInstance().getZoneByName().end())
					{
						MESSAGEWR("can't find zone by name %s", it.first.c_str());
						p_zone = nullptr;
					}
					else
					{
						p_zone = DataBase::Storage::getInstance().getZoneByName().at(it.first);
					}


					if (p_zone && (Globals::Utils::is_npc_in_zone(p_client_object, p_zone) || Globals::Utils::is_npc_in_zone(p_enemy, p_zone)))
					{
						Script_SE_SmartTerrain* p_server_smart = nullptr;

						if (Script_SimulationBoard::getInstance().getSmartTerrainsByName().find(it.second) == Script_SimulationBoard::getInstance().getSmartTerrainsByName().end())
						{
							MESSAGEWR("can't get smart_terrain by name %s", it.second.c_str());
							p_server_smart = nullptr;
						}
						else
						{
							p_server_smart = Script_SimulationBoard::getInstance().getSmartTerrainsByName().at(it.second);
						}

						if (p_server_smart && p_server_smart->getBaseOnActorControl() && p_server_smart->getBaseOnActorControl()->getState() != Script_SmartTerrainControl_States::kAlarm)
						{
							return false;
						}
					}
				}
			}

			CSE_Abstract* const p_server_object = ai().alife().objects().object(p_enemy->ID());

			if (!p_server_object)
			{
				MESSAGEWR("can't find server instance by ID %d", p_enemy->ID());
			}
			else
			{
				CSE_ALifeMonsterAbstract* const p_server_monster = p_server_object->cast_monster_abstract();

				if (p_server_monster && p_server_monster->m_smart_terrain_id && p_server_monster->m_smart_terrain_id != Globals::kUnsignedInt16Undefined)
				{
					Script_SE_SmartTerrain* const p_server_enemy_smart = ai().alife().objects().object(p_server_monster->m_smart_terrain_id)->cast_script_se_smartterrain();
					xr_string smart_terrain_name = p_server_enemy_smart->name_replace();

					if (Script_GlobalHelper::getInstance().getXRCombatIgnoreSmarts().find(smart_terrain_name) != Script_GlobalHelper::getInstance().getXRCombatIgnoreSmarts().end())
						return false;
				}
			}

			if (overrides.getCombatIgnore().IsEmpty() == false)
			{ // Lord: интересно проверить overrides
				xr_string result_name = XR_LOGIC::pick_section_from_condlist(p_enemy, p_client_object, overrides.getCombatIgnore().getCondlist());

				if (result_name == Globals::kStringTrue)
					return false;

				return true;
			}

			return true;
		}

		void Script_SchemeXRCombatIgnore::add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini, const xr_string& scheme_name, const xr_string& section_name, DataBase::Storage_Scheme& storage)
		{
			Script_SchemeXRCombatIgnore* const p_scheme = new Script_SchemeXRCombatIgnore(p_client_object, storage);
			storage.setAction(p_scheme);
		}

		void Script_SchemeXRCombatIgnore::set_combat_ignore_checker(CScriptGameObject* const p_npc, CScriptIniFile* const p_ini, const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
		{
			DataBase::Storage_Scheme* const p_storage = XR_LOGIC::assign_storage_and_bind(p_npc, p_ini, scheme_name, "", "");
		}

		void Script_SchemeXRCombatIgnore::reset_combat_ignore_checker(CScriptGameObject* const p_npc, const xr_string& scheme_name, const DataBase::Storage_Data& p_storage, const xr_string& section_name)
		{
			if (p_storage.getSchemes().find("combat_ignore") == p_storage.getSchemes().end())
			{
				MESSAGEWR("you passed wrong scheme! Must be 'combat_ignore'");
				return;
			}

			DataBase::Storage_Scheme* const p_storage_scheme = p_storage.getSchemes().at("combat_ignore");


			std::function<bool(CScriptGameObject* const, CScriptGameObject* const)> data = std::bind(&Script_SchemeXRCombatIgnore::enemy_callback, static_cast<Script_SchemeXRCombatIgnore*>(p_storage_scheme->getAction()), std::placeholders::_1, std::placeholders::_2);
			p_npc->set_enemy_callback(data);

			DataBase::Storage::getInstance().setStorageSchemesActions(p_npc->ID(), scheme_name, p_storage_scheme->getAction());
			DataBase::Storage::getInstance().setStorageSchemesActionSchemeIDForUnSubscribing(p_npc->ID(), scheme_name, p_storage_scheme->getAction()->getSchemeID());

			p_storage_scheme->setXRCombatIgnoreEnabled(true);
		}

		void Script_SchemeXRCombatIgnore::disable_scheme(CScriptGameObject* const p_client_object, const xr_string& scheme_name)
		{
			p_client_object->delete_enemy_callback();
			DataBase::Storage::getInstance().UnsubscribeAction(p_client_object->ID(), scheme_name);
		}

	}
}