#include "stdafx.h"
#include "Script_Binder_Phantom.h"

namespace Cordis
{
namespace Scripts
{
Script_Binder_Phantom::Script_Binder_Phantom(CScriptGameObject* const p_client_object) : inherited(p_client_object)
{
    Msg("[Scripts/Script_Binder_Phantom/ctor()] %s", p_client_object->Name());
    Script_PhantomManager::getInstance().add_phantom();
}

void Script_Binder_Phantom::net_Destroy(void) 
{
    Script_PhantomManager::getInstance().remove_phantom(); 
}
} // namespace Scripts
} // namespace Cordis
