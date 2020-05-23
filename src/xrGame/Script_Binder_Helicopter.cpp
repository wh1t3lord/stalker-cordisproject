#include "stdafx.h"
#include "Script_Binder_Helicopter.h"

namespace Cordis
{
    namespace Scripts
{
    Script_Binder_Helicopter::Script_Binder_Helicopter(CScriptGameObject* const p_client_object) : inherited(p_client_object), m_is_initialized(false), m_is_loaded(false) 
    {
        this->m_p_heli_logic = Script_HelicopterFireStorage::getInstance().AllocateFirer(p_client_object);
    }

    Script_Binder_Helicopter::~Script_Binder_Helicopter(void) {}

    void Script_Binder_Helicopter::reinit() {}

    void Script_Binder_Helicopter::reload(LPCSTR section) {}

    bool Script_Binder_Helicopter::net_Spawn(SpawnType DC) { return false; }

    void Script_Binder_Helicopter::net_Destroy() {}

    void Script_Binder_Helicopter::shedule_Update(u32 time_delta) {}

    void Script_Binder_Helicopter::save(NET_Packet* output_packet) {}

    void Script_Binder_Helicopter::load(IReader* input_packet) {}

    bool Script_Binder_Helicopter::net_SaveRelevant() { return true; }
    } // namespace Scripts
}
