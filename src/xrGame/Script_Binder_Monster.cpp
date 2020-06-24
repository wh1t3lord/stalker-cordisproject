#include "stdafx.h"
#include "Script_Binder_Monster.h"

namespace Cordis
{
namespace Scripts
{
Script_Binder_Monster::Script_Binder_Monster(CScriptGameObject* object) : inherited(object), m_is_loaded(false) {}

Script_Binder_Monster::~Script_Binder_Monster(void) {}

void Script_Binder_Monster::reinit(void) 
{
    inherited::reinit();
}

bool Script_Binder_Monster::net_Spawn(SpawnType DC) 
{
 
    MESSAGE("adding to database %s", this->m_object->Name());
 

    if (!CScriptBinderObject::net_Spawn(DC))
        return false;

    // LorD: должно ли быть так? 
    if (DataBase::Storage::getInstance().getStorage().find(this->m_object->ID()) != DataBase::Storage::getInstance().getStorage().end())
    {
		if (DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getOverrides().getOnOfflineCondlist().empty() == false)
		{
			XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_object, DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getOverrides().getOnOfflineCondlist());
		}
    }


    if (!this->m_object->Alive())
        return true;

    if (ai().alife().objects().object(this->m_object->ID()) == nullptr)
        return false;
   
    DataBase::Storage::getInstance().addObject(this->m_object);

    CSE_Abstract* const p_server_object = ai().alife().objects().object(this->m_object->ID());
    if (DataBase::Storage::getInstance().getSpawnedVertexByID().find(p_server_object->ID) != DataBase::Storage::getInstance().getSpawnedVertexByID().end())
    {
        this->m_object->SetNpcPosition(Globals::Game::level::vertex_position(DataBase::Storage::getInstance().getSpawnedVertexByID().at(p_server_object->ID)));
        DataBase::Storage::getInstance().setSpawnedVertexByID(p_server_object->ID, 0);
    }
    else if (DataBase::Storage::getInstance().getOfflineObjects().find(p_server_object->ID) != DataBase::Storage::getInstance().getOfflineObjects().end())
    {
        if (DataBase::Storage::getInstance().getOfflineObjects().at(p_server_object->ID).first)
        {
 
            Fvector change_pos = Globals::Game::level::vertex_position(DataBase::Storage::getInstance().getOfflineObjects().at(p_server_object->ID).first);
            MESSAGE("changing position for object[%s] from %f %f %f to %f %f %f : level_vertex [%d] to [%d]", p_server_object->name_replace(), p_server_object->position().x, p_server_object->position().y, p_server_object->position().z, change_pos.x, change_pos.y, change_pos.z, p_server_object->cast_alife_dynamic_object()->m_tNodeID, DataBase::Storage::getInstance().getOfflineObjects().at(p_server_object->ID).first);
 
            this->m_object->SetNpcPosition(change_pos);
        }
    }
    else if (p_server_object->cast_monster_abstract()->m_smart_terrain_id != Globals::kUnsignedInt16Undefined)
    {
        Script_SE_SmartTerrain* const p_smart_terrain = ai().alife().objects().object(p_server_object->cast_monster_abstract()->m_smart_terrain_id)->cast_script_se_smartterrain();

        if (p_smart_terrain->getArrivingNpc().find(p_server_object->ID) == p_smart_terrain->getArrivingNpc().end() || p_smart_terrain->getArrivingNpc().at(p_server_object->ID) == nullptr)
        {
            const xr_map<std::uint32_t, JobDataSmartTerrain*>& job_data = p_smart_terrain->getJobData();

            if (p_smart_terrain->getNpcInfo().find(p_server_object->ID) == p_smart_terrain->getNpcInfo().end())
            {
 
                MESSAGEWR("Can't find npc info from smart_terrain by ID %d", p_server_object->ID);
 
                return false;
            }

            std::uint32_t job_id = p_smart_terrain->getNpcInfo().at(p_server_object->ID).m_job_id;
            
            if (job_data.find(job_id) == job_data.end())
            {
 
                MESSAGEWR("Can't find job_data by job_id %d", p_server_object->ID);
 
                return false;
            }

            if (job_data.at(job_id)->m_alife_task == nullptr)
            {
 
                MESSAGEWR("Alife task was nullptr! Can't be!");
 
                return false;
            }

            this->m_object->SetNpcPosition(job_data.at(job_id)->m_alife_task->position());
        }
    }

    Globals::setup_gulag_and_logic_on_spawn(this->m_object, DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()), Globals::kSTypeMobile, this->m_is_loaded);

    return true; 
}

void Script_Binder_Monster::net_Destroy(void) 
{
 
    MESSAGE("deleting from database %s", this->m_object->Name()); 
 

    DataBase::Storage::getInstance().setXRCombatIgnoreFightingWithActorNpcs(this->m_object->ID(), false);

    if (DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getActiveSchemeName().empty() == false)
    {
        DataBase::Script_IComponentScheme* const p_storage = DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getSchemes().at(DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getActiveSchemeName());
        for (Script_ISchemeEntity* const it : p_storage->getActions())
        {
            if (it)
                it->net_destroy(nullptr); // Lord: протестировать насколько я понял там передаётся именно nullptr
        }
    }

    if (DataBase::Storage::getInstance().getOfflineObjects().find(this->m_object->ID()) != DataBase::Storage::getInstance().getOfflineObjects().end())
    {
        DataBase::Storage::getInstance().setOfflineObjects(this->m_object->ID(), this->m_object->level_vertex_id(), DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getActiveSectionName());
    }


    DataBase::Storage::getInstance().deleteObject(this->m_object);
    CScriptBinderObject::net_Destroy();
}

void Script_Binder_Monster::shedule_Update(std::uint32_t time_delta)
{
    inherited::shedule_Update(time_delta);

    if (DataBase::Storage::getInstance().getXRCombatIgnoreFightingWithActorNpcs().find(this->m_object->ID()) != DataBase::Storage::getInstance().getXRCombatIgnoreFightingWithActorNpcs().end())
    {
        if (DataBase::Storage::getInstance().getXRCombatIgnoreFightingWithActorNpcs().at(this->m_object->ID()) && (this->m_object->GetBestEnemy() == nullptr))
            DataBase::Storage::getInstance().setXRCombatIgnoreFightingWithActorNpcs(this->m_object->ID(), false);
    }

    Script_SE_SimulationSquad* const p_server_squad = Globals::get_object_squad(this->m_object->ID());
    bool is_object_alive = this->m_object->Alive();

    if (!is_object_alive)
        return;

    this->m_object->SetTipText("");
    const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(this->m_object->ID());
    if (storage.getActiveSchemeName().empty() == false)
    {
        XR_LOGIC::try_switch_to_another_section(this->m_object, storage.getSchemes().at(storage.getActiveSchemeName()), DataBase::Storage::getInstance().getActor());
    }

    if (p_server_squad)
    {
        if (p_server_squad->commander_id() == this->m_object->ID())
            p_server_squad->update();
    }

    this->m_object->info_clear();

    xr_string active_section_name = storage.getActiveSectionName();
    if (active_section_name.empty() == false)
    {
        this->m_object->info_add(xr_string("section: ").append(active_section_name).c_str());
    }

    CScriptGameObject* const p_best_enemy = this->m_object->GetBestEnemy();
    if (p_best_enemy)
    {
        this->m_object->info_add(xr_string("enemy: ").append(p_best_enemy->Name()).c_str());
    }

    this->m_object->info_add(xr_string(this->m_object->Name()).append("[").append(std::to_string(this->m_object->Team()).c_str()).append("]").append("[").append(std::to_string(this->m_object->Squad()).c_str()).append("]").append("[").append(std::to_string(this->m_object->Group()).c_str()).append("]").c_str());

    if (!ai().alife().objects().object(this->m_object->ID()))
        return;
    
    if (p_server_squad)
    {
        this->m_object->info_add(xr_string("squad_id: ").append(p_server_squad->name()).c_str());

        if (p_server_squad->getCurrentAction().getSquadID() != Globals::kUnsignedInt16Undefined)
        {
            if (p_server_squad->getAssignedTargetID())
            {
                this->m_object->info_add(xr_string("current_action: ").append(p_server_squad->getCurrentAction().getName().c_str()).append("[").append(ai().alife().objects().object(p_server_squad->getAssignedTargetID())->name_replace()).append("]").c_str());
            }
        }
    }

    if (this->m_object->GetEnemy())
    {
        if (this->m_object->GetScriptControl())
        {
            // Lord: протестируй 
            XR_LOGIC::mob_release(this->m_object, "xr_logic");
        }
    }

    if (p_server_squad && p_server_squad->getCurrentAction().getName() == Globals::kSimulationSquadCurrentActionIDReachTarget)
    {
        CSE_ALifeDynamicObject* const p_squad_target = Script_SimulationObjects::getInstance().getObjects().at(p_server_squad->getAssignedTargetID());

        if (p_squad_target == nullptr)
            return;

        Fvector target_pos = p_squad_target->position();
        std::uint32_t target_level_id = p_squad_target->m_tNodeID;
        std::uint16_t target_game_id = p_squad_target->m_tGraphID;

        // Lord: проверь здесь по сути должна передавать xr_logic
        XR_LOGIC::mob_capture(this->m_object, true, "xr_logic");

        if (p_server_squad->commander_id() == this->m_object->ID())
        {
            Globals::action(this->m_object, CScriptMovementAction(MonsterSpace::eMA_WalkWithLeader, &target_pos), CScriptActionCondition(CScriptActionCondition::MOVEMENT_FLAG));
        }
        else
        {
            Fvector commander_position = ai().alife().objects().object(p_server_squad->commander_id())->position();
            if (commander_position.distance_to(this->m_object->Position()) > 10.0f)
            {
                Globals::action(this->m_object, CScriptMovementAction(MonsterSpace::eMA_RunWithLeader, &target_pos), CScriptActionCondition(CScriptActionCondition::MOVEMENT_FLAG));
            }
            else
            {
                Globals::action(this->m_object, CScriptMovementAction(MonsterSpace::eMA_WalkWithLeader, &target_pos), CScriptActionCondition(CScriptActionCondition::MOVEMENT_FLAG));
            }
        }

        return;
    }

    if (DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getActiveSectionName().empty() == false)
    {
        DataBase::Script_IComponentScheme* const p_current_scheme = DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getSchemes().at(DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getActiveSchemeName());
        for (Script_ISchemeEntity* const it : p_current_scheme->getActions())
        {
            if (it)
                it->update(static_cast<float>(time_delta));
        }
   }
}

void Script_Binder_Monster::save(NET_Packet* output_packet) 
{
    Globals::set_save_marker(*output_packet, Globals::kSaveMarkerMode_Save, false, "script_binder_monster");
    CScriptBinderObject::save(output_packet);
    XR_LOGIC::save_object(this->m_object, *output_packet);
    Globals::set_save_marker(*output_packet, Globals::kSaveMarkerMode_Save, true, "script_binder_monster");
}

void Script_Binder_Monster::load(IReader* input_packet) 
{
    this->m_is_loaded = true;
    Globals::set_save_marker(*input_packet, Globals::kSaveMarkerMode_Load, false, "script_binder_monster");
    CScriptBinderObject::load(input_packet);
    XR_LOGIC::load_object(this->m_object, *input_packet);
    Globals::set_save_marker(*input_packet, Globals::kSaveMarkerMode_Load, true, "script_binder_monster");
}

bool Script_Binder_Monster::net_SaveRelevant(void)
{
    return true;
}

void Script_Binder_Monster::net_Relcase(CScriptGameObject* object) {}

void Script_Binder_Monster::waypoint_callback(CScriptGameObject* p_client_object, const std::uint32_t action_type_movement, const std::uint32_t point_index)
{
    if (DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getActiveSectionName().empty() == false)
    {
        DataBase::Script_IComponentScheme* const p_current_scheme = DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getSchemes().at(DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getActiveSchemeName());
        for (Script_ISchemeEntity* const it : p_current_scheme->getActions())
        {
            if (it)
                it->waypoint_callback(p_client_object, action_type_movement, point_index);
        }
    }
}

void Script_Binder_Monster::hit_callback(CScriptGameObject* p_client_object, const float amount, const Fvector& local_direction, CScriptGameObject* p_client_who, const std::int16_t bone_index)
{
    if (p_client_who->ID() == DataBase::Storage::getInstance().getActor()->ID())
    {
        // Lord: сделать когда будет статистика
    }

    if (DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getSchemes().find("hit") != DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getSchemes().end())
    {
        DataBase::Script_IComponentScheme* const p_current_scheme = DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getSchemes().at("hit");
        if (p_current_scheme)
        {
            for (Script_ISchemeEntity* const it : p_current_scheme->getActions())
            {
                if (it)
                    it->hit_callback(p_client_object, amount, local_direction, p_client_who, bone_index);
            }
        }
    }
}

void Script_Binder_Monster::death_callback(CScriptGameObject* p_client_victim, CScriptGameObject* p_client_who)
{
 
	MESSAGE("stop_dead_id: %d", this->m_object->ID());
 

	DataBase::Storage::getInstance().setXRCombatIgnoreFightingWithActorNpcs(this->m_object->ID(), false);

	if (p_client_who->ID() == DataBase::Storage::getInstance().getActor()->ID())
	{
		// Lord: добавить когда будет статистика
	}

	if (DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getSchemes().find("mob_death") != DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getSchemes().end())
	{
		DataBase::Script_IComponentScheme* const p_current_scheme = DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getSchemes().at("mob_death");
		if (p_current_scheme)
		{
			for (Script_ISchemeEntity* const it : p_current_scheme->getActions())
			{
				if (it)
					it->death_callback(p_client_victim, p_client_who);
			}
		}
	}

	if (DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getActiveSectionName().empty() == false)
	{
		DataBase::Script_IComponentScheme* const p_current_scheme = DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getSchemes().at(DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getActiveSchemeName());
		for (Script_ISchemeEntity* const it : p_current_scheme->getActions())
		{
			if (it)
				it->death_callback(p_client_victim, p_client_who);
		}
	}

	CScriptHit hit;
	hit.m_tpDraftsman = this->m_object;
	hit.m_tHitType = ALife::eHitTypeFireWound;
	hit.m_caBoneName = "pelvis";
	hit.m_fPower = 1.0f;
	hit.m_fImpulse = 10.0f;
	this->m_object->Hit(&hit); // Lord: зачем прописывать??

	if (this->m_object->clsid() == CLSID_SE_MONSTER_POLTERGEIST)
	{
 
		MESSAGEI("releasing object %s", this->m_object->Name());
 

		if (ai().alife().objects().object(this->m_object->ID()))
		{
			Globals::Game::alife_release(ai().alife().objects().object(this->m_object->ID()), true);
		}
	}
}

void Script_Binder_Monster::hear_callback(CScriptGameObject* p_client_object, const std::uint16_t who_id, const int sound_type, const Fvector& sound_position, const float sound_power)
{
    // Lord: пока что без него, нужно будет реализовать
}

} // namespace Scripts
} // namespace Cordis
