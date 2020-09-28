#include "stdafx.h"
#include "Script_SchemeMobDeath.h"
#include "Script_Database.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemeMobDeath::Script_SchemeMobDeath(
    CScriptGameObject* const p_client_object, DataBase::Script_ComponentScheme_MobDeath* storage)
    : inherited_scheme(p_client_object, storage)
{
}

Script_SchemeMobDeath::~Script_SchemeMobDeath(void) {}

void Script_SchemeMobDeath::death_callback(
    CScriptGameObject* const p_client_victim, CScriptGameObject* const p_client_who)
{
    if (p_client_who)
    {
        MESSAGEI("%s killed by %s", p_client_victim->Name(),
            p_client_who->Name());

        DataBase::Storage::getInstance().setStorageDeathDataKillerID(p_client_victim->ID(), p_client_who->ID());
        DataBase::Storage::getInstance().setStorageDeathDataKillerName(p_client_victim->ID(), p_client_who->Name());
    }
    else
    {
        MESSAGEI("killed by unknown!");
    }

    if (XR_LOGIC::try_switch_to_another_section(
            this->m_npc, this->m_p_true_storage, DataBase::Storage::getInstance().getActor()))
        return;
}

void Script_SchemeMobDeath::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
{
    DataBase::Script_ComponentScheme_MobDeath* p_storage =
        XR_LOGIC::assign_storage_and_bind<DataBase::Script_ComponentScheme_MobDeath>(p_client_object, p_ini, scheme_name, section_name, gulag_name);

    if (!p_storage)
    {
        R_ASSERT2(false, "it can't be!");
        return;
    }

    p_storage->setLogic(XR_LOGIC::cfg_get_switch_conditions(p_ini, section_name, p_client_object));
}

} // namespace Scripts
} // namespace Cordis
