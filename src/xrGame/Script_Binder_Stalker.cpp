#include "stdafx.h"
#include "Script_Binder_Stalker.h"

namespace Cordis
{
namespace Scripts
{
Script_Binder_Stalker::Script_Binder_Stalker(CScriptGameObject* object) : inherited(object), m_is_loaded(false), m_is_first_update(false), m_last_update(0) {}

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
    MESSAGE("add to database %s", this->m_object->Name());

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

    return true; 
}

void Script_Binder_Stalker::net_Destroy(void)
{
    MESSAGE("delete from database %s", this->m_object->Name());
    DataBase::Storage::getInstance().deleteObject(this->m_object);
}

void Script_Binder_Stalker::net_Import(NET_Packet* packet) {}

void Script_Binder_Stalker::net_Export(NET_Packet* packet) {}

void Script_Binder_Stalker::shedule_Update(std::uint32_t time_delta) {}

void Script_Binder_Stalker::save(NET_Packet* output_packet)
{
    this->m_is_loaded = true;
    Globals::set_save_marker(*output_packet, Globals::kSaveMarkerMode_Save, false, "Script_Binder_Stalker");
    inherited::save(output_packet);
    
    Globals::set_save_marker(*output_packet, Globals::kSaveMarkerMode_Save, true, "Script_Binder_Stalker");
}

void Script_Binder_Stalker::load(IReader* input_packet) {}

bool Script_Binder_Stalker::net_SaveRelevant(void) { return false; }

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
