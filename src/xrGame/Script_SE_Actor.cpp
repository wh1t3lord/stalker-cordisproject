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

} // namespace Scripts
} // namespace Cordis
