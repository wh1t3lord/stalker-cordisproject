#include "stdafx.h"
#include "Script_Binder_Stalker.h"

namespace Cordis
{
namespace Scripts
{
Script_Binder_Stalker::Script_Binder_Stalker(CScriptGameObject* object) : inherited(object), m_is_loaded(false), m_is_first_update(false), m_last_update(0), m_enemy_helicopter_id(0) {}

Script_Binder_Stalker::~Script_Binder_Stalker(void) {}

void Script_Binder_Stalker::reinit(void) 
{
    CScriptBinderObject::reinit();
 
    Script_StateManager* const p_state_manager = new Script_StateManager(this->m_object);
    DataBase::Storage::getInstance().setStorageStateManager(this->m_object, p_state_manager);
    
    Script_MoveManager* const p_move_manager = new Script_MoveManager(this->m_object);
    
    DataBase::Storage::getInstance().setStorageMoveManager(this->m_object, p_move_manager);
}

void Script_Binder_Stalker::reload(LPCSTR section_name) 
{
    
}

bool Script_Binder_Stalker::net_Spawn(SpawnType DC) 
{
    MESSAGEI("add to database %s", this->m_object->Name());

    xr_string visual_name = Globals::Utils::cfg_get_string(Globals::get_system_ini(), this->m_object->Section(), "set_visual");
    if (visual_name.empty() == false)
    {
        if (visual_name == "actor_visual")
        {
            this->m_object->set_visual_name(DataBase::Storage::getInstance().getActor()->get_visual_name());
        }
        else
        {
            this->m_object->set_visual_name(visual_name.c_str());
        }
    }

    // Lord: xrs_dyn_music - реализовать наверное

    DataBase::Storage::getInstance().addObject(this->m_object);

    this->m_object->apply_loophole_direction_distance(1.0f);

    if (this->m_is_loaded == false)
    {
        CScriptIniFile* const p_spawn_ini = this->m_object->spawn_ini();
        xr_string filename;

        if (p_spawn_ini)
        {
            filename = Globals::Utils::cfg_get_string(p_spawn_ini, "logic", "cfg");
            MESSAGEI("filename about logic %s", filename.c_str());
        }

          
        if (filename.empty() == false)
        {
            CScriptIniFile ini(filename.c_str());
            Globals::load_info(this->m_object, &ini);
        }
        else
        {
            if (p_spawn_ini)
            {
                Globals::load_info(this->m_object, p_spawn_ini);
            }
            else
            {
                CScriptIniFile ini("scripts\\dummy.ltx");
                Globals::load_info(this->m_object, &ini);
            }
        }
    }

    if (!this->m_object->Alive())
    {
        this->m_object->death_sound_enabled(false);
        // Lord: когда будет release_manager 
        return true;
    }

    if (DataBase::Storage::getInstance().getGoodwill_Relations().find(this->m_object->ID()) != DataBase::Storage::getInstance().getGoodwill_Relations().end())
    {
        if (DataBase::Storage::getInstance().getActor())
        {
            Globals::GameRelations::set_npcs_relation(this->m_object, DataBase::Storage::getInstance().getActor(), DataBase::Storage::getInstance().getGoodwill_Relations().at(this->m_object->ID()));
        }
    }

    if (DataBase::Storage::getInstance().getGoodwill_Sympathy().find(this->m_object->ID()) != DataBase::Storage::getInstance().getGoodwill_Sympathy().end())
    {
        Globals::GameRelations::set_npc_sympathy(this->m_object, DataBase::Storage::getInstance().getGoodwill_Sympathy().at(this->m_object->ID()));
    }

    DataBase::Storage::getInstance().addEnemy(this->m_object);
    this->m_enemy_helicopter_id = DataBase::Storage::getInstance().getHelicopterCount() - 1;

    // Lord: actor_stats и release_body_manager когда будет тогда и дописать 

    Globals::init_npc_sound(this->m_object);

    if (Globals::get_object_story_id(this->m_object->ID()) == "zat_b53_artefact_hunter_1")
    {
        CScriptActionPlanner* const p_planner = Globals::get_script_action_planner(this->m_object);
        if (p_planner == nullptr)
        {
            R_ASSERT2(false, "can't cast and use!");
            return true;
        }

        p_planner->remove_evaluator(StalkerDecisionSpace::eWorldPropertyAnomaly);
        p_planner->add_evaluator(StalkerDecisionSpace::eWorldPropertyAnomaly, new CPropertyEvaluatorConst<CScriptGameObject>(false));
    }

    Script_SchemeXRReachTask::add_reach_task_action(this->m_object);

    CSE_Abstract* const p_server_object = ai().alife().objects().object(this->m_object->ID());

    if (p_server_object)
    {
        if (DataBase::Storage::getInstance().getSpawnedVertexByID().find(p_server_object->ID) != DataBase::Storage::getInstance().getSpawnedVertexByID().end())
        {
            this->m_object->SetNpcPosition(Globals::Game::level::vertex_position(DataBase::Storage::getInstance().getSpawnedVertexByID().at(p_server_object->ID)));
            DataBase::Storage::getInstance().setSpawnedVertexByID(this->m_object->ID(), 0);
        }
        else if (DataBase::Storage::getInstance().getOfflineObjects().find(p_server_object->ID) != DataBase::Storage::getInstance().getOfflineObjects().end())
        {
            if (DataBase::Storage::getInstance().getOfflineObjects().at(p_server_object->ID).first)
            {
                Fvector new_position = Globals::Game::level::vertex_position(DataBase::Storage::getInstance().getOfflineObjects().at(p_server_object->ID).first);
                MESSAGEI("changed position to %f %f %f", new_position.x, new_position.y, new_position.z);
                this->m_object->SetNpcPosition(new_position);
            }
        }
        else if (p_server_object->cast_monster_abstract()->m_smart_terrain_id != Globals::kUnsignedInt16Undefined)
        {
            Script_SE_SmartTerrain* const p_smart = ai().alife().objects().object(p_server_object->cast_monster_abstract()->m_smart_terrain_id)->cast_script_se_smartterrain();
            if (p_smart == nullptr)
            {
                R_ASSERT2(false, "at this point of code must be a smart_terrain cast!");
                return true;
            }

            if (p_smart->getArrivingNpc().find(p_server_object->ID) == p_smart->getArrivingNpc().end() || !p_smart->getArrivingNpc().at(p_server_object->ID))
            {
                CALifeSmartTerrainTask* const p_task = p_smart->getJobData().at(p_smart->getNpcInfo().at(p_server_object->ID).m_job_id)->m_alife_task;
                this->m_object->SetNpcPosition(p_task->position());
            }
        }
    }

    Globals::setup_gulag_and_logic_on_spawn(this->m_object, DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()), Globals::kSTypeStalker, this->m_is_loaded);

    if (Globals::character_community(this->m_object) != "zombied")
    {
        Script_ActionPostCombatWait::add_post_combat_idle(this->m_object);
    }

    this->m_object->group_throw_time_interval(2000);

    return true; 
}

void Script_Binder_Stalker::net_Destroy(void)
{
    MESSAGEI("delete from database %s", this->m_object->Name());

    DataBase::Storage::getInstance().setXRCombatIgnoreFightingWithActorNpcs(this->m_object->ID(), false);

    XR_SOUND::stop_sounds_by_id(this->m_object->ID());

    const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(this->m_object->ID());

    if (storage.getActiveSchemeName().empty() == false)
    {
        if (storage.getSchemes().find(storage.getActiveSchemeName()) != storage.getSchemes().end())
        {
            DataBase::Script_IComponentScheme* const p_storage = storage.getSchemes().at(storage.getActiveSchemeName());
            if (p_storage)
            {
                for (Script_ISchemeEntity* it : p_storage->getActions())
                {
                    if (it)
                    {
                        it->net_destroy(this->m_object);
                    }
                }
            }
        }
    }

    if (storage.getSchemes().find("reach_task") != storage.getSchemes().end())
    {
        DataBase::Script_IComponentScheme* const p_storage = storage.getSchemes().at("reach_task");

        if (p_storage)
        {
            for (Script_ISchemeEntity* it : p_storage->getActions())
            {
                if (it)
                    it->net_destroy(this->m_object);
            }
        }
    }


    const DataBase::Data_Overrides& overrides = storage.getOverrides();
    
    if (overrides.getOnOfflineCondlist().empty() == false)
    {
        XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_object, overrides.getOnOfflineCondlist());
    }

    if (DataBase::Storage::getInstance().getOfflineObjects().find(this->m_object->ID()) != DataBase::Storage::getInstance().getOfflineObjects().end())
    {
        DataBase::Storage::getInstance().setOfflineObjects(this->m_object->ID(), storage.getActiveSectionName());
        DataBase::Storage::getInstance().setOfflineObjects(this->m_object->ID(), this->m_object->level_vertex_id());
    }

    if (this->m_enemy_helicopter_id)
    {
        DataBase::Storage::getInstance().deleteEnemy(this->m_enemy_helicopter_id);
    }

    DataBase::Storage::getInstance().deleteObject(this->m_object);

    CScriptBinderObject::net_Destroy();
}

void Script_Binder_Stalker::net_Import(NET_Packet* packet) {}

void Script_Binder_Stalker::net_Export(NET_Packet* packet) {}

void Script_Binder_Stalker::shedule_Update(std::uint32_t time_delta) 
{
    CScriptBinderObject::shedule_Update(time_delta);

    if (DataBase::Storage::getInstance().getXRCombatIgnoreFightingWithActorNpcs().find(this->m_object->ID()) != DataBase::Storage::getInstance().getXRCombatIgnoreFightingWithActorNpcs().end())
    {
        if (DataBase::Storage::getInstance().getXRCombatIgnoreFightingWithActorNpcs().at(this->m_object->ID()))
        {
			if (this->m_object->GetBestEnemy() == nullptr)
			{
                DataBase::Storage::getInstance().setXRCombatIgnoreFightingWithActorNpcs(this->m_object->ID(), false);
			}
        }
    }

    bool is_object_alive = this->m_object->Alive();
    
    if ((DataBase::Storage::getInstance().getStorage().find(this->m_object->ID()) != DataBase::Storage::getInstance().getStorage().end()) && is_object_alive && (DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getActiveSchemeName().empty() == false))
    {
		const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(this->m_object->ID());

		DataBase::Script_ComponentScheme_XRCombat* p_storage = nullptr;

		if (storage.getSchemes().find("combat") != storage.getSchemes().end())
		{
			p_storage = reinterpret_cast<DataBase::Script_ComponentScheme_XRCombat*>(storage.getSchemes().at("combat"));
		}

        bool is_switched = false;

        CScriptActionPlanner* const p_planner = Globals::get_script_action_planner(this->m_object);

        if (p_planner->initialized() && p_planner->current_action_id() == StalkerDecisionSpace::eWorldOperatorCombatPlanner)
        {
            const DataBase::Data_Overrides& overrides = storage.getOverrides();

            if (overrides.isEmpty() == false)
            {
                if (overrides.getOnCombat().IsEmpty() == false)
                {
                    XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_object, overrides.getOnCombat().getCondlist());
                }

                if (p_storage && (p_storage->getLogic().empty() == false))
                {
                    if (!XR_LOGIC::try_switch_to_another_section(this->m_object, *p_storage, DataBase::Storage::getInstance().getActor()))
                    {
                        if (overrides.getCombatType().IsEmpty() == false)
                        {
                            XR_COMBAT::set_combat_type(this->m_object, DataBase::Storage::getInstance().getActor(), overrides.getCombatType().getCondlist());
                        }
                    }
                }
            }
            else
            {
                XR_COMBAT::set_combat_type(this->m_object, DataBase::Storage::getInstance().getActor(), p_storage->getXRCombatCombatTypeCondlist());
            }
        }

        if (!is_switched)
        {
            XR_LOGIC::try_switch_to_another_section(this->m_object, *storage.getSchemes().at(storage.getActiveSchemeName()), DataBase::Storage::getInstance().getActor());
        }
    }
    else
    {
        if (DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getSchemes().find("combat") == DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getSchemes().end())
        {
            return;
        }

        DataBase::Script_ComponentScheme_XRCombat* const p_storage = reinterpret_cast<DataBase::Script_ComponentScheme_XRCombat*>(DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getSchemes().at("combat"));

        XR_COMBAT::set_combat_type(this->m_object, DataBase::Storage::getInstance().getActor(), p_storage->getXRCombatCombatTypeCondlist());
    }

    if (this->m_is_first_update == false)
    {
        if (is_object_alive == false)
        {
            // Lord: death_manager когда будет
        }

        this->m_is_first_update = true;
    }

    if (Globals::get_time_global() - this->m_last_update > 1000)
    {
        Script_SRLightManager::getInstance().check_light(this->m_object);
        this->m_last_update = Globals::get_time_global();
    }

    if (is_object_alive)
    {
        // Lord: ph_door, когда будет
    }

    if (DataBase::Storage::getInstance().getStorage().find(this->m_object->ID()) != DataBase::Storage::getInstance().getStorage().end())
    {
        const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(this->m_object->ID());

        if (storage.getStateManager())
        {
            Script_StateManager* const p_state_manager = storage.getStateManager();
            if (is_object_alive)
            {
                p_state_manager->update();

                if (p_state_manager->isCombat() == false && p_state_manager->isAlife())
                {
                    // Lord: когда trader_manager будет!
                }
            }
            else
            {
                // Lord: здесь происходит удаление, ибо НПС мёртв, можно ли без него обойтись и удалить в деструкторе? ПроверитЬ!
            }
        }
    }

    if (is_object_alive)
    {
        XR_SOUND::update(this->m_object->ID());
        Script_SchemeXRMeet::process_npc_usability(this->m_object);

        // Lord: пока что без бессмертия
    }

    Script_SE_SimulationSquad* const p_squad = Globals::get_object_squad(this->m_object->ID());

    if (p_squad)
    {
        if (p_squad->commander_id() == this->m_object->ID())
            p_squad->update();
    }

    this->m_object->info_clear();

    if (is_object_alive)
    {
        const xr_string& active_section_name = DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getActiveSectionName();

        if (active_section_name.empty() == false)
        {
            this->m_object->info_add(xr_string("section: ").append(active_section_name).c_str());
        }

        CScriptGameObject* const p_best_enemy = this->m_object->GetBestEnemy();
        if (p_best_enemy)
        {
            this->m_object->info_add(xr_string("enemy: ").append(p_best_enemy->Name()).c_str());
        }

        const CDangerObject* const p_danger = this->m_object->GetBestDanger();
        if (p_danger)
        {
            this->m_object->info_add(xr_string("danger: ").append(XR_DANGER::get_danger_name(p_danger)).c_str());
        }

        this->m_object->info_add(xr_string(this->m_object->Name()).append("[").append(std::to_string(this->m_object->Team()).c_str()).append("]").append("[").append(std::to_string(this->m_object->Squad()).c_str()).append("]").append("[").append(std::to_string(this->m_object->Group()).c_str()).append("]").c_str());

        if (ai().alife().objects().object(this->m_object->ID()) == nullptr)
            return;

        if (p_squad)
        {
            this->m_object->info_add(xr_string("squad_id: ").append(p_squad->name()).c_str());

            CSE_Abstract* const p_target = ai().alife().objects().object(p_squad->getAssignedTargetID());

            xr_string target_name = "none";
            if (p_target)
            {
                target_name = p_target->name_replace();
            }

            this->m_object->info_add(xr_string("current_action: ").append(p_squad->getCurrentAction().getName()).append("[").append(target_name).append("]").c_str());
        }
    }

}

void Script_Binder_Stalker::save(NET_Packet* output_packet)
{
    Globals::set_save_marker(*output_packet, Globals::kSaveMarkerMode_Save, false, "Script_Binder_Stalker");
    inherited::save(output_packet);
    
    Globals::set_save_marker(*output_packet, Globals::kSaveMarkerMode_Save, true, "Script_Binder_Stalker");
}

void Script_Binder_Stalker::load(IReader* input_packet)
{
    this->m_is_loaded = true;
    Globals::set_save_marker(*input_packet, Globals::kSaveMarkerMode_Load, false, "Script_Binder_Stalker");

    XR_LOGIC::load_object(this->m_object, *input_packet);
    

    Globals::set_save_marker(*input_packet, Globals::kSaveMarkerMode_Load, true, "Script_Binder_Stalker");
}

bool Script_Binder_Stalker::net_SaveRelevant(void) { return true; }

void Script_Binder_Stalker::net_Relcase(CScriptGameObject* object) {}

void Script_Binder_Stalker::waypoint_callback(CScriptGameObject* p_client_object, const std::uint32_t action_type_movement, const std::uint32_t point_index)
{
}

void Script_Binder_Stalker::hit_callback(CScriptGameObject* p_client_object, const float amount, const Fvector& local_direction, CScriptGameObject* p_client_who, const std::int16_t bone_index)
{
}

void Script_Binder_Stalker::death_callback(CScriptGameObject* p_client_victim, CScriptGameObject* p_client_who)
{
}

void Script_Binder_Stalker::hear_callback(CScriptGameObject* p_client_object, const std::uint16_t who_id, const int sound_type, const Fvector& sound_position, const float sound_power)
{
}

void Script_Binder_Stalker::use_callback(CScriptGameObject* p_client_object, CScriptGameObject* p_client_who)
{
}
} // namespace Scripts
} // namespace Cordis
