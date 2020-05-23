#include "stdafx.h"
#include "Script_Binder_Helicopter.h"

namespace Cordis
{
namespace Scripts
{
// Lord: дописать когда протестируются все основные схемы!
// Lord: суть в том что в этом биндере используется схема heli_combat, которую я не реализовал, а нужно!
Script_Binder_Helicopter::Script_Binder_Helicopter(CScriptGameObject* const p_client_object)
    : CScriptBinderObject(p_client_object), m_is_loaded(false), m_is_initialized(false),
      m_p_helicopter(Script_HelicopterFireStorage::getInstance().AllocateFirer(p_client_object)),
      m_p_ini(p_client_object->spawn_ini())
{
}

Script_Binder_Helicopter::~Script_Binder_Helicopter(void) {}

void Script_Binder_Helicopter::reinit() { CScriptBinderObject::reinit(); }

bool Script_Binder_Helicopter::net_Spawn(SpawnType DC) { return false; }

void Script_Binder_Helicopter::net_Destroy() {}

void Script_Binder_Helicopter::shedule_Update(u32 time_delta) {}

void Script_Binder_Helicopter::save(NET_Packet* output_packet) {}

void Script_Binder_Helicopter::load(IReader* input_packet) {}

bool Script_Binder_Helicopter::net_SaveRelevant() { return true; }
} // namespace Scripts
}
