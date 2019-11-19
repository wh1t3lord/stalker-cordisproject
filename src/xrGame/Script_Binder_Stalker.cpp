#include "stdafx.h"
#include "Script_Binder_Stalker.h"

namespace Cordis
{
namespace Scripts
{
Script_Binder_Stalker::Script_Binder_Stalker(CScriptGameObject* object) : inherited(object), m_is_loaded(false) {}

Script_Binder_Stalker::~Script_Binder_Stalker(void) {}

void Script_Binder_Stalker::reinit(void) {}

void Script_Binder_Stalker::reload(LPCSTR section_name) {}

bool Script_Binder_Stalker::net_Spawn(SpawnType DC) 
{
    Msg("[Scripts/Script_Binder_Stalker/net_Spawn(DC)] add to database %s", this->m_object->Name());
    DataBase::Storage::getInstance().addObject(this->m_object);
    return true; 
}

void Script_Binder_Stalker::net_Destroy(void)
{
    Msg("[Scripts/Script_Binder_Stalker/net_Destroy()] delete from database %s", this->m_object->Name());
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
} // namespace Scripts
} // namespace Cordis
