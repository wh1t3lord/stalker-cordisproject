#include "stdafx.h"
#include "Script_SchemeXRDeath.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemeXRDeath::Script_SchemeXRDeath(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage)
{
    this->m_scheme_name = "death";
}

Script_SchemeXRDeath::~Script_SchemeXRDeath(void) {}

void Script_SchemeXRDeath::death_callback(
    CScriptGameObject* const p_client_victim, CScriptGameObject* const p_client_who)
{
    if (p_client_who)
    {
        Msg("[Scripts/Script_SchemeXRDeath/death_callback(p_client_victim, p_client_who)] %s killed by %s",
            p_client_victim->Name(), p_client_who->Name());
        DataBase::Storage::getInstance().setStorageDeathDataKillerID(p_client_victim->ID(), p_client_who->ID());
    }
    else
    {
        Msg("[Scripts/Script_SchemeXRDeath/death_callback(p_client_victim, p_client_who)] %s killed by Unknown",
            p_client_victim->Name());
        DataBase::Storage::getInstance().setStorageDeathDataKillerID(
            p_client_victim->ID(), Globals::kUnsignedInt32Undefined);
    }

    if (DataBase::Storage::getInstance().getActor())
    {
        if (!this->m_p_storage->getXRDeathInfo().empty())
        {
            XR_LOGIC::pick_section_from_condlist(
                DataBase::Storage::getInstance().getActor(), this->m_npc, this->m_p_storage->getXRDeathInfo());
        }

        if (!this->m_p_storage->getXRDeathInfo2().empty())
        {
            XR_LOGIC::pick_section_from_condlist(
                DataBase::Storage::getInstance().getActor(), this->m_npc, this->m_p_storage->getXRDeathInfo2());
        }
    }
}

void Script_SchemeXRDeath::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    DataBase::Storage_Scheme* p_storage =
        XR_LOGIC::assign_storage_and_bind(p_client_object, p_ini, scheme_name, section_name, gulag_name);

    if (!p_storage)
    {
        R_ASSERT2(false, "object is null!");
    }
}

} // namespace Scripts
} // namespace Cordis
