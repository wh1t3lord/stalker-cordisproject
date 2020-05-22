#include "stdafx.h"

namespace Cordis
{
	namespace Scripts
	{
		namespace XR_LOGIC
		{
			void disable_generic_schemes(CScriptGameObject* const p_client_object, const std::uint32_t stype)
			{
				if (!p_client_object)
				{
					R_ASSERT2(false, "object is null!");
					return;
				}

				// Lord: доделать 
				switch (stype)
				{
				case Globals::kSTypeStalker:
				{
					XR_COMBAT::disable_scheme(p_client_object, "combat");
					Script_SchemeXRHit::disable_scheme(p_client_object, "hit");
					Script_SchemeXRCombatIgnore::disable_scheme(p_client_object, "combat_ignore");
					// Lord: дописать когда будет stalker_generic
					break;
				}
				case Globals::kSTypeMobile:
				{
					Script_SchemeMobCombat::disable_scheme(p_client_object, "mob_combat");
					Script_SchemeXRCombatIgnore::disable_scheme(p_client_object, "combat_ignore");
					// Lord: дописать когда будет stalker_generic
					break;
				}
				case Globals::kSTypeItem:
				{
					Script_SchemePHOnHit::disable_scheme(p_client_object, "ph_on_hit");
					break;
				}
				case Globals::kSTypeHelicopter:
				{
					Script_SchemeXRHit::disable_scheme(p_client_object, "hit");
					break;
				}
				}
			}

			void enable_generic_schemes(CScriptIniFile* const p_ini, CScriptGameObject* const p_client_object,
				const std::uint32_t stype, const xr_string& section_logic_name)
			{
				// Lord: доделать когда будут сделаны все схемы
				if (!p_client_object)
				{
					R_ASSERT2(false, "object is null!");
					return;
				}

				if (!p_ini)
				{
					R_ASSERT2(false, "object is null!");
					return;
				}

				switch (stype)
				{
				case Globals::kSTypeStalker:
				{
					Script_EvaluatorDanger::set_danger(p_client_object, p_ini, "danger", "danger", "");
					Script_EvaluatorGatherItems::set_gather_items(p_client_object, p_ini, "gather_items", "gather_items", "");

					xr_string combat_section = Globals::Utils::cfg_get_string(p_ini, section_logic_name, "on_combat");
					XR_COMBAT::set_combat_checker(p_client_object, p_ini, "combat", combat_section, "");

					// Lord: когда будет stalker_generic

					xr_string info_section_name = Globals::Utils::cfg_get_string(p_ini, section_logic_name, "info");
					if (info_section_name.empty() == false)
					{
						// Lord: когда будет stalker_generic
					}

					xr_string hit_section_name = Globals::Utils::cfg_get_string(p_ini, section_logic_name, "on_hit");
					if (hit_section_name.empty() == false)
					{
						Script_SchemeXRHit::set_hit_checker(p_client_object, p_ini, "hit", hit_section_name, "");
					}

					xr_string actor_dialogs_section = Globals::Utils::cfg_get_string(p_ini, section_logic_name, "actor_dialogs");
					if (actor_dialogs_section.empty() == false)
					{
						// Lord: если правда используется то реализовать!
						R_ASSERT(false);
					}

					xr_string wounded_section_name = Globals::Utils::cfg_get_string(p_ini, section_logic_name, "wounded");
					Script_SchemeXRWounded::set_wounded(p_client_object, p_ini, "wounded", wounded_section_name, "");
					Script_SchemeXRAbuse::set_abuse(p_client_object, p_ini, "abuse", section_logic_name, "");

					Script_ActionXRHelpWounded::set_help_wounded(p_client_object, p_ini, "help_wounded", "", "");

					// Lord: когда будет corpse_detection

					xr_string meet_section_name = Globals::Utils::cfg_get_string(p_ini, section_logic_name, "meet");
					Script_SchemeXRMeet::set_meet(p_client_object, p_ini, "meet", meet_section_name, "");

					xr_string death_section_name = Globals::Utils::cfg_get_string(p_ini, section_logic_name, "on_death");
					Script_SchemeXRDeath::set_scheme(p_client_object, p_ini, "death", death_section_name, "");

					Script_SchemeXRCombatIgnore::set_combat_ignore_checker(p_client_object, p_ini, "combat_ignore", "", "");
					Script_SchemeXRReachTask::set_reach_task(p_client_object, p_ini, "reach_task", "", "");
					break;
				}
				}
			}

			void reset_generic_schemes_on_scheme_switch(
				CScriptGameObject* const p_client_object, const xr_string& scheme_name, const xr_string& section_name)
			{
				if (!p_client_object)
				{
					R_ASSERT2(false, "object is null!");
					return;
				}

				const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(p_client_object->ID());

				switch (storage.getSchemeType())
				{
				case Globals::kSTypeStalker: {
					// Lord: доделать  

					Script_SchemeXRMeet::reset_meet(p_client_object, scheme_name, storage, section_name);
					Script_ActionXRHelpWounded::reset_help_wounded(p_client_object, scheme_name, section_name);
					Script_SchemeXRWounded::reset_wounded(p_client_object, scheme_name, storage, section_name);
					Script_SchemeXRDeath::reset_death(p_client_object, scheme_name, section_name);
					Script_EvaluatorGatherItems::reset_gather_items(p_client_object, scheme_name, section_name);
					Script_SchemeXRCombatIgnore::reset_combat_ignore_checker(p_client_object, scheme_name, storage, section_name);

					// Lord: stalker_generic, restrictor_manager


					break;
				}
				case Globals::kSTypeMobile: {
					mob_release(p_client_object, scheme_name);
					if (Globals::get_script_clsid(CLSID_SE_MONSTER_BLOODSUCKER) == p_client_object->clsid())
					{
						if (scheme_name == "nil" || scheme_name.empty()) // LorD: проверить будет ли дропать nil, если будет то найти и исправить когда это будет, чтобы все "nil" просто проверялись всегда как .empty()
						{
							p_client_object->set_manual_invisibility(false);
						}
						else
						{
							p_client_object->set_manual_invisibility(true);
						}
					}

					// Lord: доделать когда будут сделаны следующие xr_hear, restrictor_manager, stalker_generic, xr_combat_ignore

					break;
				}
				case Globals::kSTypeItem: {
					p_client_object->SetNonscriptUsable(true);
					if (Globals::get_script_clsid(CLSID_CAR) == p_client_object->clsid())
					{
						//  p_client_object->car, Lord: наверное ph_minigun, но как сделать если там простой класс
						mob_release(p_client_object, scheme_name);
					}

					break;
				}
				case Globals::kSTypeHelicopter: {
					break;
				}
				case Globals::kSTypeRestrictor: {
					break;
				}
				}
			}

			void activate_by_section(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
				const xr_string& section_name, const xr_string& gulag_name, const bool is_loading)
			{
				xr_string _section_name; // @ if argument is empty string for section_name
				if (!p_ini)
				{
					R_ASSERT2(false, "object is null!");
					return;
				}

				if (!p_client_object)
				{
					R_ASSERT2(false, "object is null!");
					return;
				}

				const std::uint16_t npc_id = p_client_object->ID();

				if (!is_loading)
				{
					DataBase::Storage::getInstance().setStorageActivationTime(npc_id, Globals::get_time_global());
					DataBase::Storage::getInstance().setStorageActivationGameTime(npc_id, Globals::Game::get_game_time());
				}

				if (section_name.empty()) // LorD: проверить будет ли дропать nil, если будет то найти и исправить когда это будет, чтобы все "nil" просто проверялись всегда как .empty()
				{
					DataBase::Storage::getInstance().setStorageOverrides(npc_id, DataBase::Data_Overrides());
					reset_generic_schemes_on_scheme_switch(p_client_object, "", ""); // имеет ли смысл? // LorD: проверить будет ли дропать nil, если будет то найти и исправить когда это будет, чтобы все "nil" просто проверялись всегда как .empty()
					DataBase::Storage::getInstance().setStorageActiveSectionName(npc_id, "");
					DataBase::Storage::getInstance().setStorageActiveSchemeName(npc_id, "");
					return;
				}

				if (section_name.empty())
				{
					CSE_ALifeDynamicObject* p_server_smart = XR_GULAG::get_npc_smart(p_client_object);
					if (!p_server_smart)
					{
						R_ASSERT2(false, "section is NIL and NPC not in smart_terrain!");
					}

					const xr_map<std::uint32_t, JobDataSmartTerrain*>& job =
						p_server_smart->cast_script_se_smartterrain()->getJobData();

					if (job.at(p_client_object->ID()))
					{
						_section_name = job.at(p_client_object->ID())->m_job_id.first;
					}
				}

				if (section_name.empty())
				{
					if (!p_ini->section_exist(_section_name.c_str()))
					{
						R_ASSERT2(false, "doesnt exist!");
					}
				}
				else
				{
					if (!p_ini->section_exist(section_name.c_str()))
					{
						R_ASSERT2(false, "doesnt exist!");
					}
				}

				xr_string _scheme_name = section_name.empty() ? Globals::Utils::get_scheme_by_section(_section_name) :
					Globals::Utils::get_scheme_by_section(xr_string(section_name));
				if (_scheme_name.empty())
				{
					R_ASSERT2(false, "can't detect scheme name");
				}

				DataBase::Storage::getInstance().setStorageOverrides(
					npc_id, cfg_get_overrides(p_ini, section_name.empty() ? _section_name : section_name, p_client_object));

				reset_generic_schemes_on_scheme_switch(
					p_client_object, _scheme_name, section_name.empty() ? _section_name : section_name);

				if (!Script_GlobalHelper::getInstance().getSchemesSetSchemeCallbacks()[_scheme_name])
				{
					R_ASSERT2(false, "doesn't exist function for _scheme_name! Check Script_GlobalHelper::ctor()");
					return;
				}

				Script_GlobalHelper::getInstance().getSchemesSetSchemeCallbacks()[_scheme_name](
					p_client_object, p_ini, _scheme_name, section_name.empty() ? _section_name : section_name, gulag_name);

				DataBase::Storage::getInstance().setStorageActiveSectionName(
					npc_id, section_name.empty() ? _section_name : section_name);
				DataBase::Storage::getInstance().setStorageActiveSchemeName(npc_id, _scheme_name);

				if (DataBase::Storage::getInstance().getStorage().at(npc_id).getSchemeType() == Globals::kSTypeStalker)
				{
					Globals::Utils::send_to_nearest_accessible_vertex(p_client_object, p_client_object->level_vertex_id());
					for (Script_ISchemeEntity* it :
						DataBase::Storage::getInstance().getStorage().at(npc_id).getSchemes().at(_scheme_name)->getActions())
					{
						if (it->isActionSubscribed())
							it->activate_scheme(is_loading, p_client_object);
					}
				}
				else
				{
					for (Script_ISchemeEntity* it :
						DataBase::Storage::getInstance().getStorage().at(npc_id).getSchemes().at(_scheme_name)->getActions())
					{
						if (it->isActionSubscribed())
							it->reset_scheme(is_loading, p_client_object);
					}
				}
			}

			// Lord: проверить на memory leak, особенно проверить факт будет ли повторно заходить в ветку else где выделяется память
			// под p_actual_ini
			CScriptIniFile* configure_schemes(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
				const xr_string& ini_filename, std::uint32_t stype, const xr_string& section_logic_name,
				const xr_string& gulag_name)
			{
				const std::uint16_t npc_id = p_client_object->ID();
				const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(npc_id);

				if (!storage.getActiveSectionName().empty())
				{
					for (Script_ISchemeEntity* it : storage.getSchemes().at(storage.getActiveSchemeName())->getActions())
					{
						if (it)
						{
							if (it->isActionSubscribed())
								it->deactivate(p_client_object);
						}
					}
				}

				xr_string actual_ini_filename;
				CScriptIniFile* p_actual_ini = nullptr;
				static bool is_allocated_for_storage =
					false; // если всё таки выделяли память, то в dtor() сторага будем сами удалять
				if (!p_ini->section_exist(section_logic_name.c_str()))
				{
					if (gulag_name.empty())
					{
						actual_ini_filename = ini_filename;
						p_actual_ini = p_ini;
					}
					else
					{
						R_ASSERT2(false, "YOu don't give a job to your entity!");
					}
				}
				else
				{
					xr_string filename = Globals::Utils::cfg_get_string(p_ini, section_logic_name, "cfg");
					if (!filename.empty())
					{
						actual_ini_filename = filename;
						p_actual_ini = new CScriptIniFile(filename.c_str());
						is_allocated_for_storage = true;
						if (!p_actual_ini->section_exist(section_logic_name.c_str()))
						{
							R_ASSERT2(false, "NOT FOUND!");
						}

						return configure_schemes(
							p_client_object, p_actual_ini, actual_ini_filename, stype, section_logic_name, gulag_name);
					}
					else
					{
						if (stype == Globals::kSTypeStalker || stype == Globals::kSTypeMobile)
						{
							Script_SE_SmartTerrain* p_server_smart =
								XR_GULAG::get_npc_smart(p_client_object)->cast_script_se_smartterrain();
							if (p_server_smart)
							{
								JobDataSmartTerrain* job = p_server_smart->getJobData()[npc_id];
								if (job)
								{
									DataBase::Storage::getInstance().setStorageJobIniName(npc_id, job->m_ini_path_name);
								}
							}
						}

						actual_ini_filename = ini_filename;
						p_actual_ini = p_ini;
						is_allocated_for_storage = false; // Lord: Needz here. check it pls
					}
				}

				disable_generic_schemes(p_client_object, stype);
				enable_generic_schemes(p_actual_ini, p_client_object, stype, section_logic_name);

				DataBase::Storage::getInstance().setStorageActiveSectionName(npc_id, "");
				DataBase::Storage::getInstance().setStorageActiveSchemeName(npc_id, "");
				DataBase::Storage::getInstance().setStorageGulagName(npc_id, gulag_name);
				DataBase::Storage::getInstance().setStorageSType(npc_id, stype);
				DataBase::Storage::getInstance().setStorageIniFile(npc_id, p_actual_ini, is_allocated_for_storage);
				DataBase::Storage::getInstance().setStorageInifilename(npc_id, actual_ini_filename);
				DataBase::Storage::getInstance().setStorageSectionLogicName(npc_id, section_logic_name);

				if (stype == Globals::kSTypeStalker)
				{
					// Lord: доделать когда будет trader_manager, spawner
				}

				return storage.getIni();
			}
		} // namespace XR_LOGIC
	}
}