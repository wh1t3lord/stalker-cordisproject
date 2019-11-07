#include "stdafx.h"
#include "Script_Binder_Monster.h"

namespace Cordis
{
namespace Scripts
{
Script_Binder_Monster::Script_Binder_Monster(CScriptGameObject* object) : inherited(object) {}

Script_Binder_Monster::~Script_Binder_Monster(void) {}

void Script_Binder_Monster::reinit(void) {}

void Script_Binder_Monster::reload(LPCSTR section_name) {}

bool Script_Binder_Monster::net_Spawn(SpawnType DC) 
{
    Msg("[Scripts/Script_Binder_Monster/net_Spawn(DC)] adding to database %s", this->m_object->Name());
    DataBase::Storage::getInstance().deleteObject(this->m_object);
    return true; 
}

void Script_Binder_Monster::net_Destroy(void) 
{
    Msg("[Scripts/Script_Binder_Monster/net_Destroy()] deleting from database %s", this->m_object->Name()); 
    DataBase::Storage::getInstance().deleteObject(this->m_object);
}

void Script_Binder_Monster::net_Import(NET_Packet* packet) {}

void Script_Binder_Monster::net_Export(NET_Packet* packet) {}

void Script_Binder_Monster::shedule_Update(std::uint32_t time_delta) {}

void Script_Binder_Monster::save(NET_Packet* output_packet) {}

void Script_Binder_Monster::load(IReader* input_packet) {}

bool Script_Binder_Monster::net_SaveRelevant(void)
{
    return false;
}

void Script_Binder_Monster::net_Relcase(CScriptGameObject* object) {}
} // namespace Scripts
} // namespace Cordis
