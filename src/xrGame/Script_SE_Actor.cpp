#include "stdafx.h"
#include "Script_SE_Actor.h"
#include "Script_StoryObject.h"

namespace Cordis
{
namespace Scripts
{
Script_SE_Actor::Script_SE_Actor(LPCSTR section)
    : inherited(section), m_is_registered(false), m_is_start_position_filled(false)
{
}

Script_SE_Actor::~Script_SE_Actor(void) {}

void Script_SE_Actor::on_register(void)
{
    inherited::on_register();
    Script_StoryObject::getInstance().registrate(this->ID, "actor");
    Script_SimulationObjects::getInstance().registrate(this);

    this->m_is_registered = true;

    if (!this->m_is_start_position_filled)
    {
        Script_SimulationBoard::getInstance().fill_start_position();
        this->m_is_start_position_filled = true;
    }
}

void Script_SE_Actor::on_unregister(void)
{
    inherited::on_unregister();
    Script_StoryObject::getInstance().unregistrate_by_id(this->ID);

    Script_SimulationObjects::getInstance().unregistrate(this);
}

void Script_SE_Actor::STATE_Write(NET_Packet& packet)
{
    inherited::STATE_Write(packet);

    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, false, "Script_SE_Actor");

    packet.w_u8(this->m_is_start_position_filled ? 1 : 0);

    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, true, "Scritp_SE_Actor");
}

void Script_SE_Actor::STATE_Read(NET_Packet& packet, std::uint16_t size)
{
    inherited::STATE_Read(packet, size);

    if (FS.IsSDK())
        return;

    if (!DataBase::Storage::getInstance().getActor())
    {
        Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Load, false, "Script_SE_Actor");
        this->m_is_start_position_filled = packet.r_u8() ? true : false;
        Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Load, true, "Script_SE_Actor");
    }
}

void Script_SE_Actor::on_reach_target(Script_SE_SimulationSquad* squad)
{
    if (!squad)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    squad->set_location_types("");

    for (AssociativeVector<std::uint16_t, CSE_ALifeMonsterAbstract*>::const_iterator it =
             squad->squad_members().begin();
         it != squad->squad_members().end(); ++it)
    {
        if (DataBase::Storage::getInstance().getOfflineObjects()[it->first].second.size())
        {
            DataBase::Storage::getInstance().getOfflineObjects()[it->first].second.clear();
            DataBase::Storage::getInstance().getOfflineObjects()[it->first].first = Globals::kUnsignedInt16Undefined;
        }
    }

    Script_SimulationBoard::getInstance().assigned_squad_to_smart(squad);
}

bool Script_SE_Actor::IsSimulationAvailable(void) { return false; }

} // namespace Scripts
} // namespace Cordis
