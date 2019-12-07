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
        if (!this->m_storage->getXRDeathInfo().empty())
        {
            XR_LOGIC::pick_section_from_condlist(
                DataBase::Storage::getInstance().getActor(), this->m_npc, this->m_storage->getXRDeathInfo());
        }

        if (!this->m_storage->getXRDeathInfo2().empty())
        {
            XR_LOGIC::pick_section_from_condlist(
                DataBase::Storage::getInstance().getActor(), this->m_npc, this->m_storage->getXRDeathInfo2());
        }
    }
}

} // namespace Scripts
} // namespace Cordis
