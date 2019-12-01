#include "stdafx.h"
#include "Script_SchemeMobDeath.h"
#include "Script_Database.h"


namespace Cordis
{
namespace Scripts
{
Script_SchemeMobDeath::Script_SchemeMobDeath(
    CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage)
{
}

Script_SchemeMobDeath::~Script_SchemeMobDeath(void) {}

void Script_SchemeMobDeath::death_callback(CScriptGameObject* const p_client_victim, CScriptGameObject* const p_client_who)
{
    if (p_client_who)
    {
        Msg("[Scripts/Script_SchemeMobDeath/death_callback()] %s killed by %s", p_client_victim->Name(),
            p_client_who->Name());

        DataBase::Storage::getInstance().setStorageDeathDataKillerID(p_client_victim->ID(), p_client_who->ID());
        DataBase::Storage::getInstance().setStorageDeathDataKillerName(p_client_victim->ID(), p_client_who->Name());
    }
    else
    {
        Msg("[Scripts/Script_SchemeMobDeath/death_callback(p_client_victim, p_client_who)] killed by unknown!");
    }

    // Lord: доделать когда будет сделан XR_LOGIC::try_switch_to_another_section
}

} // namespace Scripts
} // namespace Cordis
