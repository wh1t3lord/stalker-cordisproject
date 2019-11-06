#include "stdafx.h"
#include "script_binder_object.h"
#include "Script_Binder_Actor.h"

namespace Cordis
{
namespace Scripts
{
Script_Binder_Actor::Script_Binder_Actor(CScriptGameObject* object) : inherited(object)
{}

Script_Binder_Actor::~Script_Binder_Actor(void) {}

void Script_Binder_Actor::reinit(void) 
{

}

void Script_Binder_Actor::reload(LPCSTR section_name) {}

bool Script_Binder_Actor::net_Spawn(SpawnType DC) 
{
    DataBase::Storage::getInstance().setActor(this->m_object);
    return true; 
}

void Script_Binder_Actor::net_Destroy(void)
{
    DataBase::Storage::getInstance().deleteActor(this->m_object);
}

void Script_Binder_Actor::net_Import(NET_Packet* packet) {}

void Script_Binder_Actor::net_Export(NET_Packet* packet) {}

void Script_Binder_Actor::shedule_Update(std::uint32_t time_delta) {}

void Script_Binder_Actor::save(NET_Packet* output_packet) {}

void Script_Binder_Actor::load(IReader* input_packet) {}

bool Script_Binder_Actor::net_SaveRelevant(void) { return false; }

void Script_Binder_Actor::net_Relcase(CScriptGameObject* object) {}
} // namespace Scripts
} // namespace Cordis
